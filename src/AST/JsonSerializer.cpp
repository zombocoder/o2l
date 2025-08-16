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

#include "JsonSerializer.hpp"

#include <iomanip>
#include <sstream>
#include <typeinfo>

#include "../Runtime/JsonLibrary.hpp"
#include "BinaryOpNode.hpp"
#include "BlockNode.hpp"
#include "ComparisonNode.hpp"
#include "ConstDeclarationNode.hpp"
#include "ConstructorDeclarationNode.hpp"
#include "EnumAccessNode.hpp"
#include "EnumDeclarationNode.hpp"
#include "FunctionCallNode.hpp"
#include "IdentifierNode.hpp"
#include "IfStatementNode.hpp"
#include "ImportNode.hpp"
#include "ListLiteralNode.hpp"
#include "LiteralNode.hpp"
#include "LogicalNode.hpp"
#include "MapLiteralNode.hpp"
#include "MemberAccessNode.hpp"
#include "MethodCallNode.hpp"
#include "MethodDeclarationNode.hpp"
#include "NamespaceNode.hpp"
#include "NewExpressionNode.hpp"
#include "ObjectNode.hpp"
#include "PropertyAccessNode.hpp"
#include "PropertyAssignmentNode.hpp"
#include "PropertyDeclarationNode.hpp"
#include "ProtocolDeclarationNode.hpp"
#include "QualifiedIdentifierNode.hpp"
#include "RecordDeclarationNode.hpp"
#include "RecordFieldAccessNode.hpp"
#include "RecordInstantiationNode.hpp"
#include "ReturnNode.hpp"
#include "SetLiteralNode.hpp"
#include "ThisNode.hpp"
#include "ThrowNode.hpp"
#include "TryCatchFinallyNode.hpp"
#include "UnaryNode.hpp"
#include "VariableAssignmentNode.hpp"
#include "VariableDeclarationNode.hpp"
#include "WhileStatementNode.hpp"

namespace o2l {

std::string JsonSerializer::serializeAST(const std::vector<ASTNodePtr>& nodes, bool pretty_print) {
    std::ostringstream oss;

    if (pretty_print) {
        oss << "{\n";
        oss << "  \"type\": \"ast\",\n";
        oss << "  \"version\": \"1.0\",\n";
        oss << "  \"nodes\": [\n";

        for (size_t i = 0; i < nodes.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(nodes[i], 2, pretty_print);
        }

        oss << "\n  ]\n";
        oss << "}\n";
    } else {
        oss << "{\"type\":\"ast\",\"version\":\"1.0\",\"nodes\":[";

        for (size_t i = 0; i < nodes.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(nodes[i], 0, pretty_print);
        }

        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeNode(const ASTNodePtr& node, bool pretty_print) {
    if (!node) {
        return pretty_print ? "{\n  \"node_type\": \"null\"\n}" : "{\"node_type\":\"null\"}";
    }

    std::ostringstream oss;
    if (pretty_print) {
        oss << "{\n";
        oss << "  \"node_type\": \"" << getNodeTypeName(node.get()) << "\",\n";
        oss << "  \"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << "\n";
        oss << "}";
    } else {
        oss << "{\"node_type\":\"" << getNodeTypeName(node.get()) << "\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation())
            << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeError(const std::string& error_message,
                                           const std::string& filename, int line, int column) {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"type\": \"error\",\n";
    oss << "  \"message\": \"" << escapeJsonString(error_message) << "\",\n";
    oss << "  \"filename\": \"" << escapeJsonString(filename) << "\"";

    if (line >= 0) {
        oss << ",\n  \"line\": " << line;
    }
    if (column >= 0) {
        oss << ",\n  \"column\": " << column;
    }

    oss << "\n}";
    return oss.str();
}

std::string JsonSerializer::serializeDiagnostics(const std::vector<std::string>& diagnostics) {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"type\": \"diagnostics\",\n";
    oss << "  \"diagnostics\": [\n";

    for (size_t i = 0; i < diagnostics.size(); ++i) {
        if (i > 0) oss << ",\n";
        oss << "    {\n";
        oss << "      \"message\": \"" << escapeJsonString(diagnostics[i]) << "\",\n";
        oss << "      \"severity\": \"info\"\n";
        oss << "    }";
    }

    oss << "\n  ]\n";
    oss << "}";
    return oss.str();
}

// Simplified implementation - removed complex JsonValue methods

std::string JsonSerializer::getNodeTypeName(const ASTNode* node) {
    if (!node) return "NullNode";

    // Get type name from typeid - this is a basic implementation
    std::string type_name = typeid(*node).name();

    // Clean up the type name (remove namespace prefixes, etc.)
    size_t last_colon = type_name.find_last_of(':');
    if (last_colon != std::string::npos && last_colon + 1 < type_name.length()) {
        type_name = type_name.substr(last_colon + 1);
    }

    // Remove compiler-specific prefixes (like "N3o2l" in GCC)
    size_t first_letter = 0;
    while (first_letter < type_name.length() && !std::isalpha(type_name[first_letter])) {
        first_letter++;
    }
    if (first_letter > 0) {
        type_name = type_name.substr(first_letter);
    }

    return type_name;
}

std::string JsonSerializer::sourceLocationToJsonString(const SourceLocation& location) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"line\":" << location.line_number;
    oss << ",\"column\":" << location.column_number;
    oss << ",\"filename\":\"" << escapeJsonString(location.filename) << "\"";
    oss << "}";
    return oss.str();
}

std::string JsonSerializer::escapeJsonString(const std::string& str) {
    std::ostringstream oss;
    for (char c : str) {
        switch (c) {
            case '"':
                oss << "\\\"";
                break;
            case '\\':
                oss << "\\\\";
                break;
            case '\b':
                oss << "\\b";
                break;
            case '\f':
                oss << "\\f";
                break;
            case '\n':
                oss << "\\n";
                break;
            case '\r':
                oss << "\\r";
                break;
            case '\t':
                oss << "\\t";
                break;
            default:
                if (c < 0x20) {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    oss << c;
                }
        }
    }
    return oss.str();
}

std::string JsonSerializer::getIndent(int level) {
    return std::string(level * 2, ' ');
}

std::string JsonSerializer::serializeNodeRecursive(const ASTNodePtr& node, int indent_level,
                                                   bool pretty_print) {
    if (!node) {
        return pretty_print ? "null" : "null";
    }

    // Determine the node type and delegate to specific serializers
    if (auto object_node = dynamic_cast<ObjectNode*>(node.get())) {
        return serializeObjectNode(node, indent_level, pretty_print);
    } else if (auto method_node = dynamic_cast<MethodDeclarationNode*>(node.get())) {
        return serializeMethodNode(node, indent_level, pretty_print);
    } else if (auto import_node = dynamic_cast<ImportNode*>(node.get())) {
        return serializeImportNode(node, indent_level, pretty_print);
    } else if (auto protocol_node = dynamic_cast<ProtocolDeclarationNode*>(node.get())) {
        return serializeProtocolNode(node, indent_level, pretty_print);
    } else if (auto property_node = dynamic_cast<PropertyDeclarationNode*>(node.get())) {
        return serializePropertyNode(node, indent_level, pretty_print);
    } else if (auto constructor_node = dynamic_cast<ConstructorDeclarationNode*>(node.get())) {
        return serializeConstructorNode(node, indent_level, pretty_print);
    } else if (auto block_node = dynamic_cast<BlockNode*>(node.get())) {
        return serializeBlockNode(node, indent_level, pretty_print);
    } else if (auto method_call_node = dynamic_cast<MethodCallNode*>(node.get())) {
        return serializeMethodCallNode(node, indent_level, pretty_print);
    } else if (auto return_node = dynamic_cast<ReturnNode*>(node.get())) {
        return serializeReturnNode(node, indent_level, pretty_print);
    } else if (auto identifier_node = dynamic_cast<IdentifierNode*>(node.get())) {
        return serializeIdentifierNode(node, indent_level, pretty_print);
    } else if (auto literal_node = dynamic_cast<LiteralNode*>(node.get())) {
        return serializeLiteralNode(node, indent_level, pretty_print);
    } else if (auto variable_decl_node = dynamic_cast<VariableDeclarationNode*>(node.get())) {
        return serializeVariableDeclarationNode(node, indent_level, pretty_print);
    } else if (auto variable_assign_node = dynamic_cast<VariableAssignmentNode*>(node.get())) {
        return serializeVariableAssignmentNode(node, indent_level, pretty_print);
    } else if (auto member_access_node = dynamic_cast<MemberAccessNode*>(node.get())) {
        return serializeMemberAccessNode(node, indent_level, pretty_print);
    } else if (auto property_assign_node = dynamic_cast<PropertyAssignmentNode*>(node.get())) {
        return serializePropertyAssignmentNode(node, indent_level, pretty_print);
    } else if (auto while_node = dynamic_cast<WhileStatementNode*>(node.get())) {
        return serializeWhileStatementNode(node, indent_level, pretty_print);
    } else if (auto this_node = dynamic_cast<ThisNode*>(node.get())) {
        return serializeThisNode(node, indent_level, pretty_print);
    } else if (auto new_expr_node = dynamic_cast<NewExpressionNode*>(node.get())) {
        return serializeNewExpressionNode(node, indent_level, pretty_print);
    } else if (auto list_literal_node = dynamic_cast<ListLiteralNode*>(node.get())) {
        return serializeListLiteralNode(node, indent_level, pretty_print);
    } else if (auto map_literal_node = dynamic_cast<MapLiteralNode*>(node.get())) {
        return serializeMapLiteralNode(node, indent_level, pretty_print);
    } else if (auto binary_op_node = dynamic_cast<BinaryOpNode*>(node.get())) {
        return serializeBinaryOpNode(node, indent_level, pretty_print);
    } else if (auto logical_node = dynamic_cast<LogicalNode*>(node.get())) {
        return serializeLogicalNode(node, indent_level, pretty_print);
    } else if (auto unary_node = dynamic_cast<UnaryNode*>(node.get())) {
        return serializeUnaryNode(node, indent_level, pretty_print);
    } else if (auto comparison_node = dynamic_cast<ComparisonNode*>(node.get())) {
        return serializeComparisonNode(node, indent_level, pretty_print);
    } else if (auto if_statement_node = dynamic_cast<IfStatementNode*>(node.get())) {
        return serializeIfStatementNode(node, indent_level, pretty_print);
    } else if (auto const_decl_node = dynamic_cast<ConstDeclarationNode*>(node.get())) {
        return serializeConstDeclarationNode(node, indent_level, pretty_print);
    } else if (auto set_literal_node = dynamic_cast<SetLiteralNode*>(node.get())) {
        return serializeSetLiteralNode(node, indent_level, pretty_print);
    } else if (auto record_decl_node = dynamic_cast<RecordDeclarationNode*>(node.get())) {
        return serializeRecordDeclarationNode(node, indent_level, pretty_print);
    } else if (auto record_field_access_node = dynamic_cast<RecordFieldAccessNode*>(node.get())) {
        return serializeRecordFieldAccessNode(node, indent_level, pretty_print);
    } else if (auto record_instantiation_node =
                   dynamic_cast<RecordInstantiationNode*>(node.get())) {
        return serializeRecordInstantiationNode(node, indent_level, pretty_print);
    } else if (auto property_access_node = dynamic_cast<PropertyAccessNode*>(node.get())) {
        return serializePropertyAccessNode(node, indent_level, pretty_print);
    } else if (auto qualified_identifier_node =
                   dynamic_cast<QualifiedIdentifierNode*>(node.get())) {
        return serializeQualifiedIdentifierNode(node, indent_level, pretty_print);
    } else if (auto function_call_node = dynamic_cast<FunctionCallNode*>(node.get())) {
        return serializeFunctionCallNode(node, indent_level, pretty_print);
    } else if (auto enum_decl_node = dynamic_cast<EnumDeclarationNode*>(node.get())) {
        return serializeEnumDeclarationNode(node, indent_level, pretty_print);
    } else if (auto enum_access_node = dynamic_cast<EnumAccessNode*>(node.get())) {
        return serializeEnumAccessNode(node, indent_level, pretty_print);
    } else if (auto namespace_node = dynamic_cast<NamespaceNode*>(node.get())) {
        return serializeNamespaceNode(node, indent_level, pretty_print);
    } else if (auto throw_node = dynamic_cast<ThrowNode*>(node.get())) {
        return serializeThrowNode(node, indent_level, pretty_print);
    } else if (auto try_catch_finally_node = dynamic_cast<TryCatchFinallyNode*>(node.get())) {
        return serializeTryCatchFinallyNode(node, indent_level, pretty_print);
    }

    // For other node types, use basic serialization
    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"" << getNodeTypeName(node.get())
            << "\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << "\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"" << getNodeTypeName(node.get()) << "\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation())
            << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeImportNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print) {
    auto import_node = dynamic_cast<ImportNode*>(node.get());
    if (!import_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ImportNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";

        // Add import-specific information
        const auto& import_path = import_node->getImportPath();
        oss << getIndent(indent_level + 1) << "\"import_path\": {\n";

        // Package path
        oss << getIndent(indent_level + 2) << "\"package_path\": [";
        for (size_t i = 0; i < import_path.package_path.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "\"" << escapeJsonString(import_path.package_path[i]) << "\"";
        }
        oss << "],\n";

        // Object name
        oss << getIndent(indent_level + 2) << "\"object_name\": \""
            << escapeJsonString(import_path.object_name) << "\",\n";

        // Method name (if any)
        oss << getIndent(indent_level + 2) << "\"method_name\": \""
            << escapeJsonString(import_path.method_name) << "\",\n";

        // Is user import
        oss << getIndent(indent_level + 2)
            << "\"is_user_import\": " << (import_path.is_user_import ? "true" : "false") << "\n";

        oss << getIndent(indent_level + 1) << "}\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ImportNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"import_path\":{";
        oss << "\"package_path\":[";
        const auto& import_path = import_node->getImportPath();
        for (size_t i = 0; i < import_path.package_path.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "\"" << escapeJsonString(import_path.package_path[i]) << "\"";
        }
        oss << "],\"object_name\":\"" << escapeJsonString(import_path.object_name) << "\"";
        oss << ",\"method_name\":\"" << escapeJsonString(import_path.method_name) << "\"";
        oss << ",\"is_user_import\":" << (import_path.is_user_import ? "true" : "false") << "}}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeObjectNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print) {
    auto object_node = dynamic_cast<ObjectNode*>(node.get());
    if (!object_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ObjectNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"name\": \""
            << escapeJsonString(object_node->getName()) << "\",\n";

        // Protocol name (if any)
        if (!object_node->getProtocolName().empty()) {
            oss << getIndent(indent_level + 1) << "\"protocol_name\": \""
                << escapeJsonString(object_node->getProtocolName()) << "\",\n";
        }

        // Methods
        oss << getIndent(indent_level + 1) << "\"methods\": [\n";
        const auto& methods = object_node->getMethods();
        for (size_t i = 0; i < methods.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(methods[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "],\n";

        // Properties
        oss << getIndent(indent_level + 1) << "\"properties\": [\n";
        const auto& properties = object_node->getProperties();
        for (size_t i = 0; i < properties.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(properties[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "],\n";

        // Constructor (if present)
        const auto& constructor = object_node->getConstructor();
        if (constructor) {
            oss << getIndent(indent_level + 1) << "\"constructor\": \n";
            oss << serializeNodeRecursive(constructor, indent_level + 1, pretty_print) << "\n";
        } else {
            // Remove trailing comma and newline if no constructor
            std::string content = oss.str();
            if (content.length() >= 2 && content.substr(content.length() - 2) == ",\n") {
                oss.str(content.substr(0, content.length() - 2));
                oss << "\n";
            }
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ObjectNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"name\":\"" << escapeJsonString(object_node->getName()) << "\"";
        if (!object_node->getProtocolName().empty()) {
            oss << ",\"protocol_name\":\"" << escapeJsonString(object_node->getProtocolName())
                << "\"";
        }
        // Methods
        oss << ",\"methods\":[";
        const auto& methods = object_node->getMethods();
        for (size_t i = 0; i < methods.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(methods[i], 0, pretty_print);
        }
        oss << "]";

        // Properties
        oss << ",\"properties\":[";
        const auto& properties = object_node->getProperties();
        for (size_t i = 0; i < properties.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(properties[i], 0, pretty_print);
        }
        oss << "]";

        // Constructor (if present)
        const auto& constructor = object_node->getConstructor();
        if (constructor) {
            oss << ",\"constructor\":" << serializeNodeRecursive(constructor, 0, pretty_print);
        }

        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeMethodNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print) {
    auto method_node = dynamic_cast<MethodDeclarationNode*>(node.get());
    if (!method_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"MethodDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"name\": \""
            << escapeJsonString(method_node->getName()) << "\",\n";
        oss << getIndent(indent_level + 1) << "\"return_type\": \""
            << escapeJsonString(method_node->getReturnType()) << "\",\n";
        oss << getIndent(indent_level + 1)
            << "\"is_external\": " << (method_node->isExternal() ? "true" : "false") << ",\n";

        // Parameters
        oss << getIndent(indent_level + 1) << "\"parameters\": [\n";
        const auto& params = method_node->getParameters();
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << getIndent(indent_level + 2) << "{\n";
            oss << getIndent(indent_level + 3) << "\"name\": \"" << escapeJsonString(params[i].name)
                << "\",\n";
            oss << getIndent(indent_level + 3) << "\"type\": \"" << escapeJsonString(params[i].type)
                << "\"\n";
            oss << getIndent(indent_level + 2) << "}";
        }
        oss << "\n" << getIndent(indent_level + 1) << "],\n";

        // Method body
        oss << getIndent(indent_level + 1) << "\"body\": \n";
        const auto& body = method_node->getBody();
        if (body) {
            oss << serializeNodeRecursive(body, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"MethodDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"name\":\"" << escapeJsonString(method_node->getName()) << "\"";
        oss << ",\"return_type\":\"" << escapeJsonString(method_node->getReturnType()) << "\"";
        oss << ",\"is_external\":" << (method_node->isExternal() ? "true" : "false");
        oss << ",\"parameters\":[";
        const auto& params = method_node->getParameters();
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "{\"name\":\"" << escapeJsonString(params[i].name) << "\",\"type\":\""
                << escapeJsonString(params[i].type) << "\"}";
        }
        oss << "],\"body\":";
        const auto& body = method_node->getBody();
        if (body) {
            oss << serializeNodeRecursive(body, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeProtocolNode(const ASTNodePtr& node, int indent_level,
                                                  bool pretty_print) {
    auto protocol_node = dynamic_cast<ProtocolDeclarationNode*>(node.get());
    if (!protocol_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ProtocolDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"name\": \""
            << escapeJsonString(protocol_node->getProtocolName()) << "\",\n";

        // Method signatures
        oss << getIndent(indent_level + 1) << "\"method_signatures\": [\n";
        const auto& signatures = protocol_node->getMethodSignatures();
        for (size_t i = 0; i < signatures.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << getIndent(indent_level + 2) << "{\n";
            oss << getIndent(indent_level + 3) << "\"name\": \""
                << escapeJsonString(signatures[i].name) << "\",\n";
            oss << getIndent(indent_level + 3) << "\"return_type\": \""
                << escapeJsonString(signatures[i].return_type) << "\",\n";
            oss << getIndent(indent_level + 3) << "\"parameters\": [\n";

            // Parameters for this method signature
            for (size_t j = 0; j < signatures[i].parameters.size(); ++j) {
                if (j > 0) oss << ",\n";
                oss << getIndent(indent_level + 4) << "{\n";
                oss << getIndent(indent_level + 5) << "\"name\": \""
                    << escapeJsonString(signatures[i].parameters[j].name) << "\",\n";
                oss << getIndent(indent_level + 5) << "\"type\": \""
                    << escapeJsonString(signatures[i].parameters[j].type) << "\"\n";
                oss << getIndent(indent_level + 4) << "}";
            }
            oss << "\n" << getIndent(indent_level + 3) << "]\n";
            oss << getIndent(indent_level + 2) << "}";
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ProtocolDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"name\":\"" << escapeJsonString(protocol_node->getProtocolName()) << "\"";
        oss << ",\"method_signatures\":[";
        const auto& signatures = protocol_node->getMethodSignatures();
        for (size_t i = 0; i < signatures.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "{\"name\":\"" << escapeJsonString(signatures[i].name) << "\"";
            oss << ",\"return_type\":\"" << escapeJsonString(signatures[i].return_type) << "\"";
            oss << ",\"parameters\":[";
            for (size_t j = 0; j < signatures[i].parameters.size(); ++j) {
                if (j > 0) oss << ",";
                oss << "{\"name\":\"" << escapeJsonString(signatures[i].parameters[j].name)
                    << "\",\"type\":\"" << escapeJsonString(signatures[i].parameters[j].type)
                    << "\"}";
            }
            oss << "]}";
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializePropertyNode(const ASTNodePtr& node, int indent_level,
                                                  bool pretty_print) {
    auto property_node = dynamic_cast<PropertyDeclarationNode*>(node.get());
    if (!property_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"PropertyDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"name\": \""
            << escapeJsonString(property_node->getPropertyName()) << "\",\n";
        oss << getIndent(indent_level + 1) << "\"type\": \""
            << escapeJsonString(property_node->getTypeName()) << "\"\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"PropertyDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"name\":\"" << escapeJsonString(property_node->getPropertyName()) << "\"";
        oss << ",\"type\":\"" << escapeJsonString(property_node->getTypeName()) << "\"}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeConstructorNode(const ASTNodePtr& node, int indent_level,
                                                     bool pretty_print) {
    auto constructor_node = dynamic_cast<ConstructorDeclarationNode*>(node.get());
    if (!constructor_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ConstructorDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";

        // Parameters
        oss << getIndent(indent_level + 1) << "\"parameters\": [\n";
        const auto& params = constructor_node->getParameters();
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << getIndent(indent_level + 2) << "{\n";
            oss << getIndent(indent_level + 3) << "\"name\": \"" << escapeJsonString(params[i].name)
                << "\",\n";
            oss << getIndent(indent_level + 3) << "\"type\": \"" << escapeJsonString(params[i].type)
                << "\"\n";
            oss << getIndent(indent_level + 2) << "}";
        }
        oss << "\n" << getIndent(indent_level + 1) << "],\n";

        // Constructor body
        oss << getIndent(indent_level + 1) << "\"body\": \n";
        const auto& body = constructor_node->getBody();
        if (body) {
            oss << serializeNodeRecursive(body, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ConstructorDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"parameters\":[";
        const auto& params = constructor_node->getParameters();
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "{\"name\":\"" << escapeJsonString(params[i].name) << "\",\"type\":\""
                << escapeJsonString(params[i].type) << "\"}";
        }
        oss << "],\"body\":";
        const auto& body = constructor_node->getBody();
        if (body) {
            oss << serializeNodeRecursive(body, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeBlockNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print) {
    auto block_node = dynamic_cast<BlockNode*>(node.get());
    if (!block_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"BlockNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";

        // Statements in the block
        oss << getIndent(indent_level + 1) << "\"statements\": [\n";
        const auto& statements = block_node->getStatements();
        for (size_t i = 0; i < statements.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(statements[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"BlockNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"statements\":[";
        const auto& statements = block_node->getStatements();
        for (size_t i = 0; i < statements.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(statements[i], 0, pretty_print);
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeMethodCallNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print) {
    auto method_call_node = dynamic_cast<MethodCallNode*>(node.get());
    if (!method_call_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"MethodCallNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"method_name\": \""
            << escapeJsonString(method_call_node->getMethodName()) << "\",\n";

        // Object being called on
        oss << getIndent(indent_level + 1) << "\"object\": \n";
        const auto& object = method_call_node->getObject();
        if (object) {
            oss << serializeNodeRecursive(object, indent_level + 1, pretty_print) << ",\n";
        } else {
            oss << getIndent(indent_level + 1) << "null,\n";
        }

        // Arguments
        oss << getIndent(indent_level + 1) << "\"arguments\": [\n";
        const auto& arguments = method_call_node->getArguments();
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(arguments[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"MethodCallNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"method_name\":\"" << escapeJsonString(method_call_node->getMethodName()) << "\"";
        oss << ",\"object\":";
        const auto& object = method_call_node->getObject();
        if (object) {
            oss << serializeNodeRecursive(object, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << ",\"arguments\":[";
        const auto& arguments = method_call_node->getArguments();
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(arguments[i], 0, pretty_print);
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeReturnNode(const ASTNodePtr& node, int indent_level,
                                                bool pretty_print) {
    auto return_node = dynamic_cast<ReturnNode*>(node.get());
    if (!return_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ReturnNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";

        // Return expression
        oss << getIndent(indent_level + 1) << "\"expression\": \n";
        const auto& expression = return_node->getExpression();
        if (expression) {
            oss << serializeNodeRecursive(expression, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ReturnNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"expression\":";
        const auto& expression = return_node->getExpression();
        if (expression) {
            oss << serializeNodeRecursive(expression, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeIdentifierNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print) {
    auto identifier_node = dynamic_cast<IdentifierNode*>(node.get());
    if (!identifier_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"IdentifierNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"name\": \""
            << escapeJsonString(identifier_node->getName()) << "\"\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"IdentifierNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"name\":\"" << escapeJsonString(identifier_node->getName()) << "\"}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeLiteralNode(const ASTNodePtr& node, int indent_level,
                                                 bool pretty_print) {
    auto literal_node = dynamic_cast<LiteralNode*>(node.get());
    if (!literal_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"LiteralNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"value\": \""
            << escapeJsonString(literal_node->toString()) << "\"\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"LiteralNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"value\":\"" << escapeJsonString(literal_node->toString()) << "\"}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeVariableDeclarationNode(const ASTNodePtr& node,
                                                             int indent_level, bool pretty_print) {
    auto variable_decl_node = dynamic_cast<VariableDeclarationNode*>(node.get());
    if (!variable_decl_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"VariableDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"variable_name\": \""
            << escapeJsonString(variable_decl_node->getVariableName()) << "\",\n";
        oss << getIndent(indent_level + 1) << "\"type_name\": \""
            << escapeJsonString(variable_decl_node->getTypeName()) << "\",\n";

        // Initializer expression
        oss << getIndent(indent_level + 1) << "\"initializer\": \n";
        const auto& initializer = variable_decl_node->getInitializer();
        if (initializer) {
            oss << serializeNodeRecursive(initializer, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"VariableDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"variable_name\":\"" << escapeJsonString(variable_decl_node->getVariableName())
            << "\"";
        oss << ",\"type_name\":\"" << escapeJsonString(variable_decl_node->getTypeName()) << "\"";
        oss << ",\"initializer\":";
        const auto& initializer = variable_decl_node->getInitializer();
        if (initializer) {
            oss << serializeNodeRecursive(initializer, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeVariableAssignmentNode(const ASTNodePtr& node,
                                                            int indent_level, bool pretty_print) {
    auto variable_assign_node = dynamic_cast<VariableAssignmentNode*>(node.get());
    if (!variable_assign_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"VariableAssignmentNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"variable_name\": \""
            << escapeJsonString(variable_assign_node->getVariableName()) << "\",\n";

        // Value expression
        oss << getIndent(indent_level + 1) << "\"value\": \n";
        const auto& value_expr = variable_assign_node->getValueExpressionPtr();
        if (value_expr) {
            oss << serializeNodeRecursive(value_expr, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"VariableAssignmentNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"variable_name\":\"" << escapeJsonString(variable_assign_node->getVariableName())
            << "\"";
        oss << ",\"value\":";
        const auto& value_expr = variable_assign_node->getValueExpressionPtr();
        if (value_expr) {
            oss << serializeNodeRecursive(value_expr, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeMemberAccessNode(const ASTNodePtr& node, int indent_level,
                                                      bool pretty_print) {
    auto member_access_node = dynamic_cast<MemberAccessNode*>(node.get());
    if (!member_access_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"MemberAccessNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"member_name\": \""
            << escapeJsonString(member_access_node->getMemberName()) << "\",\n";

        // Object expression
        oss << getIndent(indent_level + 1) << "\"object\": \n";
        const auto& object_expr = member_access_node->getObjectExpression();
        if (object_expr) {
            oss << serializeNodeRecursive(object_expr, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"MemberAccessNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"member_name\":\"" << escapeJsonString(member_access_node->getMemberName())
            << "\"";
        oss << ",\"object\":";
        const auto& object_expr = member_access_node->getObjectExpression();
        if (object_expr) {
            oss << serializeNodeRecursive(object_expr, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializePropertyAssignmentNode(const ASTNodePtr& node,
                                                            int indent_level, bool pretty_print) {
    auto property_assign_node = dynamic_cast<PropertyAssignmentNode*>(node.get());
    if (!property_assign_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"PropertyAssignmentNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"property_name\": \""
            << escapeJsonString(property_assign_node->getPropertyName()) << "\",\n";

        // Value expression
        oss << getIndent(indent_level + 1) << "\"value\": \n";
        const auto& value_expr = property_assign_node->getValueExpression();
        if (value_expr) {
            oss << serializeNodeRecursive(value_expr, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"PropertyAssignmentNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"property_name\":\"" << escapeJsonString(property_assign_node->getPropertyName())
            << "\"";
        oss << ",\"value\":";
        const auto& value_expr = property_assign_node->getValueExpression();
        if (value_expr) {
            oss << serializeNodeRecursive(value_expr, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeWhileStatementNode(const ASTNodePtr& node, int indent_level,
                                                        bool pretty_print) {
    auto while_node = dynamic_cast<WhileStatementNode*>(node.get());
    if (!while_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"WhileStatementNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";

        // Condition
        oss << getIndent(indent_level + 1) << "\"condition\": \n";
        const auto& condition = while_node->getCondition();
        if (condition) {
            oss << serializeNodeRecursive(condition, indent_level + 1, pretty_print) << ",\n";
        } else {
            oss << getIndent(indent_level + 1) << "null,\n";
        }

        // Body
        oss << getIndent(indent_level + 1) << "\"body\": \n";
        const auto& body = while_node->getBody();
        if (body) {
            oss << serializeNodeRecursive(body, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"WhileStatementNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"condition\":";
        const auto& condition = while_node->getCondition();
        if (condition) {
            oss << serializeNodeRecursive(condition, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << ",\"body\":";
        const auto& body = while_node->getBody();
        if (body) {
            oss << serializeNodeRecursive(body, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeThisNode(const ASTNodePtr& node, int indent_level,
                                              bool pretty_print) {
    auto this_node = dynamic_cast<ThisNode*>(node.get());
    if (!this_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ThisNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << "\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ThisNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation())
            << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeNewExpressionNode(const ASTNodePtr& node, int indent_level,
                                                       bool pretty_print) {
    auto new_expr_node = dynamic_cast<NewExpressionNode*>(node.get());
    if (!new_expr_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"NewExpressionNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"object_type_name\": \""
            << escapeJsonString(new_expr_node->getObjectTypeName()) << "\",\n";

        // Constructor arguments
        oss << getIndent(indent_level + 1) << "\"constructor_args\": [\n";
        const auto& args = new_expr_node->getConstructorArgs();
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(args[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"NewExpressionNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"object_type_name\":\"" << escapeJsonString(new_expr_node->getObjectTypeName())
            << "\"";
        oss << ",\"constructor_args\":[";
        const auto& args = new_expr_node->getConstructorArgs();
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(args[i], 0, pretty_print);
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeListLiteralNode(const ASTNodePtr& node, int indent_level,
                                                     bool pretty_print) {
    auto list_literal_node = dynamic_cast<ListLiteralNode*>(node.get());
    if (!list_literal_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ListLiteralNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"element_type_name\": \""
            << escapeJsonString(list_literal_node->getElementTypeName()) << "\",\n";

        // List elements
        oss << getIndent(indent_level + 1) << "\"elements\": [\n";
        const auto& elements = list_literal_node->getElements();
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(elements[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ListLiteralNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"element_type_name\":\""
            << escapeJsonString(list_literal_node->getElementTypeName()) << "\"";
        oss << ",\"elements\":[";
        const auto& elements = list_literal_node->getElements();
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(elements[i], 0, pretty_print);
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeBinaryOpNode(const ASTNodePtr& node, int indent_level,
                                                  bool pretty_print) {
    auto binary_op_node = dynamic_cast<BinaryOpNode*>(node.get());
    if (!binary_op_node) return "null";

    // Helper function to convert operator enum to string
    auto operatorToString = [](BinaryOperator op) -> std::string {
        switch (op) {
            case BinaryOperator::PLUS:
                return "+";
            case BinaryOperator::MINUS:
                return "-";
            case BinaryOperator::MULTIPLY:
                return "*";
            case BinaryOperator::DIVIDE:
                return "/";
            case BinaryOperator::MODULO:
                return "%";
            default:
                return "unknown";
        }
    };

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"BinaryOpNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"operator\": \""
            << operatorToString(binary_op_node->getOperator()) << "\",\n";

        // Left operand
        oss << getIndent(indent_level + 1) << "\"left\": \n";
        const auto& left = binary_op_node->getLeft();
        if (left) {
            oss << serializeNodeRecursive(left, indent_level + 1, pretty_print) << ",\n";
        } else {
            oss << getIndent(indent_level + 1) << "null,\n";
        }

        // Right operand
        oss << getIndent(indent_level + 1) << "\"right\": \n";
        const auto& right = binary_op_node->getRight();
        if (right) {
            oss << serializeNodeRecursive(right, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"BinaryOpNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"operator\":\"" << operatorToString(binary_op_node->getOperator()) << "\"";
        oss << ",\"left\":";
        const auto& left = binary_op_node->getLeft();
        if (left) {
            oss << serializeNodeRecursive(left, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << ",\"right\":";
        const auto& right = binary_op_node->getRight();
        if (right) {
            oss << serializeNodeRecursive(right, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeLogicalNode(const ASTNodePtr& node, int indent_level,
                                                 bool pretty_print) {
    auto logical_node = dynamic_cast<LogicalNode*>(node.get());
    if (!logical_node) return "null";

    // Helper function to convert operator enum to string
    auto operatorToString = [](LogicalOperator op) -> std::string {
        switch (op) {
            case LogicalOperator::AND:
                return "&&";
            case LogicalOperator::OR:
                return "||";
            default:
                return "unknown";
        }
    };

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"LogicalNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"operator\": \""
            << operatorToString(logical_node->getOperator()) << "\",\n";

        // Left operand
        oss << getIndent(indent_level + 1) << "\"left\": \n";
        const auto& left = logical_node->getLeft();
        if (left) {
            oss << serializeNodeRecursive(left, indent_level + 1, pretty_print) << ",\n";
        } else {
            oss << getIndent(indent_level + 1) << "null,\n";
        }

        // Right operand
        oss << getIndent(indent_level + 1) << "\"right\": \n";
        const auto& right = logical_node->getRight();
        if (right) {
            oss << serializeNodeRecursive(right, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"LogicalNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"operator\":\"" << operatorToString(logical_node->getOperator()) << "\"";
        oss << ",\"left\":";
        const auto& left = logical_node->getLeft();
        if (left) {
            oss << serializeNodeRecursive(left, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << ",\"right\":";
        const auto& right = logical_node->getRight();
        if (right) {
            oss << serializeNodeRecursive(right, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeUnaryNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print) {
    auto unary_node = dynamic_cast<UnaryNode*>(node.get());
    if (!unary_node) return "null";

    // Helper function to convert operator enum to string
    auto operatorToString = [](UnaryOperator op) -> std::string {
        switch (op) {
            case UnaryOperator::NOT:
                return "!";
            case UnaryOperator::MINUS:
                return "-";
            default:
                return "unknown";
        }
    };

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"UnaryNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"operator\": \""
            << operatorToString(unary_node->getOperator()) << "\",\n";

        // Operand
        oss << getIndent(indent_level + 1) << "\"operand\": \n";
        const auto& operand = unary_node->getOperand();
        if (operand) {
            oss << serializeNodeRecursive(operand, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"UnaryNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"operator\":\"" << operatorToString(unary_node->getOperator()) << "\"";
        oss << ",\"operand\":";
        const auto& operand = unary_node->getOperand();
        if (operand) {
            oss << serializeNodeRecursive(operand, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeComparisonNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print) {
    auto comparison_node = dynamic_cast<ComparisonNode*>(node.get());
    if (!comparison_node) return "null";

    // Helper function to convert operator enum to string
    auto operatorToString = [](ComparisonOperator op) -> std::string {
        switch (op) {
            case ComparisonOperator::EQUAL:
                return "==";
            case ComparisonOperator::NOT_EQUAL:
                return "!=";
            case ComparisonOperator::LESS_THAN:
                return "<";
            case ComparisonOperator::GREATER_THAN:
                return ">";
            case ComparisonOperator::LESS_EQUAL:
                return "<=";
            case ComparisonOperator::GREATER_EQUAL:
                return ">=";
            default:
                return "unknown";
        }
    };

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ComparisonNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"operator\": \""
            << operatorToString(comparison_node->getOperator()) << "\",\n";

        // Left operand
        oss << getIndent(indent_level + 1) << "\"left\": \n";
        const auto& left = comparison_node->getLeft();
        if (left) {
            oss << serializeNodeRecursive(left, indent_level + 1, pretty_print) << ",\n";
        } else {
            oss << getIndent(indent_level + 1) << "null,\n";
        }

        // Right operand
        oss << getIndent(indent_level + 1) << "\"right\": \n";
        const auto& right = comparison_node->getRight();
        if (right) {
            oss << serializeNodeRecursive(right, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ComparisonNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"operator\":\"" << operatorToString(comparison_node->getOperator()) << "\"";
        oss << ",\"left\":";
        const auto& left = comparison_node->getLeft();
        if (left) {
            oss << serializeNodeRecursive(left, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << ",\"right\":";
        const auto& right = comparison_node->getRight();
        if (right) {
            oss << serializeNodeRecursive(right, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeIfStatementNode(const ASTNodePtr& node, int indent_level,
                                                     bool pretty_print) {
    auto if_statement_node = dynamic_cast<IfStatementNode*>(node.get());
    if (!if_statement_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"IfStatementNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";

        // Condition
        oss << getIndent(indent_level + 1) << "\"condition\": \n";
        const auto& condition = if_statement_node->getCondition();
        if (condition) {
            oss << serializeNodeRecursive(condition, indent_level + 1, pretty_print) << ",\n";
        } else {
            oss << getIndent(indent_level + 1) << "null,\n";
        }

        // Then branch
        oss << getIndent(indent_level + 1) << "\"then_branch\": \n";
        const auto& then_branch = if_statement_node->getThenBranch();
        if (then_branch) {
            oss << serializeNodeRecursive(then_branch, indent_level + 1, pretty_print);
        } else {
            oss << getIndent(indent_level + 1) << "null";
        }

        // Else branch (optional)
        if (if_statement_node->hasElseBranch()) {
            oss << ",\n";
            oss << getIndent(indent_level + 1) << "\"else_branch\": \n";
            const auto& else_branch = if_statement_node->getElseBranch();
            if (else_branch) {
                oss << serializeNodeRecursive(else_branch, indent_level + 1, pretty_print) << "\n";
            } else {
                oss << getIndent(indent_level + 1) << "null\n";
            }
        } else {
            oss << "\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"IfStatementNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());

        // Condition
        oss << ",\"condition\":";
        const auto& condition = if_statement_node->getCondition();
        if (condition) {
            oss << serializeNodeRecursive(condition, 0, pretty_print);
        } else {
            oss << "null";
        }

        // Then branch
        oss << ",\"then_branch\":";
        const auto& then_branch = if_statement_node->getThenBranch();
        if (then_branch) {
            oss << serializeNodeRecursive(then_branch, 0, pretty_print);
        } else {
            oss << "null";
        }

        // Else branch (optional)
        if (if_statement_node->hasElseBranch()) {
            oss << ",\"else_branch\":";
            const auto& else_branch = if_statement_node->getElseBranch();
            if (else_branch) {
                oss << serializeNodeRecursive(else_branch, 0, pretty_print);
            } else {
                oss << "null";
            }
        }

        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeConstDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                          bool pretty_print) {
    auto const_decl_node = dynamic_cast<ConstDeclarationNode*>(node.get());
    if (!const_decl_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"ConstDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"const_name\": \""
            << escapeJsonString(const_decl_node->getConstName()) << "\",\n";
        oss << getIndent(indent_level + 1) << "\"type_name\": \""
            << escapeJsonString(const_decl_node->getTypeName()) << "\",\n";

        // Initializer
        oss << getIndent(indent_level + 1) << "\"initializer\": \n";
        const auto& initializer = const_decl_node->getInitializer();
        if (initializer) {
            oss << serializeNodeRecursive(initializer, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"ConstDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"const_name\":\"" << escapeJsonString(const_decl_node->getConstName()) << "\"";
        oss << ",\"type_name\":\"" << escapeJsonString(const_decl_node->getTypeName()) << "\"";
        oss << ",\"initializer\":";
        const auto& initializer = const_decl_node->getInitializer();
        if (initializer) {
            oss << serializeNodeRecursive(initializer, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeSetLiteralNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print) {
    auto set_literal_node = dynamic_cast<SetLiteralNode*>(node.get());
    if (!set_literal_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"SetLiteralNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"element_type_name\": \""
            << escapeJsonString(set_literal_node->getElementTypeName()) << "\",\n";

        // Elements array
        oss << getIndent(indent_level + 1) << "\"elements\": [\n";
        const auto& elements = set_literal_node->getElements();
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << serializeNodeRecursive(elements[i], indent_level + 2, pretty_print);
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"SetLiteralNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"element_type_name\":\""
            << escapeJsonString(set_literal_node->getElementTypeName()) << "\"";

        // Elements array
        oss << ",\"elements\":[";
        const auto& elements = set_literal_node->getElements();
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) oss << ",";
            oss << serializeNodeRecursive(elements[i], 0, pretty_print);
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeRecordDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                           bool pretty_print) {
    auto record_decl_node = dynamic_cast<RecordDeclarationNode*>(node.get());
    if (!record_decl_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"RecordDeclarationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"record_name\": \""
            << escapeJsonString(record_decl_node->getRecordName()) << "\",\n";

        // Fields array
        oss << getIndent(indent_level + 1) << "\"fields\": [\n";
        const auto& fields = record_decl_node->getFields();
        for (size_t i = 0; i < fields.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << getIndent(indent_level + 2) << "{\n";
            oss << getIndent(indent_level + 3) << "\"name\": \"" << escapeJsonString(fields[i].name)
                << "\",\n";
            oss << getIndent(indent_level + 3) << "\"type\": \"" << escapeJsonString(fields[i].type)
                << "\"\n";
            oss << getIndent(indent_level + 2) << "}";
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"RecordDeclarationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"record_name\":\"" << escapeJsonString(record_decl_node->getRecordName()) << "\"";

        // Fields array
        oss << ",\"fields\":[";
        const auto& fields = record_decl_node->getFields();
        for (size_t i = 0; i < fields.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "{\"name\":\"" << escapeJsonString(fields[i].name) << "\",";
            oss << "\"type\":\"" << escapeJsonString(fields[i].type) << "\"}";
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeRecordFieldAccessNode(const ASTNodePtr& node, int indent_level,
                                                           bool pretty_print) {
    auto record_field_access_node = dynamic_cast<RecordFieldAccessNode*>(node.get());
    if (!record_field_access_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"RecordFieldAccessNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"field_name\": \""
            << escapeJsonString(record_field_access_node->getFieldName()) << "\",\n";

        // Record expression
        oss << getIndent(indent_level + 1) << "\"record_expression\": \n";
        const auto& record_expr = record_field_access_node->getRecordExpression();
        if (record_expr) {
            oss << serializeNodeRecursive(record_expr, indent_level + 1, pretty_print) << "\n";
        } else {
            oss << getIndent(indent_level + 1) << "null\n";
        }

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"RecordFieldAccessNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"field_name\":\"" << escapeJsonString(record_field_access_node->getFieldName())
            << "\"";
        oss << ",\"record_expression\":";
        const auto& record_expr = record_field_access_node->getRecordExpression();
        if (record_expr) {
            oss << serializeNodeRecursive(record_expr, 0, pretty_print);
        } else {
            oss << "null";
        }
        oss << "}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeRecordInstantiationNode(const ASTNodePtr& node,
                                                             int indent_level, bool pretty_print) {
    auto record_inst_node = dynamic_cast<RecordInstantiationNode*>(node.get());
    if (!record_inst_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"RecordInstantiationNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"record_type_name\": \""
            << escapeJsonString(record_inst_node->getRecordTypeName()) << "\",\n";

        // Field assignments array
        oss << getIndent(indent_level + 1) << "\"field_assignments\": [\n";
        const auto& assignments = record_inst_node->getFieldAssignments();
        for (size_t i = 0; i < assignments.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << getIndent(indent_level + 2) << "{\n";
            oss << getIndent(indent_level + 3) << "\"field_name\": \""
                << escapeJsonString(assignments[i].field_name) << "\",\n";
            oss << getIndent(indent_level + 3) << "\"value_expression\": \n";
            if (assignments[i].value_expr) {
                oss << serializeNodeRecursive(assignments[i].value_expr, indent_level + 3,
                                              pretty_print)
                    << "\n";
            } else {
                oss << getIndent(indent_level + 3) << "null\n";
            }
            oss << getIndent(indent_level + 2) << "}";
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"RecordInstantiationNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"record_type_name\":\"" << escapeJsonString(record_inst_node->getRecordTypeName())
            << "\"";

        // Field assignments array
        oss << ",\"field_assignments\":[";
        const auto& assignments = record_inst_node->getFieldAssignments();
        for (size_t i = 0; i < assignments.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "{\"field_name\":\"" << escapeJsonString(assignments[i].field_name) << "\",";
            oss << "\"value_expression\":";
            if (assignments[i].value_expr) {
                oss << serializeNodeRecursive(assignments[i].value_expr, 0, pretty_print);
            } else {
                oss << "null";
            }
            oss << "}";
        }
        oss << "]}";
    }

    return oss.str();
}

std::string JsonSerializer::serializePropertyAccessNode(const ASTNodePtr& node, int indent_level,
                                                        bool pretty_print) {
    auto property_access_node = dynamic_cast<PropertyAccessNode*>(node.get());
    if (!property_access_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"PropertyAccessNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"property_name\": \""
            << escapeJsonString(property_access_node->getPropertyName()) << "\"\n";
        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"PropertyAccessNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"property_name\":\"" << escapeJsonString(property_access_node->getPropertyName())
            << "\"}";
    }

    return oss.str();
}

std::string JsonSerializer::serializeQualifiedIdentifierNode(const ASTNodePtr& node,
                                                             int indent_level, bool pretty_print) {
    auto qualified_id_node = dynamic_cast<QualifiedIdentifierNode*>(node.get());
    if (!qualified_id_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"QualifiedIdentifierNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"full_qualified_name\": \""
            << escapeJsonString(qualified_id_node->getFullQualifiedName()) << "\",\n";

        // Parts array
        oss << getIndent(indent_level + 1) << "\"parts\": [";
        const auto& parts = qualified_id_node->getParts();
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "\"" << escapeJsonString(parts[i]) << "\"";
        }
        oss << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"QualifiedIdentifierNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"full_qualified_name\":\""
            << escapeJsonString(qualified_id_node->getFullQualifiedName()) << "\"";
        oss << ",\"parts\":[";
        const auto& parts = qualified_id_node->getParts();
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "\"" << escapeJsonString(parts[i]) << "\"";
        }
        oss << "]}";
    }

    return oss.str();
}

// Placeholder implementations for the remaining nodes
std::string JsonSerializer::serializeFunctionCallNode(const ASTNodePtr& node, int indent_level,
                                                      bool pretty_print) {
    return "{\"node_type\":\"FunctionCallNode\",\"source_location\":" +
           sourceLocationToJsonString(node->getSourceLocation()) +
           ",\"note\":\"Implementation pending\"}";
}

std::string JsonSerializer::serializeEnumDeclarationNode(const ASTNodePtr& node, int indent_level,
                                                         bool pretty_print) {
    return "{\"node_type\":\"EnumDeclarationNode\",\"source_location\":" +
           sourceLocationToJsonString(node->getSourceLocation()) +
           ",\"note\":\"Implementation pending\"}";
}

std::string JsonSerializer::serializeEnumAccessNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print) {
    return "{\"node_type\":\"EnumAccessNode\",\"source_location\":" +
           sourceLocationToJsonString(node->getSourceLocation()) +
           ",\"note\":\"Implementation pending\"}";
}

std::string JsonSerializer::serializeNamespaceNode(const ASTNodePtr& node, int indent_level,
                                                   bool pretty_print) {
    return "{\"node_type\":\"NamespaceNode\",\"source_location\":" +
           sourceLocationToJsonString(node->getSourceLocation()) +
           ",\"note\":\"Implementation pending\"}";
}

std::string JsonSerializer::serializeThrowNode(const ASTNodePtr& node, int indent_level,
                                               bool pretty_print) {
    return "{\"node_type\":\"ThrowNode\",\"source_location\":" +
           sourceLocationToJsonString(node->getSourceLocation()) +
           ",\"note\":\"Implementation pending\"}";
}

std::string JsonSerializer::serializeTryCatchFinallyNode(const ASTNodePtr& node, int indent_level,
                                                         bool pretty_print) {
    return "{\"node_type\":\"TryCatchFinallyNode\",\"source_location\":" +
           sourceLocationToJsonString(node->getSourceLocation()) +
           ",\"note\":\"Implementation pending\"}";
}

std::string JsonSerializer::serializeMapLiteralNode(const ASTNodePtr& node, int indent_level,
                                                    bool pretty_print) {
    auto map_literal_node = dynamic_cast<MapLiteralNode*>(node.get());
    if (!map_literal_node) return "null";

    std::ostringstream oss;
    if (pretty_print) {
        oss << getIndent(indent_level) << "{\n";
        oss << getIndent(indent_level + 1) << "\"node_type\": \"MapLiteralNode\",\n";
        oss << getIndent(indent_level + 1)
            << "\"source_location\": " << sourceLocationToJsonString(node->getSourceLocation())
            << ",\n";
        oss << getIndent(indent_level + 1) << "\"key_type_name\": \""
            << escapeJsonString(map_literal_node->getKeyTypeName()) << "\",\n";
        oss << getIndent(indent_level + 1) << "\"value_type_name\": \""
            << escapeJsonString(map_literal_node->getValueTypeName()) << "\",\n";

        // Entries array
        oss << getIndent(indent_level + 1) << "\"entries\": [\n";
        const auto& entries = map_literal_node->getEntries();
        for (size_t i = 0; i < entries.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << getIndent(indent_level + 2) << "{\n";

            // Key
            oss << getIndent(indent_level + 3) << "\"key\": \n";
            if (entries[i].first) {
                oss << serializeNodeRecursive(entries[i].first, indent_level + 3, pretty_print)
                    << ",\n";
            } else {
                oss << getIndent(indent_level + 3) << "null,\n";
            }

            // Value
            oss << getIndent(indent_level + 3) << "\"value\": \n";
            if (entries[i].second) {
                oss << serializeNodeRecursive(entries[i].second, indent_level + 3, pretty_print)
                    << "\n";
            } else {
                oss << getIndent(indent_level + 3) << "null\n";
            }

            oss << getIndent(indent_level + 2) << "}";
        }
        oss << "\n" << getIndent(indent_level + 1) << "]\n";

        oss << getIndent(indent_level) << "}";
    } else {
        oss << "{\"node_type\":\"MapLiteralNode\"";
        oss << ",\"source_location\":" << sourceLocationToJsonString(node->getSourceLocation());
        oss << ",\"key_type_name\":\"" << escapeJsonString(map_literal_node->getKeyTypeName())
            << "\"";
        oss << ",\"value_type_name\":\"" << escapeJsonString(map_literal_node->getValueTypeName())
            << "\"";

        // Entries array
        oss << ",\"entries\":[";
        const auto& entries = map_literal_node->getEntries();
        for (size_t i = 0; i < entries.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "{\"key\":";
            if (entries[i].first) {
                oss << serializeNodeRecursive(entries[i].first, 0, pretty_print);
            } else {
                oss << "null";
            }
            oss << ",\"value\":";
            if (entries[i].second) {
                oss << serializeNodeRecursive(entries[i].second, 0, pretty_print);
            } else {
                oss << "null";
            }
            oss << "}";
        }
        oss << "]}";
    }

    return oss.str();
}

}  // namespace o2l