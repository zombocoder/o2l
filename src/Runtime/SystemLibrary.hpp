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

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

class SystemLibrary {
   public:
    // Create the system.io object with native methods
    static std::shared_ptr<ObjectInstance> createIOObject();

    // Create the system.os object with native methods
    static std::shared_ptr<ObjectInstance> createOSObject();

    // Create the system.utils object with utility methods
    static std::shared_ptr<ObjectInstance> createUtilsObject();

    // Create the system.fs object with filesystem methods
    static std::shared_ptr<ObjectInstance> createFSObject();

    // Native print function implementation
    static Value nativePrint(const std::vector<Value>& args, Context& context);

    // Native input function implementation
    static Value nativeInput(const std::vector<Value>& args, Context& context);

    // Native getEnv function implementation
    static Value nativeGetEnv(const std::vector<Value>& args, Context& context);

    // Native repeat function implementation
    static Value nativeRepeat(const std::vector<Value>& args, Context& context);

    // Native args function implementation - get command line arguments
    static Value nativeGetArgs(const std::vector<Value>& args, Context& context);

    // Native argc function implementation - get argument count
    static Value nativeGetArgc(const std::vector<Value>& args, Context& context);

    // Native system information function implementations
    static Value nativeGetOSName(const std::vector<Value>& args, Context& context);
    static Value nativeGetOSVersion(const std::vector<Value>& args, Context& context);
    static Value nativeGetArchitecture(const std::vector<Value>& args, Context& context);
    static Value nativeGetHostname(const std::vector<Value>& args, Context& context);
    static Value nativeGetUsername(const std::vector<Value>& args, Context& context);
    static Value nativeGetCurrentDir(const std::vector<Value>& args, Context& context);
    static Value nativeGetProcessId(const std::vector<Value>& args, Context& context);
    static Value nativeGetParentProcessId(const std::vector<Value>& args, Context& context);
    static Value nativeGetUptime(const std::vector<Value>& args, Context& context);

    // Native memory information function implementations
    static Value nativeGetTotalMemory(const std::vector<Value>& args, Context& context);
    static Value nativeGetAvailableMemory(const std::vector<Value>& args, Context& context);
    static Value nativeGetUsedMemory(const std::vector<Value>& args, Context& context);
    static Value nativeGetMemoryUsage(const std::vector<Value>& args, Context& context);

    // Native CPU information function implementations
    static Value nativeGetCPUCount(const std::vector<Value>& args, Context& context);
    static Value nativeGetCPUModel(const std::vector<Value>& args, Context& context);
    static Value nativeGetCPUUsage(const std::vector<Value>& args, Context& context);
    static Value nativeGetLoadAverage(const std::vector<Value>& args, Context& context);

    // Native command execution function implementations
    static Value nativeExecute(const std::vector<Value>& args, Context& context);
    static Value nativeExecuteWithOutput(const std::vector<Value>& args, Context& context);
    static Value nativeExecuteWithTimeout(const std::vector<Value>& args, Context& context);
    static Value nativeExecuteAsync(const std::vector<Value>& args, Context& context);

    // Native filesystem function implementations
    static Value nativeReadText(const std::vector<Value>& args, Context& context);
    static Value nativeWriteText(const std::vector<Value>& args, Context& context);
    static Value nativeExists(const std::vector<Value>& args, Context& context);
    static Value nativeIsFile(const std::vector<Value>& args, Context& context);
    static Value nativeIsDirectory(const std::vector<Value>& args, Context& context);
    static Value nativeListFiles(const std::vector<Value>& args, Context& context);
    static Value nativeCreateDirectory(const std::vector<Value>& args, Context& context);
    static Value nativeDeleteFile(const std::vector<Value>& args, Context& context);

    // Native path manipulation function implementations
    static Value nativeBasename(const std::vector<Value>& args, Context& context);
    static Value nativeDirname(const std::vector<Value>& args, Context& context);
    static Value nativeExtname(const std::vector<Value>& args, Context& context);
    static Value nativeJoin(const std::vector<Value>& args, Context& context);
    static Value nativeNormalize(const std::vector<Value>& args, Context& context);
    static Value nativeResolve(const std::vector<Value>& args, Context& context);
    static Value nativeRelative(const std::vector<Value>& args, Context& context);
    static Value nativeIsAbsolute(const std::vector<Value>& args, Context& context);
    static Value nativeSplitPath(const std::vector<Value>& args, Context& context);
    static Value nativeGetParent(const std::vector<Value>& args, Context& context);
    static Value nativeChangeExtension(const std::vector<Value>& args, Context& context);

   private:
    // Helper function for string formatting
    static std::string formatString(const std::string& format, const std::vector<Value>& args);

    // Helper to convert Value to string for printing
    static std::string valueToDisplayString(const Value& value);

    // Helper to convert Value to string for %o format specifier (objects/records/enums)
    static std::string valueToObjectString(const Value& value);

    // Helper functions for system information
    static std::string executeSystemCommand(const std::string& command);
    static Long getMemoryInfoFromProcMeminfo(const std::string& field);
    static Double getCPUUsageFromProcStat();
    static std::string getCPUModelFromProcCpuinfo();
    static std::vector<Double> getLoadAverageFromProcLoadavg();
};

}  // namespace o2l