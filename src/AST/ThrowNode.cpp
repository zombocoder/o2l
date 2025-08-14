#include "ThrowNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

ThrowNode::ThrowNode(ASTNodePtr expression, const SourceLocation& location)
    : ASTNode(location), expression_(std::move(expression)) {}

Value ThrowNode::evaluate(Context& context) {
    // Evaluate the expression to get the value to throw
    Value thrown_value = expression_->evaluate(context);

    // Throw a UserException with the evaluated value and current context
    throw UserException(thrown_value, context);
}

std::string ThrowNode::toString() const {
    return "Throw(" + expression_->toString() + ")";
}

}  // namespace o2l