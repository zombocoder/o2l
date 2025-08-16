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
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;
using namespace o2l::ffi;

class FFISimplifiedTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
    }

    std::unique_ptr<Context> context;
};

// Test basic FFI type creation
TEST_F(FFISimplifiedTest, BasicFFITypeCreation) {
    // Test CBufferInstance creation
    auto buffer = std::make_shared<CBufferInstance>(64);
    EXPECT_EQ(buffer->size(), 64);
    EXPECT_NE(buffer->data(), nullptr);
    
    // Test data access
    uint8_t* mutable_data = buffer->mutable_data();
    EXPECT_NE(mutable_data, nullptr);
    
    // Test writing and reading data
    const char* test_str = "Hello FFI";
    std::memcpy(mutable_data, test_str, strlen(test_str) + 1);
    
    const uint8_t* const_data = buffer->data();
    EXPECT_STREQ(reinterpret_cast<const char*>(const_data), "Hello FFI");
}

// Test CStructInstance basic operations
TEST_F(FFISimplifiedTest, CStructInstanceBasic) {
    auto struct_inst = std::make_shared<CStructInstance>(64);
    EXPECT_EQ(struct_inst->size(), 64);
    EXPECT_NE(struct_inst->data(), nullptr);
    
    // Test adding fields (returns void)
    EXPECT_NO_THROW(struct_inst->addField("test_field", CType::Int32, 0));
}

// Test CArrayInstance basic operations  
TEST_F(FFISimplifiedTest, CArrayInstanceBasic) {
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 5);
    EXPECT_EQ(array->element_count(), 5);
    EXPECT_EQ(array->element_size(), sizeof(int32_t));
    EXPECT_NE(array->data(), nullptr);
}

// Test PtrInstance operations
TEST_F(FFISimplifiedTest, PtrInstanceBasic) {
    // Test null pointer
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    EXPECT_TRUE(null_ptr->isNull());
    EXPECT_EQ(null_ptr->get(), nullptr);
    
    // Test non-null pointer
    int test_value = 42;
    auto ptr = std::make_shared<PtrInstance>(&test_value);
    EXPECT_FALSE(ptr->isNull());
    EXPECT_EQ(ptr->get(), &test_value);
}

// Test toString methods
TEST_F(FFISimplifiedTest, ToStringMethods) {
    // Test buffer toString
    auto buffer = std::make_shared<CBufferInstance>(32);
    std::string buffer_str = buffer->toString();
    EXPECT_TRUE(buffer_str.find("CBuffer") != std::string::npos);
    
    // Test pointer toString
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    std::string null_str = null_ptr->toString();
    EXPECT_TRUE(null_str.find("null") != std::string::npos);
    
    int value = 123;
    auto ptr = std::make_shared<PtrInstance>(&value);
    std::string ptr_str = ptr->toString();
    EXPECT_TRUE(ptr_str.find("Ptr") != std::string::npos);
}