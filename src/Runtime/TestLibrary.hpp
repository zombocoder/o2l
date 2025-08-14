#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// Forward declaration for test results
struct TestResult {
    std::string test_name;
    bool passed;
    std::string failure_message;
    double execution_time_ms;
};

struct TestSuiteResult {
    std::string suite_name;
    std::vector<TestResult> test_results;
    int total_tests;
    int passed_tests;
    int failed_tests;
    double total_time_ms;
};

class TestLibrary {
   public:
    // Create the testing object with native methods
    static std::shared_ptr<ObjectInstance> createTestingObject();

    // Core assertion methods
    static Value assertEqual(const std::vector<Value>& args, Context& context);
    static Value assertNotEqual(const std::vector<Value>& args, Context& context);
    static Value assertTrue(const std::vector<Value>& args, Context& context);
    static Value assertFalse(const std::vector<Value>& args, Context& context);
    static Value assertNull(const std::vector<Value>& args, Context& context);
    static Value assertNotNull(const std::vector<Value>& args, Context& context);

    // Numeric assertion methods
    static Value assertGreater(const std::vector<Value>& args, Context& context);
    static Value assertGreaterEqual(const std::vector<Value>& args, Context& context);
    static Value assertLess(const std::vector<Value>& args, Context& context);
    static Value assertLessEqual(const std::vector<Value>& args, Context& context);
    static Value assertNear(const std::vector<Value>& args, Context& context);

    // String assertion methods
    static Value assertContains(const std::vector<Value>& args, Context& context);
    static Value assertNotContains(const std::vector<Value>& args, Context& context);
    static Value assertStartsWith(const std::vector<Value>& args, Context& context);
    static Value assertEndsWith(const std::vector<Value>& args, Context& context);
    static Value assertMatches(const std::vector<Value>& args, Context& context);

    // Collection assertion methods
    static Value assertEmpty(const std::vector<Value>& args, Context& context);
    static Value assertNotEmpty(const std::vector<Value>& args, Context& context);
    static Value assertSize(const std::vector<Value>& args, Context& context);
    static Value assertIn(const std::vector<Value>& args, Context& context);
    static Value assertNotIn(const std::vector<Value>& args, Context& context);

    // Exception assertion methods
    static Value assertThrows(const std::vector<Value>& args, Context& context);
    static Value assertNotThrows(const std::vector<Value>& args, Context& context);

    // Test suite management methods
    static Value createSuite(const std::vector<Value>& args, Context& context);
    static Value addTest(const std::vector<Value>& args, Context& context);
    static Value runSuite(const std::vector<Value>& args, Context& context);
    static Value runTest(const std::vector<Value>& args, Context& context);

    // Test result and reporting methods
    static Value getTestResults(const std::vector<Value>& args, Context& context);
    static Value printResults(const std::vector<Value>& args, Context& context);
    static Value getPassedCount(const std::vector<Value>& args, Context& context);
    static Value getFailedCount(const std::vector<Value>& args, Context& context);
    static Value getTotalCount(const std::vector<Value>& args, Context& context);

    // Utility methods
    static Value skip(const std::vector<Value>& args, Context& context);
    static Value fail(const std::vector<Value>& args, Context& context);
    static Value benchmark(const std::vector<Value>& args, Context& context);

    // Test lifecycle methods
    static Value setUp(const std::vector<Value>& args, Context& context);
    static Value tearDown(const std::vector<Value>& args, Context& context);
    static Value beforeEach(const std::vector<Value>& args, Context& context);
    static Value afterEach(const std::vector<Value>& args, Context& context);

   private:
    // Helper functions
    static std::string valueToString(const Value& value);
    static bool valuesEqual(const Value& a, const Value& b);
    static double extractNumericValue(const Value& value, const std::string& function_name,
                                      Context& context);
    static void recordTestResult(const std::string& test_name, bool passed,
                                 const std::string& failure_message = "");

    // Static test state management
    static std::map<std::string, TestSuiteResult> test_suites_;
    static std::string current_suite_;
    static std::string current_test_;
};

}  // namespace o2l