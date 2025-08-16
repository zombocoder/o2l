/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FFIEngine.hpp"

#include <cerrno>
#include <cstring>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace o2l::ffi {

// Private implementation struct
struct PreparedCallImpl {
#ifdef HAVE_FFI
    ffi_cif cif;
    std::vector<ffi_type*> arg_types;
    ffi_type* return_type;
#endif
};

PreparedCall::PreparedCall(Signature sig)
    : signature(std::move(sig)), impl(std::make_unique<PreparedCallImpl>()) {
#ifdef HAVE_FFI
    // Map O²L types to libffi types
    impl->return_type = nullptr;
    switch (signature.ret) {
        case CType::Void:
            impl->return_type = &ffi_type_void;
            break;
        case CType::Int32:
            impl->return_type = &ffi_type_sint32;
            break;
        case CType::Int64:
            impl->return_type = &ffi_type_sint64;
            break;
        case CType::Float32:
            impl->return_type = &ffi_type_float;
            break;
        case CType::Float64:
            impl->return_type = &ffi_type_double;
            break;
        case CType::Bool:
            impl->return_type = &ffi_type_uint8;
            break;
        case CType::Text:
        case CType::Ptr:
        case CType::Struct:
        case CType::Array:
        case CType::Callback:
        case CType::CString:
            impl->return_type = &ffi_type_pointer;
            break;
    }

    impl->arg_types.reserve(signature.args.size());
    for (CType arg_type : signature.args) {
        ffi_type* ffi_type = nullptr;
        switch (arg_type) {
            case CType::Void:
                ffi_type = &ffi_type_void;
                break;
            case CType::Int32:
                ffi_type = &ffi_type_sint32;
                break;
            case CType::Int64:
                ffi_type = &ffi_type_sint64;
                break;
            case CType::Float32:
                ffi_type = &ffi_type_float;
                break;
            case CType::Float64:
                ffi_type = &ffi_type_double;
                break;
            case CType::Bool:
                ffi_type = &ffi_type_uint8;
                break;
            case CType::Text:
            case CType::Ptr:
            case CType::Struct:
            case CType::Array:
            case CType::Callback:
            case CType::CString:
                ffi_type = &ffi_type_pointer;
                break;
        }
        impl->arg_types.push_back(ffi_type);
    }

    // Prepare the call interface
    ffi_status status = ffi_prep_cif(&impl->cif, FFI_DEFAULT_ABI, signature.args.size(),
                                     impl->return_type, impl->arg_types.data());

    if (status != FFI_OK) {
        throw std::runtime_error("Failed to prepare FFI call interface");
    }
#endif
}

PreparedCall::~PreparedCall() = default;

FFIEngine::FFIEngine() {
    last_errno_ = 0;
#ifdef _WIN32
    last_win_err_ = 0;
#endif
}

FFIEngine::~FFIEngine() {
    cleanup_temp_memory();
}

std::expected<Value, FFICallError> FFIEngine::call(void* func_ptr, const Signature& sig,
                                                   const std::vector<Value>& args) {
    if (!func_ptr) {
        return std::unexpected(FFICallError{FFICallError::CallFailed, "Function pointer is null"});
    }

    if (args.size() != sig.args.size()) {
        return std::unexpected(
            FFICallError{FFICallError::TypeMismatch, "Argument count mismatch: expected " +
                                                         std::to_string(sig.args.size()) +
                                                         ", got " + std::to_string(args.size())});
    }

    // Get or create prepared call
    PreparedCall* prepared = getOrCreateCall(sig);
    if (!prepared) {
        return std::unexpected(
            FFICallError{FFICallError::InvalidSignature, "Failed to prepare FFI call"});
    }

    // Marshal arguments - allocate storage for each argument
    std::vector<void*> arg_values;
    std::vector<std::unique_ptr<uint8_t[]>> arg_storage;

    for (size_t i = 0; i < args.size(); ++i) {
        auto marshaled = marshalValue(args[i], sig.args[i], arg_storage);
        if (!marshaled) {
            return std::unexpected(marshaled.error());
        }
        arg_values.push_back(*marshaled);
    }

    // Prepare return value storage
    void* result_storage = nullptr;
    uint64_t result_int = 0;
    double result_double = 0.0;
    float result_float = 0.0f;
    void* result_ptr = nullptr;
    uint8_t result_bool = 0;

    switch (sig.ret) {
        case CType::Void:
            result_storage = nullptr;
            break;
        case CType::Int32:
        case CType::Int64:
            result_storage = &result_int;
            break;
        case CType::Float32:
            result_storage = &result_float;
            break;
        case CType::Float64:
            result_storage = &result_double;
            break;
        case CType::Bool:
            result_storage = &result_bool;
            break;
        case CType::Text:
        case CType::Ptr:
        case CType::Struct:
        case CType::Array:
        case CType::Callback:
        case CType::CString:
            result_storage = &result_ptr;
            break;
    }

    // Clear previous error state
    errno = 0;
#ifdef _WIN32
    SetLastError(0);
#endif

    // Make the FFI call
#ifdef HAVE_FFI
    ffi_call(&prepared->impl->cif, reinterpret_cast<void (*)()>(func_ptr), result_storage,
             arg_values.data());
#else
    // Fallback - just return error
    return std::unexpected(FFICallError{FFICallError::CallFailed, "libffi not available"});
#endif

    // Capture error state
    last_errno_ = errno;
#ifdef _WIN32
    last_win_err_ = GetLastError();
#endif

    // Clean up temporary memory
    cleanup_temp_memory();

    // Unmarshal result
    if (sig.ret == CType::Void) {
        // Return a special "void" value - in O²L this could be a unit type
        return Value(Text("void"));
    }

    return unmarshalValue(result_storage, sig.ret);
}

std::string FFIEngine::signatureToKey(const Signature& sig) {
    std::ostringstream oss;
    oss << ctypeToString(sig.ret) << "(";
    for (size_t i = 0; i < sig.args.size(); ++i) {
        if (i > 0) oss << ",";
        oss << ctypeToString(sig.args[i]);
    }
    oss << ")";
    return oss.str();
}

PreparedCall* FFIEngine::getOrCreateCall(const Signature& sig) {
    std::string key = signatureToKey(sig);

    auto it = call_cache_.find(key);
    if (it != call_cache_.end()) {
        return it->second.get();
    }

    try {
        auto prepared = std::make_unique<PreparedCall>(sig);
        PreparedCall* ptr = prepared.get();
        call_cache_[key] = std::move(prepared);
        return ptr;
    } catch (const std::exception& e) {
        return nullptr;
    }
}

std::expected<void*, FFICallError> FFIEngine::marshalValue(
    const Value& value, CType expected_type, std::vector<std::unique_ptr<uint8_t[]>>& storage) {
    switch (expected_type) {
        case CType::Int32: {
            if (std::holds_alternative<Int>(value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(int32_t));
                int32_t val = static_cast<int32_t>(std::get<Int>(value));
                std::memcpy(temp.get(), &val, sizeof(int32_t));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Int for i32 parameter"});
        }

        case CType::Int64: {
            if (std::holds_alternative<Int>(value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(int64_t));
                int64_t val = std::get<Int>(value);
                std::memcpy(temp.get(), &val, sizeof(int64_t));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Int for i64 parameter"});
        }

        case CType::Float32: {
            if (std::holds_alternative<Float>(value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(float));
                float val = std::get<Float>(value);
                std::memcpy(temp.get(), &val, sizeof(float));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Float for f32 parameter"});
        }

        case CType::Float64: {
            if (std::holds_alternative<Double>(value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(double));
                double val = std::get<Double>(value);
                std::memcpy(temp.get(), &val, sizeof(double));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Double for f64 parameter"});
        }

        case CType::Bool: {
            if (std::holds_alternative<Bool>(value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(uint8_t));
                uint8_t val = std::get<Bool>(value) ? 1 : 0;
                std::memcpy(temp.get(), &val, sizeof(uint8_t));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Bool for bool parameter"});
        }

        case CType::Text: {
            if (std::holds_alternative<Text>(value)) {
                const std::string& str = std::get<Text>(value);
                // Create a temporary C string that lives for the duration of the call
                auto cstr = std::make_unique<char[]>(str.length() + 1);
                std::strcpy(cstr.get(), str.c_str());

                // Store the char* in our storage and return pointer to it
                auto temp = std::make_unique<uint8_t[]>(sizeof(char*));
                char* str_ptr = cstr.get();
                std::memcpy(temp.get(), &str_ptr, sizeof(char*));
                void* ptr = temp.get();

                temp_cstrings_.push_back(std::move(cstr));
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Text for text parameter"});
        }

        case CType::Ptr: {
            // Handle CBufferInstance (C string buffers)
            if (auto buffer = std::get_if<std::shared_ptr<ffi::CBufferInstance>>(&value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(void*));
                void* buffer_ptr = const_cast<void*>(static_cast<const void*>((*buffer)->data()));
                std::memcpy(temp.get(), &buffer_ptr, sizeof(void*));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            
            // Handle CArrayInstance (typed arrays)
            if (auto array = std::get_if<std::shared_ptr<ffi::CArrayInstance>>(&value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(void*));
                void* array_ptr = const_cast<void*>(static_cast<const void*>((*array)->data()));
                std::memcpy(temp.get(), &array_ptr, sizeof(void*));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            
            // Handle CStructInstance 
            if (auto struct_ptr = std::get_if<std::shared_ptr<ffi::CStructInstance>>(&value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(void*));
                void* struct_data_ptr = const_cast<void*>(static_cast<const void*>((*struct_ptr)->data()));
                std::memcpy(temp.get(), &struct_data_ptr, sizeof(void*));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            
            // Handle PtrInstance (including nullPtr)
            if (auto ptr_inst = std::get_if<std::shared_ptr<ffi::PtrInstance>>(&value)) {
                auto temp = std::make_unique<uint8_t[]>(sizeof(void*));
                void* actual_ptr = (*ptr_inst)->get();
                std::memcpy(temp.get(), &actual_ptr, sizeof(void*));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            
            // Handle generic ObjectInstance
            if (auto obj = std::get_if<std::shared_ptr<ObjectInstance>>(&value)) {
                // For now, allocate storage for null pointer
                auto temp = std::make_unique<uint8_t[]>(sizeof(void*));
                void* null_ptr = nullptr;
                std::memcpy(temp.get(), &null_ptr, sizeof(void*));
                void* ptr = temp.get();
                storage.push_back(std::move(temp));
                return ptr;
            }
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Expected Ptr for ptr parameter"});
        }

        case CType::Struct:
        case CType::Array:
        case CType::Callback:
        case CType::CString:
            // These are handled the same as Ptr - all become pointer types
            return marshalValue(value, CType::Ptr, storage);

        case CType::Void:
            return std::unexpected(
                FFICallError{FFICallError::TypeMismatch, "Cannot marshal value to void"});
    }

    return std::unexpected(FFICallError{FFICallError::TypeMismatch, "Unknown type"});
}

std::expected<Value, FFICallError> FFIEngine::unmarshalValue(void* result, CType type) {
    switch (type) {
        case CType::Int32: {
            int32_t val = *static_cast<int32_t*>(result);
            return Value(Int(val));
        }

        case CType::Int64: {
            int64_t val = *static_cast<int64_t*>(result);
            return Value(Int(val));
        }

        case CType::Float32: {
            float val = *static_cast<float*>(result);
            return Value(Float(val));
        }

        case CType::Float64: {
            double val = *static_cast<double*>(result);
            return Value(Double(val));
        }

        case CType::Bool: {
            uint8_t val = *static_cast<uint8_t*>(result);
            return Value(Bool(val != 0));
        }

        case CType::Text: {
            char* str = *static_cast<char**>(result);
            if (!str) {
                return std::unexpected(
                    FFICallError{FFICallError::NullResult, "C function returned null string"});
            }
            return Value(Text(str));
        }

        case CType::Ptr:
        case CType::Struct:
        case CType::Array:
        case CType::Callback:
        case CType::CString: {
            void* ptr = *static_cast<void**>(result);
            // Create a PtrInstance for proper pointer handling
            auto ptr_instance = std::make_shared<ffi::PtrInstance>(ptr);
            return Value(ptr_instance);
        }

        case CType::Void:
            return Value(Text("void"));
    }

    return std::unexpected(FFICallError{FFICallError::TypeMismatch, "Unknown return type"});
}

void FFIEngine::cleanup_temp_memory() {
    temp_cstrings_.clear();
}

}  // namespace o2l::ffi