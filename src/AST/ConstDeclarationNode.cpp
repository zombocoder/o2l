#include "ConstDeclarationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

ConstDeclarationNode::ConstDeclarationNode(std::string const_name, std::string type_name,
                                           ASTNodePtr initializer)
    : const_name_(std::move(const_name)),
      type_name_(std::move(type_name)),
      initializer_(std::move(initializer)) {}

Value ConstDeclarationNode::evaluate(Context& context) {
    // Evaluate the initializer expression
    Value value = initializer_->evaluate(context);

    // TODO: Add type checking if type_name_ is provided

    // Define the constant in the current scope with immutability protection
    context.defineConstant(const_name_, value);

    // Return the assigned value
    return value;
}

std::string ConstDeclarationNode::toString() const {
    std::string result = "const " + const_name_;
    if (!type_name_.empty()) {
        result += ": " + type_name_;
    }
    result += " = " + initializer_->toString();
    return result;
}

}  // namespace o2l