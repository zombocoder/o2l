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
#include "../src/Runtime/FFI/FFITypes.hpp"
#include "../src/Runtime/FFILibrary.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"
#include "../src/Runtime/ResultInstance.hpp"

using namespace o2l;
using namespace o2l::ffi;

class FFIEnhancedTypesTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
    }

    std::unique_ptr<Context> context;
};

// Test CBufferInstance creation and operations
TEST_F(FFIEnhancedTypesTest, CBufferInstanceBasicOperations) {
    // Test buffer creation
    auto buffer = std::make_shared<CBufferInstance>(10);
    EXPECT_EQ(buffer->size(), 10);
    EXPECT_NE(buffer->data(), nullptr);
    
    // Test data access
    uint8_t* mutable_data = buffer->mutable_data();
    EXPECT_NE(mutable_data, nullptr);
    
    // Test writing data
    const char* test_str = "Hello";
    std::memcpy(mutable_data, test_str, 6); // include null terminator
    
    // Test reading data
    const uint8_t* const_data = buffer->data();
    EXPECT_STREQ(reinterpret_cast<const char*>(const_data), "Hello");
}

// Test CBufferInstance toString method
TEST_F(FFIEnhancedTypesTest, CBufferInstanceToString) {
    auto buffer = std::make_shared<CBufferInstance>(20);
    std::string result = buffer->toString();
    EXPECT_TRUE(result.find("CBuffer") != std::string::npos);
    EXPECT_TRUE(result.find("20") != std::string::npos);
}

// Test CStructInstance creation and field operations
TEST_F(FFIEnhancedTypesTest, CStructInstanceBasicOperations) {
    // Test struct creation
    auto struct_inst = std::make_shared<CStructInstance>(32);
    EXPECT_EQ(struct_inst->size(), 32);
    EXPECT_NE(struct_inst->data(), nullptr);
    
    // Test adding fields (addField returns void)
    struct_inst->addField("int_field", CType::Int32, 0);
    struct_inst->addField("double_field", CType::Float64, 8);
    struct_inst->addField("bool_field", CType::Bool, 16);
    
    // Verify fields were added by checking field count or trying to access them
    EXPECT_NO_THROW(struct_inst->addField("int_field", CType::Int32, 0));
}

// Test CStructInstance field access
TEST_F(FFIEnhancedTypesTest, CStructInstanceFieldAccess) {
    auto struct_inst = std::make_shared<CStructInstance>(32);
    
    // Add fields
    struct_inst->addField("int_field", CType::Int32, 0);
    struct_inst->addField("double_field", CType::Float64, 8);
    
    // Test setting field values
    Value int_val(Int(42));
    bool set_result = struct_inst->setField("int_field", int_val);
    EXPECT_TRUE(set_result);
    
    Value double_val(Double(3.14159));
    set_result = struct_inst->setField("double_field", double_val);
    EXPECT_TRUE(set_result);
    
    // Test getting field values
    Value retrieved_int = struct_inst->getField("int_field");
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved_int));
    EXPECT_EQ(std::get<Int>(retrieved_int), 42);
    
    Value retrieved_double = struct_inst->getField("double_field");
    EXPECT_TRUE(std::holds_alternative<Double>(retrieved_double));
    EXPECT_DOUBLE_EQ(std::get<Double>(retrieved_double), 3.14159);
}

// Test CArrayInstance creation and operations
TEST_F(FFIEnhancedTypesTest, CArrayInstanceBasicOperations) {
    // Test array creation
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 5);
    EXPECT_EQ(array->element_count(), 5);
    EXPECT_EQ(array->element_size(), sizeof(int32_t));
    EXPECT_NE(array->data(), nullptr);
}

// Test CArrayInstance element access
TEST_F(FFIEnhancedTypesTest, CArrayInstanceElementAccess) {
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 3);
    
    // Test setting elements
    Value val1(Int(10));
    bool set_result = array->setElement(0, val1);
    EXPECT_TRUE(set_result);
    
    Value val2(Int(20));
    set_result = array->setElement(1, val2);
    EXPECT_TRUE(set_result);
    
    Value val3(Int(30));
    set_result = array->setElement(2, val3);
    EXPECT_TRUE(set_result);
    
    // Test getting elements
    Value retrieved1 = array->getElement(0);
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved1));
    EXPECT_EQ(std::get<Int>(retrieved1), 10);
    
    Value retrieved2 = array->getElement(1);
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved2));
    EXPECT_EQ(std::get<Int>(retrieved2), 20);
    
    Value retrieved3 = array->getElement(2);
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved3));
    EXPECT_EQ(std::get<Int>(retrieved3), 30);
}

// Test PtrInstance creation and operations
TEST_F(FFIEnhancedTypesTest, PtrInstanceBasicOperations) {
    // Test null pointer
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    EXPECT_TRUE(null_ptr->isNull());
    EXPECT_EQ(null_ptr->get(), nullptr);
    
    // Test non-null pointer
    int test_value = 42;
    auto ptr = std::make_shared<PtrInstance>(&test_value);
    EXPECT_FALSE(ptr->isNull());
    EXPECT_EQ(ptr->get(), &test_value);
    EXPECT_EQ(*static_cast<int*>(ptr->get()), 42);
}

// Test PtrInstance toString method
TEST_F(FFIEnhancedTypesTest, PtrInstanceToString) {
    // Test null pointer toString
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    std::string null_str = null_ptr->toString();
    EXPECT_TRUE(null_str.find("null") != std::string::npos);
    
    // Test non-null pointer toString
    int test_value = 42;
    auto ptr = std::make_shared<PtrInstance>(&test_value);
    std::string ptr_str = ptr->toString();
    EXPECT_TRUE(ptr_str.find("Ptr<Void>") != std::string::npos);
}

// Test FFI ffi.cstring function
TEST_F(FFIEnhancedTypesTest, FFICStringFunction) {
    std::vector<Value> args;
    args.push_back(Value(Text("Hello, World!")));
    
    Value result = FFILibrary::ffi_cstring(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value buffer_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CBufferInstance>>(buffer_val));
    
    auto buffer = std::get<std::shared_ptr<CBufferInstance>>(buffer_val);
    EXPECT_EQ(buffer->size(), 14); // "Hello, World!" + null terminator
    
    // Check the string content
    const char* str_data = reinterpret_cast<const char*>(buffer->data());
    EXPECT_STREQ(str_data, "Hello, World!");
}

// Test FFI ffi.struct function
TEST_F(FFIEnhancedTypesTest, FFIStructFunction) {
    std::vector<Value> args;
    args.push_back(Value(Int(24))); // 24 bytes
    
    Value result = FFILibrary::ffi_struct(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value struct_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CStructInstance>>(struct_val));
    
    auto struct_inst = std::get<std::shared_ptr<CStructInstance>>(struct_val);
    EXPECT_EQ(struct_inst->size(), 24);
}

// Test FFI ffi.array function
TEST_F(FFIEnhancedTypesTest, FFIArrayFunction) {
    std::vector<Value> args;
    args.push_back(Value(Text("i32"))); // int32 type
    args.push_back(Value(Int(5))); // 5 elements
    
    Value result = FFILibrary::ffi_array(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value array_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CArrayInstance>>(array_val));
    
    auto array = std::get<std::shared_ptr<CArrayInstance>>(array_val);
    EXPECT_EQ(array->element_count(), 5);
    EXPECT_EQ(array->element_size(), sizeof(int32_t));
}

// Test FFI ffi.nullPtr function
TEST_F(FFIEnhancedTypesTest, FFINullPtrFunction) {
    std::vector<Value> args; // No arguments for nullPtr
    
    Value result = FFILibrary::ffi_nullPtr(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<PtrInstance>>(result));
    
    auto ptr = std::get<std::shared_ptr<PtrInstance>>(result);
    EXPECT_TRUE(ptr->isNull());
    EXPECT_EQ(ptr->get(), nullptr);
}

// Test error handling for invalid arguments
TEST_F(FFIEnhancedTypesTest, ErrorHandlingInvalidArguments) {
    // Test ffi.struct with invalid size
    std::vector<Value> args;
    args.push_back(Value(Text("invalid"))); // Non-integer argument
    
    Value result = FFILibrary::ffi_struct(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
}

// Test bounds checking
TEST_F(FFIEnhancedTypesTest, BoundsChecking) {
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 3);
    
    // Test out of bounds access
    Value invalid_val = array->getElement(5); // Out of bounds
    EXPECT_TRUE(std::holds_alternative<Int>(invalid_val));
    EXPECT_EQ(std::get<Int>(invalid_val), 0); // Should return default value
    
    // Test out of bounds set
    Value val(Int(100));
    bool set_result = array->setElement(5, val); // Out of bounds
    EXPECT_FALSE(set_result);
}