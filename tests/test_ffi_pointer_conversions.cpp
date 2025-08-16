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
#include "../src/Runtime/FFILibrary.hpp"
#include "../src/Runtime/FFI/FFITypes.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;
using namespace o2l::ffi;

class FFIPointerConversionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
    }

    std::unique_ptr<Context> context;
};

// Test ffi.ptrToString function
TEST_F(FFIPointerConversionsTest, PtrToStringConversion) {
    // Create a test string in memory
    const char* test_string = "Hello, FFI World!";
    
    // Create a PtrInstance pointing to the string
    auto ptr_inst = std::make_shared<PtrInstance>(const_cast<char*>(test_string));
    
    // Create Value containing the PtrInstance
    Value ptr_value(ptr_inst);
    
    // Call ffi.ptrToString
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToString(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    // Get the converted string
    Value string_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<Text>(string_val));
    
    Text converted_string = std::get<Text>(string_val);
    EXPECT_EQ(converted_string, "Hello, FFI World!");
}

// Test ffi.ptrToString with null pointer
TEST_F(FFIPointerConversionsTest, PtrToStringNullPointer) {
    // Create a null PtrInstance
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    Value ptr_value(null_ptr);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToString(args, *context);
    
    // Should return an error Result for null pointer
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test ffi.ptrToInt function
TEST_F(FFIPointerConversionsTest, PtrToIntConversion) {
    // Create a test integer in memory
    int32_t test_int = 42;
    
    // Create a PtrInstance pointing to the integer
    auto ptr_inst = std::make_shared<PtrInstance>(&test_int);
    Value ptr_value(ptr_inst);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToInt(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    // Get the converted integer
    Value int_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<Int>(int_val));
    
    Int converted_int = std::get<Int>(int_val);
    EXPECT_EQ(converted_int, 42);
}

// Test ffi.ptrToDouble function
TEST_F(FFIPointerConversionsTest, PtrToDoubleConversion) {
    // Create a test double in memory
    double test_double = 3.14159;
    
    // Create a PtrInstance pointing to the double
    auto ptr_inst = std::make_shared<PtrInstance>(&test_double);
    Value ptr_value(ptr_inst);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToDouble(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    // Get the converted double
    Value double_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<Double>(double_val));
    
    Double converted_double = std::get<Double>(double_val);
    EXPECT_DOUBLE_EQ(converted_double, 3.14159);
}

// Test ffi.ptrToFloat function
TEST_F(FFIPointerConversionsTest, PtrToFloatConversion) {
    // Create a test float in memory
    float test_float = 2.71828f;
    
    // Create a PtrInstance pointing to the float
    auto ptr_inst = std::make_shared<PtrInstance>(&test_float);
    Value ptr_value(ptr_inst);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToFloat(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    // Get the converted float
    Value float_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<Float>(float_val));
    
    Float converted_float = std::get<Float>(float_val);
    EXPECT_FLOAT_EQ(converted_float, 2.71828f);
}

// Test ffi.ptrToBool function
TEST_F(FFIPointerConversionsTest, PtrToBoolConversion) {
    // Test with true value
    bool test_bool_true = true;
    auto ptr_inst_true = std::make_shared<PtrInstance>(&test_bool_true);
    Value ptr_value_true(ptr_inst_true);
    
    std::vector<Value> args_true;
    args_true.push_back(ptr_value_true);
    
    Value result_true = FFILibrary::ffi_ptrToBool(args_true, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result_true));
    auto result_inst_true = std::get<std::shared_ptr<ResultInstance>>(result_true);
    EXPECT_TRUE(result_inst_true->isSuccess());
    
    Value bool_val_true = result_inst_true->getResult();
    EXPECT_TRUE(std::holds_alternative<Bool>(bool_val_true));
    EXPECT_TRUE(std::get<Bool>(bool_val_true));
    
    // Test with false value
    bool test_bool_false = false;
    auto ptr_inst_false = std::make_shared<PtrInstance>(&test_bool_false);
    Value ptr_value_false(ptr_inst_false);
    
    std::vector<Value> args_false;
    args_false.push_back(ptr_value_false);
    
    Value result_false = FFILibrary::ffi_ptrToBool(args_false, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result_false));
    auto result_inst_false = std::get<std::shared_ptr<ResultInstance>>(result_false);
    EXPECT_TRUE(result_inst_false->isSuccess());
    
    Value bool_val_false = result_inst_false->getResult();
    EXPECT_TRUE(std::holds_alternative<Bool>(bool_val_false));
    EXPECT_FALSE(std::get<Bool>(bool_val_false));
}

// Test error handling for invalid pointer conversions
TEST_F(FFIPointerConversionsTest, ErrorHandlingInvalidPointer) {
    // Test with null pointer to int
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    Value ptr_value(null_ptr);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToInt(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test error handling for wrong argument types
TEST_F(FFIPointerConversionsTest, ErrorHandlingWrongArgumentType) {
    // Try to convert a non-pointer value
    std::vector<Value> args;
    args.push_back(Value(Int(42))); // Pass an integer instead of a pointer
    
    Value result = FFILibrary::ffi_ptrToString(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test error handling for missing arguments
TEST_F(FFIPointerConversionsTest, ErrorHandlingMissingArguments) {
    // Call with no arguments
    std::vector<Value> args; // Empty arguments
    
    Value result = FFILibrary::ffi_ptrToString(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test pointer conversions with buffer data
TEST_F(FFIPointerConversionsTest, BufferPointerConversions) {
    // Create a buffer with test data
    auto buffer = std::make_shared<CBufferInstance>(sizeof(int32_t));
    int32_t test_value = 1337;
    std::memcpy(buffer->mutable_data(), &test_value, sizeof(int32_t));
    
    // Create pointer to buffer data
    auto ptr_inst = std::make_shared<PtrInstance>(buffer->mutable_data());
    Value ptr_value(ptr_inst);
    
    std::vector<Value> args;
    args.push_back(ptr_value);
    
    Value result = FFILibrary::ffi_ptrToInt(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value int_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<Int>(int_val));
    EXPECT_EQ(std::get<Int>(int_val), 1337);
}

// Test string conversion with various string lengths
TEST_F(FFIPointerConversionsTest, StringConversionVariousLengths) {
    // Test empty string
    const char* empty_string = "";
    auto ptr_empty = std::make_shared<PtrInstance>(const_cast<char*>(empty_string));
    
    std::vector<Value> args_empty;
    args_empty.push_back(Value(ptr_empty));
    
    Value result_empty = FFILibrary::ffi_ptrToString(args_empty, *context);
    auto result_inst_empty = std::get<std::shared_ptr<ResultInstance>>(result_empty);
    EXPECT_TRUE(result_inst_empty->isSuccess());
    
    Value string_val_empty = result_inst_empty->getResult();
    EXPECT_TRUE(std::holds_alternative<Text>(string_val_empty));
    EXPECT_EQ(std::get<Text>(string_val_empty), "");
    
    // Test long string
    const char* long_string = "This is a very long string that tests the pointer to string conversion with a substantial amount of text content to ensure proper handling.";
    auto ptr_long = std::make_shared<PtrInstance>(const_cast<char*>(long_string));
    
    std::vector<Value> args_long;
    args_long.push_back(Value(ptr_long));
    
    Value result_long = FFILibrary::ffi_ptrToString(args_long, *context);
    auto result_inst_long = std::get<std::shared_ptr<ResultInstance>>(result_long);
    EXPECT_TRUE(result_inst_long->isSuccess());
    
    Value string_val_long = result_inst_long->getResult();
    EXPECT_TRUE(std::holds_alternative<Text>(string_val_long));
    EXPECT_EQ(std::get<Text>(string_val_long), long_string);
}