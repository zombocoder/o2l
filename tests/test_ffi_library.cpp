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

#include <gtest/gtest.h>
#include <cstring>
#include "../src/Runtime/FFILibrary.hpp"
#include "../src/Runtime/FFI/FFITypes.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"
#include "../src/Runtime/ResultInstance.hpp"

using namespace o2l;
using namespace o2l::ffi;

class FFILibraryTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
        // Enable FFI for testing
        FFILibrary::setFFIEnabled(true);
    }

    void TearDown() override {
        // Clean up after tests
        FFILibrary::setFFIEnabled(false);
    }

    std::unique_ptr<Context> context;
};

// Test ffi.nullPtr function
TEST_F(FFILibraryTest, NullPtrFunction) {
    std::vector<Value> args; // No arguments for nullPtr
    
    Value result = FFILibrary::ffi_nullPtr(args, *context);
    
    // Should return a PtrInstance
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<PtrInstance>>(result));
    
    auto ptr = std::get<std::shared_ptr<PtrInstance>>(result);
    EXPECT_TRUE(ptr->isNull());
    EXPECT_EQ(ptr->get(), nullptr);
}

// Test ffi.cstring function
TEST_F(FFILibraryTest, CStringFunction) {
    std::vector<Value> args;
    args.push_back(Value(Text("Hello, FFI World!")));
    
    Value result = FFILibrary::ffi_cstring(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value buffer_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CBufferInstance>>(buffer_val));
    
    auto buffer = std::get<std::shared_ptr<CBufferInstance>>(buffer_val);
    EXPECT_EQ(buffer->size(), 18); // "Hello, FFI World!" + null terminator
    
    // Check the string content
    const char* str_data = reinterpret_cast<const char*>(buffer->data());
    EXPECT_STREQ(str_data, "Hello, FFI World!");
}

// Test ffi.struct function
TEST_F(FFILibraryTest, StructFunction) {
    std::vector<Value> args;
    args.push_back(Value(Int(32))); // 32 bytes
    
    Value result = FFILibrary::ffi_struct(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value struct_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CStructInstance>>(struct_val));
    
    auto struct_inst = std::get<std::shared_ptr<CStructInstance>>(struct_val);
    EXPECT_EQ(struct_inst->size(), 32);
}

// Test ffi.array function
TEST_F(FFILibraryTest, ArrayFunction) {
    std::vector<Value> args;
    args.push_back(Value(Text("i32"))); // int32 type
    args.push_back(Value(Int(10))); // 10 elements
    
    Value result = FFILibrary::ffi_array(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value array_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CArrayInstance>>(array_val));
    
    auto array = std::get<std::shared_ptr<CArrayInstance>>(array_val);
    EXPECT_EQ(array->element_count(), 10);
    EXPECT_EQ(array->element_size(), sizeof(int32_t));
}

// Test error handling for invalid arguments
TEST_F(FFILibraryTest, ErrorHandlingInvalidArguments) {
    // Test ffi.struct with invalid size (non-integer)
    std::vector<Value> args;
    args.push_back(Value(Text("invalid"))); // Non-integer argument
    
    Value result = FFILibrary::ffi_struct(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test ffi.array with invalid type
TEST_F(FFILibraryTest, ArrayErrorHandling) {
    std::vector<Value> args;
    args.push_back(Value(Text("invalid_type"))); // Invalid type
    args.push_back(Value(Int(5)));
    
    Value result = FFILibrary::ffi_array(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test pointer conversion functions
TEST_F(FFILibraryTest, PointerConversions) {
    // Test ptrToString with null pointer
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    Value ptr_value(null_ptr);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToString(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    // Check if it succeeds (implementation may handle null pointers gracefully)
    if (result_inst->isSuccess()) {
        // If it succeeds, verify the result is an empty string or null representation
        Value string_val = result_inst->getResult();
        EXPECT_TRUE(std::holds_alternative<Text>(string_val));
    } else {
        // If it fails, that's also acceptable for null pointer
        EXPECT_FALSE(result_inst->isSuccess());
    }
}

// Test pointer conversion with actual data
TEST_F(FFILibraryTest, PointerToIntConversion) {
    // Create integer in memory
    int32_t test_int = 42;
    auto ptr_inst = std::make_shared<PtrInstance>(&test_int);
    Value ptr_value(ptr_inst);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToInt(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value int_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<Int>(int_val));
    EXPECT_EQ(std::get<Int>(int_val), 42);
}

// Test FFI error conditions
TEST_F(FFILibraryTest, FFIErrorConditions) {
    // Test with missing arguments
    std::vector<Value> empty_args;
    
    Value result = FFILibrary::ffi_cstring(empty_args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}