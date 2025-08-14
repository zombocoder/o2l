#pragma once

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// Forward declaration
struct JsonValue;

// JSON value types that mirror O²L's type system
struct JsonValue : public std::variant<std::nullptr_t,            // JSON null
                                       Bool,                      // JSON boolean
                                       Int,                       // JSON integer
                                       Double,                    // JSON number
                                       Text,                      // JSON string
                                       std::vector<JsonValue>,    // JSON array
                                       std::map<Text, JsonValue>  // JSON object
                                       > {
    using variant::variant;
};

class JsonLibrary {
   public:
    // Create the json module object
    static std::shared_ptr<ObjectInstance> createJsonObject();

    // JSON parsing methods
    static Value nativeParse(const std::vector<Value>& args, Context& context);
    static Value nativeParseAuto(const std::vector<Value>& args, Context& context);
    static Value nativeParseToMap(const std::vector<Value>& args, Context& context);
    static Value nativeParseToList(const std::vector<Value>& args, Context& context);
    static Value nativeIsValid(const std::vector<Value>& args, Context& context);
    static Value nativeGet(const std::vector<Value>& args, Context& context);
    static Value nativeGetType(const std::vector<Value>& args, Context& context);
    static Value nativeHasKey(const std::vector<Value>& args, Context& context);

    // JSON generation methods
    static Value nativeStringify(const std::vector<Value>& args, Context& context);
    static Value nativeCreate(const std::vector<Value>& args, Context& context);
    static Value nativeCreateArray(const std::vector<Value>& args, Context& context);
    static Value nativeCreateObject(const std::vector<Value>& args, Context& context);

    // JSON manipulation methods
    static Value nativeSet(const std::vector<Value>& args, Context& context);
    static Value nativeRemove(const std::vector<Value>& args, Context& context);
    static Value nativeMerge(const std::vector<Value>& args, Context& context);
    static Value nativeKeys(const std::vector<Value>& args, Context& context);
    static Value nativeValues(const std::vector<Value>& args, Context& context);
    static Value nativeSize(const std::vector<Value>& args, Context& context);

    // JSON array methods
    static Value nativePush(const std::vector<Value>& args, Context& context);
    static Value nativePop(const std::vector<Value>& args, Context& context);
    static Value nativeSlice(const std::vector<Value>& args, Context& context);
    static Value nativeIndexOf(const std::vector<Value>& args, Context& context);

    // JSON validation methods
    static Value nativeIsObject(const std::vector<Value>& args, Context& context);
    static Value nativeIsArray(const std::vector<Value>& args, Context& context);
    static Value nativeIsString(const std::vector<Value>& args, Context& context);
    static Value nativeIsNumber(const std::vector<Value>& args, Context& context);
    static Value nativeIsBoolean(const std::vector<Value>& args, Context& context);
    static Value nativeIsNull(const std::vector<Value>& args, Context& context);

    // JSON utility methods
    static Value nativePrettyPrint(const std::vector<Value>& args, Context& context);
    static Value nativeMinify(const std::vector<Value>& args, Context& context);
    static Value nativeEquals(const std::vector<Value>& args, Context& context);
    static Value nativeClone(const std::vector<Value>& args, Context& context);

   private:
    // Helper methods for JSON parsing
    static JsonValue parseJsonString(const std::string& jsonStr);
    static JsonValue parseJsonValue(std::istringstream& stream);
    static JsonValue parseJsonObject(std::istringstream& stream);
    static JsonValue parseJsonArray(std::istringstream& stream);
    static std::string parseJsonStringLiteral(std::istringstream& stream);
    static JsonValue parseJsonNumber(std::istringstream& stream);

    // Helper methods for JSON generation
    static std::string stringifyJsonValue(const JsonValue& value, int indent = 0);
    static std::string stringifyWithIndent(const JsonValue& value, int indent);
    static std::string escapeJsonString(const std::string& str);
    static std::string unescapeJsonString(const std::string& str);

    // Helper methods for JSON manipulation
    static JsonValue o2lValueToJson(const Value& value);
    static Value jsonValueToO2L(const JsonValue& jsonValue);
    static Value jsonValueToO2LNative(const JsonValue& jsonValue);
    static JsonValue getJsonValueAtPath(const JsonValue& root, const std::string& path);
    static JsonValue setJsonValueAtPath(JsonValue root, const std::string& path,
                                        const JsonValue& value);
    static JsonValue removeJsonValueAtPath(JsonValue root, const std::string& path);

    // Helper methods for JSON validation
    static bool isValidJsonString(const std::string& jsonStr);
    static std::string getJsonTypeName(const JsonValue& value);
    static void skipWhitespace(std::istringstream& stream);

    // Helper methods for path parsing (supports dot notation and array indices)
    static std::vector<std::string> parsePath(const std::string& path);
    static bool isArrayIndex(const std::string& part);
    static size_t parseArrayIndex(const std::string& part);
};

}  // namespace o2l