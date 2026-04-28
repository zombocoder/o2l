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

#include "BinaryLibrary.hpp"
#include "Context.hpp"
#include "ListInstance.hpp"
#include "../Common/Exceptions.hpp"
#include <fstream>

namespace o2l {

std::shared_ptr<ObjectInstance> BinaryLibrary::createBinaryObject() {
    auto binary_object = std::make_shared<ObjectInstance>("binary");
    
    binary_object->addMethod("newBuffer", [](const std::vector<Value>& args, Context& ctx) -> Value {
        size_t size = 0;
        if (!args.empty() && std::holds_alternative<Int>(args[0])) {
            size = static_cast<size_t>(std::get<Int>(args[0]));
        }
        return Value(std::make_shared<ByteBufferInstance>(size));
    }, true);

    binary_object->addMethod("load", [](const std::vector<Value>& args, Context& ctx) -> Value {
        if (args.empty() || !std::holds_alternative<Text>(args[0])) {
            throw EvaluationError("binary.load() requires a Text argument (filename)");
        }
        std::string filename = std::get<Text>(args[0]);
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) return Value();
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        auto buffer = std::make_shared<ByteBufferInstance>(static_cast<size_t>(size));
        if (file.read(reinterpret_cast<char*>(buffer->getRawData()), size)) {
            return Value(buffer);
        }
        return Value();
    }, true);
    
    return binary_object;
}

ByteBufferInstance::ByteBufferInstance(size_t initial_size) 
    : ObjectInstance("ByteBuffer"), data_(initial_size) {
    
    addMethod("writeU8", nativeWriteU8, true);
    addMethod("writeU32LE", nativeWriteU32LE, true);
    addMethod("writeU64LE", nativeWriteU64LE, true);
    addMethod("toBytes", nativeToBytes, true);
    addMethod("save", nativeSave, true);
    addMethod("size", nativeSize, true);
}

Value ByteBufferInstance::nativeWriteU8(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Int>(args[0])) {
        throw EvaluationError("writeU8() requires an Int argument");
    }
    
    auto self = std::dynamic_pointer_cast<ByteBufferInstance>(context.getThisObject());
    if (self) {
        self->data_.push_back(static_cast<uint8_t>(std::get<Int>(args[0])));
    }
    return Value();
}

Value ByteBufferInstance::nativeWriteU32LE(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Int>(args[0])) {
        throw EvaluationError("writeU32LE() requires an Int argument");
    }
    
    auto self = std::dynamic_pointer_cast<ByteBufferInstance>(context.getThisObject());
    if (self) {
        uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
        self->data_.push_back(val & 0xFF);
        self->data_.push_back((val >> 8) & 0xFF);
        self->data_.push_back((val >> 16) & 0xFF);
        self->data_.push_back((val >> 24) & 0xFF);
    }
    return Value();
}

Value ByteBufferInstance::nativeWriteU64LE(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Long>(args[0])) {
        throw EvaluationError("writeU64LE() requires a Long argument");
    }
    
    auto self = std::dynamic_pointer_cast<ByteBufferInstance>(context.getThisObject());
    if (self) {
        uint64_t val = static_cast<uint64_t>(std::get<Long>(args[0]));
        for (int i = 0; i < 8; ++i) {
            self->data_.push_back((val >> (i * 8)) & 0xFF);
        }
    }
    return Value();
}

Value ByteBufferInstance::nativeToBytes(const std::vector<Value>& args, Context& context) {
    auto self = std::dynamic_pointer_cast<ByteBufferInstance>(context.getThisObject());
    auto list = std::make_shared<ListInstance>("Int");
    if (self) {
        for (auto b : self->data_) list->add(Int(static_cast<int>(b)));
    }
    return Value(list);
}

Value ByteBufferInstance::nativeSave(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("save() requires a Text argument (filename)");
    }
    
    auto self = std::dynamic_pointer_cast<ByteBufferInstance>(context.getThisObject());
    if (self) {
        std::ofstream file(std::get<Text>(args[0]), std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(self->data_.data()), self->data_.size());
            file.close();
            return Bool(true);
        }
    }
    return Bool(false);
}

Value ByteBufferInstance::nativeSize(const std::vector<Value>& args, Context& context) {
    auto self = std::dynamic_pointer_cast<ByteBufferInstance>(context.getThisObject());
    if (self) return Int(static_cast<int>(self->data_.size()));
    return Int(0);
}

} // namespace o2l
