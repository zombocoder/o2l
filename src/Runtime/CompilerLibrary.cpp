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

#include "CompilerLibrary.hpp"
#include "Context.hpp"
#include "../Lexer.hpp"
#include "../Parser.hpp"
#include "../AST/Node.hpp"
#include "ListInstance.hpp"
#include "MapInstance.hpp"
#include "../Common/Exceptions.hpp"

// AST Node includes for dynamic_cast
#include "../AST/BinaryOpNode.hpp"
#include "../AST/BlockNode.hpp"
#include "../AST/ComparisonNode.hpp"
#include "../AST/ConstDeclarationNode.hpp"
#include "../AST/ConstructorDeclarationNode.hpp"
#include "../AST/FunctionCallNode.hpp"
#include "../AST/IdentifierNode.hpp"
#include "../AST/IfStatementNode.hpp"
#include "../AST/ImportNode.hpp"
#include "../AST/ListLiteralNode.hpp"
#include "../AST/LiteralNode.hpp"
#include "../AST/LogicalNode.hpp"
#include "../AST/MapLiteralNode.hpp"
#include "../AST/MemberAccessNode.hpp"
#include "../AST/MethodCallNode.hpp"
#include "../AST/MethodDeclarationNode.hpp"
#include "../AST/NamespaceNode.hpp"
#include "../AST/NewExpressionNode.hpp"
#include "../AST/ObjectNode.hpp"
#include "../AST/PropertyDeclarationNode.hpp"
#include "../AST/ProtocolDeclarationNode.hpp"
#include "../AST/RecordDeclarationNode.hpp"
#include "../AST/RecordInstantiationNode.hpp"
#include "../AST/ReturnNode.hpp"
#include "../AST/SetLiteralNode.hpp"
#include "../AST/ThisNode.hpp"
#include "../AST/ThrowNode.hpp"
#include "../AST/TryCatchFinallyNode.hpp"
#include "../AST/UnaryNode.hpp"
#include "../AST/VariableAssignmentNode.hpp"
#include "../AST/VariableDeclarationNode.hpp"
#include "../AST/WhileStatementNode.hpp"

#include <typeinfo>

namespace o2l {

std::shared_ptr<ObjectInstance> CompilerLibrary::createCompilerObject() {
    auto compiler_object = std::make_shared<ObjectInstance>("compiler");
    
    compiler_object->addMethod("parse", [](const std::vector<Value>& args, Context& ctx) -> Value {
        return CompilerLibrary::nativeParse(args, ctx);
    }, true);
    
    return compiler_object;
}

Value CompilerLibrary::nativeParse(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("compiler.parse() requires a Text argument");
    }
    
    std::string source = std::get<Text>(args[0]);
    Lexer lexer(source);
    Parser parser(lexer.tokenizeAll());
    
    try {
        auto nodes = parser.parse();
        auto list = std::make_shared<ListInstance>("ASTNode");
        for (auto& node : nodes) {
            list->add(Value(createASTNodeHandle(node.get())));
        }
        return Value(list);
    } catch (const SyntaxError& e) {
        throw EvaluationError("Syntax Error during compilation: " + std::string(e.what()));
    }
}

std::shared_ptr<ObjectInstance> CompilerLibrary::createASTNodeHandle(ASTNode* node) {
    if (!node) return nullptr;
    return std::make_shared<ASTNodeHandleInstance>(node);
}

// ASTNodeHandleInstance implementation

ASTNodeHandleInstance::ASTNodeHandleInstance(ASTNode* node) 
    : ObjectInstance("ASTNode"), node_(node) {
    
    addMethod("type", nativeGetType, true);
    addMethod("children", nativeGetChildren, true);
    addMethod("property", nativeGetProperty, true);
    addMethod("location", nativeGetLocation, true);
}

Value ASTNodeHandleInstance::nativeGetType(const std::vector<Value>& args, Context& context) {
    auto self = std::dynamic_pointer_cast<ASTNodeHandleInstance>(context.getThisObject());
    if (!self || !self->node_) return Text("NullNode");
    
    std::string type_name = typeid(*(self->node_)).name();
    // Simplified type name extraction (similar to JsonSerializer)
    size_t last_n = type_name.find_last_of("0123456789");
    if (last_n != std::string::npos && last_n + 1 < type_name.length()) {
        type_name = type_name.substr(last_n + 1);
    }
    return Text(type_name);
}

Value ASTNodeHandleInstance::nativeGetChildren(const std::vector<Value>& args, Context& context) {
    auto self = std::dynamic_pointer_cast<ASTNodeHandleInstance>(context.getThisObject());
    if (!self || !self->node_) return Value(std::make_shared<ListInstance>("ASTNode"));
    
    auto children = std::make_shared<ListInstance>("ASTNode");
    auto node = self->node_;
    
    // Dynamic cast chain to extract children based on node type
    if (auto n = dynamic_cast<BlockNode*>(node)) {
        for (auto& child : n->getStatements()) children->add(Value(CompilerLibrary::createASTNodeHandle(child.get())));
    } else if (auto n = dynamic_cast<IfStatementNode*>(node)) {
        children->add(Value(CompilerLibrary::createASTNodeHandle(n->getCondition().get())));
        children->add(Value(CompilerLibrary::createASTNodeHandle(n->getThenBranch().get())));
        if (n->hasElseBranch()) children->add(Value(CompilerLibrary::createASTNodeHandle(n->getElseBranch().get())));
    } else if (auto n = dynamic_cast<WhileStatementNode*>(node)) {
        children->add(Value(CompilerLibrary::createASTNodeHandle(n->getCondition().get())));
        children->add(Value(CompilerLibrary::createASTNodeHandle(n->getBody().get())));
    } else if (auto n = dynamic_cast<BinaryOpNode*>(node)) {
        children->add(Value(CompilerLibrary::createASTNodeHandle(n->getLeft().get())));
        children->add(Value(CompilerLibrary::createASTNodeHandle(n->getRight().get())));
    } else if (auto n = dynamic_cast<MethodDeclarationNode*>(node)) {
        if (n->getBody()) children->add(Value(CompilerLibrary::createASTNodeHandle(n->getBody().get())));
    } else if (auto n = dynamic_cast<ObjectNode*>(node)) {
        for (auto& m : n->getMethods()) children->add(Value(CompilerLibrary::createASTNodeHandle(m.get())));
        for (auto& p : n->getProperties()) children->add(Value(CompilerLibrary::createASTNodeHandle(p.get())));
        if (n->getConstructor()) children->add(Value(CompilerLibrary::createASTNodeHandle(n->getConstructor().get())));
    } else if (auto n = dynamic_cast<MethodCallNode*>(node)) {
        if (n->getObject()) children->add(Value(CompilerLibrary::createASTNodeHandle(n->getObject().get())));
        for (auto& arg : n->getArguments()) children->add(Value(CompilerLibrary::createASTNodeHandle(arg.get())));
    } else if (auto n = dynamic_cast<ReturnNode*>(node)) {
        if (n->getExpression()) children->add(Value(CompilerLibrary::createASTNodeHandle(n->getExpression().get())));
    }
    // ... add more as needed for the bootstrap compiler
    
    return Value(children);
}

Value ASTNodeHandleInstance::nativeGetProperty(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("property() requires a Text argument (property name)");
    }
    
    std::string name = std::get<Text>(args[0]);
    auto self = std::dynamic_pointer_cast<ASTNodeHandleInstance>(context.getThisObject());
    if (!self || !self->node_) return Value();
    
    auto node = self->node_;
    
    if (auto n = dynamic_cast<IdentifierNode*>(node)) {
        if (name == "name") return Text(n->getName());
    } else if (auto n = dynamic_cast<LiteralNode*>(node)) {
        if (name == "value") return Text(n->toString());
    } else if (auto n = dynamic_cast<BinaryOpNode*>(node)) {
        if (name == "operator") {
            switch(n->getOperator()) {
                case BinaryOperator::PLUS: return Text("+");
                case BinaryOperator::MINUS: return Text("-");
                case BinaryOperator::MULTIPLY: return Text("*");
                case BinaryOperator::DIVIDE: return Text("/");
                case BinaryOperator::BITWISE_AND: return Text("&");
                case BinaryOperator::BITWISE_OR: return Text("|");
                case BinaryOperator::BITWISE_XOR: return Text("^");
                case BinaryOperator::LSHIFT: return Text("<<");
                case BinaryOperator::RSHIFT: return Text(">>");
                default: return Text("unknown");
            }
        }
    } else if (auto n = dynamic_cast<MethodDeclarationNode*>(node)) {
        if (name == "name") return Text(n->getName());
        if (name == "return_type") return Text(n->getReturnType());
    } else if (auto n = dynamic_cast<ObjectNode*>(node)) {
        if (name == "name") return Text(n->getName());
    }
    
    return Value();
}

Value ASTNodeHandleInstance::nativeGetLocation(const std::vector<Value>& args, Context& context) {
    auto self = std::dynamic_pointer_cast<ASTNodeHandleInstance>(context.getThisObject());
    if (!self || !self->node_) return Value();
    
    auto loc = self->node_->getSourceLocation();
    auto map = std::make_shared<MapInstance>();
    map->put(Text("line"), Int(static_cast<int>(loc.line_number)));
    map->put(Text("column"), Int(static_cast<int>(loc.column_number)));
    map->put(Text("file"), Text(loc.filename));
    
    return Value(map);
}

} // namespace o2l
