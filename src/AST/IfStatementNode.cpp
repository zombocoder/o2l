#include "IfStatementNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

IfStatementNode::IfStatementNode(ASTNodePtr condition, ASTNodePtr then_branch,
                                 ASTNodePtr else_branch)
    : condition_(std::move(condition)),
      then_branch_(std::move(then_branch)),
      else_branch_(std::move(else_branch)) {}

Value IfStatementNode::evaluate(Context& context) {
    // Evaluate the condition
    Value condition_value = condition_->evaluate(context);

    // Convert to boolean
    bool is_true = false;
    if (std::holds_alternative<Bool>(condition_value)) {
        is_true = std::get<Bool>(condition_value);
    } else if (std::holds_alternative<Int>(condition_value)) {
        is_true = std::get<Int>(condition_value) != 0;
    } else if (std::holds_alternative<Text>(condition_value)) {
        is_true = !std::get<Text>(condition_value).empty();
    } else {
        // For other types, consider them truthy if they exist
        is_true = true;
    }

    // Execute the appropriate branch
    if (is_true) {
        return then_branch_->evaluate(context);
    } else if (else_branch_) {
        return else_branch_->evaluate(context);
    } else {
        // No else branch, return void/default
        return Int(0);
    }
}

std::string IfStatementNode::toString() const {
    std::string result = "if (" + condition_->toString() + ") { " + then_branch_->toString() + " }";
    if (else_branch_) {
        result += " else { " + else_branch_->toString() + " }";
    }
    return result;
}

}  // namespace o2l