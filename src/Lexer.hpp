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

#include <string>
#include <vector>
#include <string_view>

namespace o2l {

enum class TokenType {
    // Keywords
    OBJECT,
    METHOD,
    CONSTRUCTOR,
    PROPERTY,
    CONST,
    ENUM,
    RECORD,
    PROTOCOL,
    IF,
    ELSE,
    WHILE,
    BREAK,
    RETURN,
    THIS,
    TRUE,
    FALSE,
    IMPORT,
    NEW,
    THROW,
    TRY,
    CATCH,
    FINALLY,
    RESULT,
    ERROR,
    NAMESPACE,
    
    // Modifiers
    AT_EXTERNAL,
    AT_IMPORT,
    
    // Identifiers and literals
    IDENTIFIER,
    STRING,
    NUMBER,
    CHARACTER,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGN,
    DOT,
    DOLLAR,
    
    // Comparison operators
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    LESS_THAN,      // <
    GREATER_THAN,   // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    
    // Logical operators
    LOGICAL_AND,    // &&
    LOGICAL_OR,     // ||
    LOGICAL_NOT,    // !
    
    // Delimiters
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    COLON,
    COMMA,
    SEMICOLON,
    
    // Special
    NEWLINE,
    EOF_TOKEN,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    
    Token(TokenType t, std::string v, size_t l, size_t c) 
        : type(t), value(std::move(v)), line(l), column(c) {}
};

class Lexer {
private:
    std::string_view source_;
    size_t current_pos_;
    size_t line_;
    size_t column_;
    
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    void skipWhitespace();
    void skipComment();
    
    Token makeString();
    Token makeNumber();
    Token makeNegativeNumber();
    Token makeIdentifierOrKeyword();
    Token makeCharacter();
    
    TokenType getKeywordType(const std::string& identifier) const;

public:
    explicit Lexer(std::string_view source);
    
    Token nextToken();
    std::vector<Token> tokenizeAll();
    
    // For error reporting
    size_t getCurrentLine() const { return line_; }
    size_t getCurrentColumn() const { return column_; }
};

} // namespace o2l