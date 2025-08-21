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
        try {
            result = body_->evaluate(context);
        } catch (const BreakException&) {
            // Break statement was executed, exit the while loop
            break;
        }
    }

    return result;  // Return the last result from the body
}

std::string WhileStatementNode::toString() const {
    return "While(" + condition_->toString() + ", " + body_->toString() + ")";
}

}  // namespace o2l