#include "MethodDeclarationNode.hpp"

namespace o2l {

MethodDeclarationNode::MethodDeclarationNode(std::string name, std::vector<Parameter> parameters,
                                             std::string return_type, ASTNodePtr body,
                                             bool is_external, const SourceLocation& location)
    : ASTNode(location),
      method_name_(std::move(name)),
      parameters_(std::move(parameters)),
      return_type_(std::move(return_type)),
      body_(std::move(body)),
      is_external_(is_external) {}

Value MethodDeclarationNode::evaluate(Context& context) {
    // Method declarations don't evaluate to values directly
    // They are processed when creating objects
    return Int(0);
}

std::string MethodDeclarationNode::toString() const {
    std::string params = "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) params += ", ";
        params += parameters_[i].name + ": " + parameters_[i].type;
    }
    params += ")";

    std::string modifier = is_external_ ? "@external " : "";
    return modifier + "Method(" + method_name_ + params + ": " + return_type_ + ")";
}

}  // namespace o2l