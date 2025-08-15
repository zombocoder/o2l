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

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../Value.hpp"

namespace o2l::ffi {

// C type enumeration for FFI
enum class CType {
    Int32,
    Int64, 
    Float32,
    Float64,
    Bool,
    Text,
    Ptr,
    Void
};

// Function signature for FFI calls
struct Signature {
    std::vector<CType> args;
    CType ret;
    
    Signature(std::vector<CType> args, CType ret) : args(std::move(args)), ret(ret) {}
};

// Forward declarations for new FFI types
class PtrInstance;
class CBufferInstance;

// Opaque pointer wrapper
class PtrInstance {
private:
    void* ptr_;
    bool is_null_;

public:
    explicit PtrInstance(void* ptr = nullptr) : ptr_(ptr), is_null_(ptr == nullptr) {}
    
    void* get() const { return ptr_; }
    bool isNull() const { return is_null_; }
    
    std::string toString() const {
        if (is_null_) {
            return "Ptr<Void>(null)";
        }
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Ptr<Void>(%p)", ptr_);
        return buffer;
    }
};

// Immutable buffer wrapper for native memory
class CBufferInstance {
private:
    std::shared_ptr<uint8_t[]> data_;
    size_t size_;

public:
    explicit CBufferInstance(size_t size) 
        : data_(new uint8_t[size](), [](uint8_t* p) { delete[] p; })
        , size_(size) {}
    
    CBufferInstance(void* data, size_t size) 
        : data_(new uint8_t[size], [](uint8_t* p) { delete[] p; })
        , size_(size) {
        if (data) {
            std::memcpy(data_.get(), data, size);
        }
    }
    
    const uint8_t* data() const { return data_.get(); }
    uint8_t* mutable_data() { return data_.get(); }
    size_t size() const { return size_; }
    
    std::string toString() const {
        return "CBuffer(" + std::to_string(size_) + " bytes)";
    }
    
    // Create a new buffer with modified data (copy-on-write semantics)
    std::shared_ptr<CBufferInstance> write(size_t offset, const void* data, size_t len) const {
        if (offset + len > size_) {
            return nullptr;
        }
        
        auto new_buffer = std::make_shared<CBufferInstance>(size_);
        std::memcpy(new_buffer->mutable_data(), data_.get(), size_);
        std::memcpy(new_buffer->mutable_data() + offset, data, len);
        return new_buffer;
    }
};

// Convert string to CType
CType stringToCType(const std::string& type_str);

// Convert CType to string
std::string ctypeToString(CType type);

}  // namespace o2l::ffi