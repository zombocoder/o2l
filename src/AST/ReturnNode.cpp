#include "ReturnNode.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

ReturnNode::ReturnNode(ASTNodePtr expression) : expression_(std::move(expression)) {}

Value ReturnNode::evaluate(Context& context) {
    Value return_value;

    if (expression_) {
        return_value = expression_->evaluate(context);
    } else {
        // Return default value if no expression
        return_value = Int(0);
    }

    // Throw ReturnException to cause early exit from method execution
    throw ReturnException(return_value);
}

std::string ReturnNode::toString() const {
    if (expression_) {
        return "Return(" + expression_->toString() + ")";
    }
    return "Return()";
}

}  // namespace o2l