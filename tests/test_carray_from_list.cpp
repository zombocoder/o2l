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
#include "../src/AST/MethodCallNode.hpp"
#include "../src/AST/LiteralNode.hpp"
#include "../src/Runtime/FFI/FFITypes.hpp"
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"
#include "../src/Common/Exceptions.hpp"

using namespace o2l;
using namespace o2l::ffi;

class CArrayFromListTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
    }

    std::unique_ptr<Context> context;
};

// Mock ASTNode that returns a specific value
class MockCArrayNode : public ASTNode {
public:
    Value val;
    MockCArrayNode(Value v) : ASTNode(SourceLocation{}), val(v) {}
    Value evaluate(Context&) override { return val; }
    std::string toString() const override { return "MockCArrayNode"; }
};

TEST_F(CArrayFromListTest, FromListSuccess) {
    // 1. Create a CArrayInstance (Int32, size 3)
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 3);
    Value array_val(array);

    // 2. Create a ListInstance with 3 elements
    auto list = std::make_shared<ListInstance>("Int");
    list->add(Value(Int(10)));
    list->add(Value(Int(20)));
    list->add(Value(Int(30)));
    Value list_val(list);

    // 3. Create MethodCallNode for array.fromList(list)
    auto obj_node = std::make_unique<MockCArrayNode>(array_val);
    auto arg_node = std::make_unique<MockCArrayNode>(list_val);
    
    std::vector<ASTNodePtr> args;
    args.push_back(std::move(arg_node));
    
    MethodCallNode call_node(std::move(obj_node), "fromList", std::move(args), SourceLocation{});

    // 4. Evaluate
    Value result = call_node.evaluate(*context);

    // 5. Verify result is Bool(true)
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_TRUE(std::get<Bool>(result));

    // 6. Verify array contents
    EXPECT_EQ(std::get<Int>(array->getElement(0)), 10);
    EXPECT_EQ(std::get<Int>(array->getElement(1)), 20);
    EXPECT_EQ(std::get<Int>(array->getElement(2)), 30);
}

TEST_F(CArrayFromListTest, FromListSizeMismatch) {
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 3);
    auto list = std::make_shared<ListInstance>("Int");
    list->add(Value(Int(10))); // Only 1 element, expects 3
    
    auto obj_node = std::make_unique<MockCArrayNode>(Value(array));
    auto arg_node = std::make_unique<MockCArrayNode>(Value(list));
    
    std::vector<ASTNodePtr> args;
    args.push_back(std::move(arg_node));
    
    MethodCallNode call_node(std::move(obj_node), "fromList", std::move(args), SourceLocation{});

    EXPECT_THROW(call_node.evaluate(*context), EvaluationError);
}

TEST_F(CArrayFromListTest, FromListTypeMismatch) {
    auto array = std::make_shared<CArrayInstance>(CType::Int32, 1);
    
    auto obj_node = std::make_unique<MockCArrayNode>(Value(array));
    auto arg_node = std::make_unique<MockCArrayNode>(Value(Int(42))); // Not a list
    
    std::vector<ASTNodePtr> args;
    args.push_back(std::move(arg_node));
    
    MethodCallNode call_node(std::move(obj_node), "fromList", std::move(args), SourceLocation{});

    EXPECT_THROW(call_node.evaluate(*context), EvaluationError);
}
