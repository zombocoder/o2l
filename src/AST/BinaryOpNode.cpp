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

#include "BinaryOpNode.hpp"

#include <cmath>

#include "../Common/Exceptions.hpp"
#include "../Common/StackFrameGuard.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

BinaryOpNode::BinaryOpNode(ASTNodePtr left, BinaryOperator op, ASTNodePtr right,
                           const SourceLocation& location)
    : ASTNode(location), left_(std::move(left)), operator_(op), right_(std::move(right)) {}

Value BinaryOpNode::evaluate(Context& context) {
    // Add stack frame for this binary operation
    STACK_FRAME_GUARD(context, "binary_operation", "expression", *this);

    Value left_val = left_->evaluate(context);
    Value right_val = right_->evaluate(context);

    // Handle integer operations
    if (holds_Int_Value(left_val) && holds_Int_Value(right_val)) {
        Int left_int = get_Int_Value(left_val);
        Int right_int = get_Int_Value(right_val);

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Int(left_int + right_int);
            case BinaryOperator::MINUS:
                return Int(left_int - right_int);
            case BinaryOperator::MULTIPLY:
                return Int(left_int * right_int);
            case BinaryOperator::DIVIDE:
                if (right_int == 0) {
                    throw EvaluationError("Division by zero", context);
                }
                return Int(left_int / right_int);
            case BinaryOperator::MODULO:
                if (right_int == 0) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Int(left_int % right_int);
            case BinaryOperator::BITWISE_AND:
                return Int(left_int & right_int);
            case BinaryOperator::BITWISE_OR:
                return Int(left_int | right_int);
            case BinaryOperator::BITWISE_XOR:
                return Int(left_int ^ right_int);
            case BinaryOperator::LSHIFT:
                return Int(left_int << right_int);
            case BinaryOperator::RSHIFT:
                return Int(left_int >> right_int);
        }
    }

    // Handle long operations
    if (holds_Long_Value(left_val) && holds_Long_Value(right_val)) {
        Long left_long = get_Long_Value(left_val);
        Long right_long = get_Long_Value(right_val);

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Long(left_long + right_long);
            case BinaryOperator::MINUS:
                return Long(left_long - right_long);
            case BinaryOperator::MULTIPLY:
                return Long(left_long * right_long);
            case BinaryOperator::DIVIDE:
                if (right_long == 0) {
                    throw EvaluationError("Division by zero", context);
                }
                return Long(left_long / right_long);
            case BinaryOperator::MODULO:
                if (right_long == 0) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Long(left_long % right_long);
            case BinaryOperator::BITWISE_AND:
                return Long(left_long & right_long);
            case BinaryOperator::BITWISE_OR:
                return Long(left_long | right_long);
            case BinaryOperator::BITWISE_XOR:
                return Long(left_long ^ right_long);
            case BinaryOperator::LSHIFT:
                return Long(left_long << right_long);
            case BinaryOperator::RSHIFT:
                return Long(left_long >> right_long);
        }
    }

    // Handle float operations
    if (std::holds_alternative<Float>(left_val) && std::holds_alternative<Float>(right_val)) {
        Float left_float = std::get<Float>(left_val);
        Float right_float = std::get<Float>(right_val);

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Float(left_float + right_float);
            case BinaryOperator::MINUS:
                return Float(left_float - right_float);
            case BinaryOperator::MULTIPLY:
                return Float(left_float * right_float);
            case BinaryOperator::DIVIDE:
                if (right_float == 0.0f) {
                    throw EvaluationError("Division by zero", context);
                }
                return Float(left_float / right_float);
            case BinaryOperator::MODULO:
                if (right_float == 0.0f) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Float(std::fmod(left_float, right_float));
        }
    }

    // Handle double operations
    if (std::holds_alternative<Double>(left_val) && std::holds_alternative<Double>(right_val)) {
        Double left_double = std::get<Double>(left_val);
        Double right_double = std::get<Double>(right_val);

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Double(left_double + right_double);
            case BinaryOperator::MINUS:
                return Double(left_double - right_double);
            case BinaryOperator::MULTIPLY:
                return Double(left_double * right_double);
            case BinaryOperator::DIVIDE:
                if (right_double == 0.0) {
                    throw EvaluationError("Division by zero", context);
                }
                return Double(left_double / right_double);
            case BinaryOperator::MODULO:
                if (right_double == 0.0) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Double(std::fmod(left_double, right_double));
        }
    }

    // Handle mixed integer operations - Int + Long = Long
    if ((holds_Int_Value(left_val) && holds_Long_Value(right_val)) ||
        (holds_Long_Value(left_val) && holds_Int_Value(right_val))) {
        Long left_val_l = holds_Long_Value(left_val)
                              ? get_Long_Value(left_val)
                              : static_cast<Long>(get_Int_Value(left_val));
        Long right_val_l = holds_Long_Value(right_val)
                               ? get_Long_Value(right_val)
                               : static_cast<Long>(get_Int_Value(right_val));

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Long(left_val_l + right_val_l);
            case BinaryOperator::MINUS:
                return Long(left_val_l - right_val_l);
            case BinaryOperator::MULTIPLY:
                return Long(left_val_l * right_val_l);
            case BinaryOperator::DIVIDE:
                if (right_val_l == 0) {
                    throw EvaluationError("Division by zero", context);
                }
                return Long(left_val_l / right_val_l);
            case BinaryOperator::MODULO:
                if (right_val_l == 0) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Long(left_val_l % right_val_l);
            case BinaryOperator::BITWISE_AND:
                return Long(left_val_l & right_val_l);
            case BinaryOperator::BITWISE_OR:
                return Long(left_val_l | right_val_l);
            case BinaryOperator::BITWISE_XOR:
                return Long(left_val_l ^ right_val_l);
            case BinaryOperator::LSHIFT:
                return Long(left_val_l << right_val_l);
            case BinaryOperator::RSHIFT:
                return Long(left_val_l >> right_val_l);
        }
    }

    // Handle mixed numeric operations - promote to highest precision
    // Int + Float = Float, Long + Float = Float
    if ((holds_Int_Value(left_val) && std::holds_alternative<Float>(right_val)) ||
        (std::holds_alternative<Float>(left_val) && holds_Int_Value(right_val)) ||
        (holds_Long_Value(left_val) && std::holds_alternative<Float>(right_val)) ||
        (std::holds_alternative<Float>(left_val) && holds_Long_Value(right_val))) {
        Float left_val_f;
        Float right_val_f;

        if (std::holds_alternative<Float>(left_val)) {
            left_val_f = std::get<Float>(left_val);
        } else if (holds_Long_Value(left_val)) {
            left_val_f = static_cast<Float>(get_Long_Value(left_val));
        } else {
            left_val_f = static_cast<Float>(get_Int_Value(left_val));
        }

        if (std::holds_alternative<Float>(right_val)) {
            right_val_f = std::get<Float>(right_val);
        } else if (holds_Long_Value(right_val)) {
            right_val_f = static_cast<Float>(get_Long_Value(right_val));
        } else {
            right_val_f = static_cast<Float>(get_Int_Value(right_val));
        }

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Float(left_val_f + right_val_f);
            case BinaryOperator::MINUS:
                return Float(left_val_f - right_val_f);
            case BinaryOperator::MULTIPLY:
                return Float(left_val_f * right_val_f);
            case BinaryOperator::DIVIDE:
                if (right_val_f == 0.0f) {
                    throw EvaluationError("Division by zero", context);
                }
                return Float(left_val_f / right_val_f);
            case BinaryOperator::MODULO:
                if (right_val_f == 0.0f) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Float(std::fmod(left_val_f, right_val_f));
        }
    }

    // Int + Double = Double, Long + Double = Double, Float + Double = Double
    if ((holds_Int_Value(left_val) && std::holds_alternative<Double>(right_val)) ||
        (std::holds_alternative<Double>(left_val) && holds_Int_Value(right_val)) ||
        (holds_Long_Value(left_val) && std::holds_alternative<Double>(right_val)) ||
        (std::holds_alternative<Double>(left_val) && holds_Long_Value(right_val)) ||
        (std::holds_alternative<Float>(left_val) && std::holds_alternative<Double>(right_val)) ||
        (std::holds_alternative<Double>(left_val) && std::holds_alternative<Float>(right_val))) {
        Double left_val_d;
        Double right_val_d;

        if (std::holds_alternative<Double>(left_val)) {
            left_val_d = std::get<Double>(left_val);
        } else if (std::holds_alternative<Float>(left_val)) {
            left_val_d = static_cast<Double>(std::get<Float>(left_val));
        } else if (holds_Long_Value(left_val)) {
            left_val_d = static_cast<Double>(get_Long_Value(left_val));
        } else {
            left_val_d = static_cast<Double>(get_Int_Value(left_val));
        }

        if (std::holds_alternative<Double>(right_val)) {
            right_val_d = std::get<Double>(right_val);
        } else if (std::holds_alternative<Float>(right_val)) {
            right_val_d = static_cast<Double>(std::get<Float>(right_val));
        } else if (holds_Long_Value(right_val)) {
            right_val_d = static_cast<Double>(get_Long_Value(right_val));
        } else {
            right_val_d = static_cast<Double>(get_Int_Value(right_val));
        }

        switch (operator_) {
            case BinaryOperator::PLUS:
                return Double(left_val_d + right_val_d);
            case BinaryOperator::MINUS:
                return Double(left_val_d - right_val_d);
            case BinaryOperator::MULTIPLY:
                return Double(left_val_d * right_val_d);
            case BinaryOperator::DIVIDE:
                if (right_val_d == 0.0) {
                    throw EvaluationError("Division by zero", context);
                }
                return Double(left_val_d / right_val_d);
            case BinaryOperator::MODULO:
                if (right_val_d == 0.0) {
                    throw EvaluationError("Modulo by zero", context);
                }
                return Double(std::fmod(left_val_d, right_val_d));
        }
    }

    // Handle string concatenation
    if (std::holds_alternative<Text>(left_val) && std::holds_alternative<Text>(right_val) &&
        operator_ == BinaryOperator::PLUS) {
        Text left_text = std::get<Text>(left_val);
        Text right_text = std::get<Text>(right_val);
        return Text(left_text + right_text);
    }

    throw EvaluationError("Unsupported binary operation for these types", context);
}

std::string BinaryOpNode::toString() const {
    std::string op_str;
    switch (operator_) {
        case BinaryOperator::PLUS:
            op_str = "+";
            break;
        case BinaryOperator::MINUS:
            op_str = "-";
            break;
        case BinaryOperator::MULTIPLY:
            op_str = "*";
            break;
        case BinaryOperator::DIVIDE:
            op_str = "/";
            break;
        case BinaryOperator::MODULO:
            op_str = "%";
            break;
        case BinaryOperator::BITWISE_AND:
            op_str = "&";
            break;
        case BinaryOperator::BITWISE_OR:
            op_str = "|";
            break;
        case BinaryOperator::BITWISE_XOR:
            op_str = "^";
            break;
        case BinaryOperator::LSHIFT:
            op_str = "<<";
            break;
        case BinaryOperator::RSHIFT:
            op_str = ">>";
            break;
    }

    return "BinaryOp(" + left_->toString() + " " + op_str + " " + right_->toString() + ")";
}

}  // namespace o2l
