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

#include "FFILibrary.hpp"
#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "ErrorInstance.hpp"
#include "ResultInstance.hpp"
#include "ListInstance.hpp"
#include "../Common/Exceptions.hpp"

#include <filesystem>
#include <cstring>
#include <iostream>

namespace o2l {

// Static member definitions
bool FFILibrary::ffi_enabled_ = false;
std::vector<std::string> FFILibrary::allowed_paths_;
std::vector<std::string> FFILibrary::denied_symbols_;

// TODO: Make this configurable
size_t FFILibrary::max_out_bytes_ = 1024 * 1024; // 1MB default
std::unique_ptr<ffi::FFIEngine> FFILibrary::engine_;

std::shared_ptr<ObjectInstance> FFILibrary::createFFIObject() {
    auto ffi_obj = std::make_shared<ObjectInstance>("ffi");
    
    // Main ffi static methods
    ffi_obj->addMethod("load", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_load(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("getLastErrno", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_getLastErrno(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("setPolicy", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_setPolicy(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("ptr", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_ptr(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("nullPtr", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_nullPtr(args, ctx);
    }, true);  // external
    
    // Enhanced FFI types
    ffi_obj->addMethod("struct", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_struct(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("array", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_array(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("callback", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_callback(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("cstring", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_cstring(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("ptrToString", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_ptrToString(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("ptrToInt", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_ptrToInt(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("ptrToDouble", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_ptrToDouble(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("ptrToFloat", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_ptrToFloat(args, ctx);
    }, true);  // external
    
    ffi_obj->addMethod("ptrToBool", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return ffi_ptrToBool(args, ctx);
    }, true);  // external
    
    return ffi_obj;
}

Value FFILibrary::ffi_load(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled. Use --allow-ffi flag.");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (args.empty() || !std::holds_alternative<Text>(args[0])) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected Text path argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    std::string path = std::get<Text>(args[0]);
    
    if (!isPathAllowed(path)) {
        auto error = std::make_shared<ErrorInstance>("PATH_DENIED", "Path not allowed: " + path);
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    auto lib_result = ffi::SharedLibrary::open(path);
    if (!lib_result) {
        std::string error_msg = "Failed to load library: " + lib_result.error().msg;
        auto error = std::make_shared<ErrorInstance>("LOAD_FAILED", error_msg);
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    auto lib_ptr = std::make_unique<ffi::SharedLibrary>(std::move(*lib_result));
    auto lib_instance = std::make_shared<FFILibraryInstance>(std::move(lib_ptr), path);
    
    // Create Library object with methods
    auto library_obj = std::make_shared<ObjectInstance>("Library");
    
    // Add symbol method - we'll need to pass the library instance through closure
    library_obj->addMethod("symbol", [lib_instance](const std::vector<Value>& args, Context& ctx) -> Value {
        // Implementation moved here since we need access to lib_instance
        if (!ffi_enabled_) {
            auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        if (args.size() < 2 || !std::holds_alternative<Text>(args[0])) {
            auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected symbol name and signature");
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        std::string symbol_name = std::get<Text>(args[0]);
        std::string signature_str = std::get<Text>(args[1]);
        
        void* symbol_ptr = lib_instance->getSymbol(symbol_name);
        if (!symbol_ptr) {
            auto error = std::make_shared<ErrorInstance>("SYMBOL_NOT_FOUND", "Symbol not found: " + symbol_name);
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        // Parse signature string (format: "arg1,arg2,arg3->ret" or just "->ret" for no args)
        ffi::Signature parsed_sig = parseSignature(signature_str);
        if (parsed_sig.ret == ffi::CType::Void && parsed_sig.args.empty() && signature_str != "->void") {
            auto error = std::make_shared<ErrorInstance>("INVALID_SIGNATURE", "Failed to parse signature: " + signature_str);
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        auto native_fn = std::make_shared<FFINativeFnInstance>(symbol_ptr, parsed_sig, lib_instance);
        
        // Return the native function wrapped in an ObjectInstance
        auto fn_obj = std::make_shared<ObjectInstance>("NativeFn");
        
        // Add call method - capture the native_fn in the lambda
        fn_obj->addMethod("call", [native_fn](const std::vector<Value>& args, Context& ctx) -> Value {
            return nativefn_call_impl(args, ctx, native_fn);
        }, true);  // external
        
        auto result_instance = std::make_shared<ResultInstance>(Value(fn_obj), "Value", "Error");
        return Value(result_instance);
    }, true);  // external
    
    // Add close method  
    library_obj->addMethod("close", [lib_instance](const std::vector<Value>& args, Context& ctx) -> Value {
        return library_close(args, ctx);
    }, true);  // external
    
    // Note: Library instance is captured in the lambda closures above
    
    auto result_instance = std::make_shared<ResultInstance>(Value(library_obj), "Value", "Error");
    return Value(result_instance);
}

Value FFILibrary::ffi_getLastErrno(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        return Value(Int(0));
    }
    
    initializeEngine();
    return Value(Int(engine_->getLastErrno()));
}

Value FFILibrary::ffi_setPolicy(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        return Value(Text("void"));
    }
    
    // Policy setting would be implemented here
    // For now, just return void
    return Value(Text("void"));
}

Value FFILibrary::ffi_ptr(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto null_ptr = std::make_shared<ffi::PtrInstance>(nullptr);
        return Value(null_ptr);
    }
    
    if (args.empty() || !std::holds_alternative<Int>(args[0])) {
        auto null_ptr = std::make_shared<ffi::PtrInstance>(nullptr);
        return Value(null_ptr);
    }
    
    Int value = std::get<Int>(args[0]);
    void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(value));
    auto ptr_instance = std::make_shared<ffi::PtrInstance>(ptr);
    return Value(ptr_instance);
}

Value FFILibrary::ffi_nullPtr(const std::vector<Value>& args, Context& context) {
    auto null_ptr = std::make_shared<ffi::PtrInstance>(nullptr);
    return Value(null_ptr);
}

Value FFILibrary::library_symbol(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (args.size() < 2 || !std::holds_alternative<Text>(args[0])) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected symbol name and signature");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // This would need to access the library instance from the calling object
    // For now, return a basic error
    auto error = std::make_shared<ErrorInstance>("NOT_IMPLEMENTED", "Symbol lookup not yet implemented");
    return ResultInstance::createError(Value(error), "NativeFn", "Error");
}

Value FFILibrary::library_close(const std::vector<Value>& args, Context& context) {
    return Value(Text("void"));
}

Value FFILibrary::nativefn_call(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // This is the old method - should not be used anymore
    auto error = std::make_shared<ErrorInstance>("DEPRECATED", "Use nativefn_call_impl instead");
    return ResultInstance::createError(Value(error), "Value", "Error");
}

Value FFILibrary::nativefn_call_impl(const std::vector<Value>& args, Context& context, 
                                     std::shared_ptr<FFINativeFnInstance> native_fn) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (!native_fn || !native_fn->getFuncPtr()) {
        auto error = std::make_shared<ErrorInstance>("INVALID_FUNCTION", "Invalid native function");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    const auto& signature = native_fn->getSignature();
    
    // Check argument count
    if (args.size() != signature.args.size()) {
        auto error = std::make_shared<ErrorInstance>("ARGUMENT_MISMATCH", 
            "Expected " + std::to_string(signature.args.size()) + " arguments, got " + std::to_string(args.size()));
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // Initialize FFI engine
    initializeEngine();
    
    // Call the function through FFI engine
    auto result = engine_->call(native_fn->getFuncPtr(), signature, args);
    if (!result) {
        std::string error_msg = "FFI call failed: ";
        switch (result.error().kind) {
            case ffi::FFICallError::InvalidSignature:
                error_msg += "Invalid signature";
                break;
            case ffi::FFICallError::TypeMismatch:
                error_msg += "Type mismatch";
                break;
            case ffi::FFICallError::CallFailed:
                error_msg += "Call failed";
                break;
            case ffi::FFICallError::NullResult:
                error_msg += "Null result";
                break;
            default:
                error_msg += "Unknown error";
                break;
        }
        error_msg += " - " + result.error().msg;
        
        auto error = std::make_shared<ErrorInstance>("FFI_CALL_FAILED", error_msg);
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // Return the result wrapped in a Result type
    auto result_instance = std::make_shared<ResultInstance>(result.value(), "Value", "Error");
    return Value(result_instance);
}

bool FFILibrary::isPathAllowed(const std::string& path) {
    if (allowed_paths_.empty()) {
        // If no specific paths configured, allow working directory and standard paths
        return true;
    }
    
    std::filesystem::path abs_path = std::filesystem::absolute(path);
    for (const auto& allowed : allowed_paths_) {
        std::filesystem::path allowed_abs = std::filesystem::absolute(allowed);
        if (abs_path.string().starts_with(allowed_abs.string())) {
            return true;
        }
    }
    return false;
}

bool FFILibrary::isSymbolAllowed(const std::string& symbol) {
    for (const auto& denied : denied_symbols_) {
        if (symbol == denied) {
            return false;
        }
    }
    return true;
}

void FFILibrary::initializeEngine() {
    if (!engine_) {
        engine_ = std::make_unique<ffi::FFIEngine>();
    }
}

ffi::Signature FFILibrary::parseSignature(const std::string& signature_str) {
    // Format: "arg1,arg2,arg3->ret" or "->ret" for no args
    // Examples: "i32,i32->i32", "ptr->void", "->f64"
    
    size_t arrow_pos = signature_str.find("->");
    if (arrow_pos == std::string::npos) {
        // Invalid signature format
        return ffi::Signature({}, ffi::CType::Void);
    }
    
    std::string args_part = signature_str.substr(0, arrow_pos);
    std::string ret_part = signature_str.substr(arrow_pos + 2);
    
    // Parse return type
    ffi::CType ret_type;
    try {
        ret_type = ffi::stringToCType(ret_part);
    } catch (const std::exception&) {
        return ffi::Signature({}, ffi::CType::Void);
    }
    
    // Parse argument types
    std::vector<ffi::CType> arg_types;
    if (!args_part.empty()) {
        std::string current_arg;
        for (char c : args_part) {
            if (c == ',') {
                if (!current_arg.empty()) {
                    try {
                        arg_types.push_back(ffi::stringToCType(current_arg));
                        current_arg.clear();
                    } catch (const std::exception&) {
                        return ffi::Signature({}, ffi::CType::Void);
                    }
                }
            } else if (c != ' ') {  // Skip spaces
                current_arg += c;
            }
        }
        
        // Handle last argument
        if (!current_arg.empty()) {
            try {
                arg_types.push_back(ffi::stringToCType(current_arg));
            } catch (const std::exception&) {
                return ffi::Signature({}, ffi::CType::Void);
            }
        }
    }
    
    return ffi::Signature(arg_types, ret_type);
}

Value FFILibrary::ffi_struct(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // ffi.struct(size) - creates a struct with the specified byte size
    if (args.size() != 1 || !std::holds_alternative<Int>(args[0])) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected Int size argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    Int size = std::get<Int>(args[0]);
    if (size <= 0) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Struct size must be positive");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    auto struct_instance = std::make_shared<ffi::CStructInstance>(static_cast<size_t>(size));
    auto result_instance = std::make_shared<ResultInstance>(Value(struct_instance), "Value", "Error");
    return Value(result_instance);
}

Value FFILibrary::ffi_array(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // ffi.array(type, count) - creates an array of specified type and count
    if (args.size() != 2 || !std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Int>(args[1])) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected Text type and Int count arguments");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    std::string type_str = std::get<Text>(args[0]);
    Int count = std::get<Int>(args[1]);
    
    if (count <= 0) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Array count must be positive");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    try {
        ffi::CType element_type = ffi::stringToCType(type_str);
        auto array_instance = std::make_shared<ffi::CArrayInstance>(element_type, static_cast<size_t>(count));
        auto result_instance = std::make_shared<ResultInstance>(Value(array_instance), "Value", "Error");
        return Value(result_instance);
    } catch (const std::exception& e) {
        auto error = std::make_shared<ErrorInstance>("INVALID_TYPE", "Invalid array element type: " + type_str);
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
}

Value FFILibrary::ffi_callback(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // ffi.callback(function, signature) - creates a callback wrapper
    if (args.size() != 2 || !std::holds_alternative<Text>(args[1])) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected function and Text signature arguments");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    Value o2l_function = args[0];
    std::string signature_str = std::get<Text>(args[1]);
    
    try {
        ffi::Signature signature = parseSignature(signature_str);
        auto callback_instance = std::make_shared<ffi::CCallbackInstance>(o2l_function, signature);
        auto result_instance = std::make_shared<ResultInstance>(Value(callback_instance), "Value", "Error");
        return Value(result_instance);
    } catch (const std::exception& e) {
        auto error = std::make_shared<ErrorInstance>("INVALID_SIGNATURE", "Invalid callback signature: " + signature_str);
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
}

Value FFILibrary::ffi_cstring(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // ffi.cstring(text) - creates a C string with proper UTF-8 handling
    if (args.size() != 1 || !std::holds_alternative<Text>(args[0])) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected Text argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    std::string text = std::get<Text>(args[0]);
    
    // Create a CBuffer containing the UTF-8 string + null terminator
    auto buffer_instance = std::make_shared<ffi::CBufferInstance>(text.length() + 1);
    std::memcpy(buffer_instance->mutable_data(), text.c_str(), text.length() + 1);
    
    auto result_instance = std::make_shared<ResultInstance>(Value(buffer_instance), "Value", "Error");
    return Value(result_instance);
}

Value FFILibrary::ffi_ptrToString(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // ffi.ptrToString(ptr) - converts a C string pointer to O²L Text
    if (args.size() != 1) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected exactly one argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    // Handle different pointer types
    if (auto ptr_inst = std::get_if<std::shared_ptr<ffi::PtrInstance>>(&args[0])) {
        void* ptr = (*ptr_inst)->get();
        if (!ptr) {
            // Null pointer returns empty string
            auto result_instance = std::make_shared<ResultInstance>(Value(Text("")), "Value", "Error");
            return Value(result_instance);
        }
        
        // Cast to char* and create O²L Text
        const char* cstr = static_cast<const char*>(ptr);
        std::string text(cstr);
        
        auto result_instance = std::make_shared<ResultInstance>(Value(Text(text)), "Value", "Error");
        return Value(result_instance);
    }
    
    // Also handle direct Value that might contain a pointer representation
    if (auto obj = std::get_if<std::shared_ptr<ObjectInstance>>(&args[0])) {
        // This might be another type of pointer representation
        auto error = std::make_shared<ErrorInstance>("UNSUPPORTED_TYPE", "Unsupported pointer type for string conversion");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    auto error = std::make_shared<ErrorInstance>("TYPE_MISMATCH", "Expected pointer argument");
    return Value(ResultInstance::createError(Value(error), "Value", "Error"));
}

Value FFILibrary::ffi_ptrToInt(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (args.size() != 1) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected exactly one argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (auto ptr_inst = std::get_if<std::shared_ptr<ffi::PtrInstance>>(&args[0])) {
        void* ptr = (*ptr_inst)->get();
        if (!ptr) {
            auto error = std::make_shared<ErrorInstance>("NULL_POINTER", "Cannot dereference null pointer");
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        // Cast to int* and dereference
        const int32_t* int_ptr = static_cast<const int32_t*>(ptr);
        Int value = static_cast<Int>(*int_ptr);
        
        auto result_instance = std::make_shared<ResultInstance>(Value(value), "Value", "Error");
        return Value(result_instance);
    }
    
    auto error = std::make_shared<ErrorInstance>("TYPE_MISMATCH", "Expected pointer argument");
    return Value(ResultInstance::createError(Value(error), "Value", "Error"));
}

Value FFILibrary::ffi_ptrToDouble(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (args.size() != 1) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected exactly one argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (auto ptr_inst = std::get_if<std::shared_ptr<ffi::PtrInstance>>(&args[0])) {
        void* ptr = (*ptr_inst)->get();
        if (!ptr) {
            auto error = std::make_shared<ErrorInstance>("NULL_POINTER", "Cannot dereference null pointer");
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        // Cast to double* and dereference
        const double* double_ptr = static_cast<const double*>(ptr);
        Double value = *double_ptr;
        
        auto result_instance = std::make_shared<ResultInstance>(Value(value), "Value", "Error");
        return Value(result_instance);
    }
    
    auto error = std::make_shared<ErrorInstance>("TYPE_MISMATCH", "Expected pointer argument");
    return Value(ResultInstance::createError(Value(error), "Value", "Error"));
}

Value FFILibrary::ffi_ptrToFloat(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (args.size() != 1) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected exactly one argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (auto ptr_inst = std::get_if<std::shared_ptr<ffi::PtrInstance>>(&args[0])) {
        void* ptr = (*ptr_inst)->get();
        if (!ptr) {
            auto error = std::make_shared<ErrorInstance>("NULL_POINTER", "Cannot dereference null pointer");
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        // Cast to float* and dereference
        const float* float_ptr = static_cast<const float*>(ptr);
        Float value = static_cast<Float>(*float_ptr);
        
        auto result_instance = std::make_shared<ResultInstance>(Value(value), "Value", "Error");
        return Value(result_instance);
    }
    
    auto error = std::make_shared<ErrorInstance>("TYPE_MISMATCH", "Expected pointer argument");
    return Value(ResultInstance::createError(Value(error), "Value", "Error"));
}

Value FFILibrary::ffi_ptrToBool(const std::vector<Value>& args, Context& context) {
    if (!ffi_enabled_) {
        auto error = std::make_shared<ErrorInstance>("FFI_DISABLED", "FFI is disabled");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (args.size() != 1) {
        auto error = std::make_shared<ErrorInstance>("INVALID_ARGUMENT", "Expected exactly one argument");
        return Value(ResultInstance::createError(Value(error), "Value", "Error"));
    }
    
    if (auto ptr_inst = std::get_if<std::shared_ptr<ffi::PtrInstance>>(&args[0])) {
        void* ptr = (*ptr_inst)->get();
        if (!ptr) {
            auto error = std::make_shared<ErrorInstance>("NULL_POINTER", "Cannot dereference null pointer");
            return Value(ResultInstance::createError(Value(error), "Value", "Error"));
        }
        
        // Cast to bool* and dereference
        const uint8_t* bool_ptr = static_cast<const uint8_t*>(ptr);
        Bool value = (*bool_ptr != 0);
        
        auto result_instance = std::make_shared<ResultInstance>(Value(value), "Value", "Error");
        return Value(result_instance);
    }
    
    auto error = std::make_shared<ErrorInstance>("TYPE_MISMATCH", "Expected pointer argument");
    return Value(ResultInstance::createError(Value(error), "Value", "Error"));
}

}  // namespace o2l
