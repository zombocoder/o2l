#include "WhileStatementNode.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

WhileStatementNode::WhileStatementNode(ASTNodePtr condition, ASTNodePtr body)
    : condition_(std::move(condition)), body_(std::move(body)) {}

Value WhileStatementNode::evaluate(Context& context) {
    Value result = Value{};  // Default empty value

    while (true) {
        // Evaluate the condition
        Value condition_value = condition_->evaluate(context);

        // Check if condition is boolean
        if (!std::holds_alternative<Bool>(condition_value)) {
            throw TypeMismatchError("While condition must evaluate to Bool, got " +
                                    o2l::getTypeName(condition_value));
        }

        // Check if condition is true
        bool condition_bool = std::get<Bool>(condition_value);
        if (!condition_bool) {
            break;  // Exit loop if condition is false
        }

        // Execute the body
        result = body_->evaluate(context);

        // Note: In a more sophisticated implementation, we might want to handle
        // break/continue statements here, but for now we keep it simple
    }

    return result;  // Return the last result from the body
}

std::string WhileStatementNode::toString() const {
    return "While(" + condition_->toString() + ", " + body_->toString() + ")";
}

}  // namespace o2l