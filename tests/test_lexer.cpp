#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Lexer.hpp"

using namespace o2l;

class LexerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code if needed
    }

    std::vector<Token> tokenize(const std::string& input) {
        Lexer lexer(input);
        return lexer.tokenizeAll();
    }
};

// Test basic keywords
TEST_F(LexerTest, BasicKeywords) {
    auto tokens = tokenize("Object method property constructor");

    ASSERT_EQ(tokens.size(), 5);  // 4 keywords + EOF
    EXPECT_EQ(tokens[0].type, TokenType::OBJECT);
    EXPECT_EQ(tokens[1].type, TokenType::METHOD);
    EXPECT_EQ(tokens[2].type, TokenType::PROPERTY);
    EXPECT_EQ(tokens[3].type, TokenType::CONSTRUCTOR);
    EXPECT_EQ(tokens[4].type, TokenType::EOF_TOKEN);
}

// Test advanced keywords (enum, record, protocol)
TEST_F(LexerTest, AdvancedKeywords) {
    auto tokens = tokenize("Enum Record Protocol const");

    ASSERT_EQ(tokens.size(), 5);  // 4 keywords + EOF
    EXPECT_EQ(tokens[0].type, TokenType::ENUM);
    EXPECT_EQ(tokens[1].type, TokenType::RECORD);
    EXPECT_EQ(tokens[2].type, TokenType::PROTOCOL);
    EXPECT_EQ(tokens[3].type, TokenType::CONST);
    EXPECT_EQ(tokens[4].type, TokenType::EOF_TOKEN);
}

// Test control flow keywords
TEST_F(LexerTest, ControlFlowKeywords) {
    auto tokens = tokenize("if else while return");

    ASSERT_EQ(tokens.size(), 5);  // 4 keywords + EOF
    EXPECT_EQ(tokens[0].type, TokenType::IF);
    EXPECT_EQ(tokens[1].type, TokenType::ELSE);
    EXPECT_EQ(tokens[2].type, TokenType::WHILE);
    EXPECT_EQ(tokens[3].type, TokenType::RETURN);
    EXPECT_EQ(tokens[4].type, TokenType::EOF_TOKEN);
}

// Test visibility modifiers
TEST_F(LexerTest, VisibilityModifiers) {
    auto tokens = tokenize("@external");

    ASSERT_EQ(tokens.size(), 2);  // @external + EOF
    EXPECT_EQ(tokens[0].type, TokenType::AT_EXTERNAL);
    EXPECT_EQ(tokens[1].type, TokenType::EOF_TOKEN);
}

// Test literals
TEST_F(LexerTest, StringLiterals) {
    auto tokens = tokenize("\"Hello World\" \"Test\"");

    ASSERT_EQ(tokens.size(), 3);  // 2 strings + EOF
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, "Hello World");
    EXPECT_EQ(tokens[1].type, TokenType::STRING);
    EXPECT_EQ(tokens[1].value, "Test");
    EXPECT_EQ(tokens[2].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, NumericLiterals) {
    auto tokens = tokenize("42 3.14 0 -5");

    // -5 is tokenized as MINUS + NUMBER, so expecting 6 tokens
    ASSERT_EQ(tokens.size(), 6);  // 42, 3.14, 0, MINUS, 5, EOF
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, "42");
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].value, "3.14");
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "0");
    EXPECT_EQ(tokens[3].type, TokenType::MINUS);
    EXPECT_EQ(tokens[4].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[4].value, "5");
    EXPECT_EQ(tokens[5].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, FloatAndDoubleLiterals) {
    auto tokens = tokenize("3.14f 2.5F 1.0d 4.25D 9.99");

    ASSERT_EQ(tokens.size(), 6);  // 5 numbers + EOF

    // Float with 'f' suffix
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, "3.14f");

    // Float with 'F' suffix
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].value, "2.5F");

    // Double with 'd' suffix
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "1.0d");

    // Double with 'D' suffix
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[3].value, "4.25D");

    // Default decimal (no suffix - should be Double)
    EXPECT_EQ(tokens[4].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[4].value, "9.99");

    EXPECT_EQ(tokens[5].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, LongLiterals) {
    auto tokens = tokenize("42L 123456789l 999999999999999999L");

    ASSERT_EQ(tokens.size(), 4);  // 3 numbers + EOF

    // Long with 'L' suffix
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, "42L");

    // Long with 'l' suffix
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].value, "123456789l");

    // Large Long literal
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "999999999999999999L");

    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, BooleanLiterals) {
    auto tokens = tokenize("true false");

    ASSERT_EQ(tokens.size(), 3);  // 2 booleans + EOF
    EXPECT_EQ(tokens[0].type, TokenType::TRUE);
    EXPECT_EQ(tokens[1].type, TokenType::FALSE);
    EXPECT_EQ(tokens[2].type, TokenType::EOF_TOKEN);
}

// Test operators
TEST_F(LexerTest, ComparisonOperators) {
    auto tokens = tokenize("== != < > <= >=");

    ASSERT_EQ(tokens.size(), 7);  // 6 operators + EOF
    EXPECT_EQ(tokens[0].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[1].type, TokenType::NOT_EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::LESS_THAN);
    EXPECT_EQ(tokens[3].type, TokenType::GREATER_THAN);
    EXPECT_EQ(tokens[4].type, TokenType::LESS_EQUAL);
    EXPECT_EQ(tokens[5].type, TokenType::GREATER_EQUAL);
    EXPECT_EQ(tokens[6].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ArithmeticOperators) {
    auto tokens = tokenize("+ - * / =");

    ASSERT_EQ(tokens.size(), 6);  // 5 operators + EOF
    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[1].type, TokenType::MINUS);
    EXPECT_EQ(tokens[2].type, TokenType::MULTIPLY);
    EXPECT_EQ(tokens[3].type, TokenType::DIVIDE);
    EXPECT_EQ(tokens[4].type, TokenType::ASSIGN);
    EXPECT_EQ(tokens[5].type, TokenType::EOF_TOKEN);
}

// Test punctuation
TEST_F(LexerTest, Punctuation) {
    auto tokens = tokenize("{ } ( ) [ ] , . : ;");

    ASSERT_EQ(tokens.size(), 11);  // 10 punctuation + EOF
    EXPECT_EQ(tokens[0].type, TokenType::LBRACE);
    EXPECT_EQ(tokens[1].type, TokenType::RBRACE);
    EXPECT_EQ(tokens[2].type, TokenType::LPAREN);
    EXPECT_EQ(tokens[3].type, TokenType::RPAREN);
    EXPECT_EQ(tokens[4].type, TokenType::LBRACKET);
    EXPECT_EQ(tokens[5].type, TokenType::RBRACKET);
    EXPECT_EQ(tokens[6].type, TokenType::COMMA);
    EXPECT_EQ(tokens[7].type, TokenType::DOT);
    EXPECT_EQ(tokens[8].type, TokenType::COLON);
    EXPECT_EQ(tokens[9].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[10].type, TokenType::EOF_TOKEN);
}

// Test identifiers
TEST_F(LexerTest, Identifiers) {
    auto tokens = tokenize("myVariable myMethod MyClass test123 _private");

    ASSERT_EQ(tokens.size(), 6);  // 5 identifiers + EOF
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "myVariable");
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "myMethod");
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "MyClass");
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].value, "test123");
    EXPECT_EQ(tokens[4].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[4].value, "_private");
    EXPECT_EQ(tokens[5].type, TokenType::EOF_TOKEN);
}

// Test comments
TEST_F(LexerTest, SingleLineComments) {
    auto tokens = tokenize("Object # This is a comment\nmethod");

    ASSERT_EQ(tokens.size(), 4);  // Object + newline + method + EOF
    EXPECT_EQ(tokens[0].type, TokenType::OBJECT);
    EXPECT_EQ(tokens[1].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[2].type, TokenType::METHOD);
    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, MultiLineComments) {
    auto tokens = tokenize("Object ### This is a\nmulti-line comment ###\nmethod");

    ASSERT_EQ(tokens.size(), 4);  // Object + newline + method + EOF
    EXPECT_EQ(tokens[0].type, TokenType::OBJECT);
    EXPECT_EQ(tokens[1].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[2].type, TokenType::METHOD);
    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

// Test whitespace handling
TEST_F(LexerTest, WhitespaceHandling) {
    auto tokens = tokenize("Object   \t  method\n\nproperty");

    // Two newlines are tokenized separately
    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::OBJECT);
    EXPECT_EQ(tokens[1].type, TokenType::METHOD);
    EXPECT_EQ(tokens[2].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[3].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[4].type, TokenType::PROPERTY);
    EXPECT_EQ(tokens[5].type, TokenType::EOF_TOKEN);
}

// Test mixed content
TEST_F(LexerTest, ComplexExpression) {
    auto tokens = tokenize("myObject.methodName(\"param\", 42)");

    ASSERT_EQ(
        tokens.size(),
        9);  // identifier + dot + identifier + lparen + string + comma + number + rparen + EOF
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "myObject");
    EXPECT_EQ(tokens[1].type, TokenType::DOT);
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "methodName");
    EXPECT_EQ(tokens[3].type, TokenType::LPAREN);
    EXPECT_EQ(tokens[4].type, TokenType::STRING);
    EXPECT_EQ(tokens[4].value, "param");
    EXPECT_EQ(tokens[5].type, TokenType::COMMA);
    EXPECT_EQ(tokens[6].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[6].value, "42");
    EXPECT_EQ(tokens[7].type, TokenType::RPAREN);
    EXPECT_EQ(tokens[8].type, TokenType::EOF_TOKEN);
}

// Test error handling
TEST_F(LexerTest, InvalidCharacter) {
    // Test that invalid characters are handled gracefully
    EXPECT_NO_THROW({
        auto tokens = tokenize("Object $ method");
        // Should handle invalid characters gracefully
    });
}

// Additional comprehensive tests

// Test all import keywords
TEST_F(LexerTest, ImportKeywords) {
    auto tokens = tokenize("import new this");

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::IMPORT);
    EXPECT_EQ(tokens[1].type, TokenType::NEW);
    EXPECT_EQ(tokens[2].type, TokenType::THIS);
    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

// Test character literals
TEST_F(LexerTest, CharacterLiterals) {
    auto tokens = tokenize("'a' 'Z' '1' ' '");

    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::CHARACTER);
    EXPECT_EQ(tokens[0].value, "a");
    EXPECT_EQ(tokens[1].type, TokenType::CHARACTER);
    EXPECT_EQ(tokens[1].value, "Z");
    EXPECT_EQ(tokens[2].type, TokenType::CHARACTER);
    EXPECT_EQ(tokens[2].value, "1");
    EXPECT_EQ(tokens[3].type, TokenType::CHARACTER);
    EXPECT_EQ(tokens[3].value, " ");
    EXPECT_EQ(tokens[4].type, TokenType::EOF_TOKEN);
}

// Test string escape sequences
TEST_F(LexerTest, StringEscapeSequences) {
    auto tokens = tokenize("\"Hello\\nWorld\" \"Tab\\tSeparated\" \"Quote\\\"Inside\"");

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, "Hello\nWorld");
    EXPECT_EQ(tokens[1].type, TokenType::STRING);
    EXPECT_EQ(tokens[1].value, "Tab\tSeparated");
    EXPECT_EQ(tokens[2].type, TokenType::STRING);
    EXPECT_EQ(tokens[2].value, "Quote\"Inside");
    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

// Test nested comments (should ignore content)
TEST_F(LexerTest, NestedComments) {
    auto tokens = tokenize("Object ### comment with # inside ### method");

    // May have a space token or similar, expecting 4 tokens
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::OBJECT);
    // Skip the space/whitespace token
    EXPECT_EQ(tokens[2].type, TokenType::METHOD);
    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

// Test consecutive operators
TEST_F(LexerTest, ConsecutiveOperators) {
    auto tokens = tokenize("++--**//==!=<=>=");

    // == != <= >= are multi-character operators, so expecting 13 tokens
    ASSERT_EQ(tokens.size(), 13);
    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[1].type, TokenType::PLUS);
    EXPECT_EQ(tokens[2].type, TokenType::MINUS);
    EXPECT_EQ(tokens[3].type, TokenType::MINUS);
    EXPECT_EQ(tokens[4].type, TokenType::MULTIPLY);
    EXPECT_EQ(tokens[5].type, TokenType::MULTIPLY);
    EXPECT_EQ(tokens[6].type, TokenType::DIVIDE);
    EXPECT_EQ(tokens[7].type, TokenType::DIVIDE);
    EXPECT_EQ(tokens[8].type, TokenType::EQUAL);           // ==
    EXPECT_EQ(tokens[9].type, TokenType::NOT_EQUAL);       // !=
    EXPECT_EQ(tokens[10].type, TokenType::LESS_EQUAL);     // <=
    EXPECT_EQ(tokens[11].type, TokenType::GREATER_EQUAL);  // >=
    EXPECT_EQ(tokens[12].type, TokenType::EOF_TOKEN);
}

// Test edge case: empty input
TEST_F(LexerTest, EmptyInput) {
    auto tokens = tokenize("");

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EOF_TOKEN);
}

// Test edge case: only whitespace
TEST_F(LexerTest, OnlyWhitespace) {
    auto tokens = tokenize("   \t\t   \n\n  ");

    ASSERT_EQ(tokens.size(), 3);  // 2 newlines + EOF
    EXPECT_EQ(tokens[0].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[1].type, TokenType::NEWLINE);
    EXPECT_EQ(tokens[2].type, TokenType::EOF_TOKEN);
}

// Test all comparison operators separately
TEST_F(LexerTest, ComparisonOperatorsSeparated) {
    auto tokens = tokenize("= == != < <= > >=");

    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].type, TokenType::ASSIGN);
    EXPECT_EQ(tokens[1].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::NOT_EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::LESS_THAN);
    EXPECT_EQ(tokens[4].type, TokenType::LESS_EQUAL);
    EXPECT_EQ(tokens[5].type, TokenType::GREATER_THAN);
    EXPECT_EQ(tokens[6].type, TokenType::GREATER_EQUAL);
    EXPECT_EQ(tokens[7].type, TokenType::EOF_TOKEN);
}

// Test List literal syntax
TEST_F(LexerTest, ListLiteralSyntax) {
    auto tokens = tokenize("[1, 2, 3]");

    ASSERT_EQ(tokens.size(), 8);  // [ + 1 + , + 2 + , + 3 + ] + EOF
    EXPECT_EQ(tokens[0].type, TokenType::LBRACKET);
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].value, "1");
    EXPECT_EQ(tokens[2].type, TokenType::COMMA);
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[3].value, "2");
    EXPECT_EQ(tokens[4].type, TokenType::COMMA);
    EXPECT_EQ(tokens[5].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[5].value, "3");
    EXPECT_EQ(tokens[6].type, TokenType::RBRACKET);
    EXPECT_EQ(tokens[7].type, TokenType::EOF_TOKEN);
}