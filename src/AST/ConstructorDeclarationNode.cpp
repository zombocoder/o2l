#include "ConstructorDeclarationNode.hpp"

namespace o2l {

ConstructorDeclarationNode::ConstructorDeclarationNode(std::vector<Parameter> parameters,
                                                       ASTNodePtr body)
    : parameters_(std::move(parameters)), body_(std::move(body)) {}

Value ConstructorDeclarationNode::evaluate(Context& context) {
    // Constructor declarations don't evaluate to values directly
    // They are processed when creating objects
    return Int(0);
}

std::string ConstructorDeclarationNode::toString() const {
    std::string params = "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) params += ", ";
        params += parameters_[i].name + ": " + parameters_[i].type;
    }
    params += ")";

    return "Constructor" + params;
}

}  // namespace o2l