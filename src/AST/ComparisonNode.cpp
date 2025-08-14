#include "ComparisonNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Common/StackFrameGuard.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

ComparisonNode::ComparisonNode(ASTNodePtr left, ComparisonOperator op, ASTNodePtr right,
                               const SourceLocation& location)
    : ASTNode(location), left_(std::move(left)), operator_(op), right_(std::move(right)) {}

Value ComparisonNode::evaluate(Context& context) {
    // Add stack frame for this comparison operation
    STACK_FRAME_GUARD(context, "comparison", "expression", *this);

    Value left_val = left_->evaluate(context);
    Value right_val = right_->evaluate(context);

    bool result = compareValues(left_val, right_val, operator_, context);
    return Bool(result);
}

bool ComparisonNode::compareValues(const Value& left, const Value& right, ComparisonOperator op,
                                   Context& context) {
    // Handle same types
    if (left.index() == right.index()) {
        switch (left.index()) {
            case 0: {  // Int
                Int l = std::get<Int>(left);
                Int r = std::get<Int>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    case ComparisonOperator::LESS_THAN:
                        return l < r;
                    case ComparisonOperator::GREATER_THAN:
                        return l > r;
                    case ComparisonOperator::LESS_EQUAL:
                        return l <= r;
                    case ComparisonOperator::GREATER_EQUAL:
                        return l >= r;
                }
                break;
            }
            case 1: {  // Long
                Long l = std::get<Long>(left);
                Long r = std::get<Long>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    case ComparisonOperator::LESS_THAN:
                        return l < r;
                    case ComparisonOperator::GREATER_THAN:
                        return l > r;
                    case ComparisonOperator::LESS_EQUAL:
                        return l <= r;
                    case ComparisonOperator::GREATER_EQUAL:
                        return l >= r;
                }
                break;
            }
            case 2: {  // Float
                Float l = std::get<Float>(left);
                Float r = std::get<Float>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    case ComparisonOperator::LESS_THAN:
                        return l < r;
                    case ComparisonOperator::GREATER_THAN:
                        return l > r;
                    case ComparisonOperator::LESS_EQUAL:
                        return l <= r;
                    case ComparisonOperator::GREATER_EQUAL:
                        return l >= r;
                }
                break;
            }
            case 3: {  // Double
                Double l = std::get<Double>(left);
                Double r = std::get<Double>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    case ComparisonOperator::LESS_THAN:
                        return l < r;
                    case ComparisonOperator::GREATER_THAN:
                        return l > r;
                    case ComparisonOperator::LESS_EQUAL:
                        return l <= r;
                    case ComparisonOperator::GREATER_EQUAL:
                        return l >= r;
                }
                break;
            }
            case 4: {  // Text
                Text l = std::get<Text>(left);
                Text r = std::get<Text>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    case ComparisonOperator::LESS_THAN:
                        return l < r;
                    case ComparisonOperator::GREATER_THAN:
                        return l > r;
                    case ComparisonOperator::LESS_EQUAL:
                        return l <= r;
                    case ComparisonOperator::GREATER_EQUAL:
                        return l >= r;
                }
                break;
            }
            case 5: {  // Bool
                Bool l = std::get<Bool>(left);
                Bool r = std::get<Bool>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    default:
                        throw EvaluationError("Invalid comparison operator for boolean values",
                                              context);
                }
                break;
            }
            case 6: {  // Char
                Char l = std::get<Char>(left);
                Char r = std::get<Char>(right);
                switch (op) {
                    case ComparisonOperator::EQUAL:
                        return l == r;
                    case ComparisonOperator::NOT_EQUAL:
                        return l != r;
                    case ComparisonOperator::LESS_THAN:
                        return l < r;
                    case ComparisonOperator::GREATER_THAN:
                        return l > r;
                    case ComparisonOperator::LESS_EQUAL:
                        return l <= r;
                    case ComparisonOperator::GREATER_EQUAL:
                        return l >= r;
                }
                break;
            }
        }
    }

    // Handle mixed types (Int and Float)
    if ((std::holds_alternative<Int>(left) && std::holds_alternative<Float>(right)) ||
        (std::holds_alternative<Float>(left) && std::holds_alternative<Int>(right))) {
        Float l = std::holds_alternative<Int>(left) ? static_cast<Float>(std::get<Int>(left))
                                                    : std::get<Float>(left);
        Float r = std::holds_alternative<Int>(right) ? static_cast<Float>(std::get<Int>(right))
                                                     : std::get<Float>(right);

        switch (op) {
            case ComparisonOperator::EQUAL:
                return l == r;
            case ComparisonOperator::NOT_EQUAL:
                return l != r;
            case ComparisonOperator::LESS_THAN:
                return l < r;
            case ComparisonOperator::GREATER_THAN:
                return l > r;
            case ComparisonOperator::LESS_EQUAL:
                return l <= r;
            case ComparisonOperator::GREATER_EQUAL:
                return l >= r;
        }
    }

    // For different types, only equality/inequality makes sense
    if (op == ComparisonOperator::EQUAL) {
        return false;  // Different types are not equal
    } else if (op == ComparisonOperator::NOT_EQUAL) {
        return true;  // Different types are not equal
    }

    throw EvaluationError("Cannot compare values of different types", context);
}

std::string ComparisonNode::operatorToString(ComparisonOperator op) const {
    switch (op) {
        case ComparisonOperator::EQUAL:
            return "==";
        case ComparisonOperator::NOT_EQUAL:
            return "!=";
        case ComparisonOperator::LESS_THAN:
            return "<";
        case ComparisonOperator::GREATER_THAN:
            return ">";
        case ComparisonOperator::LESS_EQUAL:
            return "<=";
        case ComparisonOperator::GREATER_EQUAL:
            return ">=";
    }
    return "?";
}

std::string ComparisonNode::toString() const {
    return "(" + left_->toString() + " " + operatorToString(operator_) + " " + right_->toString() +
           ")";
}

}  // namespace o2l