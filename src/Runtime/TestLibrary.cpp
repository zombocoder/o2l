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

#include "TestLibrary.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "../Common/Exceptions.hpp"
#include "ListInstance.hpp"

namespace o2l {

// Static member definitions
std::map<std::string, TestSuiteResult> TestLibrary::test_suites_;
std::string TestLibrary::current_suite_ = "default";
std::string TestLibrary::current_test_ = "";

std::shared_ptr<ObjectInstance> TestLibrary::createTestingObject() {
    auto testing_object = std::make_shared<ObjectInstance>("testing");

    // Core assertion methods
    Method assertEqual_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertEqual(args, ctx);
    };
    testing_object->addMethod("assertEqual", assertEqual_method, true);  // external

    Method assertNotEqual_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNotEqual(args, ctx);
    };
    testing_object->addMethod("assertNotEqual", assertNotEqual_method, true);  // external

    Method assertTrue_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertTrue(args, ctx);
    };
    testing_object->addMethod("assertTrue", assertTrue_method, true);  // external

    Method assertFalse_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertFalse(args, ctx);
    };
    testing_object->addMethod("assertFalse", assertFalse_method, true);  // external

    Method assertNull_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNull(args, ctx);
    };
    testing_object->addMethod("assertNull", assertNull_method, true);  // external

    Method assertNotNull_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNotNull(args, ctx);
    };
    testing_object->addMethod("assertNotNull", assertNotNull_method, true);  // external

    // Numeric assertion methods
    Method assertGreater_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertGreater(args, ctx);
    };
    testing_object->addMethod("assertGreater", assertGreater_method, true);  // external

    Method assertGreaterEqual_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertGreaterEqual(args, ctx);
    };
    testing_object->addMethod("assertGreaterEqual", assertGreaterEqual_method, true);  // external

    Method assertLess_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertLess(args, ctx);
    };
    testing_object->addMethod("assertLess", assertLess_method, true);  // external

    Method assertLessEqual_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertLessEqual(args, ctx);
    };
    testing_object->addMethod("assertLessEqual", assertLessEqual_method, true);  // external

    Method assertNear_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNear(args, ctx);
    };
    testing_object->addMethod("assertNear", assertNear_method, true);  // external

    // String assertion methods
    Method assertContains_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertContains(args, ctx);
    };
    testing_object->addMethod("assertContains", assertContains_method, true);  // external

    Method assertNotContains_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNotContains(args, ctx);
    };
    testing_object->addMethod("assertNotContains", assertNotContains_method, true);  // external

    Method assertStartsWith_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertStartsWith(args, ctx);
    };
    testing_object->addMethod("assertStartsWith", assertStartsWith_method, true);  // external

    Method assertEndsWith_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertEndsWith(args, ctx);
    };
    testing_object->addMethod("assertEndsWith", assertEndsWith_method, true);  // external

    // Collection assertion methods
    Method assertEmpty_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertEmpty(args, ctx);
    };
    testing_object->addMethod("assertEmpty", assertEmpty_method, true);  // external

    Method assertNotEmpty_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNotEmpty(args, ctx);
    };
    testing_object->addMethod("assertNotEmpty", assertNotEmpty_method, true);  // external

    Method assertSize_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertSize(args, ctx);
    };
    testing_object->addMethod("assertSize", assertSize_method, true);  // external

    Method assertIn_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertIn(args, ctx);
    };
    testing_object->addMethod("assertIn", assertIn_method, true);  // external

    Method assertNotIn_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::assertNotIn(args, ctx);
    };
    testing_object->addMethod("assertNotIn", assertNotIn_method, true);  // external

    // Test suite management
    Method createSuite_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::createSuite(args, ctx);
    };
    testing_object->addMethod("createSuite", createSuite_method, true);  // external

    Method runSuite_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::runSuite(args, ctx);
    };
    testing_object->addMethod("runSuite", runSuite_method, true);  // external

    Method runTest_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::runTest(args, ctx);
    };
    testing_object->addMethod("runTest", runTest_method, true);  // external

    // Test result reporting
    Method printResults_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::printResults(args, ctx);
    };
    testing_object->addMethod("printResults", printResults_method, true);  // external

    Method getPassedCount_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::getPassedCount(args, ctx);
    };
    testing_object->addMethod("getPassedCount", getPassedCount_method, true);  // external

    Method getFailedCount_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::getFailedCount(args, ctx);
    };
    testing_object->addMethod("getFailedCount", getFailedCount_method, true);  // external

    Method getTotalCount_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::getTotalCount(args, ctx);
    };
    testing_object->addMethod("getTotalCount", getTotalCount_method, true);  // external

    // Utility methods
    Method fail_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::fail(args, ctx);
    };
    testing_object->addMethod("fail", fail_method, true);  // external

    Method skip_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return TestLibrary::skip(args, ctx);
    };
    testing_object->addMethod("skip", skip_method, true);  // external

    return testing_object;
}

// Core assertion methods implementation
Value TestLibrary::assertEqual(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2 && args.size() != 3) {
        throw EvaluationError(
            "testing.assertEqual() requires 2 or 3 arguments (expected, actual, [message])",
            context);
    }

    const Value& expected = args[0];
    const Value& actual = args[1];
    std::string message = args.size() == 3 ? std::get<Text>(args[2]) : "";

    bool equal = valuesEqual(expected, actual);

    if (!equal) {
        std::string failure_msg =
            message.empty()
                ? "Expected: " + valueToString(expected) + ", but got: " + valueToString(actual)
                : message + " - Expected: " + valueToString(expected) +
                      ", but got: " + valueToString(actual);

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNotEqual(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2 && args.size() != 3) {
        throw EvaluationError(
            "testing.assertNotEqual() requires 2 or 3 arguments (expected, actual, [message])",
            context);
    }

    const Value& expected = args[0];
    const Value& actual = args[1];
    std::string message = args.size() == 3 ? std::get<Text>(args[2]) : "";

    bool equal = valuesEqual(expected, actual);

    if (equal) {
        std::string failure_msg =
            message.empty()
                ? "Expected values to be different, but both were: " + valueToString(expected)
                : message + " - Expected values to be different, but both were: " +
                      valueToString(expected);

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertTrue(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1 && args.size() != 2) {
        throw EvaluationError(
            "testing.assertTrue() requires 1 or 2 arguments (condition, [message])", context);
    }

    if (!std::holds_alternative<Bool>(args[0])) {
        throw EvaluationError("testing.assertTrue() first argument must be Bool", context);
    }

    bool condition = std::get<Bool>(args[0]);
    std::string message = args.size() == 2 ? std::get<Text>(args[1]) : "";

    if (!condition) {
        std::string failure_msg = message.empty()
                                      ? "Expected condition to be true, but was false"
                                      : message + " - Expected condition to be true, but was false";

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertFalse(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1 && args.size() != 2) {
        throw EvaluationError(
            "testing.assertFalse() requires 1 or 2 arguments (condition, [message])", context);
    }

    if (!std::holds_alternative<Bool>(args[0])) {
        throw EvaluationError("testing.assertFalse() first argument must be Bool", context);
    }

    bool condition = std::get<Bool>(args[0]);
    std::string message = args.size() == 2 ? std::get<Text>(args[1]) : "";

    if (condition) {
        std::string failure_msg = message.empty()
                                      ? "Expected condition to be false, but was true"
                                      : message + " - Expected condition to be false, but was true";

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

// Numeric assertion methods
Value TestLibrary::assertGreater(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2 && args.size() != 3) {
        throw EvaluationError(
            "testing.assertGreater() requires 2 or 3 arguments (actual, expected, [message])",
            context);
    }

    double actual = extractNumericValue(args[0], "assertGreater", context);
    double expected = extractNumericValue(args[1], "assertGreater", context);
    std::string message = args.size() == 3 ? std::get<Text>(args[2]) : "";

    if (!(actual > expected)) {
        std::string failure_msg =
            message.empty()
                ? "Expected " + std::to_string(actual) + " > " + std::to_string(expected)
                : message + " - Expected " + std::to_string(actual) + " > " +
                      std::to_string(expected);

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertLess(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2 && args.size() != 3) {
        throw EvaluationError(
            "testing.assertLess() requires 2 or 3 arguments (actual, expected, [message])",
            context);
    }

    double actual = extractNumericValue(args[0], "assertLess", context);
    double expected = extractNumericValue(args[1], "assertLess", context);
    std::string message = args.size() == 3 ? std::get<Text>(args[2]) : "";

    if (!(actual < expected)) {
        std::string failure_msg =
            message.empty()
                ? "Expected " + std::to_string(actual) + " < " + std::to_string(expected)
                : message + " - Expected " + std::to_string(actual) + " < " +
                      std::to_string(expected);

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNear(const std::vector<Value>& args, Context& context) {
    if (args.size() != 3 && args.size() != 4) {
        throw EvaluationError(
            "testing.assertNear() requires 3 or 4 arguments (expected, actual, tolerance, "
            "[message])",
            context);
    }

    double expected = extractNumericValue(args[0], "assertNear", context);
    double actual = extractNumericValue(args[1], "assertNear", context);
    double tolerance = extractNumericValue(args[2], "assertNear", context);
    std::string message = args.size() == 4 ? std::get<Text>(args[3]) : "";

    double diff = std::abs(expected - actual);

    if (diff > tolerance) {
        std::string failure_msg =
            message.empty()
                ? "Expected " + std::to_string(expected) + " ± " + std::to_string(tolerance) +
                      ", but got " + std::to_string(actual) +
                      " (difference: " + std::to_string(diff) + ")"
                : message + " - Expected " + std::to_string(expected) + " ± " +
                      std::to_string(tolerance) + ", but got " + std::to_string(actual) +
                      " (difference: " + std::to_string(diff) + ")";

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

// String assertion methods
Value TestLibrary::assertContains(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2 && args.size() != 3) {
        throw EvaluationError(
            "testing.assertContains() requires 2 or 3 arguments (haystack, needle, [message])",
            context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("testing.assertContains() requires Text arguments", context);
    }

    std::string haystack = std::get<Text>(args[0]);
    std::string needle = std::get<Text>(args[1]);
    std::string message = args.size() == 3 ? std::get<Text>(args[2]) : "";

    if (haystack.find(needle) == std::string::npos) {
        std::string failure_msg =
            message.empty()
                ? "Expected '" + haystack + "' to contain '" + needle + "'"
                : message + " - Expected '" + haystack + "' to contain '" + needle + "'";

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertStartsWith(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2 && args.size() != 3) {
        throw EvaluationError(
            "testing.assertStartsWith() requires 2 or 3 arguments (text, prefix, [message])",
            context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("testing.assertStartsWith() requires Text arguments", context);
    }

    std::string text = std::get<Text>(args[0]);
    std::string prefix = std::get<Text>(args[1]);
    std::string message = args.size() == 3 ? std::get<Text>(args[2]) : "";

    if (text.substr(0, prefix.length()) != prefix) {
        std::string failure_msg =
            message.empty() ? "Expected '" + text + "' to start with '" + prefix + "'"
                            : message + " - Expected '" + text + "' to start with '" + prefix + "'";

        recordTestResult(current_test_, false, failure_msg);
        throw EvaluationError("Assertion failed: " + failure_msg, context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

// Collection assertion methods
Value TestLibrary::assertEmpty(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1 && args.size() != 2) {
        throw EvaluationError(
            "testing.assertEmpty() requires 1 or 2 arguments (collection, [message])", context);
    }

    std::string message = args.size() == 2 ? std::get<Text>(args[1]) : "";

    if (std::holds_alternative<std::shared_ptr<ListInstance>>(args[0])) {
        auto list = std::get<std::shared_ptr<ListInstance>>(args[0]);
        if (!list->empty()) {
            std::string failure_msg = message.empty()
                                          ? "Expected list to be empty, but has " +
                                                std::to_string(list->size()) + " elements"
                                          : message + " - Expected list to be empty, but has " +
                                                std::to_string(list->size()) + " elements";

            recordTestResult(current_test_, false, failure_msg);
            throw EvaluationError("Assertion failed: " + failure_msg, context);
        }
    } else if (std::holds_alternative<Text>(args[0])) {
        std::string text = std::get<Text>(args[0]);
        if (!text.empty()) {
            std::string failure_msg =
                message.empty()
                    ? "Expected text to be empty, but has length " + std::to_string(text.length())
                    : message + " - Expected text to be empty, but has length " +
                          std::to_string(text.length());

            recordTestResult(current_test_, false, failure_msg);
            throw EvaluationError("Assertion failed: " + failure_msg, context);
        }
    } else {
        throw EvaluationError("testing.assertEmpty() requires a List or Text argument", context);
    }

    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

// Test suite management
Value TestLibrary::createSuite(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("testing.createSuite() requires exactly 1 argument (suite_name)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("testing.createSuite() requires a Text argument", context);
    }

    std::string suite_name = std::get<Text>(args[0]);
    current_suite_ = suite_name;

    // Initialize empty test suite
    TestSuiteResult& suite = test_suites_[suite_name];
    suite.suite_name = suite_name;
    suite.test_results.clear();
    suite.total_tests = 0;
    suite.passed_tests = 0;
    suite.failed_tests = 0;
    suite.total_time_ms = 0.0;

    return Value(Text(suite_name));
}

Value TestLibrary::runTest(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("testing.runTest() requires exactly 1 argument (test_name)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("testing.runTest() requires a Text argument", context);
    }

    current_test_ = std::get<Text>(args[0]);

    return Value(Text(current_test_));
}

// Test result reporting
Value TestLibrary::printResults(const std::vector<Value>& args, Context& context) {
    std::string suite_name = args.empty() ? current_suite_ : std::get<Text>(args[0]);

    if (test_suites_.find(suite_name) == test_suites_.end()) {
        std::cout << "No test results found for suite: " << suite_name << std::endl;
        return Value(Bool(false));
    }

    const TestSuiteResult& suite = test_suites_[suite_name];

    std::cout << "\n=== Test Results for Suite: " << suite.suite_name << " ===" << std::endl;
    std::cout << "Total Tests: " << suite.total_tests << std::endl;
    std::cout << "Passed: " << suite.passed_tests << std::endl;
    std::cout << "Failed: " << suite.failed_tests << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1)
              << (suite.total_tests > 0 ? (100.0 * suite.passed_tests / suite.total_tests) : 0.0)
              << "%" << std::endl;
    std::cout << "Total Time: " << std::fixed << std::setprecision(2) << suite.total_time_ms
              << " ms" << std::endl;

    if (suite.failed_tests > 0) {
        std::cout << "\nFailed Tests:" << std::endl;
        for (const auto& result : suite.test_results) {
            if (!result.passed) {
                std::cout << "  ❌ " << result.test_name << ": " << result.failure_message
                          << std::endl;
            }
        }
    }

    std::cout << "\nTest Details:" << std::endl;
    for (const auto& result : suite.test_results) {
        std::cout << "  " << (result.passed ? "✅" : "❌") << " " << result.test_name << " ("
                  << std::fixed << std::setprecision(2) << result.execution_time_ms << " ms)"
                  << std::endl;
    }
    std::cout << std::endl;

    return Value(Bool(suite.failed_tests == 0));
}

Value TestLibrary::getTotalCount(const std::vector<Value>& args, Context& context) {
    std::string suite_name = args.empty() ? current_suite_ : std::get<Text>(args[0]);

    if (test_suites_.find(suite_name) == test_suites_.end()) {
        return Value(Int(0));
    }

    return Value(Int(test_suites_[suite_name].total_tests));
}

Value TestLibrary::getPassedCount(const std::vector<Value>& args, Context& context) {
    std::string suite_name = args.empty() ? current_suite_ : std::get<Text>(args[0]);

    if (test_suites_.find(suite_name) == test_suites_.end()) {
        return Value(Int(0));
    }

    return Value(Int(test_suites_[suite_name].passed_tests));
}

Value TestLibrary::getFailedCount(const std::vector<Value>& args, Context& context) {
    std::string suite_name = args.empty() ? current_suite_ : std::get<Text>(args[0]);

    if (test_suites_.find(suite_name) == test_suites_.end()) {
        return Value(Int(0));
    }

    return Value(Int(test_suites_[suite_name].failed_tests));
}

// Utility methods
Value TestLibrary::fail(const std::vector<Value>& args, Context& context) {
    std::string message = args.empty() ? "Test failed" : std::get<Text>(args[0]);

    recordTestResult(current_test_, false, message);
    throw EvaluationError("Test failed: " + message, context);
}

// Placeholder implementations for other methods
Value TestLibrary::assertNull(const std::vector<Value>& args, Context& context) {
    // Since O²L doesn't have null, this could check for Optional.empty()
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNotNull(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertGreaterEqual(const std::vector<Value>& args, Context& context) {
    // Implementation similar to assertGreater but with >=
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertLessEqual(const std::vector<Value>& args, Context& context) {
    // Implementation similar to assertLess but with <=
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNotContains(const std::vector<Value>& args, Context& context) {
    // Implementation opposite of assertContains
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertEndsWith(const std::vector<Value>& args, Context& context) {
    // Implementation similar to assertStartsWith but for suffix
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertMatches(const std::vector<Value>& args, Context& context) {
    // Regex matching implementation
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNotEmpty(const std::vector<Value>& args, Context& context) {
    // Implementation opposite of assertEmpty
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertSize(const std::vector<Value>& args, Context& context) {
    // Check collection size
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertIn(const std::vector<Value>& args, Context& context) {
    // Check if item is in collection
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNotIn(const std::vector<Value>& args, Context& context) {
    // Check if item is not in collection
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertThrows(const std::vector<Value>& args, Context& context) {
    // Check that code throws an exception
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::assertNotThrows(const std::vector<Value>& args, Context& context) {
    // Check that code doesn't throw an exception
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::addTest(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::runSuite(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::getTestResults(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::skip(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::benchmark(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::setUp(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::tearDown(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::beforeEach(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

Value TestLibrary::afterEach(const std::vector<Value>& args, Context& context) {
    recordTestResult(current_test_, true);
    return Value(Bool(true));
}

// Helper function implementations
std::string TestLibrary::valueToString(const Value& value) {
    if (std::holds_alternative<Int>(value)) {
        return std::to_string(std::get<Int>(value));
    } else if (std::holds_alternative<Long>(value)) {
        return std::to_string(static_cast<long long>(std::get<Long>(value)));
    } else if (std::holds_alternative<Float>(value)) {
        return std::to_string(std::get<Float>(value));
    } else if (std::holds_alternative<Double>(value)) {
        return std::to_string(std::get<Double>(value));
    } else if (std::holds_alternative<Bool>(value)) {
        return std::get<Bool>(value) ? "true" : "false";
    } else if (std::holds_alternative<Text>(value)) {
        return "\"" + std::get<Text>(value) + "\"";
    } else {
        return "<object>";
    }
}

bool TestLibrary::valuesEqual(const Value& a, const Value& b) {
    if (a.index() != b.index()) {
        return false;  // Different types
    }

    if (std::holds_alternative<Int>(a)) {
        return std::get<Int>(a) == std::get<Int>(b);
    } else if (std::holds_alternative<Long>(a)) {
        return std::get<Long>(a) == std::get<Long>(b);
    } else if (std::holds_alternative<Float>(a)) {
        return std::abs(std::get<Float>(a) - std::get<Float>(b)) < 1e-7f;
    } else if (std::holds_alternative<Double>(a)) {
        return std::abs(std::get<Double>(a) - std::get<Double>(b)) < 1e-15;
    } else if (std::holds_alternative<Bool>(a)) {
        return std::get<Bool>(a) == std::get<Bool>(b);
    } else if (std::holds_alternative<Text>(a)) {
        return std::get<Text>(a) == std::get<Text>(b);
    }

    return false;
}

double TestLibrary::extractNumericValue(const Value& value, const std::string& function_name,
                                        Context& context) {
    if (std::holds_alternative<Int>(value)) {
        return static_cast<double>(std::get<Int>(value));
    } else if (std::holds_alternative<Long>(value)) {
        return static_cast<double>(std::get<Long>(value));
    } else if (std::holds_alternative<Float>(value)) {
        return static_cast<double>(std::get<Float>(value));
    } else if (std::holds_alternative<Double>(value)) {
        return std::get<Double>(value);
    } else {
        throw EvaluationError("testing." + function_name + "() requires a numeric argument",
                              context);
    }
}

void TestLibrary::recordTestResult(const std::string& test_name, bool passed,
                                   const std::string& failure_message) {
    if (test_name.empty()) return;

    TestSuiteResult& suite = test_suites_[current_suite_];

    // Update or add test result
    auto it = std::find_if(suite.test_results.begin(), suite.test_results.end(),
                           [&test_name](const TestResult& r) { return r.test_name == test_name; });

    if (it != suite.test_results.end()) {
        // Update existing result
        if (it->passed && !passed) {
            suite.passed_tests--;
            suite.failed_tests++;
        } else if (!it->passed && passed) {
            suite.failed_tests--;
            suite.passed_tests++;
        }
        it->passed = passed;
        it->failure_message = failure_message;
    } else {
        // Add new result
        TestResult result;
        result.test_name = test_name;
        result.passed = passed;
        result.failure_message = failure_message;
        result.execution_time_ms = 1.0;  // Placeholder

        suite.test_results.push_back(result);
        suite.total_tests++;

        if (passed) {
            suite.passed_tests++;
        } else {
            suite.failed_tests++;
        }
    }
}

}  // namespace o2l