#include <gtest/gtest.h>

#include <memory>

#include "../src/Common/Exceptions.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/RegexpLibrary.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

class RegexpLibraryTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to create regexp object and call methods
    Value callRegexpMethod(const std::string& method_name, const std::vector<Value>& args = {}) {
        auto regexp_object = RegexpLibrary::createRegexpObject();
        EXPECT_TRUE(regexp_object->hasMethod(method_name))
            << "Method " << method_name << " not found";
        return regexp_object->callMethod(method_name, args, context);
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
        return !text.empty();
    }

    // Helper to get list size
    size_t getListSize(const Value& result) {
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result));
        auto list = std::get<std::shared_ptr<ListInstance>>(result);
        return list->size();
    }

    // Helper to get list item
    Text getListItem(const Value& result, size_t index) {
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result));
        auto list = std::get<std::shared_ptr<ListInstance>>(result);
        EXPECT_LT(index, list->size());
        Value item = list->get(static_cast<int>(index));
        EXPECT_TRUE(std::holds_alternative<Text>(item));
        return std::get<Text>(item);
    }
};

// Test match method
TEST_F(RegexpLibraryTest, MatchMethod) {
    // Basic matching
    Value result1 = callRegexpMethod("match", {Value(Text("Hello World")), Value(Text("Hello"))});
    expectBool(result1, true);

    Value result2 = callRegexpMethod("match", {Value(Text("Hello World")), Value(Text("Goodbye"))});
    expectBool(result2, false);

    // Pattern matching
    Value result3 = callRegexpMethod("match", {Value(Text("test123")), Value(Text("\\d+"))});
    expectBool(result3, true);

    Value result4 = callRegexpMethod("match", {Value(Text("testABC")), Value(Text("\\d+"))});
    expectBool(result4, false);

    // Case insensitive matching with flags
    Value result5 = callRegexpMethod(
        "match", {Value(Text("Hello World")), Value(Text("hello")), Value(Text("i"))});
    expectBool(result5, true);

    Value result6 = callRegexpMethod("match", {Value(Text("Hello World")), Value(Text("hello"))});
    expectBool(result6, false);

    // Test error cases
    EXPECT_THROW(callRegexpMethod("match", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("match", {Value(Int(123))}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("match", {Value(Text("test"))}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("match", {Value(Text("test")), Value(Int(123))}),
                 EvaluationError);
}

// Test find method
TEST_F(RegexpLibraryTest, FindMethod) {
    // Basic find
    Value result1 = callRegexpMethod("find", {Value(Text("Hello World")), Value(Text("World"))});
    expectText(result1, "World");

    Value result2 = callRegexpMethod("find", {Value(Text("Hello World")), Value(Text("Goodbye"))});
    expectText(result2, "");

    // Pattern find
    Value result3 = callRegexpMethod("find", {Value(Text("test123abc")), Value(Text("\\d+"))});
    expectText(result3, "123");

    Value result4 = callRegexpMethod(
        "find", {Value(Text("The price is $45.99")), Value(Text("\\$\\d+\\.\\d+"))});
    expectText(result4, "$45.99");

    // Case insensitive find
    Value result5 = callRegexpMethod(
        "find", {Value(Text("Hello World")), Value(Text("world")), Value(Text("i"))});
    expectText(result5, "World");

    // Test error cases
    EXPECT_THROW(callRegexpMethod("find", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("find", {Value(Text("test"))}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("find", {Value(Int(123)), Value(Text("test"))}), EvaluationError);
}

// Test findAll method
TEST_F(RegexpLibraryTest, FindAllMethod) {
    // Find all numbers
    Value result1 = callRegexpMethod(
        "findAll", {Value(Text("I have 3 apples and 7 oranges")), Value(Text("\\d+"))});
    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result1));
    EXPECT_EQ(getListSize(result1), 2);
    EXPECT_EQ(getListItem(result1, 0), "3");
    EXPECT_EQ(getListItem(result1, 1), "7");

    // Find all words
    Value result2 =
        callRegexpMethod("findAll", {Value(Text("Hello World Test")), Value(Text("\\w+"))});
    EXPECT_EQ(getListSize(result2), 3);
    EXPECT_EQ(getListItem(result2, 0), "Hello");
    EXPECT_EQ(getListItem(result2, 1), "World");
    EXPECT_EQ(getListItem(result2, 2), "Test");

    // No matches
    Value result3 = callRegexpMethod("findAll", {Value(Text("Hello World")), Value(Text("\\d+"))});
    EXPECT_EQ(getListSize(result3), 0);

    // Case insensitive find all
    Value result4 = callRegexpMethod(
        "findAll", {Value(Text("Apple apple APPLE")), Value(Text("apple")), Value(Text("i"))});
    EXPECT_EQ(getListSize(result4), 3);

    // Test error cases
    EXPECT_THROW(callRegexpMethod("findAll", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("findAll", {Value(Text("test"))}), EvaluationError);
}

// Test replace method
TEST_F(RegexpLibraryTest, ReplaceMethod) {
    // Basic replace (first occurrence only)
    Value result1 = callRegexpMethod(
        "replace", {Value(Text("Hello World Hello")), Value(Text("Hello")), Value(Text("Hi"))});
    expectText(result1, "Hi World Hello");

    // Pattern replace
    Value result2 = callRegexpMethod(
        "replace", {Value(Text("test123abc")), Value(Text("\\d+")), Value(Text("XXX"))});
    expectText(result2, "testXXXabc");

    // Replace with empty string (remove)
    Value result3 = callRegexpMethod(
        "replace", {Value(Text("Hello World")), Value(Text(" World")), Value(Text(""))});
    expectText(result3, "Hello");

    // Case insensitive replace
    Value result4 = callRegexpMethod("replace", {Value(Text("Hello World")), Value(Text("hello")),
                                                 Value(Text("Hi")), Value(Text("i"))});
    expectText(result4, "Hi World");

    // No match - return original
    Value result5 = callRegexpMethod(
        "replace", {Value(Text("Hello World")), Value(Text("Goodbye")), Value(Text("Hi"))});
    expectText(result5, "Hello World");

    // Test error cases
    EXPECT_THROW(callRegexpMethod("replace", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("replace", {Value(Text("test")), Value(Text("pattern"))}),
                 EvaluationError);
    EXPECT_THROW(callRegexpMethod("replace", {Value(Int(123)), Value(Text("pattern")),
                                              Value(Text("replacement"))}),
                 EvaluationError);
}

// Test replaceAll method
TEST_F(RegexpLibraryTest, ReplaceAllMethod) {
    // Replace all occurrences
    Value result1 = callRegexpMethod(
        "replaceAll", {Value(Text("Hello World Hello")), Value(Text("Hello")), Value(Text("Hi"))});
    expectText(result1, "Hi World Hi");

    // Replace all numbers
    Value result2 = callRegexpMethod("replaceAll", {Value(Text("I have 3 apples and 7 oranges")),
                                                    Value(Text("\\d+")), Value(Text("many"))});
    expectText(result2, "I have many apples and many oranges");

    // Replace with capture groups (backreference)
    Value result3 = callRegexpMethod(
        "replaceAll",
        {Value(Text("First Last")), Value(Text("(\\w+) (\\w+)")), Value(Text("$2, $1"))});
    expectText(result3, "Last, First");

    // Case insensitive replace all
    Value result4 =
        callRegexpMethod("replaceAll", {Value(Text("Apple apple APPLE")), Value(Text("apple")),
                                        Value(Text("orange")), Value(Text("i"))});
    expectText(result4, "orange orange orange");

    // Test error cases
    EXPECT_THROW(callRegexpMethod("replaceAll", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("replaceAll", {Value(Text("test")), Value(Text("pattern"))}),
                 EvaluationError);
}

// Test split method
TEST_F(RegexpLibraryTest, SplitMethod) {
    // Split by space
    Value result1 = callRegexpMethod("split", {Value(Text("Hello World Test")), Value(Text(" "))});
    EXPECT_EQ(getListSize(result1), 3);
    EXPECT_EQ(getListItem(result1, 0), "Hello");
    EXPECT_EQ(getListItem(result1, 1), "World");
    EXPECT_EQ(getListItem(result1, 2), "Test");

    // Split by comma with optional whitespace
    Value result2 = callRegexpMethod(
        "split", {Value(Text("apple, banana ,cherry,  date")), Value(Text(",\\s*"))});
    EXPECT_EQ(getListSize(result2), 4);
    EXPECT_EQ(getListItem(result2, 0), "apple");
    EXPECT_EQ(getListItem(result2, 1), "banana ");  // Note: space preserved before comma
    EXPECT_EQ(getListItem(result2, 2), "cherry");
    EXPECT_EQ(getListItem(result2, 3), "date");

    // Split by digits
    Value result3 =
        callRegexpMethod("split", {Value(Text("word1test2example3end")), Value(Text("\\d+"))});
    EXPECT_EQ(getListSize(result3), 4);
    EXPECT_EQ(getListItem(result3, 0), "word");
    EXPECT_EQ(getListItem(result3, 1), "test");
    EXPECT_EQ(getListItem(result3, 2), "example");
    EXPECT_EQ(getListItem(result3, 3), "end");

    // No matches - return original string as single item
    Value result4 = callRegexpMethod("split", {Value(Text("Hello World")), Value(Text("\\d+"))});
    EXPECT_EQ(getListSize(result4), 1);
    EXPECT_EQ(getListItem(result4, 0), "Hello World");

    // Test error cases
    EXPECT_THROW(callRegexpMethod("split", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("split", {Value(Text("test"))}), EvaluationError);
}

// Test groups method
TEST_F(RegexpLibraryTest, GroupsMethod) {
    // Basic capture groups
    Value result1 =
        callRegexpMethod("groups", {Value(Text("John Doe")), Value(Text("(\\w+) (\\w+)"))});
    EXPECT_EQ(getListSize(result1), 3);              // Full match + 2 groups
    EXPECT_EQ(getListItem(result1, 0), "John Doe");  // Full match
    EXPECT_EQ(getListItem(result1, 1), "John");      // First group
    EXPECT_EQ(getListItem(result1, 2), "Doe");       // Second group

    // Email extraction
    Value result2 = callRegexpMethod(
        "groups", {Value(Text("Contact: user@example.com")), Value(Text("(\\w+)@(\\w+\\.\\w+)"))});
    EXPECT_EQ(getListSize(result2), 3);
    EXPECT_EQ(getListItem(result2, 0), "user@example.com");
    EXPECT_EQ(getListItem(result2, 1), "user");
    EXPECT_EQ(getListItem(result2, 2), "example.com");

    // No match - return empty list
    Value result3 = callRegexpMethod("groups", {Value(Text("Hello World")), Value(Text("(\\d+)"))});
    EXPECT_EQ(getListSize(result3), 0);

    // Single group
    Value result4 =
        callRegexpMethod("groups", {Value(Text("Price: $42")), Value(Text("\\$(\\d+)"))});
    EXPECT_EQ(getListSize(result4), 2);
    EXPECT_EQ(getListItem(result4, 0), "$42");
    EXPECT_EQ(getListItem(result4, 1), "42");

    // Test error cases
    EXPECT_THROW(callRegexpMethod("groups", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("groups", {Value(Text("test"))}), EvaluationError);
}

// Test escape method
TEST_F(RegexpLibraryTest, EscapeMethod) {
    // Escape special regex characters
    Value result1 = callRegexpMethod("escape", {Value(Text("$42.50"))});
    expectText(result1, "\\$42\\.50");

    Value result2 = callRegexpMethod("escape", {Value(Text("(test) [pattern] {5,10}"))});
    expectText(result2, "\\(test\\) \\[pattern\\] \\{5,10\\}");

    Value result3 = callRegexpMethod("escape", {Value(Text("a+b*c?d"))});
    expectText(result3, "a\\+b\\*c\\?d");

    Value result4 = callRegexpMethod("escape", {Value(Text("file|path^start"))});
    expectText(result4, "file\\|path\\^start");

    // Regular text should remain unchanged
    Value result5 = callRegexpMethod("escape", {Value(Text("Hello World 123"))});
    expectText(result5, "Hello World 123");

    // Test error cases
    EXPECT_THROW(callRegexpMethod("escape", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("escape", {Value(Int(123))}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("escape", {Value(Text("test")), Value(Text("extra"))}),
                 EvaluationError);
}

// Test isValid method
TEST_F(RegexpLibraryTest, IsValidMethod) {
    // Valid patterns
    Value result1 = callRegexpMethod("isValid", {Value(Text("\\d+"))});
    expectBool(result1, true);

    Value result2 = callRegexpMethod("isValid", {Value(Text("(\\w+)@(\\w+\\.\\w+)"))});
    expectBool(result2, true);

    Value result3 = callRegexpMethod("isValid", {Value(Text("Hello"))});
    expectBool(result3, true);

    // Invalid patterns (unclosed brackets, etc.)
    Value result4 = callRegexpMethod("isValid", {Value(Text("[abc"))});
    expectBool(result4, false);

    Value result5 = callRegexpMethod("isValid", {Value(Text("(unclosed"))});
    expectBool(result5, false);

    // Test with flags
    Value result6 = callRegexpMethod("isValid", {Value(Text("test")), Value(Text("i"))});
    expectBool(result6, true);

    // Test error cases
    EXPECT_THROW(callRegexpMethod("isValid", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("isValid", {Value(Int(123))}), EvaluationError);
}

// Test count method
TEST_F(RegexpLibraryTest, CountMethod) {
    // Count occurrences
    Value result1 = callRegexpMethod(
        "count",
        {Value(Text("I have 3 apples and 7 oranges and 12 bananas")), Value(Text("\\d+"))});
    expectInt(result1, 3);

    Value result2 = callRegexpMethod(
        "count", {Value(Text("Hello World Hello Test Hello")), Value(Text("Hello"))});
    expectInt(result2, 3);

    Value result3 = callRegexpMethod("count", {Value(Text("abc def ghi")), Value(Text("\\w+"))});
    expectInt(result3, 3);

    // No matches
    Value result4 = callRegexpMethod("count", {Value(Text("Hello World")), Value(Text("\\d+"))});
    expectInt(result4, 0);

    // Case insensitive count
    Value result5 = callRegexpMethod(
        "count", {Value(Text("Apple apple APPLE")), Value(Text("apple")), Value(Text("i"))});
    expectInt(result5, 3);

    // Test error cases
    EXPECT_THROW(callRegexpMethod("count", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("count", {Value(Text("test"))}), EvaluationError);
}

// Test extract method
TEST_F(RegexpLibraryTest, ExtractMethod) {
    // Extract capture groups from multiple matches
    Value result1 = callRegexpMethod(
        "extract", {Value(Text("John Doe, Jane Smith")), Value(Text("(\\w+) (\\w+)"))});
    EXPECT_EQ(getListSize(result1), 4);  // 2 matches × 2 groups each
    EXPECT_EQ(getListItem(result1, 0), "John");
    EXPECT_EQ(getListItem(result1, 1), "Doe");
    EXPECT_EQ(getListItem(result1, 2), "Jane");
    EXPECT_EQ(getListItem(result1, 3), "Smith");

    // Extract numbers with context
    Value result2 = callRegexpMethod(
        "extract", {Value(Text("$42.50 and $15.99")), Value(Text("\\$(\\d+)\\.(\\d+)"))});
    EXPECT_EQ(getListSize(result2), 4);
    EXPECT_EQ(getListItem(result2, 0), "42");
    EXPECT_EQ(getListItem(result2, 1), "50");
    EXPECT_EQ(getListItem(result2, 2), "15");
    EXPECT_EQ(getListItem(result2, 3), "99");

    // Single capture group - extract just the numbers
    Value result3 =
        callRegexpMethod("extract", {Value(Text("test123abc456def")), Value(Text("(\\d+)"))});
    EXPECT_EQ(getListSize(result3), 2);  // Two number groups: "123" and "456"
    EXPECT_EQ(getListItem(result3, 0), "123");
    EXPECT_EQ(getListItem(result3, 1), "456");

    // No matches
    Value result4 =
        callRegexpMethod("extract", {Value(Text("Hello World")), Value(Text("(\\d+)"))});
    EXPECT_EQ(getListSize(result4), 0);

    // Test error cases
    EXPECT_THROW(callRegexpMethod("extract", {}), EvaluationError);
    EXPECT_THROW(callRegexpMethod("extract", {Value(Text("test"))}), EvaluationError);
}

// Test method availability
TEST_F(RegexpLibraryTest, MethodAvailability) {
    auto regexp_object = RegexpLibrary::createRegexpObject();

    // Test that all expected methods are available
    std::vector<std::string> expected_methods = {"match",      "find",    "findAll", "replace",
                                                 "replaceAll", "split",   "groups",  "test",
                                                 "escape",     "isValid", "count",   "extract"};

    for (const auto& method : expected_methods) {
        EXPECT_TRUE(regexp_object->hasMethod(method)) << "Method " << method << " not found";
        EXPECT_TRUE(regexp_object->isMethodExternal(method))
            << "Method " << method << " not external";
    }
}

// Test complex patterns
TEST_F(RegexpLibraryTest, ComplexPatterns) {
    // Email validation
    Value email_result = callRegexpMethod(
        "match",
        {Value(Text("user@example.com")), Value(Text("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,}$"))});
    expectBool(email_result, true);

    // Phone number extraction
    Value phone_result = callRegexpMethod(
        "findAll", {Value(Text("Call me at (555) 123-4567 or 555.987.6543")),
                    Value(Text("\\(?\\d{3}\\)?[\\s\\.-]?\\d{3}[\\s\\.-]?\\d{4}"))});
    EXPECT_EQ(getListSize(phone_result), 2);

    // HTML tag removal
    Value html_result = callRegexpMethod("replaceAll", {Value(Text("<p>Hello <b>World</b></p>")),
                                                        Value(Text("<[^>]+>")), Value(Text(""))});
    expectText(html_result, "Hello World");

    // URL parsing
    Value url_result = callRegexpMethod(
        "groups", {Value(Text("https://www.example.com:8080/path/to/page?param=value")),
                   Value(Text("(https?)://([^:]+)(?::(\\d+))?([^?]+)(?:\\?(.+))?"))});
    EXPECT_GE(getListSize(url_result), 4);  // At least protocol, domain, path
}

// Test flag combinations
TEST_F(RegexpLibraryTest, FlagCombinations) {
    // Case insensitive matching
    Value result1 = callRegexpMethod(
        "match", {Value(Text("Hello World")), Value(Text("HELLO")), Value(Text("i"))});
    expectBool(result1, true);

    // Multiple flags (case insensitive + global)
    Value result2 =
        callRegexpMethod("replaceAll", {Value(Text("Hello hello HELLO")), Value(Text("hello")),
                                        Value(Text("hi")), Value(Text("ig"))});
    expectText(result2, "hi hi hi");

    // Unknown flags should be ignored (not cause errors)
    Value result3 =
        callRegexpMethod("match", {Value(Text("test")), Value(Text("test")), Value(Text("ixyz"))});
    expectBool(result3, true);
}