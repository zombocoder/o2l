#include "NamespaceNode.hpp"

#include <iostream>

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "EnumDeclarationNode.hpp"
#include "ObjectNode.hpp"
#include "ProtocolDeclarationNode.hpp"
#include "RecordDeclarationNode.hpp"

namespace o2l {

Value NamespaceNode::evaluate(Context& context) {
    // Create a new context for this namespace
    Context namespace_context(context);

    std::string full_namespace = getFullNamespace();

    // Evaluate all items in the namespace body
    for (const auto& node : body_) {
        auto object_node = dynamic_cast<ObjectNode*>(node.get());
        auto enum_node = dynamic_cast<EnumDeclarationNode*>(node.get());
        auto record_node = dynamic_cast<RecordDeclarationNode*>(node.get());
        auto protocol_node = dynamic_cast<ProtocolDeclarationNode*>(node.get());

        if (object_node) {
            // Evaluate the object in the namespace context
            Value object_value = node->evaluate(namespace_context);

            // Register the object with its fully qualified name
            std::string qualified_name = full_namespace + "." + object_node->getName();
            context.defineVariable(qualified_name, object_value);

            // Also register just the object name in the parent context for direct access
            context.defineVariable(object_node->getName(), object_value);

        } else if (enum_node) {
            Value enum_value = node->evaluate(namespace_context);
            std::string qualified_name = full_namespace + "." + enum_node->getEnumName();
            context.defineVariable(qualified_name, enum_value);
            context.defineVariable(enum_node->getEnumName(), enum_value);

        } else if (record_node) {
            Value record_value = node->evaluate(namespace_context);
            std::string qualified_name = full_namespace + "." + record_node->getRecordName();
            context.defineVariable(qualified_name, record_value);
            context.defineVariable(record_node->getRecordName(), record_value);

        } else if (protocol_node) {
            Value protocol_value = node->evaluate(namespace_context);
            std::string qualified_name = full_namespace + "." + protocol_node->getProtocolName();
            context.defineVariable(qualified_name, protocol_value);
            context.defineVariable(protocol_node->getProtocolName(), protocol_value);

        } else {
            throw EvaluationError(
                "Only object, enum, record, and protocol declarations are allowed inside "
                "namespaces");
        }
    }

    // Return a Text indicating the namespace was processed
    return Text("namespace " + full_namespace + " processed");
}

std::string NamespaceNode::toString() const {
    std::string result = "namespace " + getFullNamespace() + " {\n";
    for (const auto& node : body_) {
        result += "  " + node->toString() + "\n";
    }
    result += "}";
    return result;
}

}  // namespace o2l