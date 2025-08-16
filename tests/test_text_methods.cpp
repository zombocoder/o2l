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

#include "AST/LiteralNode.hpp"
#include "AST/MethodCallNode.hpp"
#include "Common/Exceptions.hpp"
#include "Common/SourceLocation.hpp"
#include "Runtime/Context.hpp"
#include "Runtime/ListInstance.hpp"
#include "Runtime/MapInstance.hpp"
#include "Runtime/Value.hpp"

using namespace o2l;

class TextMethodTest : public ::testing::Test {
   protected:
    void SetUp() override {
        context_ = std::make_unique<Context>();
    }

    // Helper method to create a method call node for testing Text methods
    Value callTextMethod(const std::string& text_value, const std::string& method_name,
                         const std::vector<Value>& args = {}) {
        // Create text literal node
        auto text_node = std::make_unique<LiteralNode>(Text(text_value));

        // Create argument nodes
        std::vector<ASTNodePtr> arg_nodes;
        for (const auto& arg : args) {
            if (std::holds_alternative<Text>(arg)) {
                arg_nodes.push_back(std::make_unique<LiteralNode>(arg));
            } else if (std::holds_alternative<Int>(arg)) {
                arg_nodes.push_back(std::make_unique<LiteralNode>(arg));
            } else if (std::holds_alternative<Bool>(arg)) {
                arg_nodes.push_back(std::make_unique<LiteralNode>(arg));
            } else if (std::holds_alternative<std::shared_ptr<ListInstance>>(arg)) {
                arg_nodes.push_back(std::make_unique<LiteralNode>(arg));
            } else if (std::holds_alternative<std::shared_ptr<MapInstance>>(arg)) {
                arg_nodes.push_back(std::make_unique<LiteralNode>(arg));
            }
        }

        // Create method call node
        auto method_call = std::make_unique<MethodCallNode>(std::move(text_node), method_name,
                                                            std::move(arg_nodes));

        return method_call->evaluate(*context_);
    }

    std::unique_ptr<Context> context_;
};

// Test basic string case methods
TEST_F(TextMethodTest, StringCaseMethods) {
    // Test capitalize
    Value result = callTextMethod("hello world", "capitalize");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello world");

    // Test lower
    result = callTextMethod("HELLO WORLD", "lower");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "hello world");

    // Test upper
    result = callTextMethod("hello world", "upper");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "HELLO WORLD");

    // Test caseFold
    result = callTextMethod("HELLO WORLD", "caseFold");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "hello world");

    // Test swapCase
    result = callTextMethod("Hello World", "swapCase");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "hELLO wORLD");

    // Test title
    result = callTextMethod("hello world test", "title");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello World Test");
}

// Test string search methods
TEST_F(TextMethodTest, StringSearchMethods) {
    // Test find
    Value result = callTextMethod("Hello World", "find", {Text("World")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);

    result = callTextMethod("Hello World", "find", {Text("xyz")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), -1);

    // Test rfind
    result = callTextMethod("Hello Hello", "rfind", {Text("Hello")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);

    // Test count
    result = callTextMethod("Hello Hello Hello", "count", {Text("Hello")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 3);

    // Test index (found)
    result = callTextMethod("Hello World", "index", {Text("World")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);

    // Test index (not found) - should throw exception
    EXPECT_THROW({ callTextMethod("Hello World", "index", {Text("xyz")}); }, EvaluationError);

    // Test rindex (found)
    result = callTextMethod("Hello Hello", "rindex", {Text("Hello")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);

    // Test rindex (not found) - should throw exception
    EXPECT_THROW({ callTextMethod("Hello World", "rindex", {Text("xyz")}); }, EvaluationError);
}

// Test string boolean check methods
TEST_F(TextMethodTest, StringBooleanMethods) {
    // Test startswith
    Value result = callTextMethod("Hello World", "startswith", {Text("Hello")});
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello World", "startswith", {Text("World")});
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test endswith
    result = callTextMethod("Hello World", "endswith", {Text("World")});
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello World", "endswith", {Text("Hello")});
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);
}

// Test character type checking methods
TEST_F(TextMethodTest, CharacterCheckMethods) {
    // Test isAlnum
    Value result = callTextMethod("Hello123", "isAlnum");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello@123", "isAlnum");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isAlpha
    result = callTextMethod("Hello", "isAlpha");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello123", "isAlpha");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isDigit
    result = callTextMethod("12345", "isDigit");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("123abc", "isDigit");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isLower
    result = callTextMethod("hello", "isLower");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello", "isLower");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isUpper
    result = callTextMethod("HELLO", "isUpper");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello", "isUpper");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isSpace
    result = callTextMethod("   \t\n", "isSpace");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("Hello World", "isSpace");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isPrintable
    result = callTextMethod("Hello World!", "isPrintable");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    // Test isAscii
    result = callTextMethod("Hello", "isAscii");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    // Test isIdentifier
    result = callTextMethod("hello_world", "isIdentifier");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("123abc", "isIdentifier");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isDecimal
    result = callTextMethod("12345", "isDecimal");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("123.45", "isDecimal");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isNumeric
    result = callTextMethod("123.45", "isNumeric");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("123abc", "isNumeric");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    // Test isTitle
    result = callTextMethod("Hello World", "isTitle");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), true);

    result = callTextMethod("hello world", "isTitle");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);
}

// Test string manipulation methods
TEST_F(TextMethodTest, StringManipulationMethods) {
    // Test strip
    Value result = callTextMethod("  Hello World  ", "strip");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello World");

    // Test lstrip
    result = callTextMethod("  Hello World  ", "lstrip");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello World  ");

    // Test rstrip
    result = callTextMethod("  Hello World  ", "rstrip");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "  Hello World");

    // Test replace
    result = callTextMethod("Hello World", "replace", {Text("World"), Text("Universe")});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello Universe");

    // Test multiple replacements
    result = callTextMethod("Hello Hello Hello", "replace", {Text("Hello"), Text("Hi")});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hi Hi Hi");

    // Test split
    result = callTextMethod("Hello,World,Test", "split", {Text(",")});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result));
    auto list = std::get<std::shared_ptr<ListInstance>>(result);
    EXPECT_EQ(list->size(), 3);
    EXPECT_EQ(std::get<Text>(list->get(0)), "Hello");
    EXPECT_EQ(std::get<Text>(list->get(1)), "World");
    EXPECT_EQ(std::get<Text>(list->get(2)), "Test");

    // Test splitlines
    result = callTextMethod("Line1\nLine2\nLine3", "splitlines");
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result));
    list = std::get<std::shared_ptr<ListInstance>>(result);
    EXPECT_EQ(list->size(), 3);
    EXPECT_EQ(std::get<Text>(list->get(0)), "Line1");
    EXPECT_EQ(std::get<Text>(list->get(1)), "Line2");
    EXPECT_EQ(std::get<Text>(list->get(2)), "Line3");
}

// Test string positioning methods
TEST_F(TextMethodTest, StringPositioningMethods) {
    // Test center
    Value result = callTextMethod("Hi", "center", {Int(10)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "    Hi    ");

    // Test ljust
    result = callTextMethod("Hi", "ljust", {Int(10)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hi        ");

    // Test rjust
    result = callTextMethod("Hi", "rjust", {Int(10)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "        Hi");

    // Test zfill
    result = callTextMethod("42", "zfill", {Int(5)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "00042");

    // Test zfill with negative number
    result = callTextMethod("-42", "zfill", {Int(5)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "-0042");

    // Test zfill with positive sign
    result = callTextMethod("+42", "zfill", {Int(5)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "+0042");
}

// Test string formatting methods
TEST_F(TextMethodTest, StringFormattingMethods) {
    // Test format with positional arguments
    Value result =
        callTextMethod("Hello {0}, you are {1} years old!", "format", {Text("Alice"), Int(25)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello Alice, you are 25 years old!");

    // Test format with boolean
    result = callTextMethod("The answer is {0}", "format", {Bool(true)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "The answer is true");

    // Test formatMap
    auto map = std::make_shared<MapInstance>("Text", "Text");
    map->put(Value(Text("name")), Value(Text("Bob")));
    map->put(Value(Text("place")), Value(Text("O²L")));

    result = callTextMethod("Hello {name}, welcome to {place}!", "formatMap", {Value(map)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello Bob, welcome to O²L!");
}

// Test join method
TEST_F(TextMethodTest, JoinMethod) {
    // Create a list of strings to join
    auto list = std::make_shared<ListInstance>("Text");
    list->add(Value(Text("Hello")));
    list->add(Value(Text("World")));
    list->add(Value(Text("Test")));

    Value result = callTextMethod(", ", "join", {Value(list)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello, World, Test");

    // Test join with mixed types
    auto mixed_list = std::make_shared<ListInstance>("Value");
    mixed_list->add(Value(Text("Number:")));
    mixed_list->add(Value(Int(42)));
    mixed_list->add(Value(Bool(true)));

    result = callTextMethod(" ", "join", {Value(mixed_list)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Number: 42 true");
}

// Test partition methods
TEST_F(TextMethodTest, PartitionMethods) {
    // Test partition
    Value result = callTextMethod("name=value", "partition", {Text("=")});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result));
    auto list = std::get<std::shared_ptr<ListInstance>>(result);
    EXPECT_EQ(list->size(), 3);
    EXPECT_EQ(std::get<Text>(list->get(0)), "name");
    EXPECT_EQ(std::get<Text>(list->get(1)), "=");
    EXPECT_EQ(std::get<Text>(list->get(2)), "value");

    // Test partition with separator not found
    result = callTextMethod("name=value", "partition", {Text("@")});
    list = std::get<std::shared_ptr<ListInstance>>(result);
    EXPECT_EQ(list->size(), 3);
    EXPECT_EQ(std::get<Text>(list->get(0)), "name=value");
    EXPECT_EQ(std::get<Text>(list->get(1)), "");
    EXPECT_EQ(std::get<Text>(list->get(2)), "");

    // Test rpartition
    result = callTextMethod("a.b.c", "rpartition", {Text(".")});
    list = std::get<std::shared_ptr<ListInstance>>(result);
    EXPECT_EQ(list->size(), 3);
    EXPECT_EQ(std::get<Text>(list->get(0)), "a.b");
    EXPECT_EQ(std::get<Text>(list->get(1)), ".");
    EXPECT_EQ(std::get<Text>(list->get(2)), "c");
}

// Test translation methods
TEST_F(TextMethodTest, TranslationMethods) {
    // Test makeTrans
    Value result = callTextMethod("abc", "makeTrans", {Text("abc"), Text("xyz")});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(result));
    auto trans_table = std::get<std::shared_ptr<MapInstance>>(result);

    // Verify translation table contents
    Value a_trans = trans_table->get(Value(Text("a")));
    EXPECT_EQ(std::get<Text>(a_trans), "x");

    // Test translate
    result = callTextMethod("abcdef", "translate", {Value(trans_table)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "xyzdef");
}

// Test error cases
TEST_F(TextMethodTest, ErrorCases) {
    // Test methods that should throw with wrong argument count
    EXPECT_THROW({ callTextMethod("Hello", "capitalize", {Int(1)}); }, EvaluationError);

    EXPECT_THROW({ callTextMethod("Hello", "find", {}); }, EvaluationError);

    EXPECT_THROW({ callTextMethod("Hello", "replace", {Text("a")}); }, EvaluationError);

    // Test methods that should throw with wrong argument types
    EXPECT_THROW({ callTextMethod("Hello", "find", {Int(1)}); }, EvaluationError);

    EXPECT_THROW({ callTextMethod("Hello", "center", {Text("not_int")}); }, EvaluationError);
}

// Test edge cases
TEST_F(TextMethodTest, EdgeCases) {
    // Test with empty string
    Value result = callTextMethod("", "capitalize");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "");

    result = callTextMethod("", "isAlnum");
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_EQ(std::get<Bool>(result), false);

    result = callTextMethod("", "strip");
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "");

    // Test count with empty substring
    result = callTextMethod("Hello", "count", {Text("")});
    EXPECT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 0);

    // Test positioning methods with width smaller than string length
    result = callTextMethod("Hello", "center", {Int(3)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "Hello");
}