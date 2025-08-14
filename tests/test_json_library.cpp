#include <gtest/gtest.h>

#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/JsonLibrary.hpp"
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/MapInstance.hpp"

using namespace o2l;

class JsonLibraryTest : public ::testing::Test {
   protected:
    void SetUp() override {
        context = std::make_unique<Context>();
        json_lib = JsonLibrary::createJsonObject();
    }

    Context* getContext() {
        return context.get();
    }

    // Helper method to call JSON library methods
    Value callJsonMethod(const std::string& method_name, const std::vector<Value>& args) {
        return json_lib->callMethod(method_name, args, *context);
    }

    std::unique_ptr<Context> context;
    std::shared_ptr<ObjectInstance> json_lib;
};

// Test Basic JSON Parsing
TEST_F(JsonLibraryTest, BasicJsonParsing) {
    // Test parse method
    std::vector<Value> args = {Text("{\"name\": \"Alice\", \"age\": 30}")};
    Value result = callJsonMethod("parse", args);

    ASSERT_TRUE(std::holds_alternative<Text>(result));
    Text parsed = std::get<Text>(result);

    // Should be valid JSON (order might be different)
    std::vector<Value> valid_args = {parsed};
    Value is_valid = callJsonMethod("isValid", valid_args);
    EXPECT_TRUE(std::get<Bool>(is_valid));
}

// Test JSON Validation
TEST_F(JsonLibraryTest, JsonValidation) {
    // Test valid JSON
    std::vector<Value> valid_args = {Text("{\"test\": 123}")};
    Value valid_result = callJsonMethod("isValid", valid_args);
    EXPECT_TRUE(std::get<Bool>(valid_result));

    // Test invalid JSON
    std::vector<Value> invalid_args = {Text("{invalid}")};
    Value invalid_result = callJsonMethod("isValid", invalid_args);
    EXPECT_FALSE(std::get<Bool>(invalid_result));
}

// Test Type Detection
TEST_F(JsonLibraryTest, TypeDetection) {
    Text obj_json = "{\"test\": 123}";
    Text array_json = "[1, 2, 3]";
    Text string_json = "\"hello\"";
    Text number_json = "456";
    Text bool_json = "true";
    Text null_json = "null";

    // Test isObject
    EXPECT_TRUE(std::get<Bool>(callJsonMethod("isObject", {obj_json})));
    EXPECT_FALSE(std::get<Bool>(callJsonMethod("isObject", {array_json})));

    // Test isArray
    EXPECT_TRUE(std::get<Bool>(callJsonMethod("isArray", {array_json})));
    EXPECT_FALSE(std::get<Bool>(callJsonMethod("isArray", {obj_json})));

    // Test isString
    EXPECT_TRUE(std::get<Bool>(callJsonMethod("isString", {string_json})));
    EXPECT_FALSE(std::get<Bool>(callJsonMethod("isString", {number_json})));

    // Test isNumber
    EXPECT_TRUE(std::get<Bool>(callJsonMethod("isNumber", {number_json})));
    EXPECT_FALSE(std::get<Bool>(callJsonMethod("isNumber", {string_json})));

    // Test isBoolean
    EXPECT_TRUE(std::get<Bool>(callJsonMethod("isBoolean", {bool_json})));
    EXPECT_FALSE(std::get<Bool>(callJsonMethod("isBoolean", {number_json})));

    // Test isNull
    EXPECT_TRUE(std::get<Bool>(callJsonMethod("isNull", {null_json})));
    EXPECT_FALSE(std::get<Bool>(callJsonMethod("isNull", {string_json})));
}

// Test Path Navigation
TEST_F(JsonLibraryTest, PathNavigation) {
    Text json_obj = "{\"name\": \"Alice\", \"age\": 30, \"active\": true}";

    // Test string path
    Value name_result = callJsonMethod("get", {json_obj, Text("name")});
    EXPECT_EQ(std::get<Text>(name_result), "Alice");

    // Test integer path
    Value age_result = callJsonMethod("get", {json_obj, Text("age")});
    EXPECT_EQ(std::get<Int>(age_result), 30);

    // Test boolean path
    Value active_result = callJsonMethod("get", {json_obj, Text("active")});
    EXPECT_TRUE(std::get<Bool>(active_result));
}

// Test Nested Path Navigation
TEST_F(JsonLibraryTest, NestedPathNavigation) {
    Text nested_json = "{\"user\": {\"profile\": {\"name\": \"Bob\", \"score\": 95}}}";

    // Test nested string access
    Value name_result = callJsonMethod("get", {nested_json, Text("user.profile.name")});
    EXPECT_EQ(std::get<Text>(name_result), "Bob");

    // Test nested number access
    Value score_result = callJsonMethod("get", {nested_json, Text("user.profile.score")});
    EXPECT_EQ(std::get<Int>(score_result), 95);
}

// Test Array Path Navigation
TEST_F(JsonLibraryTest, ArrayPathNavigation) {
    Text array_json = "{\"items\": [\"first\", \"second\", \"third\"]}";

    // Test array index access
    Value first_result = callJsonMethod("get", {array_json, Text("items.0")});
    EXPECT_EQ(std::get<Text>(first_result), "first");

    Value second_result = callJsonMethod("get", {array_json, Text("items.1")});
    EXPECT_EQ(std::get<Text>(second_result), "second");
}

// Test Auto-Detection Parsing
TEST_F(JsonLibraryTest, AutoDetectionParsing) {
    // Test object auto-detection
    Text obj_json = "{\"name\": \"Bob\", \"count\": 5}";
    Value obj_result = callJsonMethod("parseAuto", {obj_json});

    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(obj_result));
    auto map_instance = std::get<std::shared_ptr<MapInstance>>(obj_result);

    Value name_val = map_instance->get(Text("name"));
    Value count_val = map_instance->get(Text("count"));

    EXPECT_EQ(std::get<Text>(name_val), "Bob");
    EXPECT_EQ(std::get<Int>(count_val), 5);

    // Test array auto-detection
    Text arr_json = "[\"red\", \"green\", \"blue\"]";
    Value arr_result = callJsonMethod("parseAuto", {arr_json});

    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(arr_result));
    auto list_instance = std::get<std::shared_ptr<ListInstance>>(arr_result);

    EXPECT_EQ(list_instance->size(), 3);
    Value first_item = list_instance->get(0);
    EXPECT_EQ(std::get<Text>(first_item), "red");

    // Test primitive auto-detection
    Value num_result = callJsonMethod("parseAuto", {Text("42")});
    EXPECT_EQ(std::get<Int>(num_result), 42);

    Value str_result = callJsonMethod("parseAuto", {Text("\"hello\"")});
    EXPECT_EQ(std::get<Text>(str_result), "hello");

    Value bool_result = callJsonMethod("parseAuto", {Text("true")});
    EXPECT_TRUE(std::get<Bool>(bool_result));
}

// Test Native Map Parsing
TEST_F(JsonLibraryTest, NativeMapParsing) {
    Text obj_json = "{\"x\": 10, \"y\": 20}";
    Value map_result = callJsonMethod("parseToMap", {obj_json});

    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(map_result));
    auto map_instance = std::get<std::shared_ptr<MapInstance>>(map_result);

    Value x_val = map_instance->get(Text("x"));
    Value y_val = map_instance->get(Text("y"));

    EXPECT_EQ(std::get<Int>(x_val), 10);
    EXPECT_EQ(std::get<Int>(y_val), 20);
}

// Test Native List Parsing
TEST_F(JsonLibraryTest, NativeListParsing) {
    Text list_json = "[1, 2, 3, 4, 5]";
    Value list_result = callJsonMethod("parseToList", {list_json});

    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(list_result));
    auto list_instance = std::get<std::shared_ptr<ListInstance>>(list_result);

    EXPECT_EQ(list_instance->size(), 5);

    Value third_item = list_instance->get(2);
    EXPECT_EQ(std::get<Int>(third_item), 3);
}

// Test JSON Generation
TEST_F(JsonLibraryTest, JsonGeneration) {
    // Test createObject
    Value obj_result =
        callJsonMethod("createObject", {Text("title"), Text("Book"), Text("pages"), Int(200)});

    Text created_obj = std::get<Text>(obj_result);
    Value is_valid = callJsonMethod("isValid", {created_obj});
    EXPECT_TRUE(std::get<Bool>(is_valid));

    // Test createArray
    Value arr_result =
        callJsonMethod("createArray", {Text("apple"), Text("banana"), Text("cherry")});

    Text created_arr = std::get<Text>(arr_result);
    Value is_arr_valid = callJsonMethod("isValid", {created_arr});
    EXPECT_TRUE(std::get<Bool>(is_arr_valid));
}

// Test JSON Manipulation
TEST_F(JsonLibraryTest, JsonManipulation) {
    // Test set operation
    Text orig_json = "{\"count\": 5}";
    Value updated_result = callJsonMethod("set", {orig_json, Text("count"), Int(10)});
    Text updated_json = std::get<Text>(updated_result);

    Value new_count = callJsonMethod("get", {updated_json, Text("count")});
    EXPECT_EQ(std::get<Int>(new_count), 10);

    // Test merge operation
    Text json1 = "{\"a\": 1}";
    Text json2 = "{\"b\": 2}";
    Value merged_result = callJsonMethod("merge", {json1, json2});
    Text merged_json = std::get<Text>(merged_result);

    Value has_a = callJsonMethod("hasKey", {merged_json, Text("a")});
    Value has_b = callJsonMethod("hasKey", {merged_json, Text("b")});

    EXPECT_TRUE(std::get<Bool>(has_a));
    EXPECT_TRUE(std::get<Bool>(has_b));
}

// Test Array Operations
TEST_F(JsonLibraryTest, ArrayOperations) {
    // Test push
    Text orig_arr = "[1, 2, 3]";
    Value pushed_result = callJsonMethod("push", {orig_arr, Int(4)});
    Text pushed_arr = std::get<Text>(pushed_result);

    Value pushed_size = callJsonMethod("size", {pushed_arr});
    EXPECT_EQ(std::get<Int>(pushed_size), 4);

    // Test pop
    Value popped_result = callJsonMethod("pop", {pushed_arr});
    Text popped_arr = std::get<Text>(popped_result);

    Value popped_size = callJsonMethod("size", {popped_arr});
    EXPECT_EQ(std::get<Int>(popped_size), 3);

    // Test slice
    Text test_arr = "[10, 20, 30, 40, 50]";
    Value sliced_result = callJsonMethod("slice", {test_arr, Int(1), Int(4)});
    Text sliced_arr = std::get<Text>(sliced_result);

    Value sliced_size = callJsonMethod("size", {sliced_arr});
    EXPECT_EQ(std::get<Int>(sliced_size), 3);

    // Test indexOf
    Text search_arr = "[\"a\", \"b\", \"c\"]";
    Value index_b = callJsonMethod("indexOf", {search_arr, Text("b")});
    Value index_missing = callJsonMethod("indexOf", {search_arr, Text("x")});

    EXPECT_EQ(std::get<Int>(index_b), 1);
    EXPECT_EQ(std::get<Int>(index_missing), -1);
}

// Test Utility Functions
TEST_F(JsonLibraryTest, UtilityFunctions) {
    // Test pretty print and minify
    Text compact = "{\"a\":1,\"b\":2}";

    Value pretty_result = callJsonMethod("prettyPrint", {compact});
    Text pretty = std::get<Text>(pretty_result);

    Value minified_result = callJsonMethod("minify", {pretty});
    Text minified = std::get<Text>(minified_result);

    Value compact_valid = callJsonMethod("isValid", {compact});
    Value pretty_valid = callJsonMethod("isValid", {pretty});
    Value minified_valid = callJsonMethod("isValid", {minified});

    EXPECT_TRUE(std::get<Bool>(compact_valid));
    EXPECT_TRUE(std::get<Bool>(pretty_valid));
    EXPECT_TRUE(std::get<Bool>(minified_valid));

    // Test equals
    Text json_a = "{\"x\": 1, \"y\": 2}";
    Text json_b = "{\"y\": 2, \"x\": 1}";
    Text json_c = "{\"x\": 1, \"y\": 3}";

    Value equal_ab = callJsonMethod("equals", {json_a, json_b});
    Value equal_ac = callJsonMethod("equals", {json_a, json_c});

    EXPECT_TRUE(std::get<Bool>(equal_ab));
    EXPECT_FALSE(std::get<Bool>(equal_ac));

    // Test clone
    Text original = "{\"data\": [1, 2, 3]}";
    Value cloned_result = callJsonMethod("clone", {original});
    Text cloned = std::get<Text>(cloned_result);

    Value clones_equal = callJsonMethod("equals", {original, cloned});
    EXPECT_TRUE(std::get<Bool>(clones_equal));
}

// Test Edge Cases
TEST_F(JsonLibraryTest, EdgeCases) {
    // Test empty structures
    Text empty_obj = "{}";
    Text empty_arr = "[]";

    Value empty_obj_size = callJsonMethod("size", {empty_obj});
    Value empty_arr_size = callJsonMethod("size", {empty_arr});

    EXPECT_EQ(std::get<Int>(empty_obj_size), 0);
    EXPECT_EQ(std::get<Int>(empty_arr_size), 0);

    // Test nested empty structures
    Text nested_empty = "{\"data\": [], \"meta\": {}}";
    Value nested_valid = callJsonMethod("isValid", {nested_empty});
    EXPECT_TRUE(std::get<Bool>(nested_valid));

    // Test special values
    Text special_json = "{\"null_val\": null, \"bool_val\": false, \"zero\": 0}";

    Value null_type = callJsonMethod("getType", {special_json, Text("null_val")});
    Value bool_type = callJsonMethod("getType", {special_json, Text("bool_val")});
    Value zero_type = callJsonMethod("getType", {special_json, Text("zero")});

    EXPECT_EQ(std::get<Text>(null_type), "null");
    EXPECT_EQ(std::get<Text>(bool_type), "boolean");
    EXPECT_EQ(std::get<Text>(zero_type), "number");
}

// Test Error Handling
TEST_F(JsonLibraryTest, ErrorHandling) {
    // Test invalid JSON parsing should not crash
    EXPECT_NO_THROW({
        Value result = callJsonMethod("isValid", {Text("{invalid json}")});
        EXPECT_FALSE(std::get<Bool>(result));
    });

    // Test path not found should throw appropriate error
    Text valid_json = "{\"existing\": \"value\"}";
    EXPECT_THROW({ callJsonMethod("get", {valid_json, Text("nonexistent")}); }, std::exception);

    // Test type mismatch for parseToMap
    Text array_json = "[1, 2, 3]";
    EXPECT_THROW({ callJsonMethod("parseToMap", {array_json}); }, std::exception);

    // Test type mismatch for parseToList
    Text object_json = "{\"key\": \"value\"}";
    EXPECT_THROW({ callJsonMethod("parseToList", {object_json}); }, std::exception);
}

// Performance Test (basic)
TEST_F(JsonLibraryTest, BasicPerformance) {
    // Test with moderately sized JSON
    std::string large_json = "{";
    for (int i = 0; i < 100; ++i) {
        if (i > 0) large_json += ",";
        large_json += "\"key" + std::to_string(i) + "\": " + std::to_string(i);
    }
    large_json += "}";

    // Should handle reasonably quickly without throwing
    EXPECT_NO_THROW({
        Value valid = callJsonMethod("isValid", {Text(large_json)});
        EXPECT_TRUE(std::get<Bool>(valid));

        Value size = callJsonMethod("size", {Text(large_json)});
        EXPECT_EQ(std::get<Int>(size), 100);
    });
}