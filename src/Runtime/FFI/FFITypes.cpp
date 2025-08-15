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
    }
    return "unknown";
}

}  // namespace o2l::ffi