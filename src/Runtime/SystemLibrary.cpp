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

#include "SystemLibrary.hpp"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <thread>

#include "../Common/Exceptions.hpp"
#include "EnumInstance.hpp"
#include "ListInstance.hpp"
#include "MapInstance.hpp"
#include "MapIterator.hpp"
#include "MapObject.hpp"
#include "RecordInstance.hpp"
#include "RecordType.hpp"
#include "RepeatIterator.hpp"
#include "SetInstance.hpp"
#include "SetIterator.hpp"
#include "Value.hpp"

// Platform-specific includes
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define popen _popen
#define pclose _pclose
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

// Forward declaration of longToString from Value.cpp
extern std::string longToString(o2l::Long value);

namespace o2l {

std::shared_ptr<ObjectInstance> SystemLibrary::createIOObject() {
    auto io_object = std::make_shared<ObjectInstance>("io");

    // Add native print method
    Method print_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativePrint(args, ctx);
    };
    io_object->addMethod("print", print_method, true);  // external

    // Add native input method
    Method input_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeInput(args, ctx);
    };
    io_object->addMethod("input", input_method, true);  // external

    return io_object;
}

std::shared_ptr<ObjectInstance> SystemLibrary::createOSObject() {
    auto os_object = std::make_shared<ObjectInstance>("os");

    // Add native getEnv method
    Method getEnv_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeGetEnv(args, ctx);
    };
    os_object->addMethod("getEnv", getEnv_method, true);  // external

    // Add native args method - get command line arguments as List<Text>
    Method args_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeGetArgs(args, ctx);
    };
    os_object->addMethod("args", args_method, true);  // external

    // Add native argc method - get argument count
    Method argc_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeGetArgc(args, ctx);
    };
    os_object->addMethod("argc", argc_method, true);  // external

    // Add system information methods
    os_object->addMethod(
        "getOSName",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetOSName(args, ctx);
        },
        true);

    os_object->addMethod(
        "getOSVersion",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetOSVersion(args, ctx);
        },
        true);

    os_object->addMethod(
        "getArchitecture",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetArchitecture(args, ctx);
        },
        true);

    os_object->addMethod(
        "getHostname",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetHostname(args, ctx);
        },
        true);

    os_object->addMethod(
        "getUsername",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetUsername(args, ctx);
        },
        true);

    os_object->addMethod(
        "getCurrentDir",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetCurrentDir(args, ctx);
        },
        true);

    os_object->addMethod(
        "getProcessId",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetProcessId(args, ctx);
        },
        true);

    os_object->addMethod(
        "getParentProcessId",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetParentProcessId(args, ctx);
        },
        true);

    os_object->addMethod(
        "getUptime",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetUptime(args, ctx);
        },
        true);

    // Add memory information methods
    os_object->addMethod(
        "getTotalMemory",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetTotalMemory(args, ctx);
        },
        true);

    os_object->addMethod(
        "getAvailableMemory",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetAvailableMemory(args, ctx);
        },
        true);

    os_object->addMethod(
        "getUsedMemory",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetUsedMemory(args, ctx);
        },
        true);

    os_object->addMethod(
        "getMemoryUsage",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetMemoryUsage(args, ctx);
        },
        true);

    // Add CPU information methods
    os_object->addMethod(
        "getCPUCount",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetCPUCount(args, ctx);
        },
        true);

    os_object->addMethod(
        "getCPUModel",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetCPUModel(args, ctx);
        },
        true);

    os_object->addMethod(
        "getCPUUsage",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetCPUUsage(args, ctx);
        },
        true);

    os_object->addMethod(
        "getLoadAverage",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeGetLoadAverage(args, ctx);
        },
        true);

    // Add command execution methods
    os_object->addMethod(
        "execute",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeExecute(args, ctx);
        },
        true);

    os_object->addMethod(
        "executeWithOutput",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeExecuteWithOutput(args, ctx);
        },
        true);

    os_object->addMethod(
        "executeWithTimeout",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeExecuteWithTimeout(args, ctx);
        },
        true);

    os_object->addMethod(
        "executeAsync",
        [](const std::vector<Value>& args, Context& ctx) -> Value {
            return SystemLibrary::nativeExecuteAsync(args, ctx);
        },
        true);

    return os_object;
}

std::shared_ptr<ObjectInstance> SystemLibrary::createUtilsObject() {
    auto utils_object = std::make_shared<ObjectInstance>("utils");

    // Add native repeat method
    Method repeat_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeRepeat(args, ctx);
    };
    utils_object->addMethod("repeat", repeat_method, true);  // external

    return utils_object;
}

std::shared_ptr<ObjectInstance> SystemLibrary::createFSObject() {
    auto fs_object = std::make_shared<ObjectInstance>("fs");

    // Add native readText method
    Method readText_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeReadText(args, ctx);
    };
    fs_object->addMethod("readText", readText_method, true);  // external

    // Add native writeText method
    Method writeText_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeWriteText(args, ctx);
    };
    fs_object->addMethod("writeText", writeText_method, true);  // external

    // Add native exists method
    Method exists_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeExists(args, ctx);
    };
    fs_object->addMethod("exists", exists_method, true);  // external

    // Add native isFile method
    Method isFile_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeIsFile(args, ctx);
    };
    fs_object->addMethod("isFile", isFile_method, true);  // external

    // Add native isDirectory method
    Method isDirectory_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeIsDirectory(args, ctx);
    };
    fs_object->addMethod("isDirectory", isDirectory_method, true);  // external

    // Add native listFiles method
    Method listFiles_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeListFiles(args, ctx);
    };
    fs_object->addMethod("listFiles", listFiles_method, true);  // external

    // Add native createDirectory method
    Method createDirectory_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeCreateDirectory(args, ctx);
    };
    fs_object->addMethod("createDirectory", createDirectory_method, true);  // external

    // Add native deleteFile method
    Method deleteFile_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeDeleteFile(args, ctx);
    };
    fs_object->addMethod("deleteFile", deleteFile_method, true);  // external

    // Add native path manipulation methods
    Method basename_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeBasename(args, ctx);
    };
    fs_object->addMethod("basename", basename_method, true);  // external

    Method dirname_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeDirname(args, ctx);
    };
    fs_object->addMethod("dirname", dirname_method, true);  // external

    Method extname_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeExtname(args, ctx);
    };
    fs_object->addMethod("extname", extname_method, true);  // external

    Method join_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeJoin(args, ctx);
    };
    fs_object->addMethod("join", join_method, true);  // external

    Method normalize_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeNormalize(args, ctx);
    };
    fs_object->addMethod("normalize", normalize_method, true);  // external

    Method resolve_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeResolve(args, ctx);
    };
    fs_object->addMethod("resolve", resolve_method, true);  // external

    Method relative_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeRelative(args, ctx);
    };
    fs_object->addMethod("relative", relative_method, true);  // external

    Method isAbsolute_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeIsAbsolute(args, ctx);
    };
    fs_object->addMethod("isAbsolute", isAbsolute_method, true);  // external

    Method splitPath_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeSplitPath(args, ctx);
    };
    fs_object->addMethod("splitPath", splitPath_method, true);  // external

    Method getParent_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeGetParent(args, ctx);
    };
    fs_object->addMethod("getParent", getParent_method, true);  // external

    Method changeExtension_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return SystemLibrary::nativeChangeExtension(args, ctx);
    };
    fs_object->addMethod("changeExtension", changeExtension_method, true);  // external

    return fs_object;
}

Value SystemLibrary::nativePrint(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        std::cout << std::endl;
        return Text("");
    }

    // First argument is the format string
    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("print() first argument must be a Text (format string)");
    }

    std::string format = std::get<Text>(args[0]);

    // If only format string provided, print it directly
    if (args.size() == 1) {
        std::cout << format << std::endl;
        return Text(format);
    }

    // Format string with additional arguments
    std::vector<Value> format_args(args.begin() + 1, args.end());
    std::string formatted = formatString(format, format_args);

    std::cout << formatted << std::endl;
    return Text(formatted);
}

Value SystemLibrary::nativeInput(const std::vector<Value>& args, Context& context) {
    // Optional prompt message
    if (!args.empty()) {
        if (!std::holds_alternative<Text>(args[0])) {
            throw EvaluationError("input() argument must be a Text (prompt)");
        }
        std::string prompt = std::get<Text>(args[0]);
        std::cout << prompt;
    }

    // Read line from stdin
    std::string input_line;
    std::getline(std::cin, input_line);

    return Text(input_line);
}

std::string SystemLibrary::formatString(const std::string& format, const std::vector<Value>& args) {
    std::string result;
    size_t arg_index = 0;

    for (size_t i = 0; i < format.length(); ++i) {
        if (format[i] == '%' && i + 1 < format.length()) {
            // Handle %% escape sequence specially
            if (format[i + 1] == '%') {
                result += "%";
                i++;  // Skip the second %, the loop will increment i again
                continue;
            }

            if (arg_index < args.size()) {
                std::string replacement;
                size_t format_start = i;
                i++;  // Move past the %

                // Parse precision specifier (e.g., .1 in %.1f)
                int precision = -1;
                if (i < format.length() && format[i] == '.') {
                    i++;  // Skip the dot
                    std::string precision_str;
                    while (i < format.length() && std::isdigit(format[i])) {
                        precision_str += format[i];
                        i++;
                    }
                    if (!precision_str.empty()) {
                        precision = std::stoi(precision_str);
                    }
                }

                // Get the format character
                if (i < format.length()) {
                    char format_char = format[i];

                    switch (format_char) {
                        case 's':
                            // String format - convert any value to string
                            replacement = valueToDisplayString(args[arg_index]);
                            break;

                        case 'd':
                            // Integer format - works with Int and Long
                            if (std::holds_alternative<Int>(args[arg_index])) {
                                replacement = std::to_string(std::get<Int>(args[arg_index]));
                            } else if (std::holds_alternative<Long>(args[arg_index])) {
                                replacement = longToString(std::get<Long>(args[arg_index]));
                            } else {
                                replacement = "[non-integer]";
                            }
                            break;

                        case 'l':
                            // Long format - specifically for Long integers
                            if (std::holds_alternative<Long>(args[arg_index])) {
                                replacement = longToString(std::get<Long>(args[arg_index]));
                            } else if (std::holds_alternative<Int>(args[arg_index])) {
                                // Allow Int to be formatted as Long
                                replacement =
                                    longToString(static_cast<Long>(std::get<Int>(args[arg_index])));
                            } else {
                                replacement = "[non-long]";
                            }
                            break;

                        case 'f':
                            // Float format - works with Float and Double, with precision support
                            if (std::holds_alternative<Float>(args[arg_index])) {
                                double val = static_cast<double>(std::get<Float>(args[arg_index]));
                                if (precision >= 0) {
                                    std::ostringstream oss;
                                    oss << std::fixed << std::setprecision(precision) << val;
                                    replacement = oss.str();
                                } else {
                                    replacement = std::to_string(val);
                                }
                            } else if (std::holds_alternative<Double>(args[arg_index])) {
                                double val = std::get<Double>(args[arg_index]);
                                if (precision >= 0) {
                                    std::ostringstream oss;
                                    oss << std::fixed << std::setprecision(precision) << val;
                                    replacement = oss.str();
                                } else {
                                    replacement = std::to_string(val);
                                }
                            } else if (std::holds_alternative<Int>(args[arg_index])) {
                                // Allow integers to be formatted as floats
                                double val = static_cast<double>(std::get<Int>(args[arg_index]));
                                if (precision >= 0) {
                                    std::ostringstream oss;
                                    oss << std::fixed << std::setprecision(precision) << val;
                                    replacement = oss.str();
                                } else {
                                    replacement = std::to_string(val);
                                }
                            } else {
                                replacement = "[non-numeric]";
                            }
                            break;

                        case 'o':
                            // Object format - specialized formatting for objects/records/enums
                            replacement = valueToObjectString(args[arg_index]);
                            break;

                        default:
                            // Unknown format specifier, keep as-is
                            result += format.substr(format_start, i - format_start + 1);
                            continue;
                    }

                    result += replacement;
                    arg_index++;
                } else {
                    // Incomplete format specifier at end of string
                    result += format.substr(format_start);
                    break;
                }
            } else {
                // No more arguments, keep format specifier as-is
                result += format[i];
            }
        } else {
            result += format[i];
        }
    }

    return result;
}

std::string SystemLibrary::valueToDisplayString(const Value& value) {
    if (std::holds_alternative<Text>(value)) {
        return std::get<Text>(value);
    } else if (std::holds_alternative<Int>(value)) {
        return std::to_string(std::get<Int>(value));
    } else if (std::holds_alternative<Long>(value)) {
        return longToString(std::get<Long>(value));
    } else if (std::holds_alternative<Float>(value)) {
        return std::to_string(std::get<Float>(value));
    } else if (std::holds_alternative<Double>(value)) {
        return std::to_string(std::get<Double>(value));
    } else if (std::holds_alternative<Bool>(value)) {
        return std::get<Bool>(value) ? "true" : "false";
    } else if (std::holds_alternative<Char>(value)) {
        return std::string(1, std::get<Char>(value));
    } else if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(value)) {
        auto obj = std::get<std::shared_ptr<ObjectInstance>>(value);
        return "Object(" + obj->getName() + ")";
    } else if (std::holds_alternative<std::shared_ptr<EnumInstance>>(value)) {
        auto enum_inst = std::get<std::shared_ptr<EnumInstance>>(value);
        return "Enum(" + enum_inst->getEnumName() + ")";
    } else if (std::holds_alternative<std::shared_ptr<RecordInstance>>(value)) {
        auto record_inst = std::get<std::shared_ptr<RecordInstance>>(value);
        return record_inst->toString();
    } else if (std::holds_alternative<std::shared_ptr<RecordType>>(value)) {
        auto record_type = std::get<std::shared_ptr<RecordType>>(value);
        return "RecordType(" + record_type->getRecordName() + ")";
    } else if (std::holds_alternative<std::shared_ptr<ListInstance>>(value)) {
        auto list_inst = std::get<std::shared_ptr<ListInstance>>(value);
        return list_inst->toString();
    } else if (std::holds_alternative<std::shared_ptr<MapInstance>>(value)) {
        auto map_inst = std::get<std::shared_ptr<MapInstance>>(value);
        return map_inst->toString();
    } else if (std::holds_alternative<std::shared_ptr<MapIterator>>(value)) {
        auto map_iter = std::get<std::shared_ptr<MapIterator>>(value);
        return map_iter->toString();
    } else if (std::holds_alternative<std::shared_ptr<MapObject>>(value)) {
        auto map_obj = std::get<std::shared_ptr<MapObject>>(value);
        return map_obj->toString();
    } else if (std::holds_alternative<std::shared_ptr<SetInstance>>(value)) {
        auto set_inst = std::get<std::shared_ptr<SetInstance>>(value);
        return set_inst->toString();
    } else if (std::holds_alternative<std::shared_ptr<SetIterator>>(value)) {
        auto set_iter = std::get<std::shared_ptr<SetIterator>>(value);
        return set_iter->toString();
    }

    return "[Unknown Value]";
}

Value SystemLibrary::nativeGetEnv(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw EvaluationError("getEnv() requires one argument: the environment variable name");
    }

    // First argument must be the environment variable name
    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("getEnv() argument must be a Text (environment variable name)");
    }

    std::string env_var_name = std::get<Text>(args[0]);

    // Get the environment variable value
    const char* env_value = std::getenv(env_var_name.c_str());

    if (env_value == nullptr) {
        // Environment variable not found, return empty string
        return Text("");
    }

    return Text(std::string(env_value));
}

std::string SystemLibrary::valueToObjectString(const Value& value) {
    // %o format specifier provides detailed formatting for objects, records, and enums
    if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(value)) {
        auto obj = std::get<std::shared_ptr<ObjectInstance>>(value);
        return "Object " + obj->getName();
    } else if (std::holds_alternative<std::shared_ptr<EnumInstance>>(value)) {
        auto enum_inst = std::get<std::shared_ptr<EnumInstance>>(value);
        return enum_inst->toString();  // This gives detailed enum representation
    } else if (std::holds_alternative<std::shared_ptr<RecordInstance>>(value)) {
        auto record_inst = std::get<std::shared_ptr<RecordInstance>>(value);
        return record_inst->toString();  // This gives detailed record representation
    } else if (std::holds_alternative<std::shared_ptr<RecordType>>(value)) {
        auto record_type = std::get<std::shared_ptr<RecordType>>(value);
        return record_type->toString();  // This gives detailed record type representation
    } else if (std::holds_alternative<std::shared_ptr<MapInstance>>(value)) {
        auto map_inst = std::get<std::shared_ptr<MapInstance>>(value);
        return map_inst->toString();  // This gives detailed map representation
    } else if (std::holds_alternative<std::shared_ptr<MapIterator>>(value)) {
        auto map_iter = std::get<std::shared_ptr<MapIterator>>(value);
        return map_iter->toString();  // This gives detailed map iterator representation
    } else if (std::holds_alternative<std::shared_ptr<MapObject>>(value)) {
        auto map_obj = std::get<std::shared_ptr<MapObject>>(value);
        return map_obj->toString();  // This gives detailed map object representation
    } else {
        // For non-object types, fall back to regular display string
        return valueToDisplayString(value);
    }
}

Value SystemLibrary::nativeRepeat(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("repeat() requires exactly one argument: the number of repetitions");
    }

    // First argument must be the count
    if (!std::holds_alternative<Int>(args[0])) {
        throw EvaluationError("repeat() argument must be an Int (count)");
    }

    Int count = std::get<Int>(args[0]);

    if (count < 0) {
        throw EvaluationError("repeat() count cannot be negative: " + std::to_string(count));
    }

    // Create and return a RepeatIterator
    return Value(std::make_shared<RepeatIterator>(count));
}

Value SystemLibrary::nativeGetArgs(const std::vector<Value>& args, Context& context) {
    // No arguments expected for args()
    if (!args.empty()) {
        throw EvaluationError("args() expects no arguments");
    }

    // Try to get the program arguments from the global context
    if (context.hasVariable("__program_args__")) {
        return context.getVariable("__program_args__");
    }

    // If no arguments were set, return an empty list
    auto empty_list = std::make_shared<ListInstance>("Text");
    return Value(empty_list);
}

Value SystemLibrary::nativeGetArgc(const std::vector<Value>& args, Context& context) {
    // No arguments expected for argc()
    if (!args.empty()) {
        throw EvaluationError("argc() expects no arguments");
    }

    // Try to get the program arguments count from the global context
    if (context.hasVariable("__program_args__")) {
        Value args_value = context.getVariable("__program_args__");

        if (std::holds_alternative<std::shared_ptr<ListInstance>>(args_value)) {
            auto args_list = std::get<std::shared_ptr<ListInstance>>(args_value);
            return Int(static_cast<int>(args_list->size()));
        }
    }

    // If no arguments were set, return 0
    return Int(0);
}

Value SystemLibrary::nativeReadText(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("readText() requires exactly one argument (file path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("readText() argument must be a Text (file path)");
    }

    std::string filepath = std::get<Text>(args[0]);

    try {
        // Check if file exists
        if (!std::filesystem::exists(filepath)) {
            throw EvaluationError("File does not exist: " + filepath);
        }

        // Check if it's actually a file (not a directory)
        if (!std::filesystem::is_regular_file(filepath)) {
            throw EvaluationError("Path is not a regular file: " + filepath);
        }

        // Read the file
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw EvaluationError("Failed to open file for reading: " + filepath);
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();

        return Text(content);

    } catch (const std::filesystem::filesystem_error& e) {
        throw EvaluationError("Filesystem error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("Error reading file: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeWriteText(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("writeText() requires exactly two arguments (file path, content)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("writeText() first argument must be a Text (file path)");
    }

    if (!std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("writeText() second argument must be a Text (content)");
    }

    std::string filepath = std::get<Text>(args[0]);
    std::string content = std::get<Text>(args[1]);

    try {
        // Create parent directories if they don't exist
        std::filesystem::path path(filepath);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        // Write the file
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw EvaluationError("Failed to open file for writing: " + filepath);
        }

        file << content;
        file.close();

        return Bool(true);

    } catch (const std::filesystem::filesystem_error& e) {
        throw EvaluationError("Filesystem error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("Error writing file: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeExists(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("exists() requires exactly one argument (path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("exists() argument must be a Text (path)");
    }

    std::string path = std::get<Text>(args[0]);

    try {
        return Bool(std::filesystem::exists(path));
    } catch (const std::filesystem::filesystem_error& e) {
        // Return false if we can't determine existence due to permissions or other issues
        return Bool(false);
    }
}

Value SystemLibrary::nativeIsFile(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("isFile() requires exactly one argument (path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("isFile() argument must be a Text (path)");
    }

    std::string path = std::get<Text>(args[0]);

    try {
        return Bool(std::filesystem::is_regular_file(path));
    } catch (const std::filesystem::filesystem_error& e) {
        return Bool(false);
    }
}

Value SystemLibrary::nativeIsDirectory(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("isDirectory() requires exactly one argument (path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("isDirectory() argument must be a Text (path)");
    }

    std::string path = std::get<Text>(args[0]);

    try {
        return Bool(std::filesystem::is_directory(path));
    } catch (const std::filesystem::filesystem_error& e) {
        return Bool(false);
    }
}

Value SystemLibrary::nativeListFiles(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("listFiles() requires exactly one argument (directory path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("listFiles() argument must be a Text (directory path)");
    }

    std::string dirpath = std::get<Text>(args[0]);

    try {
        if (!std::filesystem::exists(dirpath)) {
            throw EvaluationError("Directory does not exist: " + dirpath);
        }

        if (!std::filesystem::is_directory(dirpath)) {
            throw EvaluationError("Path is not a directory: " + dirpath);
        }

        // Create a List<Text> to hold the file names
        auto files_list = std::make_shared<ListInstance>("Text");

        // Iterate through directory entries
        for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
            std::string filename = entry.path().filename().string();
            files_list->add(Text(filename));
        }

        return Value(files_list);

    } catch (const std::filesystem::filesystem_error& e) {
        throw EvaluationError("Filesystem error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("Error listing directory: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeCreateDirectory(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("createDirectory() requires exactly one argument (directory path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("createDirectory() argument must be a Text (directory path)");
    }

    std::string dirpath = std::get<Text>(args[0]);

    try {
        // Create directories recursively (like mkdir -p)
        bool created = std::filesystem::create_directories(dirpath);

        // Return true if directory was created or already exists
        return Bool(created || std::filesystem::exists(dirpath));

    } catch (const std::filesystem::filesystem_error& e) {
        throw EvaluationError("Filesystem error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("Error creating directory: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeDeleteFile(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("deleteFile() requires exactly one argument (file path)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("deleteFile() argument must be a Text (file path)");
    }

    std::string filepath = std::get<Text>(args[0]);

    try {
        if (!std::filesystem::exists(filepath)) {
            throw EvaluationError("File does not exist: " + filepath);
        }

        // Use remove to delete file or directory
        bool removed = std::filesystem::remove(filepath);
        return Bool(removed);

    } catch (const std::filesystem::filesystem_error& e) {
        throw EvaluationError("Filesystem error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("Error deleting file: " + std::string(e.what()));
    }
}

// ============================================================================
// System Information Methods
// ============================================================================

Value SystemLibrary::nativeGetOSName(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getOSName() takes no arguments");
    }

#ifdef __linux__
    return Text("Linux");
#elif __APPLE__
    return Text("macOS");
#elif _WIN32
    return Text("Windows");
#elif __FreeBSD__
    return Text("FreeBSD");
#elif __OpenBSD__
    return Text("OpenBSD");
#elif __NetBSD__
    return Text("NetBSD");
#else
    return Text("Unknown");
#endif
}

Value SystemLibrary::nativeGetOSVersion(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getOSVersion() takes no arguments");
    }

    try {
#ifdef __APPLE__
        std::string version = executeSystemCommand("sw_vers -productVersion");
        if (!version.empty() && version.back() == '\n') {
            version.pop_back();
        }
        return Text(version);
#elif __linux__
        std::string version = executeSystemCommand("uname -r");
        if (!version.empty() && version.back() == '\n') {
            version.pop_back();
        }
        return Text(version);
#elif _WIN32
        return Text(executeSystemCommand("ver"));
#else
        return Text("Unknown");
#endif
    } catch (const std::exception& e) {
        return Text("Unknown");
    }
}

Value SystemLibrary::nativeGetArchitecture(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getArchitecture() takes no arguments");
    }

    try {
        std::string arch = executeSystemCommand("uname -m");
        if (!arch.empty() && arch.back() == '\n') {
            arch.pop_back();
        }
        return Text(arch);
    } catch (const std::exception& e) {
#ifdef __x86_64__
        return Text("x86_64");
#elif __i386__
        return Text("i386");
#elif __arm__
        return Text("arm");
#elif __aarch64__
        return Text("aarch64");
#else
        return Text("unknown");
#endif
    }
}

Value SystemLibrary::nativeGetHostname(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getHostname() takes no arguments");
    }

    try {
        std::string hostname = executeSystemCommand("hostname");
        if (!hostname.empty() && hostname.back() == '\n') {
            hostname.pop_back();
        }
        return Text(hostname);
    } catch (const std::exception& e) {
        return Text("unknown");
    }
}

Value SystemLibrary::nativeGetUsername(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getUsername() takes no arguments");
    }

    const char* username = std::getenv("USER");
    if (username == nullptr) {
        username = std::getenv("USERNAME");  // Windows fallback
    }
    if (username == nullptr) {
        try {
            std::string whoami = executeSystemCommand("whoami");
            if (!whoami.empty() && whoami.back() == '\n') {
                whoami.pop_back();
            }
            return Text(whoami);
        } catch (const std::exception& e) {
            return Text("unknown");
        }
    }
    return Text(username);
}

Value SystemLibrary::nativeGetCurrentDir(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getCurrentDir() takes no arguments");
    }

    try {
        std::string cwd = std::filesystem::current_path().string();
        return Text(cwd);
    } catch (const std::exception& e) {
        return Text("unknown");
    }
}

Value SystemLibrary::nativeGetProcessId(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getProcessId() takes no arguments");
    }

#ifdef _WIN32
    return Int(static_cast<Int>(GetCurrentProcessId()));
#else
    return Int(static_cast<Int>(getpid()));
#endif
}

Value SystemLibrary::nativeGetParentProcessId(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getParentProcessId() takes no arguments");
    }

#ifdef _WIN32
    // Windows implementation would require more complex code
    return Int(0);
#else
    return Int(static_cast<Int>(getppid()));
#endif
}

Value SystemLibrary::nativeGetUptime(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getUptime() takes no arguments");
    }

    try {
#ifdef __linux__
        std::ifstream uptime_file("/proc/uptime");
        if (uptime_file.is_open()) {
            double uptime_seconds;
            uptime_file >> uptime_seconds;
            return Long(static_cast<Long>(uptime_seconds));
        }
#elif __APPLE__
        std::string uptime_str = executeSystemCommand("sysctl -n kern.boottime");
        // Parse boottime and calculate uptime
        // For now, return a simple implementation
        return Long(0);
#endif
        return Long(0);
    } catch (const std::exception& e) {
        return Long(0);
    }
}

// ============================================================================
// Memory Information Methods
// ============================================================================

Value SystemLibrary::nativeGetTotalMemory(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getTotalMemory() takes no arguments");
    }

    try {
#ifdef __linux__
        return Long(getMemoryInfoFromProcMeminfo("MemTotal"));
#elif __APPLE__
        std::string mem_str = executeSystemCommand("sysctl -n hw.memsize");
        if (!mem_str.empty()) {
            Long bytes = std::stoll(mem_str);
            return Long(bytes / 1024);  // Convert to KB
        }
#endif
        return Long(0);
    } catch (const std::exception& e) {
        return Long(0);
    }
}

Value SystemLibrary::nativeGetAvailableMemory(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getAvailableMemory() takes no arguments");
    }

    try {
#ifdef __linux__
        return Long(getMemoryInfoFromProcMeminfo("MemAvailable"));
#elif __APPLE__
        std::string vm_stat_output =
            executeSystemCommand("vm_stat | grep 'Pages free' | awk '{print $3}' | sed 's/\\.//'");
        if (!vm_stat_output.empty()) {
            Long free_pages = std::stoll(vm_stat_output);
            // Assuming 4KB page size on macOS
            return Long(free_pages * 4);
        }
        return Long(0);
#else
        return Long(0);
#endif
    } catch (const std::exception& e) {
        return Long(0);
    }
}

Value SystemLibrary::nativeGetUsedMemory(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getUsedMemory() takes no arguments");
    }

    try {
#ifdef __linux__
        Long total = getMemoryInfoFromProcMeminfo("MemTotal");
        Long available = getMemoryInfoFromProcMeminfo("MemAvailable");
        return Long(total - available);
#elif __APPLE__
        std::string total_mem = executeSystemCommand("sysctl -n hw.memsize");
        std::string vm_stat_output =
            executeSystemCommand("vm_stat | grep 'Pages free' | awk '{print $3}' | sed 's/\\.//'");
        if (!total_mem.empty() && !vm_stat_output.empty()) {
            Long total_kb = std::stoll(total_mem) / 1024;
            Long free_pages = std::stoll(vm_stat_output);
            Long free_kb = free_pages * 4;
            return Long(total_kb - free_kb);
        }
        return Long(0);
#else
        return Long(0);
#endif
    } catch (const std::exception& e) {
        return Long(0);
    }
}

Value SystemLibrary::nativeGetMemoryUsage(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getMemoryUsage() takes no arguments");
    }

    try {
#ifdef __linux__
        Long total = getMemoryInfoFromProcMeminfo("MemTotal");
        Long available = getMemoryInfoFromProcMeminfo("MemAvailable");
        if (total > 0) {
            double usage =
                static_cast<double>(total - available) / static_cast<double>(total) * 100.0;
            return Double(usage);
        }
#elif __APPLE__
        std::string total_mem = executeSystemCommand("sysctl -n hw.memsize");
        std::string vm_stat_output =
            executeSystemCommand("vm_stat | grep 'Pages free' | awk '{print $3}' | sed 's/\\.//'");
        if (!total_mem.empty() && !vm_stat_output.empty()) {
            Long total_kb = std::stoll(total_mem) / 1024;
            Long free_pages = std::stoll(vm_stat_output);
            Long free_kb = free_pages * 4;
            Long used_kb = total_kb - free_kb;
            if (total_kb > 0) {
                double usage = static_cast<double>(used_kb) / static_cast<double>(total_kb) * 100.0;
                return Double(usage);
            }
        }
#endif
        return Double(0.0);
    } catch (const std::exception& e) {
        return Double(0.0);
    }
}

// ============================================================================
// CPU Information Methods
// ============================================================================

Value SystemLibrary::nativeGetCPUCount(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getCPUCount() takes no arguments");
    }

    try {
#ifdef __linux__
        std::string nproc_str = executeSystemCommand("nproc");
        if (!nproc_str.empty()) {
            return Int(std::stoi(nproc_str));
        }
#elif __APPLE__
        std::string cpu_count = executeSystemCommand("sysctl -n hw.ncpu");
        if (!cpu_count.empty()) {
            return Int(std::stoi(cpu_count));
        }
#endif

        // Fallback to C++ standard library
        unsigned int cores = std::thread::hardware_concurrency();
        return Int(static_cast<Int>(cores));
    } catch (const std::exception& e) {
        return Int(1);
    }
}

Value SystemLibrary::nativeGetCPUModel(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getCPUModel() takes no arguments");
    }

    try {
#ifdef __linux__
        return Text(getCPUModelFromProcCpuinfo());
#elif __APPLE__
        std::string cpu_model = executeSystemCommand("sysctl -n machdep.cpu.brand_string");
        if (!cpu_model.empty() && cpu_model.back() == '\n') {
            cpu_model.pop_back();
        }
        return Text(cpu_model);
#else
        return Text("Unknown CPU");
#endif
    } catch (const std::exception& e) {
        return Text("Unknown CPU");
    }
}

Value SystemLibrary::nativeGetCPUUsage(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getCPUUsage() takes no arguments");
    }

    try {
#ifdef __linux__
        return Double(getCPUUsageFromProcStat());
#elif __APPLE__
        std::string cpu_usage = executeSystemCommand(
            "top -l1 -n0 | grep 'CPU usage:' | awk '{print $3}' | sed 's/%//'");
        if (!cpu_usage.empty()) {
            return Double(std::stod(cpu_usage));
        }
        return Double(0.0);
#else
        return Double(0.0);
#endif
    } catch (const std::exception& e) {
        return Double(0.0);
    }
}

Value SystemLibrary::nativeGetLoadAverage(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("getLoadAverage() takes no arguments");
    }

    try {
#ifdef __linux__
        auto load_avg = getLoadAverageFromProcLoadavg();
        auto list_instance = std::make_shared<ListInstance>();
        for (const auto& avg : load_avg) {
            list_instance->add(Double(avg));
        }
        return Value(list_instance);
#elif __APPLE__
        std::string load_str =
            executeSystemCommand("uptime | awk -F'load averages:' '{ print $2 }'");
        auto list_instance = std::make_shared<ListInstance>();

        if (!load_str.empty()) {
            // Parse the load average values
            std::stringstream ss(load_str);
            std::string token;
            int count = 0;
            while (count < 3 && std::getline(ss, token, ' ')) {
                token.erase(0, token.find_first_not_of(" \t"));    // trim leading spaces
                token.erase(token.find_last_not_of(" \t\n") + 1);  // trim trailing spaces
                if (!token.empty()) {
                    try {
                        double val = std::stod(token);
                        list_instance->add(Double(val));
                        count++;
                    } catch (const std::exception&) {
                        // Skip invalid values
                    }
                }
            }

            // Fill remaining slots with 0.0 if needed
            while (count < 3) {
                list_instance->add(Double(0.0));
                count++;
            }
        } else {
            list_instance->add(Double(0.0));
            list_instance->add(Double(0.0));
            list_instance->add(Double(0.0));
        }
        return Value(list_instance);
#else
        auto list_instance = std::make_shared<ListInstance>();
        list_instance->add(Double(0.0));
        list_instance->add(Double(0.0));
        list_instance->add(Double(0.0));
        return Value(list_instance);
#endif
    } catch (const std::exception& e) {
        auto list_instance = std::make_shared<ListInstance>();
        list_instance->add(Double(0.0));
        list_instance->add(Double(0.0));
        list_instance->add(Double(0.0));
        return Value(list_instance);
    }
}

// ============================================================================
// Command Execution Methods
// ============================================================================

Value SystemLibrary::nativeExecute(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("execute() requires exactly one argument (command)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("execute() argument must be a Text (command)");
    }

    std::string command = std::get<Text>(args[0]);

    try {
        int exit_code = std::system(command.c_str());
        return Int(exit_code);
    } catch (const std::exception& e) {
        throw EvaluationError("Error executing command: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeExecuteWithOutput(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("executeWithOutput() requires exactly one argument (command)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("executeWithOutput() argument must be a Text (command)");
    }

    std::string command = std::get<Text>(args[0]);

    try {
        std::string output = executeSystemCommand(command);
        return Text(output);
    } catch (const std::exception& e) {
        throw EvaluationError("Error executing command: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeExecuteWithTimeout(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError(
            "executeWithTimeout() requires exactly two arguments (command, timeout_seconds)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("executeWithTimeout() first argument must be a Text (command)");
    }

    if (!std::holds_alternative<Int>(args[1])) {
        throw EvaluationError(
            "executeWithTimeout() second argument must be an Int (timeout in seconds)");
    }

    std::string command = std::get<Text>(args[0]);
    Int timeout_seconds = std::get<Int>(args[1]);

    try {
// Add timeout prefix to command
#ifdef __linux__
        std::string timeout_command = "timeout " + std::to_string(timeout_seconds) + " " + command;
#elif __APPLE__
        // macOS doesn't have built-in timeout command, use a different approach
        std::string timeout_command =
            "bash -c 'timeout() { perl -e \"alarm shift; exec @ARGV\" \"$@\"; }; timeout " +
            std::to_string(timeout_seconds) + " " + command + "'";
#else
        std::string timeout_command = command;  // No timeout support
#endif

        std::string output = executeSystemCommand(timeout_command);
        return Text(output);
    } catch (const std::exception& e) {
        throw EvaluationError("Error executing command with timeout: " + std::string(e.what()));
    }
}

Value SystemLibrary::nativeExecuteAsync(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("executeAsync() requires exactly one argument (command)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("executeAsync() argument must be a Text (command)");
    }

    std::string command = std::get<Text>(args[0]);

    try {
        // Run command in background
        std::string async_command = command + " &";
        int result = std::system(async_command.c_str());
        return Int(result);
    } catch (const std::exception& e) {
        throw EvaluationError("Error executing async command: " + std::string(e.what()));
    }
}

// ============================================================================
// Helper Functions
// ============================================================================

std::string SystemLibrary::executeSystemCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

#ifdef _WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
#else
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
#endif

    if (!pipe) {
        throw std::runtime_error("Failed to execute command: " + command);
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

Long SystemLibrary::getMemoryInfoFromProcMeminfo(const std::string& field) {
#ifdef __linux__
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return 0;
    }

    std::string line;
    while (std::getline(meminfo, line)) {
        if (line.find(field + ":") == 0) {
            std::istringstream iss(line);
            std::string name, unit;
            long long temp_value;
            iss >> name >> temp_value >> unit;
            return static_cast<Long>(temp_value);  // Returns value in KB
        }
    }
#endif
    return 0;
}

Double SystemLibrary::getCPUUsageFromProcStat() {
#ifdef __linux__
    std::ifstream stat_file("/proc/stat");
    if (!stat_file.is_open()) {
        return 0.0;
    }

    std::string line;
    std::getline(stat_file, line);

    std::istringstream iss(line);
    std::string cpu_label;
    long long temp_user, temp_nice, temp_system, temp_idle, temp_iowait, temp_irq, temp_softirq,
        temp_steal;

    iss >> cpu_label >> temp_user >> temp_nice >> temp_system >> temp_idle >> temp_iowait >>
        temp_irq >> temp_softirq >> temp_steal;

    Long user = static_cast<Long>(temp_user);
    Long nice = static_cast<Long>(temp_nice);
    Long system = static_cast<Long>(temp_system);
    Long idle = static_cast<Long>(temp_idle);
    Long iowait = static_cast<Long>(temp_iowait);
    Long irq = static_cast<Long>(temp_irq);
    Long softirq = static_cast<Long>(temp_softirq);
    Long steal = static_cast<Long>(temp_steal);

    Long total = user + nice + system + idle + iowait + irq + softirq + steal;
    Long total_idle = idle + iowait;

    // This is a simplified single-sample CPU usage - normally you'd need two samples
    if (total > 0) {
        return static_cast<Double>((total - total_idle) * 100.0 / total);
    }
#endif
    return 0.0;
}

std::string SystemLibrary::getCPUModelFromProcCpuinfo() {
#ifdef __linux__
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return "Unknown CPU";
    }

    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") == 0) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string model = line.substr(colon_pos + 1);
                // Trim leading spaces
                size_t start = model.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    return model.substr(start);
                }
            }
        }
    }
#endif
    return "Unknown CPU";
}

std::vector<Double> SystemLibrary::getLoadAverageFromProcLoadavg() {
    std::vector<Double> load_avg = {0.0, 0.0, 0.0};

#ifdef __linux__
    std::ifstream loadavg("/proc/loadavg");
    if (loadavg.is_open()) {
        Double load1, load5, load15;
        loadavg >> load1 >> load5 >> load15;
        load_avg = {load1, load5, load15};
    }
#endif

    return load_avg;
}

// Path manipulation method implementations

Value SystemLibrary::nativeBasename(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("basename() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("basename() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        return Text(path.filename().string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

Value SystemLibrary::nativeDirname(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("dirname() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("dirname() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        return Text(path.parent_path().string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

Value SystemLibrary::nativeExtname(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("extname() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("extname() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        return Text(path.extension().string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

Value SystemLibrary::nativeJoin(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw EvaluationError("join() requires at least 1 argument");
    }

    try {
        std::filesystem::path result;

        for (const auto& arg : args) {
            if (!std::holds_alternative<Text>(arg)) {
                throw EvaluationError("join() all arguments must be Text");
            }
            std::string path_component = std::get<Text>(arg);
            if (!path_component.empty()) {
                result /= path_component;
            }
        }

        return Text(result.string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

Value SystemLibrary::nativeNormalize(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("normalize() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("normalize() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        path = path.lexically_normal();
        return Text(path.string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text(std::get<Text>(args[0]));
    }
}

Value SystemLibrary::nativeResolve(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("resolve() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("resolve() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        std::filesystem::path absolute = std::filesystem::absolute(path);
        return Text(absolute.string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text(std::get<Text>(args[0]));
    }
}

Value SystemLibrary::nativeRelative(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("relative() requires exactly 2 arguments (from: Text, to: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("relative() all arguments must be Text");
    }

    try {
        std::string from_str = std::get<Text>(args[0]);
        std::string to_str = std::get<Text>(args[1]);

        std::filesystem::path from(from_str);
        std::filesystem::path to(to_str);

        std::filesystem::path relative = std::filesystem::relative(to, from);
        return Text(relative.string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

Value SystemLibrary::nativeIsAbsolute(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("isAbsolute() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("isAbsolute() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        return Bool(path.is_absolute());
    } catch (const std::filesystem::filesystem_error& e) {
        return Bool(false);
    }
}

Value SystemLibrary::nativeSplitPath(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("splitPath() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("splitPath() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);

        auto list = std::make_shared<ListInstance>();

        for (const auto& component : path) {
            if (!component.empty() && component != "/") {
                list->add(Text(component.string()));
            }
        }

        return Value(list);
    } catch (const std::filesystem::filesystem_error& e) {
        auto list = std::make_shared<ListInstance>();
        return Value(list);
    }
}

Value SystemLibrary::nativeGetParent(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("getParent() requires exactly 1 argument (path: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("getParent() argument must be a Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::filesystem::path path(path_str);
        return Text(path.parent_path().string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

Value SystemLibrary::nativeChangeExtension(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError(
            "changeExtension() requires exactly 2 arguments (path: Text, newExt: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("changeExtension() all arguments must be Text");
    }

    try {
        std::string path_str = std::get<Text>(args[0]);
        std::string new_ext = std::get<Text>(args[1]);

        std::filesystem::path path(path_str);

        // Ensure extension starts with dot if not empty
        if (!new_ext.empty() && new_ext[0] != '.') {
            new_ext = "." + new_ext;
        }

        path.replace_extension(new_ext);
        return Text(path.string());
    } catch (const std::filesystem::filesystem_error& e) {
        return Text("");
    }
}

}  // namespace o2l