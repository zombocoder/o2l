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

#include "UnaryNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Common/StackFrameGuard.hpp"
#include "../Runtime/Value.hpp"

namespace o2l {

UnaryNode::UnaryNode(UnaryOperator op, ASTNodePtr operand, const SourceLocation& location)
    : ASTNode(location), operator_(op), operand_(std::move(operand)) {}

Value UnaryNode::evaluate(Context& context) {
    try {
        // Create stack frame for this unary operation
        STACK_FRAME_GUARD(context, "unary_" + operatorToString(operator_), "UnaryExpression",
                          *this);

        // Evaluate the operand
        Value operand_value = operand_->evaluate(context);

        switch (operator_) {
            case UnaryOperator::NOT:
                // Logical NOT: operand must be a Bool
                if (!std::holds_alternative<Bool>(operand_value)) {
                    throw TypeMismatchError("NOT operator requires a Bool operand, got " + getTypeName(operand_value));
                }
                return Bool(!std::get<Bool>(operand_value));

            case UnaryOperator::MINUS:
                // Unary minus: operand must be a numeric type
                if (std::holds_alternative<Int>(operand_value)) {
                    return Int(-std::get<Int>(operand_value));
                } else if (std::holds_alternative<Long>(operand_value)) {
                    return Long(-std::get<Long>(operand_value));
                } else if (std::holds_alternative<Float>(operand_value)) {
                    return Float(-std::get<Float>(operand_value));
                } else if (std::holds_alternative<Double>(operand_value)) {
                    return Double(-std::get<Double>(operand_value));
                } else {
                    throw TypeMismatchError("Unary minus operator requires a numeric operand");
                }
        }

        throw EvaluationError("Unknown unary operator");

    } catch (const o2lException& e) {
        // Re-throw with current context if it doesn't already have a stack trace
        if (e.getStackTrace().empty()) {
            throw EvaluationError(e.getMessage(), context);
        } else {
            throw;  // Re-throw as-is if it already has stack trace
        }
    } catch (const std::exception& e) {
        // Convert standard exceptions to our exception type with stack trace
        throw EvaluationError(std::string("Standard exception in unary expression: ") + e.what(),
                              context);
    }
}

std::string UnaryNode::toString() const {
    return "UnaryExpression(" + operatorToString(operator_) + " " + operand_->toString() + ")";
}

std::string UnaryNode::operatorToString(UnaryOperator op) const {
    switch (op) {
        case UnaryOperator::NOT:
            return "!";
        case UnaryOperator::MINUS:
            return "-";
        default:
            return "unknown";
    }
}

}  // namespace o2l