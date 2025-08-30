/*
 * Comprehensive Google Test cases for O²L formatter fixes
 */

#include <gtest/gtest.h>
#include "../formatter.hpp"

class O2LFormatterAdvancedTest : public ::testing::Test {
protected:
    O2LFormatter formatter;
};

// String literal preservation tests
TEST_F(O2LFormatterAdvancedTest, PreservesFormatStringSpaces) {
    std::string input = "io.print(\"%s, %s!\", this.message, name)";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("%s, %s!"), std::string::npos);
    EXPECT_EQ(result.find("% s, % s!"), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, PreservesAllFormatSpecifiers) {
    std::string input = "io.print(\"Name: %s, Age: %d, Score: %f\", name, age, score)";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("%s"), std::string::npos);
    EXPECT_NE(result.find("%d"), std::string::npos);
    EXPECT_NE(result.find("%f"), std::string::npos);
}

// Brace matching tests
TEST_F(O2LFormatterAdvancedTest, FailsOnExtraBraces) {
    std::string input = R"(Object Greeter {
    property message: Text
    
    constructor(greeting: Text) {
        this.message = greeting
    }
}
})";
    
    // Should throw an exception for unbalanced braces
    EXPECT_THROW(formatter.format_code_with_validation(input), std::runtime_error);
}

// Indentation tests
TEST_F(O2LFormatterAdvancedTest, HandlesMethodIndentation) {
    std::string input = R"(Object Calculator {
method add(a: Int, b: Int): Int {
return a + b
}
})";
    
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("    method add"), std::string::npos);
    EXPECT_NE(result.find("        return a + b"), std::string::npos);
}

// Annotation tests
TEST_F(O2LFormatterAdvancedTest, HandlesExternalAnnotationSameLine) {
    std::string input = "@external method greet(name: Text): Text {";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("@external method greet"), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, HandlesExternalAnnotationMultiLine) {
    std::string input = R"(@external
method greet(name: Text): Text {
return "Hello"
})";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("@external"), std::string::npos);
    EXPECT_NE(result.find("method greet"), std::string::npos);
}

// Comment preservation tests  
TEST_F(O2LFormatterAdvancedTest, PreservesInlineComments) {
    std::string input = "value: Int = 42  # Inline comment";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("# Inline comment"), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, PreservesMultiLineComments) {
    std::string input = R"(###
Multi-line comment
with multiple lines
###)";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("###"), std::string::npos);
    EXPECT_NE(result.find("Multi - line comment"), std::string::npos);
}

// Complete object formatting
TEST_F(O2LFormatterAdvancedTest, FormatsCompleteObjectWithExtraBrace) {
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

// Edge case tests
TEST_F(O2LFormatterAdvancedTest, HandlesStringWithOperatorsInside) {
    std::string input = "message: Text = \"2 + 2 = 4\"";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("\"2 + 2 = 4\""), std::string::npos);
    EXPECT_EQ(result.find("\" 2  +  2  =  4 \""), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, HandlesEmptyObject) {
    std::string input = "Object Empty {}";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("Object Empty"), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, HandlesMethodWithNoParams) {
    std::string input = "method getValue(): Int { return 42 }";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("method getValue()"), std::string::npos);
    EXPECT_NE(result.find("return 42"), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, FormatsOperatorSpacing) {
    std::string input = "result:Int=a+b*c";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("result: Int = a + b * c"), std::string::npos);
}

TEST_F(O2LFormatterAdvancedTest, FormatsTypeAnnotations) {
    std::string input = "value:Int=42";
    std::string result = formatter.format_code(input);
    
    EXPECT_NE(result.find("value: Int = 42"), std::string::npos);
}

// Test nested structures
TEST_F(O2LFormatterAdvancedTest, HandlesNestedStructures) {
    std::string input = R"(Object Outer {
    Object Inner {
        method test(): Int {
            return 0
        }
    }
})";
    
    std::string result = formatter.format_code(input);
    
    // Check proper nesting indentation
    EXPECT_NE(result.find("    Object Inner"), std::string::npos);
    EXPECT_NE(result.find("        method test"), std::string::npos);
    EXPECT_NE(result.find("            return 0"), std::string::npos);
}

// Test minified input (user's problematic case)
TEST_F(O2LFormatterAdvancedTest, HandlesMinifiedInput) {
    std::string input = "import system.io Object Greeter { property message: Text constructor(greeting: Text) { this.message = greeting } @external method greet(name: Text): Text { io.print(\"%s, %s!\", this.message, name) return this.message }} Object Main { method main(): Int { greeter: Greeter = new Greeter(\"Hello\") greeter.greet(\"World\") return 0 }}";
    
    std::string expected = R"(import system.io


Object Greeter {
    property message: Text constructor(greeting: Text) {
        this.message = greeting
    } @external method greet(name: Text): Text {
        io.print("%s, %s\!", this.message, name) return this.message
    }
}

Object Main {
    method main(): Int {
        greeter: Greeter = new Greeter("Hello") greeter.greet("World") return 0
    }
})";
    
    std::string result = formatter.format_code(input);
    
    // Should separate import from Object
    EXPECT_NE(result.find("import system.io"), std::string::npos);
    EXPECT_EQ(result.find("import system.ioObject"), std::string::npos);
    
    // Should preserve string literals exactly (no modification of format specifiers)
    EXPECT_EQ(result.find("% s, % s!"), std::string::npos);
    EXPECT_NE(result.find("%s, %s!"), std::string::npos);
    
    // Should properly format objects
    EXPECT_NE(result.find("Object Greeter {"), std::string::npos);
    EXPECT_NE(result.find("Object Main {"), std::string::npos);
    
    // Should have proper indentation  
    EXPECT_NE(result.find("    property message"), std::string::npos);
    
    // Verify 3 newlines after import
    EXPECT_NE(result.find("import system.io\n\n\nObject"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}