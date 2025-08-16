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

#include "JsonLibrary.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <regex>
#include <sstream>

#include "../Common/Exceptions.hpp"
#include "ListInstance.hpp"
#include "MapInstance.hpp"

namespace o2l {

std::shared_ptr<ObjectInstance> JsonLibrary::createJsonObject() {
    auto jsonObject = std::make_shared<ObjectInstance>("json");

    // JSON parsing methods
    Method parse_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeParse(args, ctx);
    };
    jsonObject->addMethod("parse", parse_method, true);

    Method parseAuto_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeParseAuto(args, ctx);
    };
    jsonObject->addMethod("parseAuto", parseAuto_method, true);

    Method parseToMap_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeParseToMap(args, ctx);
    };
    jsonObject->addMethod("parseToMap", parseToMap_method, true);

    Method parseToList_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeParseToList(args, ctx);
    };
    jsonObject->addMethod("parseToList", parseToList_method, true);

    Method isValid_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsValid(args, ctx);
    };
    jsonObject->addMethod("isValid", isValid_method, true);

    Method get_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeGet(args, ctx);
    };
    jsonObject->addMethod("get", get_method, true);

    Method getType_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeGetType(args, ctx);
    };
    jsonObject->addMethod("getType", getType_method, true);

    Method hasKey_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeHasKey(args, ctx);
    };
    jsonObject->addMethod("hasKey", hasKey_method, true);

    // JSON generation methods
    Method stringify_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeStringify(args, ctx);
    };
    jsonObject->addMethod("stringify", stringify_method, true);

    Method create_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeCreate(args, ctx);
    };
    jsonObject->addMethod("create", create_method, true);

    Method createArray_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeCreateArray(args, ctx);
    };
    jsonObject->addMethod("createArray", createArray_method, true);

    Method createObject_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeCreateObject(args, ctx);
    };
    jsonObject->addMethod("createObject", createObject_method, true);

    // JSON manipulation methods
    Method set_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeSet(args, ctx);
    };
    jsonObject->addMethod("set", set_method, true);

    Method remove_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeRemove(args, ctx);
    };
    jsonObject->addMethod("remove", remove_method, true);

    Method merge_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeMerge(args, ctx);
    };
    jsonObject->addMethod("merge", merge_method, true);

    Method keys_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeKeys(args, ctx);
    };
    jsonObject->addMethod("keys", keys_method, true);

    Method values_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeValues(args, ctx);
    };
    jsonObject->addMethod("values", values_method, true);

    Method size_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeSize(args, ctx);
    };
    jsonObject->addMethod("size", size_method, true);

    // JSON array methods
    Method push_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativePush(args, ctx);
    };
    jsonObject->addMethod("push", push_method, true);

    Method pop_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativePop(args, ctx);
    };
    jsonObject->addMethod("pop", pop_method, true);

    Method slice_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeSlice(args, ctx);
    };
    jsonObject->addMethod("slice", slice_method, true);

    Method indexOf_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIndexOf(args, ctx);
    };
    jsonObject->addMethod("indexOf", indexOf_method, true);

    // JSON validation methods
    Method isObject_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsObject(args, ctx);
    };
    jsonObject->addMethod("isObject", isObject_method, true);

    Method isArray_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsArray(args, ctx);
    };
    jsonObject->addMethod("isArray", isArray_method, true);

    Method isString_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsString(args, ctx);
    };
    jsonObject->addMethod("isString", isString_method, true);

    Method isNumber_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsNumber(args, ctx);
    };
    jsonObject->addMethod("isNumber", isNumber_method, true);

    Method isBoolean_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsBoolean(args, ctx);
    };
    jsonObject->addMethod("isBoolean", isBoolean_method, true);

    Method isNull_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeIsNull(args, ctx);
    };
    jsonObject->addMethod("isNull", isNull_method, true);

    // JSON utility methods
    Method prettyPrint_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativePrettyPrint(args, ctx);
    };
    jsonObject->addMethod("prettyPrint", prettyPrint_method, true);

    Method minify_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeMinify(args, ctx);
    };
    jsonObject->addMethod("minify", minify_method, true);

    Method equals_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeEquals(args, ctx);
    };
    jsonObject->addMethod("equals", equals_method, true);

    Method clone_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return JsonLibrary::nativeClone(args, ctx);
    };
    jsonObject->addMethod("clone", clone_method, true);

    return jsonObject;
}

// JSON parsing methods
Value JsonLibrary::nativeParse(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.parse() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.parse() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue jsonValue = parseJsonString(jsonStr);
        return Text(stringifyJsonValue(jsonValue));  // Return as JSON string for now
    } catch (const std::exception& e) {
        throw EvaluationError("JSON parsing failed: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeParseAuto(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.parseAuto() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.parseAuto() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue jsonValue = parseJsonString(jsonStr);

        // Automatically return native Map for objects, List for arrays, or primitive values as-is
        return jsonValueToO2LNative(jsonValue);
    } catch (const std::exception& e) {
        throw EvaluationError("JSON auto-parsing failed: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeParseToMap(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.parseToMap() requires exactly 1 argument (jsonString)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.parseToMap() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue jsonValue = parseJsonString(jsonStr);

        if (!std::holds_alternative<std::map<Text, JsonValue>>(jsonValue)) {
            throw EvaluationError("JSON string does not represent an object", context);
        }

        return jsonValueToO2LNative(jsonValue);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("JSON parsing to Map failed: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeParseToList(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.parseToList() requires exactly 1 argument (jsonString)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.parseToList() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue jsonValue = parseJsonString(jsonStr);

        if (!std::holds_alternative<std::vector<JsonValue>>(jsonValue)) {
            throw EvaluationError("JSON string does not represent an array", context);
        }

        return jsonValueToO2LNative(jsonValue);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("JSON parsing to List failed: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeIsValid(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isValid() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isValid() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        return Bool(isValidJsonString(jsonStr));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativeGet(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("json.get() requires exactly 2 arguments (jsonString, path)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.get() arguments must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        std::string path = std::get<Text>(args[1]);

        JsonValue root = parseJsonString(jsonStr);
        JsonValue result = getJsonValueAtPath(root, path);

        return jsonValueToO2L(result);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get JSON value: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeGetType(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("json.getType() requires exactly 2 arguments (jsonString, path)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.getType() arguments must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        std::string path = std::get<Text>(args[1]);

        JsonValue root = parseJsonString(jsonStr);
        JsonValue value = getJsonValueAtPath(root, path);

        return Text(getJsonTypeName(value));
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get JSON type: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeHasKey(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("json.hasKey() requires exactly 2 arguments (jsonString, key)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.hasKey() arguments must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        std::string key = std::get<Text>(args[1]);

        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::map<Text, JsonValue>>(root)) {
            auto& obj = std::get<std::map<Text, JsonValue>>(root);
            return Bool(obj.find(key) != obj.end());
        }

        return Bool(false);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to check JSON key: " + std::string(e.what()), context);
    }
}

// JSON generation methods
Value JsonLibrary::nativeStringify(const std::vector<Value>& args, Context& context) {
    if (args.size() < 1 || args.size() > 2) {
        throw EvaluationError("json.stringify() requires 1-2 arguments (value, [indent])", context);
    }

    try {
        JsonValue jsonValue = o2lValueToJson(args[0]);

        int indent = 0;
        if (args.size() == 2) {
            if (std::holds_alternative<Int>(args[1])) {
                indent = std::get<Int>(args[1]);
            }
        }

        if (indent > 0) {
            return Text(stringifyWithIndent(jsonValue, indent));
        } else {
            return Text(stringifyJsonValue(jsonValue));
        }
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to stringify JSON: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeCreate(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.create() requires exactly 1 argument (value)", context);
    }

    try {
        JsonValue jsonValue = o2lValueToJson(args[0]);
        return Text(stringifyJsonValue(jsonValue));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to create JSON: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeCreateArray(const std::vector<Value>& args, Context& context) {
    try {
        std::vector<JsonValue> jsonArray;

        for (const auto& arg : args) {
            JsonValue jsonValue = o2lValueToJson(arg);
            jsonArray.push_back(jsonValue);
        }

        return Text(stringifyJsonValue(JsonValue(jsonArray)));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to create JSON array: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeCreateObject(const std::vector<Value>& args, Context& context) {
    if (args.size() % 2 != 0) {
        throw EvaluationError(
            "json.createObject() requires even number of arguments (key-value pairs)", context);
    }

    try {
        std::map<Text, JsonValue> jsonObject;

        for (size_t i = 0; i < args.size(); i += 2) {
            if (!std::holds_alternative<Text>(args[i])) {
                throw EvaluationError("JSON object keys must be Text", context);
            }

            std::string key = std::get<Text>(args[i]);
            JsonValue value = o2lValueToJson(args[i + 1]);
            jsonObject[key] = value;
        }

        return Text(stringifyJsonValue(JsonValue(jsonObject)));
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to create JSON object: " + std::string(e.what()), context);
    }
}

// JSON manipulation methods
Value JsonLibrary::nativeSet(const std::vector<Value>& args, Context& context) {
    if (args.size() != 3) {
        throw EvaluationError("json.set() requires exactly 3 arguments (jsonString, path, value)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.set() first two arguments must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        std::string path = std::get<Text>(args[1]);

        JsonValue root = parseJsonString(jsonStr);
        JsonValue newValue = o2lValueToJson(args[2]);

        JsonValue result = setJsonValueAtPath(root, path, newValue);
        return Text(stringifyJsonValue(result));
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set JSON value: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeRemove(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("json.remove() requires exactly 2 arguments (jsonString, path)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.remove() arguments must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        std::string path = std::get<Text>(args[1]);

        JsonValue root = parseJsonString(jsonStr);
        JsonValue result = removeJsonValueAtPath(root, path);

        return Text(stringifyJsonValue(result));
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to remove JSON value: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeMerge(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError(
            "json.merge() requires exactly 2 arguments (jsonString1, jsonString2)", context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.merge() arguments must be Text", context);
    }

    try {
        std::string jsonStr1 = std::get<Text>(args[0]);
        std::string jsonStr2 = std::get<Text>(args[1]);

        JsonValue json1 = parseJsonString(jsonStr1);
        JsonValue json2 = parseJsonString(jsonStr2);

        if (std::holds_alternative<std::map<Text, JsonValue>>(json1) &&
            std::holds_alternative<std::map<Text, JsonValue>>(json2)) {
            auto obj1 = std::get<std::map<Text, JsonValue>>(json1);
            auto& obj2 = std::get<std::map<Text, JsonValue>>(json2);

            // Merge obj2 into obj1
            for (const auto& [key, value] : obj2) {
                obj1[key] = value;
            }

            return Text(stringifyJsonValue(JsonValue(obj1)));
        }

        throw EvaluationError("json.merge() can only merge JSON objects", context);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to merge JSON: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeKeys(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.keys() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.keys() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::map<Text, JsonValue>>(root)) {
            auto& obj = std::get<std::map<Text, JsonValue>>(root);

            auto keysList = std::make_shared<ListInstance>();
            for (const auto& [key, value] : obj) {
                keysList->add(Text(key));
            }

            return keysList;
        }

        throw EvaluationError("json.keys() can only get keys from JSON objects", context);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get JSON keys: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeValues(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.values() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.values() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::map<Text, JsonValue>>(root)) {
            auto& obj = std::get<std::map<Text, JsonValue>>(root);

            auto valuesList = std::make_shared<ListInstance>();
            for (const auto& [key, value] : obj) {
                valuesList->add(jsonValueToO2L(value));
            }

            return valuesList;
        }

        throw EvaluationError("json.values() can only get values from JSON objects", context);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get JSON values: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeSize(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.size() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.size() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::map<Text, JsonValue>>(root)) {
            auto& obj = std::get<std::map<Text, JsonValue>>(root);
            return Int(static_cast<Int>(obj.size()));
        } else if (std::holds_alternative<std::vector<JsonValue>>(root)) {
            auto& arr = std::get<std::vector<JsonValue>>(root);
            return Int(static_cast<Int>(arr.size()));
        }

        return Int(0);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get JSON size: " + std::string(e.what()), context);
    }
}

// Validation methods - implementing the first few key ones
Value JsonLibrary::nativeIsObject(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isObject() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isObject() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Bool(std::holds_alternative<std::map<Text, JsonValue>>(root));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativeIsArray(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isArray() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isArray() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Bool(std::holds_alternative<std::vector<JsonValue>>(root));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

// For now, implementing placeholder versions of remaining methods to keep it compilable
Value JsonLibrary::nativeIsString(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isString() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isString() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Bool(std::holds_alternative<Text>(root));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativeIsNumber(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isNumber() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isNumber() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Bool(std::holds_alternative<Int>(root) || std::holds_alternative<Double>(root));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativeIsBoolean(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isBoolean() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isBoolean() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Bool(std::holds_alternative<Bool>(root));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativeIsNull(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.isNull() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.isNull() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Bool(std::holds_alternative<std::nullptr_t>(root));
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativePush(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("json.push() requires exactly 2 arguments (jsonArrayString, value)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.push() first argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::vector<JsonValue>>(root)) {
            auto arr = std::get<std::vector<JsonValue>>(root);
            JsonValue newValue = o2lValueToJson(args[1]);
            arr.push_back(newValue);

            return Text(stringifyJsonValue(JsonValue(arr)));
        }

        throw EvaluationError("json.push() can only push to JSON arrays", context);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to push to JSON array: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativePop(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.pop() requires exactly 1 argument (jsonArrayString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.pop() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::vector<JsonValue>>(root)) {
            auto arr = std::get<std::vector<JsonValue>>(root);
            if (!arr.empty()) {
                arr.pop_back();
            }

            return Text(stringifyJsonValue(JsonValue(arr)));
        }

        throw EvaluationError("json.pop() can only pop from JSON arrays", context);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to pop from JSON array: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeSlice(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError("json.slice() requires 2-3 arguments (jsonArrayString, start, [end])",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("json.slice() first argument must be Text, second must be Int",
                              context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        int start = static_cast<int>(std::get<Int>(args[1]));

        JsonValue root = parseJsonString(jsonStr);

        if (std::holds_alternative<std::vector<JsonValue>>(root)) {
            const auto& arr = std::get<std::vector<JsonValue>>(root);
            int size = static_cast<int>(arr.size());

            // Handle negative indices
            if (start < 0) start += size;
            start = std::max(0, std::min(start, size));

            int end = size;
            if (args.size() == 3) {
                if (std::holds_alternative<Int>(args[2])) {
                    end = static_cast<int>(std::get<Int>(args[2]));
                    if (end < 0) end += size;
                }
            }
            end = std::max(start, std::min(end, size));

            std::vector<JsonValue> sliced(arr.begin() + start, arr.begin() + end);
            return Text(stringifyJsonValue(JsonValue(sliced)));
        }

        throw EvaluationError("json.slice() can only slice JSON arrays", context);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to slice JSON array: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeIndexOf(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError(
            "json.indexOf() requires exactly 2 arguments (jsonArrayString, value)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.indexOf() first argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);
        JsonValue searchValue = o2lValueToJson(args[1]);

        if (std::holds_alternative<std::vector<JsonValue>>(root)) {
            const auto& arr = std::get<std::vector<JsonValue>>(root);

            for (size_t i = 0; i < arr.size(); ++i) {
                if (arr[i].index() == searchValue.index()) {
                    // Compare values of same type
                    if (std::holds_alternative<Int>(arr[i]) &&
                        std::holds_alternative<Int>(searchValue)) {
                        if (std::get<Int>(arr[i]) == std::get<Int>(searchValue)) {
                            return Int(static_cast<Int>(i));
                        }
                    } else if (std::holds_alternative<Text>(arr[i]) &&
                               std::holds_alternative<Text>(searchValue)) {
                        if (std::get<Text>(arr[i]) == std::get<Text>(searchValue)) {
                            return Int(static_cast<Int>(i));
                        }
                    } else if (std::holds_alternative<Bool>(arr[i]) &&
                               std::holds_alternative<Bool>(searchValue)) {
                        if (std::get<Bool>(arr[i]) == std::get<Bool>(searchValue)) {
                            return Int(static_cast<Int>(i));
                        }
                    }
                }
            }
        }

        return Int(-1);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to find index in JSON array: " + std::string(e.what()),
                              context);
    }
}

Value JsonLibrary::nativePrettyPrint(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.prettyPrint() requires exactly 1 argument (jsonString)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.prettyPrint() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Text(stringifyWithIndent(root, 2));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to pretty print JSON: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeMinify(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.minify() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.minify() argument must be Text", context);
    }

    try {
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Text(stringifyJsonValue(root, 0));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to minify JSON: " + std::string(e.what()), context);
    }
}

Value JsonLibrary::nativeEquals(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError(
            "json.equals() requires exactly 2 arguments (jsonString1, jsonString2)", context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("json.equals() arguments must be Text", context);
    }

    try {
        std::string jsonStr1 = std::get<Text>(args[0]);
        std::string jsonStr2 = std::get<Text>(args[1]);

        // Parse both JSON values and compare their string representations
        JsonValue json1 = parseJsonString(jsonStr1);
        JsonValue json2 = parseJsonString(jsonStr2);

        std::string normalized1 = stringifyJsonValue(json1);
        std::string normalized2 = stringifyJsonValue(json2);

        return Bool(normalized1 == normalized2);
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value JsonLibrary::nativeClone(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("json.clone() requires exactly 1 argument (jsonString)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("json.clone() argument must be Text", context);
    }

    try {
        // Cloning via parse/stringify cycle
        std::string jsonStr = std::get<Text>(args[0]);
        JsonValue root = parseJsonString(jsonStr);

        return Text(stringifyJsonValue(root));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to clone JSON: " + std::string(e.what()), context);
    }
}

// Helper method implementations
JsonValue JsonLibrary::parseJsonString(const std::string& jsonStr) {
    std::istringstream stream(jsonStr);
    skipWhitespace(stream);
    return parseJsonValue(stream);
}

JsonValue JsonLibrary::parseJsonValue(std::istringstream& stream) {
    skipWhitespace(stream);

    char c = stream.peek();

    if (c == '"') {
        return JsonValue(parseJsonStringLiteral(stream));
    } else if (c == '{') {
        return parseJsonObject(stream);
    } else if (c == '[') {
        return parseJsonArray(stream);
    } else if (c == 't' || c == 'f') {
        // Boolean
        std::string word;
        while (stream.peek() != EOF && std::isalpha(stream.peek())) {
            word += stream.get();
        }
        if (word == "true") return JsonValue(Bool(true));
        if (word == "false") return JsonValue(Bool(false));
        throw std::runtime_error("Invalid boolean value: " + word);
    } else if (c == 'n') {
        // null
        std::string word;
        while (stream.peek() != EOF && std::isalpha(stream.peek())) {
            word += stream.get();
        }
        if (word == "null") return JsonValue(nullptr);
        throw std::runtime_error("Invalid null value: " + word);
    } else if (std::isdigit(c) || c == '-') {
        return parseJsonNumber(stream);
    }

    throw std::runtime_error("Invalid JSON value");
}

JsonValue JsonLibrary::parseJsonObject(std::istringstream& stream) {
    std::map<Text, JsonValue> obj;

    stream.get();  // consume '{'
    skipWhitespace(stream);

    if (stream.peek() == '}') {
        stream.get();  // consume '}'
        return JsonValue(obj);
    }

    while (true) {
        skipWhitespace(stream);

        // Parse key
        if (stream.peek() != '"') {
            throw std::runtime_error("Expected string key in JSON object");
        }
        std::string key = parseJsonStringLiteral(stream);

        skipWhitespace(stream);

        // Expect ':'
        if (stream.get() != ':') {
            throw std::runtime_error("Expected ':' after key in JSON object");
        }

        skipWhitespace(stream);

        // Parse value
        JsonValue value = parseJsonValue(stream);
        obj[key] = value;

        skipWhitespace(stream);

        char next = stream.get();
        if (next == '}') {
            break;
        } else if (next == ',') {
            continue;
        } else {
            throw std::runtime_error("Expected ',' or '}' in JSON object");
        }
    }

    return JsonValue(obj);
}

JsonValue JsonLibrary::parseJsonArray(std::istringstream& stream) {
    std::vector<JsonValue> arr;

    stream.get();  // consume '['
    skipWhitespace(stream);

    if (stream.peek() == ']') {
        stream.get();  // consume ']'
        return JsonValue(arr);
    }

    while (true) {
        skipWhitespace(stream);

        JsonValue value = parseJsonValue(stream);
        arr.push_back(value);

        skipWhitespace(stream);

        char next = stream.get();
        if (next == ']') {
            break;
        } else if (next == ',') {
            continue;
        } else {
            throw std::runtime_error("Expected ',' or ']' in JSON array");
        }
    }

    return JsonValue(arr);
}

std::string JsonLibrary::parseJsonStringLiteral(std::istringstream& stream) {
    std::string result;

    stream.get();  // consume opening '"'

    while (stream.peek() != '"' && stream.peek() != EOF) {
        char c = stream.get();
        if (c == '\\') {
            char escaped = stream.get();
            switch (escaped) {
                case '"':
                    result += '"';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '/':
                    result += '/';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
                case 'n':
                    result += '\n';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case 't':
                    result += '\t';
                    break;
                default:
                    result += escaped;
                    break;
            }
        } else {
            result += c;
        }
    }

    if (stream.peek() != '"') {
        throw std::runtime_error("Unterminated string literal");
    }

    stream.get();  // consume closing '"'
    return result;
}

JsonValue JsonLibrary::parseJsonNumber(std::istringstream& stream) {
    std::string numStr;

    char c = stream.peek();
    if (c == '-') {
        numStr += stream.get();
    }

    while (stream.peek() != EOF && (std::isdigit(stream.peek()) || stream.peek() == '.')) {
        numStr += stream.get();
    }

    try {
        if (numStr.find('.') != std::string::npos) {
            return JsonValue(std::stod(numStr));
        } else {
            return JsonValue(static_cast<Int>(std::stoll(numStr)));
        }
    } catch (const std::exception&) {
        throw std::runtime_error("Invalid number: " + numStr);
    }
}

std::string JsonLibrary::stringifyJsonValue(const JsonValue& value, int indent) {
    if (std::holds_alternative<std::nullptr_t>(value)) {
        return "null";
    } else if (std::holds_alternative<Bool>(value)) {
        return std::get<Bool>(value) ? "true" : "false";
    } else if (std::holds_alternative<Int>(value)) {
        return std::to_string(std::get<Int>(value));
    } else if (std::holds_alternative<Double>(value)) {
        return std::to_string(std::get<Double>(value));
    } else if (std::holds_alternative<Text>(value)) {
        return "\"" + escapeJsonString(std::get<Text>(value)) + "\"";
    } else if (std::holds_alternative<std::vector<JsonValue>>(value)) {
        std::ostringstream oss;
        oss << "[";
        const auto& arr = std::get<std::vector<JsonValue>>(value);
        for (size_t i = 0; i < arr.size(); ++i) {
            if (i > 0) oss << ",";
            oss << stringifyJsonValue(arr[i], indent);
        }
        oss << "]";
        return oss.str();
    } else if (std::holds_alternative<std::map<Text, JsonValue>>(value)) {
        std::ostringstream oss;
        oss << "{";
        const auto& obj = std::get<std::map<Text, JsonValue>>(value);
        bool first = true;
        for (const auto& [key, val] : obj) {
            if (!first) oss << ",";
            oss << "\"" << escapeJsonString(key) << "\":" << stringifyJsonValue(val, indent);
            first = false;
        }
        oss << "}";
        return oss.str();
    }

    return "null";
}

std::string JsonLibrary::stringifyWithIndent(const JsonValue& value, int indent) {
    // Simplified pretty printing - would need full implementation
    return stringifyJsonValue(value, indent);
}

std::string JsonLibrary::escapeJsonString(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"':
                result += "\\\"";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            default:
                result += c;
                break;
        }
    }
    return result;
}

JsonValue JsonLibrary::o2lValueToJson(const Value& value) {
    if (std::holds_alternative<Bool>(value)) {
        return JsonValue(std::get<Bool>(value));
    } else if (std::holds_alternative<Int>(value)) {
        return JsonValue(std::get<Int>(value));
    } else if (std::holds_alternative<Float>(value)) {
        return JsonValue(static_cast<Double>(std::get<Float>(value)));
    } else if (std::holds_alternative<Double>(value)) {
        return JsonValue(std::get<Double>(value));
    } else if (std::holds_alternative<Text>(value)) {
        return JsonValue(std::get<Text>(value));
    }

    // For now, return null for complex types
    return JsonValue(nullptr);
}

Value JsonLibrary::jsonValueToO2L(const JsonValue& jsonValue) {
    if (std::holds_alternative<std::nullptr_t>(jsonValue)) {
        return Text("null");
    } else if (std::holds_alternative<Bool>(jsonValue)) {
        return std::get<Bool>(jsonValue);
    } else if (std::holds_alternative<Int>(jsonValue)) {
        return std::get<Int>(jsonValue);
    } else if (std::holds_alternative<Double>(jsonValue)) {
        // Convert Double to Int if it's a whole number, otherwise return as Double
        Double doubleVal = std::get<Double>(jsonValue);
        if (doubleVal == static_cast<Int>(doubleVal)) {
            return static_cast<Int>(doubleVal);
        }
        return doubleVal;
    } else if (std::holds_alternative<Text>(jsonValue)) {
        return std::get<Text>(jsonValue);
    }

    // For complex types, return as JSON string
    return Text(stringifyJsonValue(jsonValue));
}

Value JsonLibrary::jsonValueToO2LNative(const JsonValue& jsonValue) {
    if (std::holds_alternative<std::nullptr_t>(jsonValue)) {
        return Text("null");
    } else if (std::holds_alternative<Bool>(jsonValue)) {
        return std::get<Bool>(jsonValue);
    } else if (std::holds_alternative<Int>(jsonValue)) {
        return std::get<Int>(jsonValue);
    } else if (std::holds_alternative<Double>(jsonValue)) {
        return std::get<Double>(jsonValue);
    } else if (std::holds_alternative<Text>(jsonValue)) {
        return std::get<Text>(jsonValue);
    } else if (std::holds_alternative<std::vector<JsonValue>>(jsonValue)) {
        // Convert JSON array to O²L List
        const auto& jsonArray = std::get<std::vector<JsonValue>>(jsonValue);
        auto o2lList = std::make_shared<ListInstance>();

        for (const auto& item : jsonArray) {
            Value o2lValue = jsonValueToO2LNative(item);
            o2lList->add(o2lValue);
        }

        return o2lList;
    } else if (std::holds_alternative<std::map<Text, JsonValue>>(jsonValue)) {
        // Convert JSON object to O²L Map
        const auto& jsonObject = std::get<std::map<Text, JsonValue>>(jsonValue);
        auto o2lMap = std::make_shared<MapInstance>();

        for (const auto& [key, value] : jsonObject) {
            Value o2lKey = Text(key);
            Value o2lValue = jsonValueToO2LNative(value);
            o2lMap->put(o2lKey, o2lValue);
        }

        return o2lMap;
    }

    // Fallback
    return Text("null");
}

JsonValue JsonLibrary::getJsonValueAtPath(const JsonValue& root, const std::string& path) {
    if (path.empty()) {
        return root;
    }

    std::vector<std::string> pathParts = parsePath(path);
    JsonValue current = root;

    for (const auto& part : pathParts) {
        if (std::holds_alternative<std::map<Text, JsonValue>>(current)) {
            const auto& obj = std::get<std::map<Text, JsonValue>>(current);
            auto it = obj.find(part);
            if (it != obj.end()) {
                // Use std::visit for safer variant copying
                current = std::visit(
                    [](const auto& value) -> JsonValue { return JsonValue(value); }, it->second);
            } else {
                throw std::runtime_error("Path not found: " + part);
            }
        } else if (std::holds_alternative<std::vector<JsonValue>>(current)) {
            if (isArrayIndex(part)) {
                size_t index = parseArrayIndex(part);
                const auto& arr = std::get<std::vector<JsonValue>>(current);
                if (index < arr.size()) {
                    // Use std::visit for safer variant copying
                    current =
                        std::visit([](const auto& value) -> JsonValue { return JsonValue(value); },
                                   arr[index]);
                } else {
                    throw std::runtime_error("Array index out of bounds: " + part);
                }
            } else {
                throw std::runtime_error("Invalid array index: " + part);
            }
        } else {
            throw std::runtime_error("Cannot navigate into non-object/non-array type");
        }
    }

    return current;
}

JsonValue JsonLibrary::setJsonValueAtPath(JsonValue root, const std::string& path,
                                          const JsonValue& value) {
    if (path.empty()) {
        return value;
    }

    std::vector<std::string> pathParts = parsePath(path);
    if (pathParts.empty()) {
        return value;
    }

    // Helper function to recursively set value
    std::function<JsonValue(JsonValue, const std::vector<std::string>&, size_t)> setRecursive =
        [&](JsonValue current, const std::vector<std::string>& parts, size_t index) -> JsonValue {
        if (index >= parts.size()) {
            return current;
        }

        const std::string& part = parts[index];
        bool isLast = (index == parts.size() - 1);

        if (isLast) {
            // Set the value at this level
            if (std::holds_alternative<std::map<Text, JsonValue>>(current)) {
                auto obj = std::get<std::map<Text, JsonValue>>(current);
                obj[part] = value;
                return JsonValue(obj);
            } else if (std::holds_alternative<std::vector<JsonValue>>(current)) {
                if (isArrayIndex(part)) {
                    auto arr = std::get<std::vector<JsonValue>>(current);
                    size_t idx = parseArrayIndex(part);
                    if (idx < arr.size()) {
                        arr[idx] = value;
                        return JsonValue(arr);
                    }
                }
            }
        } else {
            // Navigate deeper
            if (std::holds_alternative<std::map<Text, JsonValue>>(current)) {
                auto obj = std::get<std::map<Text, JsonValue>>(current);
                auto it = obj.find(part);
                if (it != obj.end()) {
                    obj[part] = setRecursive(it->second, parts, index + 1);
                    return JsonValue(obj);
                }
            } else if (std::holds_alternative<std::vector<JsonValue>>(current)) {
                if (isArrayIndex(part)) {
                    auto arr = std::get<std::vector<JsonValue>>(current);
                    size_t idx = parseArrayIndex(part);
                    if (idx < arr.size()) {
                        arr[idx] = setRecursive(arr[idx], parts, index + 1);
                        return JsonValue(arr);
                    }
                }
            }
        }

        return current;
    };

    return setRecursive(root, pathParts, 0);
}

JsonValue JsonLibrary::removeJsonValueAtPath(JsonValue root, const std::string& path) {
    if (path.empty()) {
        return root;
    }

    std::vector<std::string> pathParts = parsePath(path);
    if (pathParts.empty()) {
        return root;
    }

    // Helper function to recursively remove value
    std::function<JsonValue(JsonValue, const std::vector<std::string>&, size_t)> removeRecursive =
        [&](JsonValue current, const std::vector<std::string>& parts, size_t index) -> JsonValue {
        if (index >= parts.size()) {
            return current;
        }

        const std::string& part = parts[index];
        bool isLast = (index == parts.size() - 1);

        if (isLast) {
            // Remove the value at this level
            if (std::holds_alternative<std::map<Text, JsonValue>>(current)) {
                auto obj = std::get<std::map<Text, JsonValue>>(current);
                obj.erase(part);
                return JsonValue(obj);
            } else if (std::holds_alternative<std::vector<JsonValue>>(current)) {
                if (isArrayIndex(part)) {
                    auto arr = std::get<std::vector<JsonValue>>(current);
                    size_t idx = parseArrayIndex(part);
                    if (idx < arr.size()) {
                        arr.erase(arr.begin() + idx);
                        return JsonValue(arr);
                    }
                }
            }
        } else {
            // Navigate deeper
            if (std::holds_alternative<std::map<Text, JsonValue>>(current)) {
                auto obj = std::get<std::map<Text, JsonValue>>(current);
                auto it = obj.find(part);
                if (it != obj.end()) {
                    obj[part] = removeRecursive(it->second, parts, index + 1);
                    return JsonValue(obj);
                }
            } else if (std::holds_alternative<std::vector<JsonValue>>(current)) {
                if (isArrayIndex(part)) {
                    auto arr = std::get<std::vector<JsonValue>>(current);
                    size_t idx = parseArrayIndex(part);
                    if (idx < arr.size()) {
                        arr[idx] = removeRecursive(arr[idx], parts, index + 1);
                        return JsonValue(arr);
                    }
                }
            }
        }

        return current;
    };

    return removeRecursive(root, pathParts, 0);
}

bool JsonLibrary::isValidJsonString(const std::string& jsonStr) {
    try {
        parseJsonString(jsonStr);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

std::string JsonLibrary::getJsonTypeName(const JsonValue& value) {
    if (std::holds_alternative<std::nullptr_t>(value)) {
        return "null";
    } else if (std::holds_alternative<Bool>(value)) {
        return "boolean";
    } else if (std::holds_alternative<Int>(value)) {
        return "number";
    } else if (std::holds_alternative<Double>(value)) {
        return "number";
    } else if (std::holds_alternative<Text>(value)) {
        return "string";
    } else if (std::holds_alternative<std::vector<JsonValue>>(value)) {
        return "array";
    } else if (std::holds_alternative<std::map<Text, JsonValue>>(value)) {
        return "object";
    }

    return "unknown";
}

void JsonLibrary::skipWhitespace(std::istringstream& stream) {
    while (stream.peek() != EOF && std::isspace(stream.peek())) {
        stream.get();
    }
}

std::vector<std::string> JsonLibrary::parsePath(const std::string& path) {
    // Simple implementation - split by dots
    std::vector<std::string> parts;
    std::istringstream stream(path);
    std::string part;

    while (std::getline(stream, part, '.')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }

    return parts;
}

bool JsonLibrary::isArrayIndex(const std::string& part) {
    return !part.empty() && std::all_of(part.begin(), part.end(), ::isdigit);
}

size_t JsonLibrary::parseArrayIndex(const std::string& part) {
    return std::stoull(part);
}

}  // namespace o2l