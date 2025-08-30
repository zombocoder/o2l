/*
 * Google Test cases for O²L formatter
 */

#include <gtest/gtest.h>
#include "../formatter.hpp"

class O2LFormatterTest : public ::testing::Test {
protected:
    O2LFormatter formatter;
};

// Test string literal preservation
TEST_F(O2LFormatterTest, PreservesStringLiterals) {
    std::string input = "io.print(\"%s, %s!\", this.message, name)";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("%s, %s!"), std::string::npos);
    EXPECT_EQ(result.find("% s, % s!"), std::string::npos);
}

TEST_F(O2LFormatterTest, PreservesMultipleFormatSpecifiers) {
    std::string input = "io.print(\"Name: %s, Age: %d, Score: %f\", name, age, score)";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("%s"), std::string::npos);
    EXPECT_NE(result.find("%d"), std::string::npos);
    EXPECT_NE(result.find("%f"), std::string::npos);
}

// Test that formatter fails on unbalanced braces
TEST_F(O2LFormatterTest, FailsOnExtraClosingBraces) {
    std::string input = R"(Object Test {
    method demo(): Int {
        return 0
    }
}
})";
    
    // Should throw an exception for unbalanced braces
    EXPECT_THROW(formatter.format_code_with_validation(input), std::runtime_error);
}

// Test proper indentation
TEST_F(O2LFormatterTest, HandlesIndentationCorrectly) {
    std::string input = R"(Object Calculator {
method add(a: Int, b: Int): Int {
return a + b
}
})";
    
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("    method add"), std::string::npos);
    EXPECT_NE(result.find("        return a + b"), std::string::npos);
}

// Test annotation handling
TEST_F(O2LFormatterTest, HandlesExternalAnnotation) {
    std::string input = "@external method greet(name: Text): Text {";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("@external method greet"), std::string::npos);
}

// Test inline comment preservation
TEST_F(O2LFormatterTest, PreservesInlineComments) {
    std::string input = "value: Int = 42  # Inline comment";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("# Inline comment"), std::string::npos);
}

// Test complete object formatting
TEST_F(O2LFormatterTest, FormatsCompleteObjectCorrectly) {
    std::string input = R"(import system.io

Object Greeter {
    property message: Text

    constructor(greeting: Text) {
        this.message = greeting
    }

    @external method greet(name: Text): Text {
        io.print("%s, %s!", this.message, name)
        return this.message
    }
}

Object Main {
    method main(): Int {
        greeter: Greeter = new Greeter("Hello")
        greeter.greet("World")
        return 0
    }
})";
    
    std::string result = formatter.format_code(input);
    
    // Verify string literals are preserved
    EXPECT_NE(result.find("%s, %s!"), std::string::npos);
    EXPECT_EQ(result.find("% s, % s!"), std::string::npos);
    
    // Verify proper structure
    EXPECT_NE(result.find("Object Greeter {"), std::string::npos);
    EXPECT_NE(result.find("Object Main {"), std::string::npos);
    
    // Check brace balance
    int open_braces = 0, close_braces = 0;
    for (char c : result) {
        if (c == '{') open_braces++;
        if (c == '}') close_braces++;
    }
    EXPECT_EQ(open_braces, close_braces);
}

// Test edge cases
TEST_F(O2LFormatterTest, HandlesStringWithOperators) {
    std::string input = "message: Text = \"2 + 2 = 4\"";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("\"2 + 2 = 4\""), std::string::npos);
    EXPECT_EQ(result.find("\" 2  +  2  =  4 \""), std::string::npos);
}

TEST_F(O2LFormatterTest, HandlesEmptyObject) {
    std::string input = "Object Empty {}";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("Object Empty"), std::string::npos);
}

TEST_F(O2LFormatterTest, HandlesMethodWithNoParameters) {
    std::string input = "method getValue(): Int { return 42 }";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("method getValue()"), std::string::npos);
    EXPECT_NE(result.find("return 42"), std::string::npos);
}

// Test operator spacing
TEST_F(O2LFormatterTest, FormatsOperatorsCorrectly) {
    std::string input = "result:Int=a+b*c";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("result: Int = a + b * c"), std::string::npos);
}

// Test type annotations
TEST_F(O2LFormatterTest, FormatsTypeAnnotations) {
    std::string input = "value:Int=42";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("value: Int = 42"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}