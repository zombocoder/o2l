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

#include <memory>

#include "../src/Common/Exceptions.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/TestLibrary.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

class TestLibraryTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to create testing object and call methods
    Value callTestMethod(const std::string& method_name, const std::vector<Value>& args = {}) {
        auto testing_object = TestLibrary::createTestingObject();
        EXPECT_TRUE(testing_object->hasMethod(method_name))
            << "Method " << method_name << " not found";
        return testing_object->callMethod(method_name, args, context);
    }

    // Helper to check boolean results
    void expectBool(const Value& result, Bool expected) {
        ASSERT_TRUE(std::holds_alternative<Bool>(result));
        EXPECT_EQ(std::get<Bool>(result), expected);
    }

    // Helper to check integer results
    void expectInt(const Value& result, Int expected) {
        ASSERT_TRUE(std::holds_alternative<Int>(result));
        EXPECT_EQ(std::get<Int>(result), expected);
    }

    // Helper to check text results
    void expectText(const Value& result, const std::string& expected) {
        ASSERT_TRUE(std::holds_alternative<Text>(result));
        EXPECT_EQ(std::get<Text>(result), expected);
    }
};

// Test core assertion methods
TEST_F(TestLibraryTest, AssertEqual) {
    // Test successful equality
    EXPECT_NO_THROW(callTestMethod("assertEqual", {Value(Int(5)), Value(Int(5))}));
    EXPECT_NO_THROW(callTestMethod("assertEqual", {Value(Text("hello")), Value(Text("hello"))}));
    EXPECT_NO_THROW(callTestMethod("assertEqual", {Value(Bool(true)), Value(Bool(true))}));

    // Test failure cases
    EXPECT_THROW(callTestMethod("assertEqual", {Value(Int(5)), Value(Int(3))}), EvaluationError);
    EXPECT_THROW(callTestMethod("assertEqual", {Value(Text("hello")), Value(Text("world"))}),
                 EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertEqual",
                                {Value(Int(5)), Value(Int(3)), Value(Text("Custom message"))}),
                 EvaluationError);

    // Test argument validation
    EXPECT_THROW(callTestMethod("assertEqual", {Value(Int(5))}), EvaluationError);  // Too few args
    EXPECT_THROW(callTestMethod("assertEqual",
                                {Value(Int(5)), Value(Int(3)), Value(Text("msg")), Value(Int(1))}),
                 EvaluationError);  // Too many args
}

TEST_F(TestLibraryTest, AssertNotEqual) {
    // Test successful inequality
    EXPECT_NO_THROW(callTestMethod("assertNotEqual", {Value(Int(5)), Value(Int(3))}));
    EXPECT_NO_THROW(callTestMethod("assertNotEqual", {Value(Text("hello")), Value(Text("world"))}));
    EXPECT_NO_THROW(callTestMethod("assertNotEqual", {Value(Bool(true)), Value(Bool(false))}));

    // Test failure cases
    EXPECT_THROW(callTestMethod("assertNotEqual", {Value(Int(5)), Value(Int(5))}), EvaluationError);
    EXPECT_THROW(callTestMethod("assertNotEqual", {Value(Text("hello")), Value(Text("hello"))}),
                 EvaluationError);
}

TEST_F(TestLibraryTest, AssertTrue) {
    // Test successful true assertion
    EXPECT_NO_THROW(callTestMethod("assertTrue", {Value(Bool(true))}));

    // Test failure case
    EXPECT_THROW(callTestMethod("assertTrue", {Value(Bool(false))}), EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertTrue", {Value(Bool(false)), Value(Text("Should be true"))}),
                 EvaluationError);

    // Test non-boolean argument
    EXPECT_THROW(callTestMethod("assertTrue", {Value(Int(1))}), EvaluationError);
}

TEST_F(TestLibraryTest, AssertFalse) {
    // Test successful false assertion
    EXPECT_NO_THROW(callTestMethod("assertFalse", {Value(Bool(false))}));

    // Test failure case
    EXPECT_THROW(callTestMethod("assertFalse", {Value(Bool(true))}), EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertFalse", {Value(Bool(true)), Value(Text("Should be false"))}),
                 EvaluationError);

    // Test non-boolean argument
    EXPECT_THROW(callTestMethod("assertFalse", {Value(Int(0))}), EvaluationError);
}

// Test numeric assertion methods
TEST_F(TestLibraryTest, AssertGreater) {
    // Test successful greater than
    EXPECT_NO_THROW(callTestMethod("assertGreater", {Value(Double(5.0)), Value(Double(3.0))}));
    EXPECT_NO_THROW(callTestMethod("assertGreater", {Value(Int(10)), Value(Int(5))}));

    // Test failure cases
    EXPECT_THROW(callTestMethod("assertGreater", {Value(Double(3.0)), Value(Double(5.0))}),
                 EvaluationError);
    EXPECT_THROW(callTestMethod("assertGreater", {Value(Double(5.0)), Value(Double(5.0))}),
                 EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertGreater",
                                {Value(Int(3)), Value(Int(5)), Value(Text("3 should be > 5"))}),
                 EvaluationError);

    // Test mixed numeric types
    EXPECT_NO_THROW(callTestMethod("assertGreater", {Value(Double(5.5)), Value(Int(5))}));
}

TEST_F(TestLibraryTest, AssertLess) {
    // Test successful less than
    EXPECT_NO_THROW(callTestMethod("assertLess", {Value(Double(3.0)), Value(Double(5.0))}));
    EXPECT_NO_THROW(callTestMethod("assertLess", {Value(Int(5)), Value(Int(10))}));

    // Test failure cases
    EXPECT_THROW(callTestMethod("assertLess", {Value(Double(5.0)), Value(Double(3.0))}),
                 EvaluationError);
    EXPECT_THROW(callTestMethod("assertLess", {Value(Double(5.0)), Value(Double(5.0))}),
                 EvaluationError);
}

TEST_F(TestLibraryTest, AssertNear) {
    // Test successful near assertion
    EXPECT_NO_THROW(callTestMethod(
        "assertNear", {Value(Double(3.14159)), Value(Double(3.14160)), Value(Double(0.001))}));
    EXPECT_NO_THROW(callTestMethod("assertNear", {Value(Double(5.0)), Value(Double(5.0)),
                                                  Value(Double(0.0))}));  // Exact match

    // Test failure case
    EXPECT_THROW(callTestMethod("assertNear", {Value(Double(3.14159)), Value(Double(3.15000)),
                                               Value(Double(0.001))}),
                 EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertNear", {Value(Double(1.0)), Value(Double(2.0)),
                                               Value(Double(0.5)), Value(Text("Too far apart"))}),
                 EvaluationError);

    // Test argument validation
    EXPECT_THROW(callTestMethod("assertNear", {Value(Double(1.0)), Value(Double(2.0))}),
                 EvaluationError);  // Missing tolerance
}

// Test string assertion methods
TEST_F(TestLibraryTest, AssertContains) {
    // Test successful contains assertion
    EXPECT_NO_THROW(
        callTestMethod("assertContains", {Value(Text("Hello World")), Value(Text("World"))}));
    EXPECT_NO_THROW(callTestMethod("assertContains", {Value(Text("Testing")), Value(Text("est"))}));

    // Test failure case
    EXPECT_THROW(callTestMethod("assertContains", {Value(Text("Hello World")), Value(Text("xyz"))}),
                 EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertContains", {Value(Text("Hello")), Value(Text("World")),
                                                   Value(Text("Should contain World"))}),
                 EvaluationError);

    // Test non-text arguments
    EXPECT_THROW(callTestMethod("assertContains", {Value(Int(123)), Value(Text("1"))}),
                 EvaluationError);
}

TEST_F(TestLibraryTest, AssertStartsWith) {
    // Test successful starts with assertion
    EXPECT_NO_THROW(
        callTestMethod("assertStartsWith", {Value(Text("Hello World")), Value(Text("Hello"))}));
    EXPECT_NO_THROW(callTestMethod("assertStartsWith",
                                   {Value(Text("Testing")), Value(Text(""))}));  // Empty prefix

    // Test failure case
    EXPECT_THROW(
        callTestMethod("assertStartsWith", {Value(Text("Hello World")), Value(Text("World"))}),
        EvaluationError);

    // Test with custom message
    EXPECT_THROW(callTestMethod("assertStartsWith", {Value(Text("World")), Value(Text("Hello")),
                                                     Value(Text("Should start with Hello"))}),
                 EvaluationError);
}

// Test collection assertion methods
TEST_F(TestLibraryTest, AssertEmpty) {
    // Test successful empty assertion for text
    EXPECT_NO_THROW(callTestMethod("assertEmpty", {Value(Text(""))}));

    // Test failure case for text
    EXPECT_THROW(callTestMethod("assertEmpty", {Value(Text("not empty"))}), EvaluationError);

    // Test with custom message
    EXPECT_THROW(
        callTestMethod("assertEmpty", {Value(Text("not empty")), Value(Text("Should be empty"))}),
        EvaluationError);

    // Note: List testing would require creating ListInstance objects
}

// Test test suite management
TEST_F(TestLibraryTest, CreateSuite) {
    // Test successful suite creation
    Value result = callTestMethod("createSuite", {Value(Text("TestSuite1"))});
    expectText(result, "TestSuite1");

    // Test with different suite names
    Value result2 = callTestMethod("createSuite", {Value(Text("AnotherSuite"))});
    expectText(result2, "AnotherSuite");

    // Test argument validation
    EXPECT_THROW(callTestMethod("createSuite", {}), EvaluationError);  // No arguments
    EXPECT_THROW(callTestMethod("createSuite", {Value(Int(123))}),
                 EvaluationError);  // Non-text argument
}

TEST_F(TestLibraryTest, RunTest) {
    // Test successful test run setup
    Value result = callTestMethod("runTest", {Value(Text("myTest"))});
    expectText(result, "myTest");

    // Test argument validation
    EXPECT_THROW(callTestMethod("runTest", {}), EvaluationError);  // No arguments
    EXPECT_THROW(callTestMethod("runTest", {Value(Int(123))}),
                 EvaluationError);  // Non-text argument
}

// Test result reporting methods
TEST_F(TestLibraryTest, GetCounts) {
    // Initialize a test suite
    callTestMethod("createSuite", {Value(Text("CountTests"))});

    // Initially should have zero counts
    Value total = callTestMethod("getTotalCount");
    expectInt(total, 0);

    Value passed = callTestMethod("getPassedCount");
    expectInt(passed, 0);

    Value failed = callTestMethod("getFailedCount");
    expectInt(failed, 0);
}

TEST_F(TestLibraryTest, PrintResults) {
    // Test print results doesn't crash
    EXPECT_NO_THROW(callTestMethod("printResults"));

    // Test with specific suite name
    callTestMethod("createSuite", {Value(Text("PrintTestSuite"))});
    EXPECT_NO_THROW(callTestMethod("printResults", {Value(Text("PrintTestSuite"))}));
}

// Test utility methods
TEST_F(TestLibraryTest, Fail) {
    // Test fail with default message
    EXPECT_THROW(callTestMethod("fail"), EvaluationError);

    // Test fail with custom message
    EXPECT_THROW(callTestMethod("fail", {Value(Text("Custom failure message"))}), EvaluationError);
}

// Test argument validation for all methods
TEST_F(TestLibraryTest, ArgumentValidation) {
    // Test functions that require exactly 2 arguments
    EXPECT_THROW(callTestMethod("assertEqual", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callTestMethod("assertNotEqual", {Value(Int(1))}), EvaluationError);

    // Test functions that require numeric arguments
    EXPECT_THROW(callTestMethod("assertGreater", {Value(Text("not a number")), Value(Int(5))}),
                 EvaluationError);
    EXPECT_THROW(callTestMethod("assertLess", {Value(Int(5)), Value(Text("not a number"))}),
                 EvaluationError);

    // Test functions that require text arguments
    EXPECT_THROW(callTestMethod("assertContains", {Value(Int(123)), Value(Text("text"))}),
                 EvaluationError);
    EXPECT_THROW(callTestMethod("assertStartsWith", {Value(Text("text")), Value(Int(123))}),
                 EvaluationError);
}

// Test method availability
TEST_F(TestLibraryTest, MethodAvailability) {
    auto testing_object = TestLibrary::createTestingObject();

    // Test that all expected methods are available
    EXPECT_TRUE(testing_object->hasMethod("assertEqual"));
    EXPECT_TRUE(testing_object->hasMethod("assertNotEqual"));
    EXPECT_TRUE(testing_object->hasMethod("assertTrue"));
    EXPECT_TRUE(testing_object->hasMethod("assertFalse"));
    EXPECT_TRUE(testing_object->hasMethod("assertGreater"));
    EXPECT_TRUE(testing_object->hasMethod("assertLess"));
    EXPECT_TRUE(testing_object->hasMethod("assertNear"));
    EXPECT_TRUE(testing_object->hasMethod("assertContains"));
    EXPECT_TRUE(testing_object->hasMethod("assertStartsWith"));
    EXPECT_TRUE(testing_object->hasMethod("assertEmpty"));
    EXPECT_TRUE(testing_object->hasMethod("createSuite"));
    EXPECT_TRUE(testing_object->hasMethod("runTest"));
    EXPECT_TRUE(testing_object->hasMethod("printResults"));
    EXPECT_TRUE(testing_object->hasMethod("getTotalCount"));
    EXPECT_TRUE(testing_object->hasMethod("getPassedCount"));
    EXPECT_TRUE(testing_object->hasMethod("getFailedCount"));
    EXPECT_TRUE(testing_object->hasMethod("fail"));

    // Test methods are external (publicly accessible)
    EXPECT_TRUE(testing_object->isMethodExternal("assertEqual"));
    EXPECT_TRUE(testing_object->isMethodExternal("assertTrue"));
    EXPECT_TRUE(testing_object->isMethodExternal("createSuite"));
}

// Test edge cases and boundary conditions
TEST_F(TestLibraryTest, EdgeCases) {
    // Test near assertion with zero tolerance
    EXPECT_NO_THROW(
        callTestMethod("assertNear", {Value(Double(5.0)), Value(Double(5.0)), Value(Double(0.0))}));

    // Test string operations with empty strings
    EXPECT_NO_THROW(callTestMethod("assertContains", {Value(Text("hello")), Value(Text(""))}));
    EXPECT_NO_THROW(callTestMethod("assertStartsWith", {Value(Text("hello")), Value(Text(""))}));

    // Test numeric comparisons with very small differences
    EXPECT_NO_THROW(callTestMethod(
        "assertNear", {Value(Double(1.000000001)), Value(Double(1.0)), Value(Double(1e-8))}));

    // Test mixed numeric types in comparisons
    EXPECT_NO_THROW(callTestMethod("assertGreater", {Value(Long(1000L)), Value(Int(999))}));
    EXPECT_NO_THROW(callTestMethod("assertLess", {Value(Float(2.5f)), Value(Double(3.0))}));
}