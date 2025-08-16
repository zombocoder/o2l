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
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/SystemLibrary.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

class SystemOSExtendedTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to create system.os object and call methods
    Value callOSMethod(const std::string& method_name, const std::vector<Value>& args = {}) {
        auto os_object = SystemLibrary::createOSObject();
        EXPECT_TRUE(os_object->hasMethod(method_name)) << "Method " << method_name << " not found";
        return os_object->callMethod(method_name, args, context);
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

    // Helper to check if result is a non-empty text
    bool isNonEmptyText(const Value& result) {
        if (!std::holds_alternative<Text>(result)) {
            return false;
        }
        std::string text = std::get<Text>(result);
        return !text.empty() && text != "unknown" && text != "Unknown";
    }
};

// Test system information methods
TEST_F(SystemOSExtendedTest, SystemInformation) {
    // Test OS name
    Value os_name = callOSMethod("getOSName");
    EXPECT_TRUE(isNonEmptyText(os_name));

    // Test OS version
    Value os_version = callOSMethod("getOSVersion");
    EXPECT_TRUE(isNonEmptyText(os_version));

    // Test architecture
    Value architecture = callOSMethod("getArchitecture");
    EXPECT_TRUE(isNonEmptyText(architecture));

    // Test hostname
    Value hostname = callOSMethod("getHostname");
    EXPECT_TRUE(isNonEmptyText(hostname));

    // Test username
    Value username = callOSMethod("getUsername");
    EXPECT_TRUE(isNonEmptyText(username));

    // Test current directory
    Value current_dir = callOSMethod("getCurrentDir");
    EXPECT_TRUE(isNonEmptyText(current_dir));

    // Test argument validation
    EXPECT_THROW(callOSMethod("getOSName", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getOSVersion", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getArchitecture", {Value(Int(1))}), EvaluationError);
}

// Test process information methods
TEST_F(SystemOSExtendedTest, ProcessInformation) {
    // Test process ID
    Value pid = callOSMethod("getProcessId");
    ASSERT_TRUE(std::holds_alternative<Int>(pid));
    EXPECT_GT(std::get<Int>(pid), 0);

    // Test parent process ID
    Value ppid = callOSMethod("getParentProcessId");
    ASSERT_TRUE(std::holds_alternative<Int>(ppid));
    EXPECT_GE(std::get<Int>(ppid), 0);

    // Test uptime
    Value uptime = callOSMethod("getUptime");
    ASSERT_TRUE(std::holds_alternative<Long>(uptime));
    EXPECT_GE(std::get<Long>(uptime), 0);

    // Test argument validation
    EXPECT_THROW(callOSMethod("getProcessId", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getParentProcessId", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getUptime", {Value(Int(1))}), EvaluationError);
}

// Test memory information methods
TEST_F(SystemOSExtendedTest, MemoryInformation) {
    // Test total memory
    Value total_memory = callOSMethod("getTotalMemory");
    ASSERT_TRUE(std::holds_alternative<Long>(total_memory));
    EXPECT_GE(std::get<Long>(total_memory), 0);

    // Test available memory
    Value available_memory = callOSMethod("getAvailableMemory");
    ASSERT_TRUE(std::holds_alternative<Long>(available_memory));
    EXPECT_GE(std::get<Long>(available_memory), 0);

    // Test used memory
    Value used_memory = callOSMethod("getUsedMemory");
    ASSERT_TRUE(std::holds_alternative<Long>(used_memory));
    EXPECT_GE(std::get<Long>(used_memory), 0);

    // Test memory usage percentage
    Value memory_usage = callOSMethod("getMemoryUsage");
    ASSERT_TRUE(std::holds_alternative<Double>(memory_usage));
    EXPECT_GE(std::get<Double>(memory_usage), 0.0);
    EXPECT_LE(std::get<Double>(memory_usage), 100.0);

    // Test argument validation
    EXPECT_THROW(callOSMethod("getTotalMemory", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getAvailableMemory", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getUsedMemory", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getMemoryUsage", {Value(Int(1))}), EvaluationError);
}

// Test CPU information methods
TEST_F(SystemOSExtendedTest, CPUInformation) {
    // Test CPU count
    Value cpu_count = callOSMethod("getCPUCount");
    ASSERT_TRUE(std::holds_alternative<Int>(cpu_count));
    EXPECT_GT(std::get<Int>(cpu_count), 0);

    // Test CPU model
    Value cpu_model = callOSMethod("getCPUModel");
    EXPECT_TRUE(isNonEmptyText(cpu_model));

    // Test CPU usage
    Value cpu_usage = callOSMethod("getCPUUsage");
    ASSERT_TRUE(std::holds_alternative<Double>(cpu_usage));
    EXPECT_GE(std::get<Double>(cpu_usage), 0.0);
    EXPECT_LE(std::get<Double>(cpu_usage), 100.0);

    // Test load average
    Value load_average = callOSMethod("getLoadAverage");
    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(load_average));
    auto load_list = std::get<std::shared_ptr<ListInstance>>(load_average);
    EXPECT_EQ(load_list->size(), 3);  // Should have 3 values (1, 5, 15 minute averages)

    // Test argument validation
    EXPECT_THROW(callOSMethod("getCPUCount", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getCPUModel", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getCPUUsage", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callOSMethod("getLoadAverage", {Value(Int(1))}), EvaluationError);
}

// Test command execution methods
TEST_F(SystemOSExtendedTest, CommandExecution) {
    // Test basic execute
    Value exit_code = callOSMethod("execute", {Value(Text("echo 'test'"))});
    ASSERT_TRUE(std::holds_alternative<Int>(exit_code));
    EXPECT_EQ(std::get<Int>(exit_code), 0);

    // Test execute with output
    Value output = callOSMethod("executeWithOutput", {Value(Text("echo 'hello world'"))});
    ASSERT_TRUE(std::holds_alternative<Text>(output));
    std::string output_str = std::get<Text>(output);
    EXPECT_TRUE(output_str.find("hello world") != std::string::npos);

    // Test execute with timeout
    Value timeout_output =
        callOSMethod("executeWithTimeout", {Value(Text("echo 'timeout test'")), Value(Int(5))});
    ASSERT_TRUE(std::holds_alternative<Text>(timeout_output));

    // Test async execute
    Value async_result = callOSMethod(
        "executeAsync", {Value(Text("echo 'async test' > /tmp/o2l_test_async.txt"))});
    ASSERT_TRUE(std::holds_alternative<Int>(async_result));

    // Test argument validation
    EXPECT_THROW(callOSMethod("execute", {}), EvaluationError);
    EXPECT_THROW(callOSMethod("executeWithOutput", {}), EvaluationError);
    EXPECT_THROW(callOSMethod("executeWithTimeout", {Value(Text("test"))}), EvaluationError);
    EXPECT_THROW(callOSMethod("executeAsync", {}), EvaluationError);

    EXPECT_THROW(callOSMethod("execute", {Value(Int(123))}), EvaluationError);
    EXPECT_THROW(callOSMethod("executeWithOutput", {Value(Int(123))}), EvaluationError);
    EXPECT_THROW(callOSMethod("executeWithTimeout", {Value(Int(123)), Value(Int(5))}),
                 EvaluationError);
    EXPECT_THROW(callOSMethod("executeWithTimeout", {Value(Text("test")), Value(Text("invalid"))}),
                 EvaluationError);
}

// Test method availability
TEST_F(SystemOSExtendedTest, MethodAvailability) {
    auto os_object = SystemLibrary::createOSObject();

    // Test that all expected methods are available
    std::vector<std::string> expected_methods = {
        // Original methods
        "getEnv", "args", "argc",
        // System information methods
        "getOSName", "getOSVersion", "getArchitecture", "getHostname", "getUsername",
        "getCurrentDir", "getProcessId", "getParentProcessId", "getUptime",
        // Memory information methods
        "getTotalMemory", "getAvailableMemory", "getUsedMemory", "getMemoryUsage",
        // CPU information methods
        "getCPUCount", "getCPUModel", "getCPUUsage", "getLoadAverage",
        // Command execution methods
        "execute", "executeWithOutput", "executeWithTimeout", "executeAsync"};

    for (const auto& method : expected_methods) {
        EXPECT_TRUE(os_object->hasMethod(method)) << "Method " << method << " not found";
        EXPECT_TRUE(os_object->isMethodExternal(method)) << "Method " << method << " not external";
    }
}

// Test error handling
TEST_F(SystemOSExtendedTest, ErrorHandling) {
    // Test invalid command - should return non-zero exit code, not throw
    Value invalid_result = callOSMethod("execute", {Value(Text("nonexistent-command-12345"))});
    ASSERT_TRUE(std::holds_alternative<Int>(invalid_result));
    EXPECT_NE(std::get<Int>(invalid_result), 0);  // Should return non-zero exit code

    // These shouldn't throw but should return safe values
    Value memory_usage = callOSMethod("getMemoryUsage");
    ASSERT_TRUE(std::holds_alternative<Double>(memory_usage));

    Value cpu_usage = callOSMethod("getCPUUsage");
    ASSERT_TRUE(std::holds_alternative<Double>(cpu_usage));

    Value load_average = callOSMethod("getLoadAverage");
    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(load_average));
}