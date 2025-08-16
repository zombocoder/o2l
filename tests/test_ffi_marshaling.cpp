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
#include "../src/Runtime/FFI/FFIEngine.hpp"
#include "../src/Runtime/FFI/FFITypes.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;
using namespace o2l::ffi;

class FFIMarshalingTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
        ffi_engine = std::make_unique<FFIEngine>();
    }

    std::unique_ptr<Context> context;
    std::unique_ptr<FFIEngine> ffi_engine;
};

// Test basic type marshaling
TEST_F(FFIMarshalingTest, BasicTypeMarshaling) {
    // Test integer marshaling
    Value int_val(Int(42));
    void* marshaled_int = ffi_engine->marshalValue(int_val, ffi_type_sint32);
    EXPECT_NE(marshaled_int, nullptr);
    
    // Verify the marshaled value
    int32_t* int_ptr = static_cast<int32_t*>(marshaled_int);
    EXPECT_EQ(*int_ptr, 42);
    
    // Test double marshaling
    Value double_val(Double(3.14159));
    void* marshaled_double = ffi_engine->marshalValue(double_val, ffi_type_double);
    EXPECT_NE(marshaled_double, nullptr);
    
    // Verify the marshaled value
    double* double_ptr = static_cast<double*>(marshaled_double);
    EXPECT_DOUBLE_EQ(*double_ptr, 3.14159);
    
    // Test boolean marshaling
    Value bool_val(Bool(true));
    void* marshaled_bool = ffi_engine->marshalValue(bool_val, ffi_type_uint8);
    EXPECT_NE(marshaled_bool, nullptr);
    
    // Verify the marshaled value
    uint8_t* bool_ptr = static_cast<uint8_t*>(marshaled_bool);
    EXPECT_EQ(*bool_ptr, 1);
}

// Test enhanced FFI type marshaling
TEST_F(FFIMarshalingTest, EnhancedFFITypeMarshaling) {
    // Test CBufferInstance marshaling
    auto buffer = std::make_shared<CBufferInstance>(64);
    const char* test_data = "Hello, FFI!";
    std::memcpy(buffer->mutable_data(), test_data, strlen(test_data) + 1);
    
    Value buffer_val(buffer);
    void* marshaled_buffer = ffi_engine->marshalValue(buffer_val, ffi_type_pointer);
    EXPECT_NE(marshaled_buffer, nullptr);
    
    // Verify the marshaled pointer points to buffer data
    void** ptr_ptr = static_cast<void**>(marshaled_buffer);
    EXPECT_EQ(*ptr_ptr, buffer->data());
    
    // Test CArrayInstance marshaling
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 5);
    for (int i = 0; i < 5; i++) {
        array->setElement(i, Value(Int(i * 10)));
    }
    
    Value array_val(array);
    void* marshaled_array = ffi_engine->marshalValue(array_val, ffi_type_pointer);
    EXPECT_NE(marshaled_array, nullptr);
    
    // Verify the marshaled pointer points to array data
    void** array_ptr_ptr = static_cast<void**>(marshaled_array);
    EXPECT_EQ(*array_ptr_ptr, array->data());
    
    // Test PtrInstance marshaling
    int test_int = 1337;
    auto ptr_inst = std::make_shared<PtrInstance>(&test_int);
    Value ptr_val(ptr_inst);
    
    void* marshaled_ptr = ffi_engine->marshalValue(ptr_val, ffi_type_pointer);
    EXPECT_NE(marshaled_ptr, nullptr);
    
    // Verify the marshaled pointer
    void** ptr_ptr_ptr = static_cast<void**>(marshaled_ptr);
    EXPECT_EQ(*ptr_ptr_ptr, &test_int);
}

// Test multiple argument marshaling consistency
TEST_F(FFIMarshalingTest, MultipleArgumentMarshalingConsistency) {
    // This test verifies the fix for the original argument marshaling bug
    // where multiple arguments would overwrite each other due to static variables
    
    // Create multiple arguments of different types
    Value arg1(Int(100));
    Value arg2(Double(2.5));
    Value arg3(Bool(false));
    Value arg4(Int(200));
    
    // Marshal all arguments
    void* marshaled_arg1 = ffi_engine->marshalValue(arg1, ffi_type_sint32);
    void* marshaled_arg2 = ffi_engine->marshalValue(arg2, ffi_type_double);
    void* marshaled_arg3 = ffi_engine->marshalValue(arg3, ffi_type_uint8);
    void* marshaled_arg4 = ffi_engine->marshalValue(arg4, ffi_type_sint32);
    
    // Verify all marshaled values are still correct
    // This would fail with the old static variable approach
    EXPECT_EQ(*static_cast<int32_t*>(marshaled_arg1), 100);
    EXPECT_DOUBLE_EQ(*static_cast<double*>(marshaled_arg2), 2.5);
    EXPECT_EQ(*static_cast<uint8_t*>(marshaled_arg3), 0);
    EXPECT_EQ(*static_cast<int32_t*>(marshaled_arg4), 200);
    
    // Verify that pointers are different (not overwriting)
    EXPECT_NE(marshaled_arg1, marshaled_arg4); // Both int32, but should have different storage
    EXPECT_NE(marshaled_arg1, marshaled_arg2);
    EXPECT_NE(marshaled_arg1, marshaled_arg3);
    EXPECT_NE(marshaled_arg2, marshaled_arg3);
    EXPECT_NE(marshaled_arg2, marshaled_arg4);
    EXPECT_NE(marshaled_arg3, marshaled_arg4);
}

// Test marshaling with struct instances
TEST_F(FFIMarshalingTest, StructInstanceMarshaling) {
    // Create a struct with multiple fields
    auto struct_inst = std::make_shared<CStructInstance>(32);
    
    // Add fields
    struct_inst->addField("int_field", CType::Int32, 0);
    struct_inst->addField("double_field", CType::Float64, 8);
    struct_inst->addField("bool_field", CType::Bool, 16);
    
    // Set field values
    struct_inst->setField("int_field", Value(Int(42)));
    struct_inst->setField("double_field", Value(Double(3.14)));
    struct_inst->setField("bool_field", Value(Bool(true)));
    
    Value struct_val(struct_inst);
    void* marshaled_struct = ffi_engine->marshalValue(struct_val, ffi_type_pointer);
    EXPECT_NE(marshaled_struct, nullptr);
    
    // Verify the marshaled pointer points to struct data
    void** struct_ptr_ptr = static_cast<void**>(marshaled_struct);
    EXPECT_EQ(*struct_ptr_ptr, struct_inst->data());
    
    // Verify struct contents through the marshaled pointer
    uint8_t* struct_data = static_cast<uint8_t*>(*struct_ptr_ptr);
    
    // Check int field at offset 0
    int32_t int_value = *reinterpret_cast<int32_t*>(struct_data + 0);
    EXPECT_EQ(int_value, 42);
    
    // Check double field at offset 8
    double double_value = *reinterpret_cast<double*>(struct_data + 8);
    EXPECT_DOUBLE_EQ(double_value, 3.14);
    
    // Check bool field at offset 16
    bool bool_value = *reinterpret_cast<bool*>(struct_data + 16);
    EXPECT_EQ(bool_value, true);
}

// Test return value unmarshaling
TEST_F(FFIMarshalingTest, ReturnValueUnmarshaling) {
    // Test integer return value
    int32_t return_int = 1337;
    Value unmarshaled_int = ffi_engine->unmarshalValue(&return_int, ffi_type_sint32);
    EXPECT_TRUE(std::holds_alternative<Int>(unmarshaled_int));
    EXPECT_EQ(std::get<Int>(unmarshaled_int), 1337);
    
    // Test double return value
    double return_double = 2.71828;
    Value unmarshaled_double = ffi_engine->unmarshalValue(&return_double, ffi_type_double);
    EXPECT_TRUE(std::holds_alternative<Double>(unmarshaled_double));
    EXPECT_DOUBLE_EQ(std::get<Double>(unmarshaled_double), 2.71828);
    
    // Test float return value
    float return_float = 1.414f;
    Value unmarshaled_float = ffi_engine->unmarshalValue(&return_float, ffi_type_float);
    EXPECT_TRUE(std::holds_alternative<Float>(unmarshaled_float));
    EXPECT_FLOAT_EQ(std::get<Float>(unmarshaled_float), 1.414f);
    
    // Test boolean return value
    uint8_t return_bool = 1;
    Value unmarshaled_bool = ffi_engine->unmarshalValue(&return_bool, ffi_type_uint8);
    EXPECT_TRUE(std::holds_alternative<Bool>(unmarshaled_bool));
    EXPECT_TRUE(std::get<Bool>(unmarshaled_bool));
}

// Test pointer return value unmarshaling
TEST_F(FFIMarshalingTest, PointerReturnValueUnmarshaling) {
    // Test pointer return value - should create PtrInstance
    const char* test_string = "Hello, World!";
    void* return_ptr = const_cast<char*>(test_string);
    
    Value unmarshaled_ptr = ffi_engine->unmarshalValue(&return_ptr, ffi_type_pointer);
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<PtrInstance>>(unmarshaled_ptr));
    
    auto ptr_inst = std::get<std::shared_ptr<PtrInstance>>(unmarshaled_ptr);
    EXPECT_FALSE(ptr_inst->isNull());
    EXPECT_EQ(ptr_inst->get(), test_string);
    
    // Test null pointer return value
    void* null_return_ptr = nullptr;
    Value unmarshaled_null = ffi_engine->unmarshalValue(&null_return_ptr, ffi_type_pointer);
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<PtrInstance>>(unmarshaled_null));
    
    auto null_ptr_inst = std::get<std::shared_ptr<PtrInstance>>(unmarshaled_null);
    EXPECT_TRUE(null_ptr_inst->isNull());
    EXPECT_EQ(null_ptr_inst->get(), nullptr);
}

// Test marshaling with text to C string conversion
TEST_F(FFIMarshalingTest, TextToCStringMarshaling) {
    // Test text marshaling as C string
    Value text_val(Text("Hello, C World!"));
    void* marshaled_text = ffi_engine->marshalValue(text_val, ffi_type_pointer);
    EXPECT_NE(marshaled_text, nullptr);
    
    // Verify the marshaled pointer points to a null-terminated C string
    char** str_ptr = static_cast<char**>(marshaled_text);
    EXPECT_NE(*str_ptr, nullptr);
    EXPECT_STREQ(*str_ptr, "Hello, C World!");
}

// Test marshaling error conditions
TEST_F(FFIMarshalingTest, MarshalingErrorConditions) {
    // Test marshaling incompatible types (should handle gracefully)
    Value text_val(Text("not a number"));
    
    // Attempting to marshal text as integer should return nullptr or handle gracefully
    void* marshaled = ffi_engine->marshalValue(text_val, ffi_type_sint32);
    // Depending on implementation, this might return nullptr or convert to 0
    // The important thing is it doesn't crash
    EXPECT_NO_FATAL_FAILURE({
        if (marshaled != nullptr) {
            // If conversion happens, verify it's a reasonable default
            int32_t* int_ptr = static_cast<int32_t*>(marshaled);
            // Should be 0 or some default value
            EXPECT_TRUE(*int_ptr == 0 || *int_ptr != *int_ptr); // 0 or NaN converted to int
        }
    });
}

// Test memory management in marshaling
TEST_F(FFIMarshalingTest, MarshalingMemoryManagement) {
    // Test that marshaling creates proper storage that doesn't interfere
    std::vector<void*> marshaled_values;
    
    // Create many values and marshal them
    for (int i = 0; i < 100; i++) {
        Value val(Int(i));
        void* marshaled = ffi_engine->marshalValue(val, ffi_type_sint32);
        EXPECT_NE(marshaled, nullptr);
        marshaled_values.push_back(marshaled);
    }
    
    // Verify all values are still correct
    for (int i = 0; i < 100; i++) {
        int32_t* int_ptr = static_cast<int32_t*>(marshaled_values[i]);
        EXPECT_EQ(*int_ptr, i);
    }
    
    // Verify all pointers are unique (no reuse/overwriting)
    for (size_t i = 0; i < marshaled_values.size(); i++) {
        for (size_t j = i + 1; j < marshaled_values.size(); j++) {
            EXPECT_NE(marshaled_values[i], marshaled_values[j]);
        }
    }
}

// Test the specific bug fix for argument marshaling
TEST_F(FFIMarshalingTest, ArgumentMarshalingBugFix) {
    // This test specifically targets the bug where test_add(5, 3) returned 6 instead of 8
    // The issue was static variables being overwritten in marshalValue
    
    // Simulate the test_add function call scenario
    Value arg1(Int(5));   // First argument
    Value arg2(Int(3));   // Second argument
    
    // Marshal both arguments in sequence
    void* marshaled_arg1 = ffi_engine->marshalValue(arg1, ffi_type_sint32);
    void* marshaled_arg2 = ffi_engine->marshalValue(arg2, ffi_type_sint32);
    
    // Both marshaled values should be valid and different
    EXPECT_NE(marshaled_arg1, nullptr);
    EXPECT_NE(marshaled_arg2, nullptr);
    EXPECT_NE(marshaled_arg1, marshaled_arg2);
    
    // Both values should retain their original values
    int32_t* int_ptr1 = static_cast<int32_t*>(marshaled_arg1);
    int32_t* int_ptr2 = static_cast<int32_t*>(marshaled_arg2);
    
    EXPECT_EQ(*int_ptr1, 5);  // Should still be 5, not overwritten by second argument
    EXPECT_EQ(*int_ptr2, 3);  // Should be 3
    
    // Simulate what the add function would see
    int32_t result = *int_ptr1 + *int_ptr2;
    EXPECT_EQ(result, 8);  // Should be 8, not 6
}