#include "QualifiedIdentifierNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

Value QualifiedIdentifierNode::evaluate(Context& context) {
    std::string full_name = getFullQualifiedName();

    // First try to resolve the full qualified name
    if (context.hasVariable(full_name)) {
        return context.getVariable(full_name);
    }

    // If not found, try to resolve step by step
    // Start with the first part and see if it exists
    if (parts_.empty()) {
        throw EvaluationError("Empty qualified identifier");
    }

    // Try just the last part (for simple access within same namespace)
    std::string last_part = getLastPart();
    if (context.hasVariable(last_part)) {
        return context.getVariable(last_part);
    }

    // If neither worked, throw an error
    throw UnresolvedReferenceError("Qualified identifier '" + full_name +
                                   "' not found in current context");
}

std::string QualifiedIdentifierNode::toString() const {
    return "QualifiedIdentifier(" + getFullQualifiedName() + ")";
}

}  // namespace o2l