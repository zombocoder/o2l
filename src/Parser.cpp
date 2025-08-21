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

#include "Parser.hpp"
#include "Common/Exceptions.hpp"
#include "AST/LiteralNode.hpp"
#include "AST/ObjectNode.hpp"
#include "AST/MethodDeclarationNode.hpp"
#include "AST/ConstructorDeclarationNode.hpp"
#include "AST/IdentifierNode.hpp"
#include "AST/MethodCallNode.hpp"
#include "AST/ImportNode.hpp"
#include "AST/NewExpressionNode.hpp"
#include "AST/PropertyAccessNode.hpp"
#include "AST/PropertyAssignmentNode.hpp"
#include "AST/ThisNode.hpp"
#include "AST/ReturnNode.hpp"
#include "AST/BinaryOpNode.hpp"
#include "AST/VariableDeclarationNode.hpp"
#include "AST/VariableAssignmentNode.hpp"
#include "AST/BlockNode.hpp"
#include "AST/PropertyDeclarationNode.hpp"
#include "AST/FunctionCallNode.hpp"
#include "AST/ConstDeclarationNode.hpp"
#include "AST/IfStatementNode.hpp"
#include "AST/WhileStatementNode.hpp"
#include "AST/BreakNode.hpp"
#include "AST/ComparisonNode.hpp"
#include "AST/LogicalNode.hpp"
#include "AST/UnaryNode.hpp"
#include "AST/EnumDeclarationNode.hpp"
#include "AST/EnumAccessNode.hpp"
#include "AST/RecordDeclarationNode.hpp"
#include "AST/RecordInstantiationNode.hpp"
#include "AST/RecordFieldAccessNode.hpp"
#include "AST/MemberAccessNode.hpp"
#include "AST/ProtocolDeclarationNode.hpp"
#include "AST/ThrowNode.hpp"
#include "AST/TryCatchFinallyNode.hpp"
#include "AST/ListLiteralNode.hpp"
#include "AST/MapLiteralNode.hpp"
#include "AST/SetLiteralNode.hpp"
#include "AST/NamespaceNode.hpp"
#include "AST/QualifiedIdentifierNode.hpp"

namespace o2l {

Parser::Parser(std::vector<Token> tokens, const std::string& filename) 
    : tokens_(std::move(tokens)), current_token_(0), filename_(filename) {}

const Token& Parser::currentToken() const {
    if (current_token_ >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[current_token_];
}

const Token& Parser::peekToken(size_t offset) const {
    size_t peek_pos = current_token_ + offset;
    if (peek_pos >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[peek_pos];
}

void Parser::advance() {
    if (current_token_ < tokens_.size()) {
        current_token_++;
    }
}

bool Parser::match(TokenType type) {
    if (currentToken().type == type) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& error_message) {
    if (currentToken().type == type) {
        Token token = currentToken();
        advance();
        return token;
    }
    
    throw SyntaxError(error_message + " at line " + std::to_string(currentToken().line));
}

std::vector<ASTNodePtr> Parser::parse() {
    std::vector<ASTNodePtr> nodes;
    
    while (currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines at top level
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        if (currentToken().type == TokenType::OBJECT) {
            nodes.push_back(parseObjectDeclaration());
        } else if (currentToken().type == TokenType::IMPORT) {
            nodes.push_back(parseImportDeclaration());
        } else if (currentToken().type == TokenType::AT_IMPORT) {
            nodes.push_back(parseUserImportDeclaration());
        } else if (currentToken().type == TokenType::ENUM) {
            nodes.push_back(parseEnumDeclaration());
        } else if (currentToken().type == TokenType::RECORD) {
            nodes.push_back(parseRecordDeclaration());
        } else if (currentToken().type == TokenType::PROTOCOL) {
            nodes.push_back(parseProtocolDeclaration());
        } else if (currentToken().type == TokenType::NAMESPACE) {
#if O2L_ENABLE_NAMESPACES
            nodes.push_back(parseNamespaceDeclaration());
#else
            throw SyntaxError("Namespace functionality is disabled in this build. Use -DENABLE_NAMESPACES=ON to enable.");
#endif
        } else {
            throw SyntaxError("Only object declarations, enum declarations, record declarations, protocol declarations, namespace declarations, and import statements are allowed at the top level. Found '" + 
                            currentToken().value + "' at line " + std::to_string(currentToken().line));
        }
    }
    
    return nodes;
}

ASTNodePtr Parser::parseExpression() {
    return parseLogicalExpression();
}

ASTNodePtr Parser::parseLogicalExpression() {
    ASTNodePtr left = parseBinaryExpression();
    
    // Handle logical operators (AND has higher precedence than OR)
    while (currentToken().type == TokenType::LOGICAL_AND ||
           currentToken().type == TokenType::LOGICAL_OR) {
        
        LogicalOperator op;
        switch (currentToken().type) {
            case TokenType::LOGICAL_AND: op = LogicalOperator::AND; break;
            case TokenType::LOGICAL_OR: op = LogicalOperator::OR; break;
            default: break; // Won't reach here due to while condition
        }
        
        // Capture source location before consuming operator
        SourceLocation opLocation(filename_, currentToken().line, currentToken().column);
        advance(); // consume operator
        ASTNodePtr right = parseBinaryExpression();
        left = std::make_unique<LogicalNode>(std::move(left), op, std::move(right), opLocation);
    }
    
    return left;
}

ASTNodePtr Parser::parseBinaryExpression() {
    ASTNodePtr left = parseComparisonExpression();
    
    // Handle binary operators with left-associativity
    while (currentToken().type == TokenType::PLUS || 
           currentToken().type == TokenType::MINUS ||
           currentToken().type == TokenType::MULTIPLY ||
           currentToken().type == TokenType::DIVIDE ||
           currentToken().type == TokenType::MODULO) {
        
        BinaryOperator op;
        switch (currentToken().type) {
            case TokenType::PLUS: op = BinaryOperator::PLUS; break;
            case TokenType::MINUS: op = BinaryOperator::MINUS; break;
            case TokenType::MULTIPLY: op = BinaryOperator::MULTIPLY; break;
            case TokenType::DIVIDE: op = BinaryOperator::DIVIDE; break;
            case TokenType::MODULO: op = BinaryOperator::MODULO; break;
            default: break; // Won't reach here due to while condition
        }
        
        // Capture source location before consuming operator
        SourceLocation opLocation(filename_, currentToken().line, currentToken().column);
        advance(); // consume operator
        ASTNodePtr right = parseComparisonExpression();
        left = std::make_unique<BinaryOpNode>(std::move(left), op, std::move(right), opLocation);
    }
    
    return left;
}

ASTNodePtr Parser::parseComparisonExpression() {
    ASTNodePtr left = parseUnaryExpression();
    
    // Handle comparison operators
    while (currentToken().type == TokenType::EQUAL ||
           currentToken().type == TokenType::NOT_EQUAL ||
           currentToken().type == TokenType::LESS_THAN ||
           currentToken().type == TokenType::GREATER_THAN ||
           currentToken().type == TokenType::LESS_EQUAL ||
           currentToken().type == TokenType::GREATER_EQUAL) {
        
        ComparisonOperator op;
        switch (currentToken().type) {
            case TokenType::EQUAL: op = ComparisonOperator::EQUAL; break;
            case TokenType::NOT_EQUAL: op = ComparisonOperator::NOT_EQUAL; break;
            case TokenType::LESS_THAN: op = ComparisonOperator::LESS_THAN; break;
            case TokenType::GREATER_THAN: op = ComparisonOperator::GREATER_THAN; break;
            case TokenType::LESS_EQUAL: op = ComparisonOperator::LESS_EQUAL; break;
            case TokenType::GREATER_EQUAL: op = ComparisonOperator::GREATER_EQUAL; break;
            default: break; // Won't reach here due to while condition
        }
        
        // Capture source location before consuming operator
        SourceLocation opLocation(filename_, currentToken().line, currentToken().column);
        advance(); // consume operator
        ASTNodePtr right = parseUnaryExpression();
        left = std::make_unique<ComparisonNode>(std::move(left), op, std::move(right), opLocation);
    }
    
    return left;
}

ASTNodePtr Parser::parsePrimaryExpression() {
    ASTNodePtr expr = parseAtomicExpression();
    
    // Handle remaining chained member access and method calls
    while (currentToken().type == TokenType::DOT) {
        advance(); // consume dot
        
        Token member_name_token = consume(TokenType::IDENTIFIER, "Expected member name after '.'");
        std::string member_name = member_name_token.value;
        
        // Check if this is a method call (has parentheses)
        if (currentToken().type == TokenType::LPAREN) {
            // Check if this looks like a static method call (multiple chained parts)
            // Count the chain depth by checking if expr is already a method call or member access
            int chain_depth = 1; // At least the current call
            ASTNode* current = expr.get();
            while (auto method_call = dynamic_cast<MethodCallNode*>(current)) {
                chain_depth++;
                current = method_call->getObject().get();
            }
            while (auto member_access = dynamic_cast<MemberAccessNode*>(current)) {
                chain_depth++;
                current = member_access->getObjectExpression().get();
            }
            
            // If we have a deep chain (3+ parts), it might be a static method call
            if (chain_depth >= 3) {
                // Try to reconstruct the full qualified name for error message
                std::string full_name = reconstructQualifiedName(expr.get()) + "." + member_name;
                
                // Only throw error if this looks like a namespace pattern
                if (full_name.find('.') != std::string::npos) {
                    size_t last_dot = full_name.find_last_of('.');
                    std::string namespace_part = full_name.substr(0, last_dot);
                    std::string method_part = full_name.substr(last_dot + 1);
                    
                    throw SyntaxError("Static method calls are not allowed. Use 'import " + namespace_part + 
                                    "' then create an instance with 'new " + method_part + "()'");
                }
            }
            advance(); // consume '('
            
            std::vector<ASTNodePtr> arguments;
            while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
                // Skip newlines
                while (match(TokenType::NEWLINE)) {
                    // Skip newlines
                }
                
                if (currentToken().type == TokenType::RPAREN) {
                    break; // End of arguments
                }
                
                arguments.push_back(parseExpression());
                
                if (currentToken().type == TokenType::COMMA) {
                    advance(); // consume comma
                    
                    // Skip newlines after comma
                    while (match(TokenType::NEWLINE)) {
                        // Skip newlines
                    }
                } else if (currentToken().type == TokenType::NEWLINE) {
                    // Allow newlines without comma
                    while (match(TokenType::NEWLINE)) {
                        // Skip newlines
                    }
                } else if (currentToken().type != TokenType::RPAREN) {
                    throw SyntaxError("Expected ',' or ')' in argument list at line " + std::to_string(currentToken().line));
                }
            }
            
            consume(TokenType::RPAREN, "Expected ')' to close argument list");
            
            // Create method call node
            SourceLocation location(filename_, member_name_token.line, member_name_token.column);
            expr = std::make_unique<MethodCallNode>(std::move(expr), member_name, std::move(arguments), location);
        } else {
            // This is member access (property or field access)
            auto member_access_node = std::make_unique<MemberAccessNode>(std::move(expr), member_name);
            SourceLocation location(filename_, member_name_token.line, member_name_token.column);
            member_access_node->setSourceLocation(location);
            expr = std::move(member_access_node);
        }
    }
    
    return expr;
}

ASTNodePtr Parser::parseUnaryExpression() {
    // Handle unary operators
    if (currentToken().type == TokenType::MINUS || currentToken().type == TokenType::LOGICAL_NOT) {
        UnaryOperator op;
        switch (currentToken().type) {
            case TokenType::MINUS: op = UnaryOperator::MINUS; break;
            case TokenType::LOGICAL_NOT: op = UnaryOperator::NOT; break;
            default: break; // Won't reach here due to if condition
        }
        
        // Capture source location before consuming operator
        SourceLocation opLocation(filename_, currentToken().line, currentToken().column);
        advance(); // consume unary operator
        
        // Parse the operand - now calls parsePrimaryExpression to include method calls
        ASTNodePtr operand = parseUnaryExpression(); // Recursive call for chained unary operators
        
        return std::make_unique<UnaryNode>(op, std::move(operand), opLocation);
    }
    
    // No unary operator, parse primary expression (method calls, member access, etc.)
    return parsePrimaryExpression();
}

ASTNodePtr Parser::parseAtomicExpression() {
    
    // Handle literals and basic expressions
    const Token& token = currentToken();
    
    switch (token.type) {
        case TokenType::STRING: {
            SourceLocation location(filename_, token.line, token.column);
            advance();
            auto literal_node = std::make_unique<LiteralNode>(Text(token.value));
            literal_node->setSourceLocation(location);
            return literal_node;
        }
            
        case TokenType::NUMBER: {
            SourceLocation location(filename_, token.line, token.column);
            std::string token_value = token.value; // Save token value before advancing
            advance();
            auto literal_node = [&]() -> std::unique_ptr<LiteralNode> {
                // Parse number based on suffix and decimal point
                if (token_value.back() == 'f' || token_value.back() == 'F') {
                    // Float literal with 'f' suffix
                    std::string number_part = token_value.substr(0, token_value.length() - 1);
                    return std::make_unique<LiteralNode>(Float(std::stof(number_part)));
                } else if (token_value.back() == 'd' || token_value.back() == 'D') {
                    // Double literal with 'd' suffix
                    std::string number_part = token_value.substr(0, token_value.length() - 1);
                    return std::make_unique<LiteralNode>(Double(std::stod(number_part)));
                } else if (token_value.back() == 'l' || token_value.back() == 'L') {
                    // Long literal with 'l' suffix
                    std::string number_part = token_value.substr(0, token_value.length() - 1);
                    Long long_value;
#ifdef __SIZEOF_INT128__
                    // Parse manually for 128-bit integers
                    long_value = 0;
                    bool negative = false;
                    size_t start = 0;
                    if (!number_part.empty() && number_part[0] == '-') {
                        negative = true;
                        start = 1;
                    }
                    for (size_t i = start; i < number_part.length(); ++i) {
                        long_value = long_value * 10 + (number_part[i] - '0');
                    }
                    if (negative) long_value = -long_value;
#else
                    long_value = std::stoll(number_part);
#endif
                    return std::make_unique<LiteralNode>(long_value);
                } else if (token_value.find('.') != std::string::npos) {
                    // Decimal without suffix defaults to Double
                    return std::make_unique<LiteralNode>(Double(std::stod(token_value)));
                } else {
                    // Integer literal
                    return std::make_unique<LiteralNode>(Int(std::stoll(token_value)));
                }
            }();
            literal_node->setSourceLocation(location);
            return literal_node;
        }
            
        case TokenType::TRUE: {
            SourceLocation location(filename_, token.line, token.column);
            advance();
            auto literal_node = std::make_unique<LiteralNode>(Bool(true));
            literal_node->setSourceLocation(location);
            return literal_node;
        }
            
        case TokenType::FALSE: {
            SourceLocation location(filename_, token.line, token.column);
            advance();
            auto literal_node = std::make_unique<LiteralNode>(Bool(false));
            literal_node->setSourceLocation(location);
            return literal_node;
        }
            
        case TokenType::CHARACTER: {
            SourceLocation location(filename_, token.line, token.column);
            advance();
            auto literal_node = std::make_unique<LiteralNode>(Char(token.value[0]));
            literal_node->setSourceLocation(location);
            return literal_node;
        }
            
        case TokenType::IDENTIFIER:
            return parseIdentifierExpression();
            
        case TokenType::DOLLAR:
            {
                Token dollar_token = currentToken();
                advance(); // consume $
                Token identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier after '$'");
                std::string variable_name = "$" + identifier_token.value;
                auto identifier_node = std::make_unique<IdentifierNode>(variable_name);
                SourceLocation location(filename_, dollar_token.line, dollar_token.column);
                identifier_node->setSourceLocation(location);
                return identifier_node;
            }
            
        case TokenType::NEW:
            return parseNewExpression();
            
        case TokenType::THIS:
            return parseThisExpression();
            
        case TokenType::RETURN:
            return parseReturnStatement();
            
        case TokenType::LBRACKET:
            return parseListLiteral();
            
        case TokenType::LBRACE:
            return parseMapLiteral();
            
        case TokenType::RESULT:
            return parseResultStaticCall();
            
        case TokenType::LPAREN:
            // Handle parenthesized expressions
            advance(); // consume '('
            {
                ASTNodePtr expr = parseExpression();
                consume(TokenType::RPAREN, "Expected ')' to close parenthesized expression");
                return expr;
            }
            
        default:
            throw SyntaxError("Unexpected token '" + token.value + "' at line " + std::to_string(token.line));
    }
}

ASTNodePtr Parser::parseObjectDeclaration() {
    Token object_token = consume(TokenType::OBJECT, "Expected 'Object'");
    SourceLocation location(filename_, object_token.line, object_token.column);
    
    Token name_token = consume(TokenType::IDENTIFIER, "Expected object name");
    std::string object_name = name_token.value;
    
    // Check for protocol inheritance: Object Name: Protocol
    std::string protocol_name;
    if (currentToken().type == TokenType::COLON) {
        advance(); // consume ':'
        Token protocol_token = consume(TokenType::IDENTIFIER, "Expected protocol name after ':'");
        protocol_name = protocol_token.value;
    }
    
    consume(TokenType::LBRACE, "Expected '{' after object name");
    
    std::vector<ASTNodePtr> methods;
    std::vector<ASTNodePtr> properties;
    ASTNodePtr constructor = nullptr;
    
    // Parse properties, methods and constructor until we hit the closing brace
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        if (currentToken().type == TokenType::PROPERTY) {
            properties.push_back(parsePropertyDeclaration());
        } else if (currentToken().type == TokenType::METHOD) {
            methods.push_back(parseMethodDeclaration(false)); // not external
        } else if (currentToken().type == TokenType::AT_EXTERNAL) {
            advance(); // consume @external
            if (currentToken().type == TokenType::METHOD) {
                methods.push_back(parseMethodDeclaration(true)); // external
            } else {
                throw SyntaxError("@external modifier can only be applied to methods at line " + std::to_string(currentToken().line));
            }
        } else if (currentToken().type == TokenType::CONSTRUCTOR) {
            if (constructor) {
                throw SyntaxError("Only one constructor allowed per object at line " + std::to_string(currentToken().line));
            }
            constructor = parseConstructorDeclaration();
        } else if (currentToken().type == TokenType::RBRACE) {
            break; // End of object
        } else {
            throw SyntaxError("Expected property, method or constructor declaration in object at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close object declaration");
    
    auto object_node = std::make_unique<ObjectNode>(object_name, std::move(methods), std::move(properties), std::move(constructor), protocol_name);
    object_node->setSourceLocation(location);
    return object_node;
}

ASTNodePtr Parser::parseMethodDeclaration(bool is_external) {
    Token method_token = consume(TokenType::METHOD, "Expected 'method'");
    
    Token method_name_token = consume(TokenType::IDENTIFIER, "Expected method name");
    std::string method_name = method_name_token.value;
    
    consume(TokenType::LPAREN, "Expected '(' after method name");
    
    // Parse parameters
    std::vector<Parameter> parameters;
    
    while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
        // Check for system parameter prefix $
        bool is_system_param = false;
        if (currentToken().type == TokenType::DOLLAR) {
            is_system_param = true;
            advance(); // consume $
        }
        
        Token param_name_token = consume(TokenType::IDENTIFIER, "Expected parameter name");
        consume(TokenType::COLON, "Expected ':' after parameter name");
        std::string param_type = parseTypeName();
        
        // Add $ prefix to parameter name if it's a system parameter
        std::string param_name = is_system_param ? "$" + param_name_token.value : param_name_token.value;
        parameters.emplace_back(param_name, param_type);
        
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
        } else if (currentToken().type != TokenType::RPAREN) {
            throw SyntaxError("Expected ',' or ')' in parameter list at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RPAREN, "Expected ')' after parameters");
    consume(TokenType::COLON, "Expected ':' after parameters");
    
    std::string return_type = parseTypeName();
    
    Token lbrace_token = consume(TokenType::LBRACE, "Expected '{' to start method body");
    
    // Parse method body as a block of statements
    std::vector<ASTNodePtr> statements;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        statements.push_back(parseStatement());
    }
    
    auto body = std::make_unique<BlockNode>(std::move(statements));
    SourceLocation block_location(filename_, lbrace_token.line, lbrace_token.column);
    body->setSourceLocation(block_location);
    
    consume(TokenType::RBRACE, "Expected '}' to end method body");
    
    // Create source location from the method token
    SourceLocation location(filename_, method_token.line, method_token.column);
    
    return std::make_unique<MethodDeclarationNode>(method_name, std::move(parameters), return_type, std::move(body), is_external, location);
}

ASTNodePtr Parser::parseConstructorDeclaration() {
    Token constructor_token = consume(TokenType::CONSTRUCTOR, "Expected 'constructor'");
    
    consume(TokenType::LPAREN, "Expected '(' after constructor");
    
    // Parse parameters
    std::vector<Parameter> parameters;
    
    while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
        Token param_name_token = consume(TokenType::IDENTIFIER, "Expected parameter name");
        consume(TokenType::COLON, "Expected ':' after parameter name");
        std::string param_type = parseTypeName();
        
        parameters.emplace_back(param_name_token.value, param_type);
        
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
        } else if (currentToken().type != TokenType::RPAREN) {
            throw SyntaxError("Expected ',' or ')' in parameter list at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RPAREN, "Expected ')' after parameters");
    
    // Skip return type if present (constructors don't return values)
    if (currentToken().type == TokenType::COLON) {
        advance(); // consume colon
        consume(TokenType::IDENTIFIER, "Expected return type after ':'");
    }
    
    Token body_lbrace_token = consume(TokenType::LBRACE, "Expected '{' to start constructor body");
    
    // Parse constructor body as a block of statements
    std::vector<ASTNodePtr> statements;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        statements.push_back(parseStatement());
    }
    
    auto body = std::make_unique<BlockNode>(std::move(statements));
    SourceLocation body_location(filename_, body_lbrace_token.line, body_lbrace_token.column);
    body->setSourceLocation(body_location);
    
    consume(TokenType::RBRACE, "Expected '}' to end constructor body");
    
    auto constructor_decl = std::make_unique<ConstructorDeclarationNode>(std::move(parameters), std::move(body));
    SourceLocation location(filename_, constructor_token.line, constructor_token.column);
    constructor_decl->setSourceLocation(location);
    return constructor_decl;
}

ASTNodePtr Parser::parseIdentifierExpression() {
    Token identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");
    std::string identifier = identifier_token.value;
    
    // Check if this is a function call (identifier(...)) or record instantiation (RecordType(field=value, ...))
    if (currentToken().type == TokenType::LPAREN) {
        advance(); // consume '('
        
        // Look ahead to determine if this is record instantiation (has field=value syntax)
        bool is_record_instantiation = false;
        size_t saved_pos = current_token_;
        
        // Skip newlines for lookahead
        while (currentToken().type == TokenType::NEWLINE) {
            advance();
        }
        
        // Check if first argument has assignment syntax (identifier = expression)
        if (currentToken().type == TokenType::IDENTIFIER && peekToken().type == TokenType::ASSIGN) {
            is_record_instantiation = true;
        }
        current_token_ = saved_pos; // Restore position
        
        if (is_record_instantiation) {
            // Parse record instantiation
            std::vector<RecordFieldAssignment> field_assignments;
            
            while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
                // Skip newlines
                while (match(TokenType::NEWLINE)) {
                    // Skip newlines
                }
                
                if (currentToken().type == TokenType::RPAREN) {
                    break; // End of record fields
                }
                
                Token field_name_token = consume(TokenType::IDENTIFIER, "Expected field name");
                std::string field_name = field_name_token.value;
                
                consume(TokenType::ASSIGN, "Expected '=' after field name");
                
                ASTNodePtr field_value = parseExpression();
                field_assignments.emplace_back(field_name, std::move(field_value));
                
                if (currentToken().type == TokenType::COMMA) {
                    advance(); // consume comma
                    
                    // Skip newlines after comma
                    while (match(TokenType::NEWLINE)) {
                        // Skip newlines
                    }
                } else if (currentToken().type == TokenType::NEWLINE) {
                    // Allow newlines without comma (optional comma style)
                    while (match(TokenType::NEWLINE)) {
                        // Skip newlines
                    }
                } else if (currentToken().type != TokenType::RPAREN) {
                    throw SyntaxError("Expected ',' or ')' in record field list at line " + std::to_string(currentToken().line));
                }
            }
            
            consume(TokenType::RPAREN, "Expected ')' after record fields");
            
            // Create record instantiation node
            auto record_inst = std::make_unique<RecordInstantiationNode>(identifier, std::move(field_assignments));
            SourceLocation location(filename_, identifier_token.line, identifier_token.column);
            record_inst->setSourceLocation(location);
            return record_inst;
        } else {
            // Parse function call
            std::vector<ASTNodePtr> arguments;
            
            while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
                // Skip newlines
                while (match(TokenType::NEWLINE)) {
                    // Skip newlines
                }
                
                if (currentToken().type == TokenType::RPAREN) {
                    break; // End of arguments
                }
                
                arguments.push_back(parseExpression());
                
                if (currentToken().type == TokenType::COMMA) {
                    advance(); // consume comma
                    
                    // Skip newlines after comma
                    while (match(TokenType::NEWLINE)) {
                        // Skip newlines
                    }
                } else if (currentToken().type == TokenType::NEWLINE) {
                    // Allow newlines without comma
                    while (match(TokenType::NEWLINE)) {
                        // Skip newlines
                    }
                } else if (currentToken().type != TokenType::RPAREN) {
                    throw SyntaxError("Expected ',' or ')' in argument list at line " + std::to_string(currentToken().line));
                }
            }
            
            consume(TokenType::RPAREN, "Expected ')' after arguments");
            
            // Create function call node
            auto function_call_node = std::make_unique<FunctionCallNode>(identifier, std::move(arguments));
            SourceLocation location(filename_, identifier_token.line, identifier_token.column);
            function_call_node->setSourceLocation(location);
            return function_call_node;
        }
    }
    
    // Just a simple identifier reference
    auto identifier_node = std::make_unique<IdentifierNode>(identifier);
    SourceLocation location(filename_, identifier_token.line, identifier_token.column);
    identifier_node->setSourceLocation(location);
    return identifier_node;
}

ASTNodePtr Parser::parseImportDeclaration() {
    Token import_token = consume(TokenType::IMPORT, "Expected 'import'");
    SourceLocation location(filename_, import_token.line, import_token.column);
    
    ImportPath import_path;
    std::vector<std::string> parts;
    
    // Parse all dot-separated parts first
    while (true) {
        Token identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier in import path");
        parts.push_back(identifier_token.value);
        
        if (currentToken().type == TokenType::DOT) {
            advance(); // consume dot
            
            // Check if next is wildcard
            if (currentToken().type == TokenType::MULTIPLY) {
                advance(); // consume *
                import_path.method_name = "*";
                break;
            }
            // Otherwise continue parsing identifiers
        } else {
            break;
        }
    }
    
    // Now assign parts based on count:
    // 1 part: object_name
    // 2+ parts: package...package.object_name (unless wildcard, then last part before * is method)
    if (parts.size() == 1) {
        import_path.object_name = parts[0];
    } else {
        // All but last are package path, last is object name
        for (size_t i = 0; i < parts.size() - 1; ++i) {
            import_path.package_path.push_back(parts[i]);
        }
        import_path.object_name = parts.back();
        
        // method_name is only set for wildcard imports (handled above)
    }
    
    auto import_node = std::make_unique<ImportNode>(std::move(import_path));
    import_node->setSourceLocation(location);
    return import_node;
}

ASTNodePtr Parser::parseUserImportDeclaration() {
    Token import_token = consume(TokenType::AT_IMPORT, "Expected '@import'");
    SourceLocation location(filename_, import_token.line, import_token.column);
    
    ImportPath import_path;
    import_path.is_user_import = true; // Mark as user import
    std::vector<std::string> parts;
    
    // Parse all dot-separated parts first
    while (true) {
        Token identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier in @import path");
        parts.push_back(identifier_token.value);
        
        if (currentToken().type == TokenType::DOT) {
            advance(); // consume dot
            
            // Check if next is wildcard
            if (currentToken().type == TokenType::MULTIPLY) {
                advance(); // consume *
                import_path.method_name = "*";
                break;
            }
            // Otherwise continue parsing identifiers
        } else {
            break;
        }
    }
    
    // Now assign parts based on count:
    // For user imports (@import), we don't support method-specific imports
    // All parts except the last are package path, last part is object name
    if (parts.size() == 1) {
        import_path.object_name = parts[0];
    } else {
        // All but last part are package path
        for (size_t i = 0; i < parts.size() - 1; ++i) {
            import_path.package_path.push_back(parts[i]);
        }
        import_path.object_name = parts[parts.size() - 1];
    }
    
    auto import_node = std::make_unique<ImportNode>(std::move(import_path));
    import_node->setSourceLocation(location);
    return import_node;
}

ASTNodePtr Parser::parseNewExpression() {
    Token new_token = consume(TokenType::NEW, "Expected 'new'");
    
    std::string object_type_name;
    if (currentToken().type == TokenType::IDENTIFIER) {
        Token object_type_token = consume(TokenType::IDENTIFIER, "Expected object type name after 'new'");
        object_type_name = object_type_token.value;
        
        // Check for qualified type name (namespace.path.Type)
        while (currentToken().type == TokenType::DOT) {
            advance(); // consume dot
            Token part_token = consume(TokenType::IDENTIFIER, "Expected identifier after '.' in type name");
            object_type_name += "." + part_token.value;
        }
    } else if (currentToken().type == TokenType::ERROR) {
        Token object_type_token = consume(TokenType::ERROR, "Expected Error type after 'new'");
        object_type_name = object_type_token.value;
    } else if (currentToken().type == TokenType::RESULT) {
        Token object_type_token = consume(TokenType::RESULT, "Expected Result type after 'new'");
        object_type_name = object_type_token.value;
    } else {
        throw SyntaxError("Expected object type name after 'new' at line " + std::to_string(currentToken().line));
    }
    
    consume(TokenType::LPAREN, "Expected '(' after object type name");
    
    // Parse constructor arguments
    std::vector<ASTNodePtr> constructor_args;
    
    while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
        constructor_args.push_back(parseExpression());
        
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
        } else if (currentToken().type != TokenType::RPAREN) {
            throw SyntaxError("Expected ',' or ')' in constructor arguments at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RPAREN, "Expected ')' after constructor arguments");
    
    // Create the new expression node
    auto new_expr = std::make_unique<NewExpressionNode>(object_type_name, std::move(constructor_args));
    SourceLocation location(filename_, new_token.line, new_token.column);
    new_expr->setSourceLocation(location);
    
    // Check for method calls on the newly created object
    if (currentToken().type == TokenType::DOT) {
        advance(); // consume dot
        
        Token method_name_token = consume(TokenType::IDENTIFIER, "Expected method name after '.'");
        std::string method_name = method_name_token.value;
        
        consume(TokenType::LPAREN, "Expected '(' after method name");
        
        // Parse arguments
        std::vector<ASTNodePtr> arguments;
        
        while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
            arguments.push_back(parseExpression());
            
            if (currentToken().type == TokenType::COMMA) {
                advance(); // consume comma
            } else if (currentToken().type != TokenType::RPAREN) {
                throw SyntaxError("Expected ',' or ')' in argument list at line " + std::to_string(currentToken().line));
            }
        }
        
        consume(TokenType::RPAREN, "Expected ')' after arguments");
        
        // Create method call node with the new expression as the object
        SourceLocation location(filename_, method_name_token.line, method_name_token.column);
        return std::make_unique<MethodCallNode>(std::move(new_expr), method_name, std::move(arguments), location);
    }
    
    return new_expr;
}

ASTNodePtr Parser::parseThisExpression() {
    Token this_token = consume(TokenType::THIS, "Expected 'this'");
    
    // Return a ThisNode - the general member access logic in parsePrimary() 
    // will handle any following .property or .method() calls
    auto this_node = std::make_unique<ThisNode>();
    SourceLocation location(filename_, this_token.line, this_token.column);
    this_node->setSourceLocation(location);
    return this_node;
}

ASTNodePtr Parser::parseThisAssignment() {
    Token this_token = consume(TokenType::THIS, "Expected 'this'");
    consume(TokenType::DOT, "Expected '.' after 'this'");
    
    Token property_name_token = consume(TokenType::IDENTIFIER, "Expected property name after 'this.'");
    std::string property_name = property_name_token.value;
    
    consume(TokenType::ASSIGN, "Expected '=' after property name");
    
    ASTNodePtr value_expr = parseExpression();
    auto property_assignment = std::make_unique<PropertyAssignmentNode>(property_name, std::move(value_expr));
    SourceLocation location(filename_, this_token.line, this_token.column);
    property_assignment->setSourceLocation(location);
    return property_assignment;
}

ASTNodePtr Parser::parseReturnStatement() {
    Token return_token = consume(TokenType::RETURN, "Expected 'return'");
    
    // Check if there's an expression to return
    if (currentToken().type == TokenType::RBRACE || currentToken().type == TokenType::NEWLINE || currentToken().type == TokenType::EOF_TOKEN) {
        // No expression, return void/default
        auto return_node = std::make_unique<ReturnNode>(nullptr);
        SourceLocation location(filename_, return_token.line, return_token.column);
        return_node->setSourceLocation(location);
        return return_node;
    }
    
    // Parse the return expression
    ASTNodePtr expr = parseExpression();
    auto return_node = std::make_unique<ReturnNode>(std::move(expr));
    SourceLocation location(filename_, return_token.line, return_token.column);
    return_node->setSourceLocation(location);
    return return_node;
}

ASTNodePtr Parser::parseStatement() {
    // Skip newlines
    while (match(TokenType::NEWLINE)) {
        // Skip newlines
    }
    
    const Token& token = currentToken();
    
    // Check for return statements
    if (token.type == TokenType::RETURN) {
        return parseReturnStatement();
    }
    
    // Check for const declarations
    if (token.type == TokenType::CONST) {
        return parseConstDeclaration();
    }
    
    // Check for if statements
    if (token.type == TokenType::IF) {
        return parseIfStatement();
    }
    
    // Check for while statements
    if (token.type == TokenType::WHILE) {
        return parseWhileStatement();
    }
    
    // Check for break statements
    if (token.type == TokenType::BREAK) {
        return parseBreakStatement();
    }
    
    // Check for throw statements
    if (token.type == TokenType::THROW) {
        return parseThrowStatement();
    }
    
    // Check for try/catch/finally statements
    if (token.type == TokenType::TRY) {
        return parseTryCatchFinallyStatement();
    }
    
    // Check for variable declarations (identifier followed by colon)
    if (token.type == TokenType::IDENTIFIER && peekToken().type == TokenType::COLON) {
        return parseVariableDeclaration();
    }
    
    // Check for variable assignments (identifier followed by assignment)
    if (token.type == TokenType::IDENTIFIER && peekToken().type == TokenType::ASSIGN) {
        return parseVariableAssignment();
    }
    
    // Check for this.property = value assignments
    if (token.type == TokenType::THIS) {
        // Look ahead to see if this is this.property = value
        size_t saved_pos = current_token_;
        advance(); // consume 'this'
        if (currentToken().type == TokenType::DOT) {
            advance(); // consume '.'
            if (currentToken().type == TokenType::IDENTIFIER) {
                advance(); // consume property name
                if (currentToken().type == TokenType::ASSIGN) {
                    // This is a this.property = value assignment
                    current_token_ = saved_pos; // restore position
                    return parseThisAssignment();
                }
            }
        }
        current_token_ = saved_pos; // restore position if not assignment
    }
    
    // Otherwise parse as expression statement
    return parseExpression();
}

ASTNodePtr Parser::parseVariableDeclaration() {
    Token variable_name_token = consume(TokenType::IDENTIFIER, "Expected variable name");
    std::string variable_name = variable_name_token.value;
    
    consume(TokenType::COLON, "Expected ':' after variable name");
    
    std::string type_name = parseTypeName();
    
    consume(TokenType::ASSIGN, "Expected '=' after type");
    
    // Special handling for Set literals with parentheses syntax
    ASTNodePtr initializer;
    if (type_name.find("Set<") == 0 && currentToken().type == TokenType::LPAREN) {
        initializer = parseSetLiteral();
    } else {
        initializer = parseExpression();
    }
    
    auto variable_node = std::make_unique<VariableDeclarationNode>(variable_name, type_name, std::move(initializer));
    SourceLocation location(filename_, variable_name_token.line, variable_name_token.column);
    variable_node->setSourceLocation(location);
    return variable_node;
}

ASTNodePtr Parser::parseVariableAssignment() {
    Token variable_name_token = consume(TokenType::IDENTIFIER, "Expected variable name");
    std::string variable_name = variable_name_token.value;
    
    consume(TokenType::ASSIGN, "Expected '=' after variable name");
    
    ASTNodePtr value_expr = parseExpression();
    
    auto assignment_node = std::make_unique<VariableAssignmentNode>(variable_name, std::move(value_expr));
    SourceLocation location(filename_, variable_name_token.line, variable_name_token.column);
    assignment_node->setSourceLocation(location);
    return assignment_node;
}

ASTNodePtr Parser::parsePropertyDeclaration() {
    Token property_token = consume(TokenType::PROPERTY, "Expected 'property'");
    
    Token property_name_token = consume(TokenType::IDENTIFIER, "Expected property name");
    std::string property_name = property_name_token.value;
    
    consume(TokenType::COLON, "Expected ':' after property name");
    
    Token type_name_token = consume(TokenType::IDENTIFIER, "Expected type name");
    std::string type_name = type_name_token.value;
    
    auto property_decl = std::make_unique<PropertyDeclarationNode>(property_name, type_name);
    SourceLocation location(filename_, property_token.line, property_token.column);
    property_decl->setSourceLocation(location);
    return property_decl;
}

ASTNodePtr Parser::parseConstDeclaration() {
    Token const_token = consume(TokenType::CONST, "Expected 'const'");
    
    Token const_name_token = consume(TokenType::IDENTIFIER, "Expected constant name");
    std::string const_name = const_name_token.value;
    
    consume(TokenType::COLON, "Expected ':' after constant name");
    
    Token type_name_token = consume(TokenType::IDENTIFIER, "Expected type name");
    std::string type_name = type_name_token.value;
    
    consume(TokenType::ASSIGN, "Expected '=' after type");
    
    ASTNodePtr initializer = parseExpression();
    
    auto const_decl = std::make_unique<ConstDeclarationNode>(const_name, type_name, std::move(initializer));
    SourceLocation location(filename_, const_token.line, const_token.column);
    const_decl->setSourceLocation(location);
    return const_decl;
}

ASTNodePtr Parser::parseIfStatement() {
    Token if_token = consume(TokenType::IF, "Expected 'if'");
    
    consume(TokenType::LPAREN, "Expected '(' after 'if'");
    
    ASTNodePtr condition = parseExpression();
    
    consume(TokenType::RPAREN, "Expected ')' after if condition");
    
    Token then_lbrace_token = consume(TokenType::LBRACE, "Expected '{' after if condition");
    
    // Parse then branch as a block of statements
    std::vector<ASTNodePtr> then_statements;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        then_statements.push_back(parseStatement());
    }
    
    auto then_branch = std::make_unique<BlockNode>(std::move(then_statements));
    SourceLocation then_block_location(filename_, then_lbrace_token.line, then_lbrace_token.column);
    then_branch->setSourceLocation(then_block_location);
    
    consume(TokenType::RBRACE, "Expected '}' to close if body");
    
    // Check for optional else clause
    ASTNodePtr else_branch = nullptr;
    if (currentToken().type == TokenType::ELSE) {
        advance(); // consume 'else'
        
        // Check for 'else if' - if next token is 'if', parse another if statement
        if (currentToken().type == TokenType::IF) {
            // This is an 'else if', parse it as another if statement
            else_branch = parseIfStatement();
        } else {
            // Regular 'else' clause
            Token else_lbrace_token = consume(TokenType::LBRACE, "Expected '{' after 'else'");
            
            // Parse else branch as a block of statements
            std::vector<ASTNodePtr> else_statements;
            
            while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
                // Skip newlines
                if (match(TokenType::NEWLINE)) {
                    continue;
                }
                
                else_statements.push_back(parseStatement());
            }
            
            else_branch = std::make_unique<BlockNode>(std::move(else_statements));
            SourceLocation else_block_location(filename_, else_lbrace_token.line, else_lbrace_token.column);
            else_branch->setSourceLocation(else_block_location);
            
            consume(TokenType::RBRACE, "Expected '}' to close else body");
        }
    }
    
    auto if_statement = std::make_unique<IfStatementNode>(std::move(condition), std::move(then_branch), std::move(else_branch));
    SourceLocation location(filename_, if_token.line, if_token.column);
    if_statement->setSourceLocation(location);
    return if_statement;
}

ASTNodePtr Parser::parseEnumDeclaration() {
    Token enum_token = consume(TokenType::ENUM, "Expected 'enum'");
    
    Token enum_name_token = consume(TokenType::IDENTIFIER, "Expected enum name");
    std::string enum_name = enum_name_token.value;
    
    consume(TokenType::LBRACE, "Expected '{' after enum name");
    
    std::vector<EnumMember> members;
    int current_value = 0;  // Default starting value
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        Token member_name_token = consume(TokenType::IDENTIFIER, "Expected enum member name");
        std::string member_name = member_name_token.value;
        
        int member_value = current_value;
        
        // Check if explicit value is provided
        if (currentToken().type == TokenType::ASSIGN) {
            advance(); // consume '='
            
            Token value_token = consume(TokenType::NUMBER, "Expected integer value after '='");
            member_value = std::stoll(value_token.value);
            current_value = member_value;
        }
        
        members.emplace_back(member_name, member_value);
        current_value++; // Increment for next member
        
        // Handle comma separator
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
            // After consuming comma, continue to next iteration
        } else if (currentToken().type == TokenType::RBRACE) {
            // End of enum members, break out of loop
            break;
        }
        // If neither comma nor closing brace, continue (allows for missing commas)
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close enum declaration");
    
    auto enum_decl = std::make_unique<EnumDeclarationNode>(enum_name, std::move(members));
    SourceLocation location(filename_, enum_token.line, enum_token.column);
    enum_decl->setSourceLocation(location);
    return enum_decl;
}

ASTNodePtr Parser::parseRecordDeclaration() {
    Token record_token = consume(TokenType::RECORD, "Expected 'record'");
    
    Token record_name_token = consume(TokenType::IDENTIFIER, "Expected record name");
    std::string record_name = record_name_token.value;
    
    consume(TokenType::LBRACE, "Expected '{' after record name");
    
    std::vector<RecordField> fields;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        Token field_name_token = consume(TokenType::IDENTIFIER, "Expected field name");
        std::string field_name = field_name_token.value;
        
        consume(TokenType::COLON, "Expected ':' after field name");
        
        Token field_type_token = consume(TokenType::IDENTIFIER, "Expected field type");
        std::string field_type = field_type_token.value;
        
        fields.emplace_back(field_name, field_type);
        
        // Handle comma separator
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
            // After consuming comma, continue to next iteration
        } else if (currentToken().type == TokenType::RBRACE) {
            // End of record fields, break out of loop
            break;
        }
        // If neither comma nor closing brace, continue (allows for missing commas)
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close record declaration");
    
    auto record_decl = std::make_unique<RecordDeclarationNode>(record_name, std::move(fields));
    SourceLocation location(filename_, record_token.line, record_token.column);
    record_decl->setSourceLocation(location);
    return record_decl;
}

ASTNodePtr Parser::parseProtocolDeclaration() {
    Token protocol_token = consume(TokenType::PROTOCOL, "Expected 'Protocol'");
    SourceLocation location(filename_, protocol_token.line, protocol_token.column);
    
    Token protocol_name_token = consume(TokenType::IDENTIFIER, "Expected protocol name");
    std::string protocol_name = protocol_name_token.value;
    
    consume(TokenType::LBRACE, "Expected '{' after protocol name");
    
    std::vector<ProtocolMethodSignature> method_signatures;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        // Parse method signature: method name(param: Type, ...): ReturnType
        consume(TokenType::METHOD, "Expected 'method' in protocol");
        
        Token method_name_token = consume(TokenType::IDENTIFIER, "Expected method name");
        std::string method_name = method_name_token.value;
        
        consume(TokenType::LPAREN, "Expected '(' after method name");
        
        // Parse parameters
        std::vector<Parameter> parameters;
        while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
            // Skip newlines
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
            
            if (currentToken().type == TokenType::RPAREN) {
                break; // End of parameters
            }
            
            Token param_name_token = consume(TokenType::IDENTIFIER, "Expected parameter name");
            std::string param_name = param_name_token.value;
            
            consume(TokenType::COLON, "Expected ':' after parameter name");
            
            Token param_type_token = consume(TokenType::IDENTIFIER, "Expected parameter type");
            std::string param_type = param_type_token.value;
            
            parameters.emplace_back(param_name, param_type);
            
            if (currentToken().type == TokenType::COMMA) {
                advance(); // consume comma
                
                // Skip newlines after comma
                while (match(TokenType::NEWLINE)) {
                    // Skip newlines
                }
            } else if (currentToken().type == TokenType::NEWLINE) {
                // Allow newlines without comma
                while (match(TokenType::NEWLINE)) {
                    // Skip newlines
                }
            } else if (currentToken().type != TokenType::RPAREN) {
                throw SyntaxError("Expected ',' or ')' in parameter list at line " + std::to_string(currentToken().line));
            }
        }
        
        consume(TokenType::RPAREN, "Expected ')' after parameters");
        consume(TokenType::COLON, "Expected ':' after parameter list");
        
        Token return_type_token = consume(TokenType::IDENTIFIER, "Expected return type");
        std::string return_type = return_type_token.value;
        
        // Protocol methods don't have bodies, just signatures
        method_signatures.emplace_back(method_name, std::move(parameters), return_type);
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close protocol declaration");
    
    auto protocol_node = std::make_unique<ProtocolDeclarationNode>(protocol_name, std::move(method_signatures));
    protocol_node->setSourceLocation(location);
    return protocol_node;
}

ASTNodePtr Parser::parseNamespaceDeclaration() {
    consume(TokenType::NAMESPACE, "Expected 'namespace'");
    
    // Parse namespace path (e.g., mylib.collections.utils)
    std::vector<std::string> namespace_path;
    
    Token first_part = consume(TokenType::IDENTIFIER, "Expected namespace name");
    namespace_path.push_back(first_part.value);
    
    // Parse additional parts separated by dots
    while (currentToken().type == TokenType::DOT) {
        advance(); // consume dot
        Token part = consume(TokenType::IDENTIFIER, "Expected namespace part after '.'");
        namespace_path.push_back(part.value);
    }
    
    consume(TokenType::LBRACE, "Expected '{' after namespace declaration");
    
    // Parse namespace body (objects, enums, records, protocols)
    std::vector<ASTNodePtr> body;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        if (currentToken().type == TokenType::OBJECT) {
            body.push_back(parseObjectDeclaration());
        } else if (currentToken().type == TokenType::ENUM) {
            body.push_back(parseEnumDeclaration());
        } else if (currentToken().type == TokenType::RECORD) {
            body.push_back(parseRecordDeclaration());
        } else if (currentToken().type == TokenType::PROTOCOL) {
            body.push_back(parseProtocolDeclaration());
        } else {
            throw SyntaxError("Only object, enum, record, and protocol declarations are allowed inside namespaces. Found '" +
                            currentToken().value + "' at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close namespace declaration");
    
    return std::make_unique<NamespaceNode>(std::move(namespace_path), std::move(body));
}

ASTNodePtr Parser::parseListLiteral() {
    Token lbracket_token = consume(TokenType::LBRACKET, "Expected '['");
    
    std::vector<ASTNodePtr> elements;
    
    // Handle empty list []
    if (currentToken().type == TokenType::RBRACKET) {
        advance(); // consume ']'
        auto list_literal = std::make_unique<ListLiteralNode>(std::move(elements));
        SourceLocation location(filename_, lbracket_token.line, lbracket_token.column);
        list_literal->setSourceLocation(location);
        return list_literal;
    }
    
    // Parse elements
    while (currentToken().type != TokenType::RBRACKET && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        if (currentToken().type == TokenType::RBRACKET) {
            break; // End of list
        }
        
        elements.push_back(parseExpression());
        
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
            
            // Skip newlines after comma
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
        } else if (currentToken().type == TokenType::NEWLINE) {
            // Allow newlines without comma
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
        } else if (currentToken().type != TokenType::RBRACKET) {
            throw SyntaxError("Expected ',' or ']' in list literal at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RBRACKET, "Expected ']' to close list literal");
    
    auto list_literal = std::make_unique<ListLiteralNode>(std::move(elements));
    SourceLocation location(filename_, lbracket_token.line, lbracket_token.column);
    list_literal->setSourceLocation(location);
    return list_literal;
}

ASTNodePtr Parser::parseMapLiteral() {
    Token lbrace_token = consume(TokenType::LBRACE, "Expected '{'");
    
    std::vector<std::pair<ASTNodePtr, ASTNodePtr>> entries;
    
    // Handle empty map {}
    if (currentToken().type == TokenType::RBRACE) {
        advance(); // consume '}'
        auto map_literal = std::make_unique<MapLiteralNode>(std::move(entries));
        SourceLocation location(filename_, lbrace_token.line, lbrace_token.column);
        map_literal->setSourceLocation(location);
        return map_literal;
    }
    
    // Parse key-value pairs
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        if (currentToken().type == TokenType::RBRACE) {
            break; // End of map
        }
        
        // Parse key
        ASTNodePtr key = parseExpression();
        
        // Skip newlines before colon
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        // Expect colon
        consume(TokenType::COLON, "Expected ':' after map key");
        
        // Skip newlines after colon
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        // Parse value
        ASTNodePtr value = parseExpression();
        
        entries.emplace_back(std::move(key), std::move(value));
        
        // Skip newlines
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
            
            // Skip newlines after comma
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
        } else if (currentToken().type == TokenType::NEWLINE) {
            // Allow newlines without comma
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
        } else if (currentToken().type != TokenType::RBRACE) {
            throw SyntaxError("Expected ',' or '}' in map literal at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close map literal");
    
    auto map_literal = std::make_unique<MapLiteralNode>(std::move(entries));
    SourceLocation location(filename_, lbrace_token.line, lbrace_token.column);
    map_literal->setSourceLocation(location);
    return map_literal;
}

ASTNodePtr Parser::parseSetLiteral() {
    Token lparen_token = consume(TokenType::LPAREN, "Expected '(' to start set literal");
    
    std::vector<ASTNodePtr> elements;
    
    // Skip newlines
    while (match(TokenType::NEWLINE)) {
        // Skip newlines
    }
    
    // Handle empty set
    if (currentToken().type == TokenType::RPAREN) {
        advance(); // consume ')'
        auto set_literal = std::make_unique<SetLiteralNode>(std::move(elements));
        SourceLocation location(filename_, lparen_token.line, lparen_token.column);
        set_literal->setSourceLocation(location);
        return set_literal;
    }
    
    // Parse elements
    while (currentToken().type != TokenType::RPAREN && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        // Parse element
        ASTNodePtr element = parseExpression();
        elements.push_back(std::move(element));
        
        // Skip newlines
        while (match(TokenType::NEWLINE)) {
            // Skip newlines
        }
        
        if (currentToken().type == TokenType::COMMA) {
            advance(); // consume comma
            
            // Skip newlines after comma
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
        } else if (currentToken().type == TokenType::NEWLINE) {
            // Allow newlines without comma
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
        } else if (currentToken().type != TokenType::RPAREN) {
            throw SyntaxError("Expected ',' or ')' in set literal at line " + std::to_string(currentToken().line));
        }
    }
    
    consume(TokenType::RPAREN, "Expected ')' to close set literal");
    
    auto set_literal = std::make_unique<SetLiteralNode>(std::move(elements));
    SourceLocation location(filename_, lparen_token.line, lparen_token.column);
    set_literal->setSourceLocation(location);
    return set_literal;
}

ASTNodePtr Parser::parseWhileStatement() {
    Token while_token = consume(TokenType::WHILE, "Expected 'while'");
    
    consume(TokenType::LPAREN, "Expected '(' after 'while'");
    
    ASTNodePtr condition = parseExpression();
    
    consume(TokenType::RPAREN, "Expected ')' after while condition");
    
    Token while_lbrace_token = consume(TokenType::LBRACE, "Expected '{' after while condition");
    
    // Parse body as a block of statements
    std::vector<ASTNodePtr> body_statements;
    
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        body_statements.push_back(parseStatement());
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close while body");
    
    // Create a block node for the body
    auto body = std::make_unique<BlockNode>(std::move(body_statements));
    SourceLocation while_body_location(filename_, while_lbrace_token.line, while_lbrace_token.column);
    body->setSourceLocation(while_body_location);
    
    auto while_statement = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));
    SourceLocation location(filename_, while_token.line, while_token.column);
    while_statement->setSourceLocation(location);
    return while_statement;
}

ASTNodePtr Parser::parseBreakStatement() {
    Token break_token = consume(TokenType::BREAK, "Expected 'break'");
    
    auto break_node = std::make_unique<BreakNode>();
    SourceLocation break_location(filename_, break_token.line, break_token.column);
    break_node->setSourceLocation(break_location);
    
    return break_node;
}

std::string Parser::parseTypeName() {
    std::string type_name;
    
    // Accept both identifiers and specific type keywords
    if (currentToken().type == TokenType::IDENTIFIER) {
        Token type_name_token = consume(TokenType::IDENTIFIER, "Expected type name");
        type_name = type_name_token.value;
    } else if (currentToken().type == TokenType::RESULT) {
        Token type_name_token = consume(TokenType::RESULT, "Expected Result type");
        type_name = type_name_token.value;
    } else if (currentToken().type == TokenType::ERROR) {
        Token type_name_token = consume(TokenType::ERROR, "Expected Error type");
        type_name = type_name_token.value;
    } else {
        throw SyntaxError("Expected type name at line " + std::to_string(currentToken().line));
    }
    
    // Handle generic types like List<Int> and Map<Key, Value>
    if (currentToken().type == TokenType::LESS_THAN) {
        advance(); // consume '<'
        
        // Parse first type parameter
        std::string first_type;
        if (currentToken().type == TokenType::IDENTIFIER) {
            Token first_type_token = consume(TokenType::IDENTIFIER, "Expected generic type parameter");
            first_type = first_type_token.value;
        } else if (currentToken().type == TokenType::ERROR) {
            Token first_type_token = consume(TokenType::ERROR, "Expected Error type parameter");
            first_type = first_type_token.value;
        } else if (currentToken().type == TokenType::RESULT) {
            Token first_type_token = consume(TokenType::RESULT, "Expected Result type parameter");
            first_type = first_type_token.value;
        } else {
            throw SyntaxError("Expected generic type parameter at line " + std::to_string(currentToken().line));
        }
        
        // Check if this is a two-parameter generic type (Map, Result) or single-parameter (List, Set)
        if ((type_name == "Map" || type_name == "Result") && currentToken().type == TokenType::COMMA) {
            advance(); // consume ','
            
            // Skip whitespace
            while (match(TokenType::NEWLINE)) {
                // Skip newlines
            }
            
            // Parse second type parameter
            std::string second_type;
            if (currentToken().type == TokenType::IDENTIFIER) {
                Token second_type_token = consume(TokenType::IDENTIFIER, "Expected second generic type parameter");
                second_type = second_type_token.value;
            } else if (currentToken().type == TokenType::ERROR) {
                Token second_type_token = consume(TokenType::ERROR, "Expected Error type parameter");
                second_type = second_type_token.value;
            } else if (currentToken().type == TokenType::RESULT) {
                Token second_type_token = consume(TokenType::RESULT, "Expected Result type parameter");
                second_type = second_type_token.value;
            } else {
                throw SyntaxError("Expected second generic type parameter at line " + std::to_string(currentToken().line));
            }
            
            consume(TokenType::GREATER_THAN, "Expected '>' after generic type parameters");
            
            // Combine into full type name
            type_name = type_name + "<" + first_type + ", " + second_type + ">";
        } else {
            // Single type parameter (List<T>)
            consume(TokenType::GREATER_THAN, "Expected '>' after generic type parameter");
            
            // Combine into full type name
            type_name = type_name + "<" + first_type + ">";
        }
    }
    
    return type_name;
}

ASTNodePtr Parser::parseThrowStatement() {
    Token throw_token = consume(TokenType::THROW, "Expected 'throw'");
    
    consume(TokenType::LPAREN, "Expected '(' after 'throw'");
    
    // Parse the expression to throw
    ASTNodePtr expression = parseExpression();
    
    consume(TokenType::RPAREN, "Expected ')' after throw expression");
    
    // Create source location from the throw token
    SourceLocation location(filename_, throw_token.line, throw_token.column);
    
    return std::make_unique<ThrowNode>(std::move(expression), location);
}

ASTNodePtr Parser::parseTryCatchFinallyStatement() {
    Token try_token = consume(TokenType::TRY, "Expected 'try'");
    
    // Parse try block
    consume(TokenType::LBRACE, "Expected '{' after 'try'");
    
    std::vector<ASTNodePtr> try_statements;
    while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        try_statements.push_back(parseStatement());
    }
    
    consume(TokenType::RBRACE, "Expected '}' to close try block");
    
    ASTNodePtr try_block = std::make_unique<BlockNode>(std::move(try_statements));
    
    // Parse optional catch block
    ASTNodePtr catch_block = nullptr;
    std::string catch_variable;
    
    if (currentToken().type == TokenType::CATCH) {
        advance(); // consume 'catch'
        
        consume(TokenType::LPAREN, "Expected '(' after 'catch'");
        
        // Parse catch variable name
        Token catch_var_token = consume(TokenType::IDENTIFIER, "Expected catch variable name");
        catch_variable = catch_var_token.value;
        
        consume(TokenType::RPAREN, "Expected ')' after catch variable");
        
        consume(TokenType::LBRACE, "Expected '{' after catch clause");
        
        std::vector<ASTNodePtr> catch_statements;
        while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
            if (match(TokenType::NEWLINE)) {
                continue;
            }
            catch_statements.push_back(parseStatement());
        }
        
        consume(TokenType::RBRACE, "Expected '}' to close catch block");
        
        catch_block = std::make_unique<BlockNode>(std::move(catch_statements));
    }
    
    // Parse optional finally block
    ASTNodePtr finally_block = nullptr;
    
    if (currentToken().type == TokenType::FINALLY) {
        advance(); // consume 'finally'
        
        consume(TokenType::LBRACE, "Expected '{' after 'finally'");
        
        std::vector<ASTNodePtr> finally_statements;
        while (currentToken().type != TokenType::RBRACE && currentToken().type != TokenType::EOF_TOKEN) {
            if (match(TokenType::NEWLINE)) {
                continue;
            }
            finally_statements.push_back(parseStatement());
        }
        
        consume(TokenType::RBRACE, "Expected '}' to close finally block");
        
        finally_block = std::make_unique<BlockNode>(std::move(finally_statements));
    }
    
    // At least one of catch or finally must be present
    if (!catch_block && !finally_block) {
        throw SyntaxError("Expected 'catch' or 'finally' after 'try' block");
    }
    
    // Create source location from the try token
    SourceLocation location(filename_, try_token.line, try_token.column);
    
    return std::make_unique<TryCatchFinallyNode>(
        std::move(try_block), 
        std::move(catch_block), 
        std::move(finally_block),
        catch_variable,
        location
    );
}

ASTNodePtr Parser::parseResultStaticCall() {
    Token result_token = consume(TokenType::RESULT, "Expected 'Result'");
    consume(TokenType::DOT, "Expected '.' after 'Result'");
    
    Token method_token = consume(TokenType::IDENTIFIER, "Expected 'success' or 'error' after 'Result.'");
    std::string method_name = method_token.value;
    
    if (method_name != "success" && method_name != "error") {
        throw SyntaxError("Expected 'success' or 'error' after 'Result.' at line " + std::to_string(method_token.line));
    }
    
    consume(TokenType::LPAREN, "Expected '(' after 'Result." + method_name + "'");
    
    // Parse argument (should be exactly one)
    std::vector<ASTNodePtr> arguments;
    if (currentToken().type != TokenType::RPAREN) {
        arguments.push_back(parseExpression());
    }
    
    consume(TokenType::RPAREN, "Expected ')' after Result." + method_name + " argument");
    
    // Create a special function call node for Result static methods
    auto function_call = std::make_unique<FunctionCallNode>("Result." + method_name, std::move(arguments));
    SourceLocation location(filename_, result_token.line, result_token.column);
    function_call->setSourceLocation(location);
    return function_call;
}

std::string Parser::reconstructQualifiedName(ASTNode* node) {
    if (auto identifier = dynamic_cast<IdentifierNode*>(node)) {
        return identifier->getName();
    } else if (auto member_access = dynamic_cast<MemberAccessNode*>(node)) {
        return reconstructQualifiedName(member_access->getObjectExpression().get()) + "." + member_access->getMemberName();
    } else if (auto method_call = dynamic_cast<MethodCallNode*>(node)) {
        return reconstructQualifiedName(method_call->getObject().get()) + "." + method_call->getMethodName();
    } else if (auto qualified = dynamic_cast<QualifiedIdentifierNode*>(node)) {
        const auto& parts = qualified->getParts();
        std::string result = parts[0];
        for (size_t i = 1; i < parts.size(); ++i) {
            result += "." + parts[i];
        }
        return result;
    }
    return "unknown";
}

} // namespace o2l