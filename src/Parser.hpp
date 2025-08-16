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

#include <memory>
#include <vector>

#include "AST/Node.hpp"
#include "Lexer.hpp"

namespace o2l {

class Parser {
   private:
    std::vector<Token> tokens_;
    size_t current_token_;
    std::string filename_;

    const Token& currentToken() const;
    const Token& peekToken(size_t offset = 1) const;
    void advance();
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& error_message);

   public:
    explicit Parser(std::vector<Token> tokens, const std::string& filename = "");

    // Parse the entire program
    std::vector<ASTNodePtr> parse();

    // Individual parsing methods (to be implemented)
    ASTNodePtr parseExpression();
    ASTNodePtr parseLogicalExpression();
    ASTNodePtr parseBinaryExpression();
    ASTNodePtr parseComparisonExpression();
    ASTNodePtr parseUnaryExpression();
    ASTNodePtr parsePrimaryExpression();
    ASTNodePtr parseAtomicExpression();
    ASTNodePtr parseObjectDeclaration();
    ASTNodePtr parseMethodDeclaration(bool is_external = false);
    ASTNodePtr parseConstructorDeclaration();
    ASTNodePtr parseIdentifierExpression();
    ASTNodePtr parseImportDeclaration();
    ASTNodePtr parseUserImportDeclaration();
    ASTNodePtr parseNewExpression();
    ASTNodePtr parseThisExpression();
    ASTNodePtr parseThisAssignment();
    ASTNodePtr parseReturnStatement();
    ASTNodePtr parseStatement();
    ASTNodePtr parseVariableDeclaration();
    ASTNodePtr parseVariableAssignment();
    ASTNodePtr parsePropertyDeclaration();
    ASTNodePtr parseConstDeclaration();
    ASTNodePtr parseIfStatement();
    ASTNodePtr parseWhileStatement();
    ASTNodePtr parseEnumDeclaration();
    ASTNodePtr parseRecordDeclaration();
    ASTNodePtr parseProtocolDeclaration();
    ASTNodePtr parseListLiteral();
    ASTNodePtr parseMapLiteral();
    ASTNodePtr parseSetLiteral();
    ASTNodePtr parseThrowStatement();
    ASTNodePtr parseTryCatchFinallyStatement();
    ASTNodePtr parseResultStaticCall();
    ASTNodePtr parseNamespaceDeclaration();

    // Helper method to parse type names including generics
    std::string parseTypeName();

    // Helper method to reconstruct qualified names from AST nodes
    std::string reconstructQualifiedName(ASTNode* node);
};

}  // namespace o2l