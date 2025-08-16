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
#include <string>
#include <vector>

#include "../Runtime/JsonLibrary.hpp"
#include "Node.hpp"

namespace o2l {

/**
 * JSON Serializer for AST Nodes
 * Converts AST nodes to JSON format for LSP server integration
 */
class JsonSerializer {
   public:
    // Serialize a list of AST nodes to JSON string
    static std::string serializeAST(const std::vector<ASTNodePtr>& nodes,
                                    bool pretty_print = false);

    // Serialize a single AST node to JSON string
    static std::string serializeNode(const ASTNodePtr& node, bool pretty_print = false);

    // Serialize parsing errors to JSON format
    static std::string serializeError(const std::string& error_message, const std::string& filename,
                                      int line = -1, int column = -1);

    // Serialize diagnostic information (warnings, hints, etc.)
    static std::string serializeDiagnostics(const std::vector<std::string>& diagnostics);

   private:
    // Helper method to get node type name as string
    static std::string getNodeTypeName(const ASTNode* node);

    // Helper method to serialize source location as JSON string
    static std::string sourceLocationToJsonString(const SourceLocation& location);

    // Helper method to escape JSON strings
    static std::string escapeJsonString(const std::string& str);

    // Recursive node serialization with full nested structure
    static std::string serializeNodeRecursive(const ASTNodePtr& node, int indent_level,
                                              bool pretty_print);

    // Helper methods for specific node types
    static std::string serializeObjectNode(const ASTNodePtr& node, int indent_level,
                                           bool pretty_print);
    static std::string serializeMethodNode(const ASTNodePtr& node, int indent_level,
                                           bool pretty_print);
    static std::string serializeImportNode(const ASTNodePtr& node, int indent_level,
                                           bool pretty_print);
    static std::string serializeProtocolNode(const ASTNodePtr& node, int indent_level,
                                             bool pretty_print);
    static std::string serializePropertyNode(const ASTNodePtr& node, int indent_level,
                                             bool pretty_print);
    static std::string serializeConstructorNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print);
    static std::string serializeBlockNode(const ASTNodePtr& node, int indent_level,
                                          bool pretty_print);
    static std::string serializeMethodCallNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print);
    static std::string serializeReturnNode(const ASTNodePtr& node, int indent_level,
                                           bool pretty_print);
    static std::string serializeIdentifierNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print);
    static std::string serializeLiteralNode(const ASTNodePtr& node, int indent_level,
                                            bool pretty_print);
    static std::string serializeVariableDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                        bool pretty_print);
    static std::string serializeVariableAssignmentNode(const ASTNodePtr& node, int indent_level,
                                                       bool pretty_print);
    static std::string serializeMemberAccessNode(const ASTNodePtr& node, int indent_level,
                                                 bool pretty_print);
    static std::string serializePropertyAssignmentNode(const ASTNodePtr& node, int indent_level,
                                                       bool pretty_print);
    static std::string serializeWhileStatementNode(const ASTNodePtr& node, int indent_level,
                                                   bool pretty_print);
    static std::string serializeThisNode(const ASTNodePtr& node, int indent_level,
                                         bool pretty_print);
    static std::string serializeNewExpressionNode(const ASTNodePtr& node, int indent_level,
                                                  bool pretty_print);
    static std::string serializeListLiteralNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print);
    static std::string serializeMapLiteralNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print);
    static std::string serializeBinaryOpNode(const ASTNodePtr& node, int indent_level,
                                             bool pretty_print);
    static std::string serializeLogicalNode(const ASTNodePtr& node, int indent_level,
                                            bool pretty_print);
    static std::string serializeUnaryNode(const ASTNodePtr& node, int indent_level,
                                          bool pretty_print);
    static std::string serializeComparisonNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print);
    static std::string serializeIfStatementNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print);
    static std::string serializeConstDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                     bool pretty_print);
    static std::string serializeSetLiteralNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print);
    static std::string serializeRecordDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                      bool pretty_print);
    static std::string serializeRecordFieldAccessNode(const ASTNodePtr& node, int indent_level,
                                                      bool pretty_print);
    static std::string serializeRecordInstantiationNode(const ASTNodePtr& node, int indent_level,
                                                        bool pretty_print);
    static std::string serializePropertyAccessNode(const ASTNodePtr& node, int indent_level,
                                                   bool pretty_print);
    static std::string serializeQualifiedIdentifierNode(const ASTNodePtr& node, int indent_level,
                                                        bool pretty_print);
    static std::string serializeFunctionCallNode(const ASTNodePtr& node, int indent_level,
                                                 bool pretty_print);
    static std::string serializeEnumDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print);
    static std::string serializeEnumAccessNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print);
    static std::string serializeNamespaceNode(const ASTNodePtr& node, int indent_level,
                                              bool pretty_print);
    static std::string serializeThrowNode(const ASTNodePtr& node, int indent_level,
                                          bool pretty_print);
    static std::string serializeTryCatchFinallyNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print);

    // Helper method for indentation
    static std::string getIndent(int level);
};

}  // namespace o2l