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

#include "PropertyAssignmentNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ObjectInstance.hpp"

namespace o2l {

PropertyAssignmentNode::PropertyAssignmentNode(std::string property_name, ASTNodePtr value_expr)
    : property_name_(std::move(property_name)), value_expr_(std::move(value_expr)) {}

Value PropertyAssignmentNode::evaluate(Context& context) {
    // Get the current 'this' object from context
    if (!context.hasThisObject()) {
        throw EvaluationError(
            "'this." + property_name_ + "' can only be used inside object methods", context);
    }

    // Evaluate the value expression
    Value value = value_expr_->evaluate(context);

    // Set the property on the this object
    auto this_object = context.getThisObject();
    this_object->setProperty(property_name_, value);

    // Return the assigned value
    return value;
}

std::string PropertyAssignmentNode::toString() const {
    return "this." + property_name_ + " = " + value_expr_->toString();
}

}  // namespace o2l