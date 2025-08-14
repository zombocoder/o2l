#include "VariableAssignmentNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

Value VariableAssignmentNode::evaluate(Context& context) {
    // Evaluate the new value
    Value new_value = value_expr_->evaluate(context);

    // Use the new reassignVariable method which handles all validation
    context.reassignVariable(variable_name_, new_value);

    return new_value;
}

std::string VariableAssignmentNode::toString() const {
    return variable_name_ + " = " + value_expr_->toString();
}

}  // namespace o2l