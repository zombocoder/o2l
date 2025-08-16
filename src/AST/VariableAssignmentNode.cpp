/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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