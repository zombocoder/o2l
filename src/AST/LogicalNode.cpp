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

#include "LogicalNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Common/StackFrameGuard.hpp"

namespace o2l {

LogicalNode::LogicalNode(ASTNodePtr left, LogicalOperator op, ASTNodePtr right,
                         const SourceLocation& location)
    : ASTNode(location), left_(std::move(left)), operator_(op), right_(std::move(right)) {}

Value LogicalNode::evaluate(Context& context) {
    try {
        // Create stack frame for this logical operation
        STACK_FRAME_GUARD(context, "logical_" + operatorToString(operator_), "LogicalExpression",
                          *this);

        // Evaluate left operand first
        Value left_value = left_->evaluate(context);

        // Check if left operand is a boolean
        if (!std::holds_alternative<Bool>(left_value)) {
            throw TypeMismatchError("Left operand of logical operator must be a Bool");
        }

        bool left_bool = std::get<Bool>(left_value);

        // Short-circuit evaluation
        if (operator_ == LogicalOperator::AND) {
            // For AND: if left is false, return false without evaluating right
            if (!left_bool) {
                return Bool(false);
            }
            // If left is true, evaluate right and return its value
            Value right_value = right_->evaluate(context);
            if (!std::holds_alternative<Bool>(right_value)) {
                throw TypeMismatchError("Right operand of logical AND must be a Bool");
            }
            return Bool(std::get<Bool>(right_value));

        } else if (operator_ == LogicalOperator::OR) {
            // For OR: if left is true, return true without evaluating right
            if (left_bool) {
                return Bool(true);
            }
            // If left is false, evaluate right and return its value
            Value right_value = right_->evaluate(context);
            if (!std::holds_alternative<Bool>(right_value)) {
                throw TypeMismatchError("Right operand of logical OR must be a Bool");
            }
            return Bool(std::get<Bool>(right_value));
        }

        throw EvaluationError("Unknown logical operator");

    } catch (const o2lException& e) {
        // Re-throw with current context if it doesn't already have a stack trace
        if (e.getStackTrace().empty()) {
            throw EvaluationError(e.getMessage(), context);
        } else {
            throw;  // Re-throw as-is if it already has stack trace
        }
    } catch (const std::exception& e) {
        // Convert standard exceptions to our exception type with stack trace
        throw EvaluationError(std::string("Standard exception in logical expression: ") + e.what(),
                              context);
    }
}

std::string LogicalNode::toString() const {
    return "LogicalExpression(" + left_->toString() + " " + operatorToString(operator_) + " " +
           right_->toString() + ")";
}

std::string LogicalNode::operatorToString(LogicalOperator op) const {
    switch (op) {
        case LogicalOperator::AND:
            return "&&";
        case LogicalOperator::OR:
            return "||";
        default:
            return "unknown";
    }
}

}  // namespace o2l