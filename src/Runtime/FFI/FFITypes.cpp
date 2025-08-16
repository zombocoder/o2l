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

#include "FFITypes.hpp"

namespace o2l::ffi {

CType stringToCType(const std::string& type_str) {
    if (type_str == "i32") return CType::Int32;
    if (type_str == "i64") return CType::Int64;
    if (type_str == "f32") return CType::Float32;
    if (type_str == "f64") return CType::Float64;
    if (type_str == "bool") return CType::Bool;
    if (type_str == "text") return CType::Text;
    if (type_str == "ptr") return CType::Ptr;
    if (type_str == "void") return CType::Void;
    // Enhanced types
    if (type_str == "struct") return CType::Struct;
    if (type_str == "array") return CType::Array;
    if (type_str == "callback") return CType::Callback;
    if (type_str == "cstring") return CType::CString;
    
    throw std::invalid_argument("Unknown C type: " + type_str);
}

std::string ctypeToString(CType type) {
    switch (type) {
        case CType::Int32: return "i32";
        case CType::Int64: return "i64";
        case CType::Float32: return "f32";
        case CType::Float64: return "f64";
        case CType::Bool: return "bool";
        case CType::Text: return "text";
        case CType::Ptr: return "ptr";
        case CType::Void: return "void";
        // Enhanced types
        case CType::Struct: return "struct";
        case CType::Array: return "array";
        case CType::Callback: return "callback";
        case CType::CString: return "cstring";
    }
    return "unknown";
}

// CStructInstance implementation
Value CStructInstance::getField(const std::string& name) const {
    for (const auto& field : fields_) {
        if (field.first == name) {
            CType type = field.second.first;
            size_t offset = field.second.second;
            const uint8_t* field_ptr = data_.get() + offset;
            
            switch (type) {
                case CType::Int32:
                    return Value(Int(*reinterpret_cast<const int32_t*>(field_ptr)));
                case CType::Int64:
                    return Value(Int(*reinterpret_cast<const int64_t*>(field_ptr)));
                case CType::Float32:
                    return Value(Float(*reinterpret_cast<const float*>(field_ptr)));
                case CType::Float64:
                    return Value(Double(*reinterpret_cast<const double*>(field_ptr)));
                case CType::Bool:
                    return Value(Bool(*reinterpret_cast<const uint8_t*>(field_ptr) != 0));
                case CType::Ptr: {
                    void* ptr_val = *reinterpret_cast<void* const*>(field_ptr);
                    return Value(std::make_shared<PtrInstance>(ptr_val));
                }
                default:
                    throw std::runtime_error("Unsupported field type for struct access");
            }
        }
    }
    throw std::runtime_error("Field '" + name + "' not found in struct");
}

bool CStructInstance::setField(const std::string& name, const Value& value) {
    for (const auto& field : fields_) {
        if (field.first == name) {
            CType type = field.second.first;
            size_t offset = field.second.second;
            uint8_t* field_ptr = data_.get() + offset;
            
            switch (type) {
                case CType::Int32:
                    if (std::holds_alternative<Int>(value)) {
                        *reinterpret_cast<int32_t*>(field_ptr) = static_cast<int32_t>(std::get<Int>(value));
                        return true;
                    }
                    break;
                case CType::Int64:
                    if (std::holds_alternative<Int>(value)) {
                        *reinterpret_cast<int64_t*>(field_ptr) = std::get<Int>(value);
                        return true;
                    }
                    break;
                case CType::Float32:
                    if (std::holds_alternative<Float>(value)) {
                        *reinterpret_cast<float*>(field_ptr) = std::get<Float>(value);
                        return true;
                    }
                    break;
                case CType::Float64:
                    if (std::holds_alternative<Double>(value)) {
                        *reinterpret_cast<double*>(field_ptr) = std::get<Double>(value);
                        return true;
                    }
                    break;
                case CType::Bool:
                    if (std::holds_alternative<Bool>(value)) {
                        *reinterpret_cast<uint8_t*>(field_ptr) = std::get<Bool>(value) ? 1 : 0;
                        return true;
                    }
                    break;
                case CType::Ptr:
                    if (auto ptr_inst = std::get_if<std::shared_ptr<PtrInstance>>(&value)) {
                        *reinterpret_cast<void**>(field_ptr) = (*ptr_inst)->get();
                        return true;
                    }
                    break;
                default:
                    return false;
            }
            return false;
        }
    }
    return false;
}

// CArrayInstance implementation
size_t CArrayInstance::getElementSize(CType type) {
    switch (type) {
        case CType::Int32: return sizeof(int32_t);
        case CType::Int64: return sizeof(int64_t);
        case CType::Float32: return sizeof(float);
        case CType::Float64: return sizeof(double);
        case CType::Bool: return sizeof(uint8_t);
        case CType::Ptr: return sizeof(void*);
        default: return 1; // uint8 for unknown types
    }
}

Value CArrayInstance::getElement(size_t index) const {
    if (index >= element_count_) {
        throw std::out_of_range("Array index out of bounds");
    }
    
    const uint8_t* elem_ptr = data_.get() + (index * element_size_);
    
    switch (element_type_) {
        case CType::Int32:
            return Value(Int(*reinterpret_cast<const int32_t*>(elem_ptr)));
        case CType::Int64:
            return Value(Int(*reinterpret_cast<const int64_t*>(elem_ptr)));
        case CType::Float32:
            return Value(Float(*reinterpret_cast<const float*>(elem_ptr)));
        case CType::Float64:
            return Value(Double(*reinterpret_cast<const double*>(elem_ptr)));
        case CType::Bool:
            return Value(Bool(*elem_ptr != 0));
        case CType::Ptr: {
            void* ptr_val = *reinterpret_cast<void* const*>(elem_ptr);
            return Value(std::make_shared<PtrInstance>(ptr_val));
        }
        default:
            return Value(Int(*elem_ptr)); // Return as uint8
    }
}

bool CArrayInstance::setElement(size_t index, const Value& value) {
    if (index >= element_count_) {
        return false;
    }
    
    uint8_t* elem_ptr = data_.get() + (index * element_size_);
    
    switch (element_type_) {
        case CType::Int32:
            if (std::holds_alternative<Int>(value)) {
                *reinterpret_cast<int32_t*>(elem_ptr) = static_cast<int32_t>(std::get<Int>(value));
                return true;
            }
            break;
        case CType::Int64:
            if (std::holds_alternative<Int>(value)) {
                *reinterpret_cast<int64_t*>(elem_ptr) = std::get<Int>(value);
                return true;
            }
            break;
        case CType::Float32:
            if (std::holds_alternative<Float>(value)) {
                *reinterpret_cast<float*>(elem_ptr) = std::get<Float>(value);
                return true;
            }
            break;
        case CType::Float64:
            if (std::holds_alternative<Double>(value)) {
                *reinterpret_cast<double*>(elem_ptr) = std::get<Double>(value);
                return true;
            }
            break;
        case CType::Bool:
            if (std::holds_alternative<Bool>(value)) {
                *elem_ptr = std::get<Bool>(value) ? 1 : 0;
                return true;
            }
            break;
        case CType::Ptr:
            if (auto ptr_inst = std::get_if<std::shared_ptr<PtrInstance>>(&value)) {
                *reinterpret_cast<void**>(elem_ptr) = (*ptr_inst)->get();
                return true;
            }
            break;
        default:
            if (std::holds_alternative<Int>(value)) {
                Int val = std::get<Int>(value);
                if (val >= 0 && val <= 255) {
                    *elem_ptr = static_cast<uint8_t>(val);
                    return true;
                }
            }
            break;
    }
    return false;
}

std::vector<Value> CArrayInstance::toList() const {
    std::vector<Value> result;
    result.reserve(element_count_);
    
    for (size_t i = 0; i < element_count_; ++i) {
        result.push_back(getElement(i));
    }
    
    return result;
}

bool CArrayInstance::fromList(const std::vector<Value>& values) {
    if (values.size() != element_count_) {
        return false;
    }
    
    for (size_t i = 0; i < values.size(); ++i) {
        if (!setElement(i, values[i])) {
            return false;
        }
    }
    
    return true;
}

std::string CArrayInstance::toString() const {
    return "CArray<" + ctypeToString(element_type_) + ">[" + 
           std::to_string(element_count_) + "]";
}

// CCallbackInstance implementation (basic stub for now)
CCallbackInstance::CCallbackInstance(const Value& o2l_func, const Signature& sig)
    : signature_(sig), o2l_function_(o2l_func), is_valid_(false), function_ptr_(nullptr) {
    // TODO: Implement callback trampoline generation
    // For now, just mark as invalid
    is_valid_ = false;
}

CCallbackInstance::~CCallbackInstance() {
    if (function_ptr_) {
        destroyTrampoline(function_ptr_);
    }
}

Value CCallbackInstance::invoke(const std::vector<Value>& args) {
    // TODO: Implement callback invocation
    // This would call the O²L function with the given arguments
    throw std::runtime_error("Callback invocation not yet implemented");
}

void* CCallbackInstance::createTrampoline(CCallbackInstance* instance) {
    // TODO: Implement platform-specific trampoline generation
    // This would create executable code that bridges C calls to O²L function calls
    return nullptr;
}

void CCallbackInstance::destroyTrampoline(void* trampoline) {
    // TODO: Implement trampoline cleanup
    // This would free the generated executable code
}

}  // namespace o2l::ffi