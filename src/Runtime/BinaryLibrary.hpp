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
#include <vector>
#include <cstdint>
#include "Value.hpp"
#include "ObjectInstance.hpp"

namespace o2l {

class BinaryLibrary {
public:
    static std::shared_ptr<ObjectInstance> createBinaryObject();
    
    // ByteBuffer implementation
    static std::shared_ptr<ObjectInstance> createByteBuffer(size_t initial_size = 0);
};

class ByteBufferInstance : public ObjectInstance {
private:
    std::vector<uint8_t> data_;
public:
    explicit ByteBufferInstance(size_t initial_size);
    
    uint8_t* getRawData() { return data_.data(); }
    
    // Native methods
    static Value nativeWriteU8(const std::vector<Value>& args, Context& context);
    static Value nativeWriteU32LE(const std::vector<Value>& args, Context& context);
    static Value nativeWriteU64LE(const std::vector<Value>& args, Context& context);
    static Value nativeToBytes(const std::vector<Value>& args, Context& context);
    static Value nativeSave(const std::vector<Value>& args, Context& context);
    static Value nativeSize(const std::vector<Value>& args, Context& context);
};

} // namespace o2l
