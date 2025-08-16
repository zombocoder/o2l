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
#include <cstring>
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
    Void,
    // Enhanced types
    Struct,
    Array,
    Callback,
    CString  // UTF-8 aware C string
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
class CStructInstance;
class CArrayInstance;
class CCallbackInstance;

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

// Enhanced C struct wrapper for native data layouts
class CStructInstance {
private:
    std::shared_ptr<uint8_t[]> data_;
    size_t size_;
    std::vector<std::pair<std::string, std::pair<CType, size_t>>> fields_; // name -> (type, offset)
    
public:
    explicit CStructInstance(size_t size) 
        : data_(new uint8_t[size](), [](uint8_t* p) { delete[] p; })
        , size_(size) {}
    
    // Add a field to the struct layout
    void addField(const std::string& name, CType type, size_t offset) {
        fields_.emplace_back(name, std::make_pair(type, offset));
    }
    
    // Get field value by name
    Value getField(const std::string& name) const;
    
    // Set field value by name
    bool setField(const std::string& name, const Value& value);
    
    const uint8_t* data() const { return data_.get(); }
    uint8_t* mutable_data() { return data_.get(); }
    size_t size() const { return size_; }
    
    const std::vector<std::pair<std::string, std::pair<CType, size_t>>>& fields() const { 
        return fields_; 
    }
    
    std::string toString() const {
        return "CStruct(" + std::to_string(size_) + " bytes, " + 
               std::to_string(fields_.size()) + " fields)";
    }
};

// Enhanced array wrapper for uint8[], char* arrays
class CArrayInstance {
private:
    std::shared_ptr<uint8_t[]> data_;
    size_t element_count_;
    size_t element_size_;
    CType element_type_;
    
public:
    CArrayInstance(CType element_type, size_t count) 
        : element_count_(count), element_type_(element_type) {
        element_size_ = getElementSize(element_type);
        size_t total_size = element_size_ * count;
        data_ = std::shared_ptr<uint8_t[]>(new uint8_t[total_size](), 
                                          [](uint8_t* p) { delete[] p; });
    }
    
    // Copy from existing data
    CArrayInstance(CType element_type, size_t count, const void* source_data)
        : CArrayInstance(element_type, count) {
        if (source_data) {
            std::memcpy(data_.get(), source_data, element_size_ * count);
        }
    }
    
    // Array access
    Value getElement(size_t index) const;
    bool setElement(size_t index, const Value& value);
    
    // Bulk operations
    std::vector<Value> toList() const;
    bool fromList(const std::vector<Value>& values);
    
    const uint8_t* data() const { return data_.get(); }
    uint8_t* mutable_data() { return data_.get(); }
    size_t element_count() const { return element_count_; }
    size_t element_size() const { return element_size_; }
    CType element_type() const { return element_type_; }
    size_t total_size() const { return element_size_ * element_count_; }
    
    std::string toString() const;
    
private:
    static size_t getElementSize(CType type);
};

// Function callback wrapper to map O²L functions to C function pointers
class CCallbackInstance {
private:
    void* function_ptr_;
    Signature signature_;
    Value o2l_function_; // O²L function/method to call
    bool is_valid_;
    
public:
    CCallbackInstance(const Value& o2l_func, const Signature& sig);
    ~CCallbackInstance();
    
    void* getFunctionPtr() const { return function_ptr_; }
    const Signature& getSignature() const { return signature_; }
    bool isValid() const { return is_valid_; }
    
    // This will be called by the generated trampoline
    Value invoke(const std::vector<Value>& args);
    
    std::string toString() const {
        return std::string("CCallback(") + (is_valid_ ? "valid" : "invalid") + ")";
    }
    
    // Static callback registry for managing trampolines
    static void* createTrampoline(CCallbackInstance* instance);
    static void destroyTrampoline(void* trampoline);
};

// Convert string to CType
CType stringToCType(const std::string& type_str);

// Convert CType to string
std::string ctypeToString(CType type);

}  // namespace o2l::ffi