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

#include "Lexer.hpp"
#include "Common/Exceptions.hpp"
#include <cctype>

namespace o2l {

Lexer::Lexer(std::string_view source) 
    : source_(source), current_pos_(0), line_(1), column_(1) {}

char Lexer::currentChar() const {
    if (current_pos_ >= source_.size()) {
        return '\0';
    }
    return source_[current_pos_];
}

char Lexer::peekChar(size_t offset) const {
    size_t peek_pos = current_pos_ + offset;
    if (peek_pos >= source_.size()) {
        return '\0';
    }
    return source_[peek_pos];
}

void Lexer::advance() {
    if (current_pos_ < source_.size()) {
        if (currentChar() == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        current_pos_++;
    }
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar()) && currentChar() != '\n') {
        advance();
    }
}

void Lexer::skipComment() {
    if (currentChar() == '#') {
        if (peekChar() == '#' && peekChar(2) == '#') {
            // Skip multi-line comment ###...###
            advance(); // Skip first '#'
            advance(); // Skip second '#'
            advance(); // Skip third '#'
            
            while (currentChar() != '\0') {
                if (currentChar() == '#' && peekChar() == '#' && peekChar(2) == '#') {
                    advance(); // Skip first '#'
                    advance(); // Skip second '#'
                    advance(); // Skip third '#'
                    break;
                }
                advance();
            }
        } else {
            // Skip single-line comment #
            while (currentChar() != '\n' && currentChar() != '\0') {
                advance();
            }
        }
    }
}

Token Lexer::makeString() {
    size_t start_line = line_;
    size_t start_column = column_;
    
    advance(); // Skip opening quote
    std::string value;
    
    while (currentChar() != '"' && currentChar() != '\0') {
        if (currentChar() == '\\') {
            advance();
            switch (currentChar()) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default:
                    throw SyntaxError("Invalid escape sequence at line " + std::to_string(line_));
            }
        } else {
            value += currentChar();
        }
        advance();
    }
    
    if (currentChar() != '"') {
        throw SyntaxError("Unterminated string at line " + std::to_string(start_line));
    }
    
    advance(); // Skip closing quote
    return Token(TokenType::STRING, value, start_line, start_column);
}

Token Lexer::makeNumber() {
    size_t start_line = line_;
    size_t start_column = column_;
    std::string value;
    
    while (std::isdigit(currentChar())) {
        value += currentChar();
        advance();
    }
    
    // Check for decimal point
    if (currentChar() == '.' && std::isdigit(peekChar())) {
        value += currentChar();
        advance();
        while (std::isdigit(currentChar())) {
            value += currentChar();
            advance();
        }
    }
    
    // Check for type suffix (f for Float, d for Double, l for Long)
    if (currentChar() == 'f' || currentChar() == 'F') {
        value += currentChar();
        advance();
    } else if (currentChar() == 'd' || currentChar() == 'D') {
        value += currentChar();
        advance();
    } else if (currentChar() == 'l' || currentChar() == 'L') {
        // Long suffix - only for integer literals (no decimal point)
        if (value.find('.') == std::string::npos) {
            value += currentChar();
            advance();
        }
    }
    
    return Token(TokenType::NUMBER, value, start_line, start_column);
}

Token Lexer::makeNegativeNumber() {
    size_t start_line = line_;
    size_t start_column = column_;
    std::string value = "-";
    
    // Skip the minus sign
    advance();
    
    // Parse the number part
    while (std::isdigit(currentChar())) {
        value += currentChar();
        advance();
    }
    
    // Check for decimal point
    if (currentChar() == '.' && std::isdigit(peekChar())) {
        value += currentChar();
        advance();
        while (std::isdigit(currentChar())) {
            value += currentChar();
            advance();
        }
    }
    
    // Check for type suffix (f for Float, d for Double, l for Long)
    if (currentChar() == 'f' || currentChar() == 'F') {
        value += currentChar();
        advance();
    } else if (currentChar() == 'd' || currentChar() == 'D') {
        value += currentChar();
        advance();
    } else if (currentChar() == 'l' || currentChar() == 'L') {
        // Long suffix - only for integer literals (no decimal point)
        if (value.find('.') == std::string::npos) {
            value += currentChar();
            advance();
        }
    }
    
    return Token(TokenType::NUMBER, value, start_line, start_column);
}

Token Lexer::makeIdentifierOrKeyword() {
    size_t start_line = line_;
    size_t start_column = column_;
    std::string value;
    
    while (std::isalnum(currentChar()) || currentChar() == '_') {
        value += currentChar();
        advance();
    }
    
    TokenType type = getKeywordType(value);
    return Token(type, value, start_line, start_column);
}

Token Lexer::makeCharacter() {
    size_t start_line = line_;
    size_t start_column = column_;
    
    advance(); // Skip opening single quote
    
    if (currentChar() == '\0') {
        throw SyntaxError("Unterminated character literal at line " + std::to_string(start_line));
    }
    
    char value = currentChar();
    advance();
    
    if (currentChar() != '\'') {
        throw SyntaxError("Expected closing single quote at line " + std::to_string(line_));
    }
    
    advance(); // Skip closing quote
    return Token(TokenType::CHARACTER, std::string(1, value), start_line, start_column);
}

TokenType Lexer::getKeywordType(const std::string& identifier) const {
    if (identifier == "Object") return TokenType::OBJECT;
    if (identifier == "method") return TokenType::METHOD;
    if (identifier == "constructor") return TokenType::CONSTRUCTOR;
    if (identifier == "property") return TokenType::PROPERTY;
    if (identifier == "const") return TokenType::CONST;
    if (identifier == "Enum") return TokenType::ENUM;
    if (identifier == "Record") return TokenType::RECORD;
    if (identifier == "Protocol") return TokenType::PROTOCOL;
    if (identifier == "if") return TokenType::IF;
    if (identifier == "else") return TokenType::ELSE;
    if (identifier == "while") return TokenType::WHILE;
    if (identifier == "break") return TokenType::BREAK;
    if (identifier == "continue") return TokenType::CONTINUE;
    if (identifier == "return") return TokenType::RETURN;
    if (identifier == "this") return TokenType::THIS;
    if (identifier == "true") return TokenType::TRUE;
    if (identifier == "false") return TokenType::FALSE;
    if (identifier == "import") return TokenType::IMPORT;
    if (identifier == "new") return TokenType::NEW;
    if (identifier == "throw") return TokenType::THROW;
    if (identifier == "try") return TokenType::TRY;
    if (identifier == "catch") return TokenType::CATCH;
    if (identifier == "finally") return TokenType::FINALLY;
    if (identifier == "Result") return TokenType::RESULT;
    if (identifier == "Error") return TokenType::ERROR;
#if O2L_ENABLE_NAMESPACES
    if (identifier == "namespace") return TokenType::NAMESPACE;
#endif
    return TokenType::IDENTIFIER;
}

Token Lexer::nextToken() {
    skipWhitespace();
    skipComment();
    
    size_t current_line = line_;
    size_t current_column = column_;
    char ch = currentChar();
    
    if (ch == '\0') {
        return Token(TokenType::EOF_TOKEN, "", current_line, current_column);
    }
    
    if (ch == '\n') {
        advance();
        return Token(TokenType::NEWLINE, "\n", current_line, current_column);
    }
    
    if (ch == '"') {
        return makeString();
    }
    
    if (ch == '\'') {
        return makeCharacter();
    }
    
    if (std::isdigit(ch)) {
        return makeNumber();
    }
    
    if (std::isalpha(ch) || ch == '_') {
        return makeIdentifierOrKeyword();
    }
    
    // Check for multi-character operators first
    if (ch == '=' && peekChar() == '=') {
        advance(); advance(); // Skip both '=' characters
        return Token(TokenType::EQUAL, "==", current_line, current_column);
    }
    if (ch == '!' && peekChar() == '=') {
        advance(); advance(); // Skip both characters
        return Token(TokenType::NOT_EQUAL, "!=", current_line, current_column);
    }
    if (ch == '<' && peekChar() == '=') {
        advance(); advance(); // Skip both characters
        return Token(TokenType::LESS_EQUAL, "<=", current_line, current_column);
    }
    if (ch == '>' && peekChar() == '=') {
        advance(); advance(); // Skip both characters
        return Token(TokenType::GREATER_EQUAL, ">=", current_line, current_column);
    }
    if (ch == '&' && peekChar() == '&') {
        advance(); advance(); // Skip both '&' characters
        return Token(TokenType::LOGICAL_AND, "&&", current_line, current_column);
    }
    if (ch == '|' && peekChar() == '|') {
        advance(); advance(); // Skip both '|' characters
        return Token(TokenType::LOGICAL_OR, "||", current_line, current_column);
    }
    
    // Check for @external and @import first
    if (ch == '@') {
        if (peekChar() == 'e' && peekChar(2) == 'x' && peekChar(3) == 't' && 
            peekChar(4) == 'e' && peekChar(5) == 'r' && peekChar(6) == 'n' && 
            peekChar(7) == 'a' && peekChar(8) == 'l') {
            // Skip @external
            for (int i = 0; i < 9; i++) advance();
            return Token(TokenType::AT_EXTERNAL, "@external", current_line, current_column);
        } else if (peekChar() == 'i' && peekChar(2) == 'm' && peekChar(3) == 'p' && 
                   peekChar(4) == 'o' && peekChar(5) == 'r' && peekChar(6) == 't') {
            // Skip @import
            for (int i = 0; i < 7; i++) advance();
            return Token(TokenType::AT_IMPORT, "@import", current_line, current_column);
        }
    }
    
    // Single character tokens
    advance();
    switch (ch) {
        case '+': return Token(TokenType::PLUS, "+", current_line, current_column);
        case '-': 
            return Token(TokenType::MINUS, "-", current_line, current_column);
        case '*': return Token(TokenType::MULTIPLY, "*", current_line, current_column);
        case '/': return Token(TokenType::DIVIDE, "/", current_line, current_column);
        case '%': return Token(TokenType::MODULO, "%", current_line, current_column);
        case '=': return Token(TokenType::ASSIGN, "=", current_line, current_column);
        case '!': return Token(TokenType::LOGICAL_NOT, "!", current_line, current_column);
        case '<': return Token(TokenType::LESS_THAN, "<", current_line, current_column);
        case '>': return Token(TokenType::GREATER_THAN, ">", current_line, current_column);
        case '.': return Token(TokenType::DOT, ".", current_line, current_column);
        case '$': return Token(TokenType::DOLLAR, "$", current_line, current_column);
        case '(': return Token(TokenType::LPAREN, "(", current_line, current_column);
        case ')': return Token(TokenType::RPAREN, ")", current_line, current_column);
        case '{': return Token(TokenType::LBRACE, "{", current_line, current_column);
        case '}': return Token(TokenType::RBRACE, "}", current_line, current_column);
        case '[': return Token(TokenType::LBRACKET, "[", current_line, current_column);
        case ']': return Token(TokenType::RBRACKET, "]", current_line, current_column);
        case ':': return Token(TokenType::COLON, ":", current_line, current_column);
        case ',': return Token(TokenType::COMMA, ",", current_line, current_column);
        case ';': return Token(TokenType::SEMICOLON, ";", current_line, current_column);
        default:
            return Token(TokenType::INVALID, std::string(1, ch), current_line, current_column);
    }
}

std::vector<Token> Lexer::tokenizeAll() {
    std::vector<Token> tokens;
    
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        if (token.type == TokenType::EOF_TOKEN) {
            break;
        }
    }
    
    return tokens;
}

} // namespace o2l