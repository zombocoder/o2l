#include "MemberAccessNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/EnumInstance.hpp"
#include "../Runtime/ObjectInstance.hpp"
#include "../Runtime/RecordInstance.hpp"

namespace o2l {

MemberAccessNode::MemberAccessNode(ASTNodePtr object_expr, std::string member_name)
    : object_expr_(std::move(object_expr)), member_name_(std::move(member_name)) {}

Value MemberAccessNode::evaluate(Context& context) {
    // Evaluate the object expression
    Value object_value = object_expr_->evaluate(context);

    // Check if it's an enum instance (for enum member access)
    if (std::holds_alternative<std::shared_ptr<EnumInstance>>(object_value)) {
        auto enum_instance = std::get<std::shared_ptr<EnumInstance>>(object_value);

        if (!enum_instance->hasMember(member_name_)) {
            throw EvaluationError(
                "Enum '" + enum_instance->getEnumName() + "' has no member '" + member_name_ + "'",
                context);
        }

        int member_value = enum_instance->getMemberValue(member_name_);
        return Int(member_value);
    }

    // Check if it's a record instance (for record field access)
    if (std::holds_alternative<std::shared_ptr<RecordInstance>>(object_value)) {
        auto record_instance = std::get<std::shared_ptr<RecordInstance>>(object_value);

        if (!record_instance->hasField(member_name_)) {
            throw EvaluationError("Record instance has no field '" + member_name_ + "'", context);
        }

        return record_instance->getFieldValue(member_name_);
    }

    // Check if it's an object instance (for object property access)
    if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(object_value)) {
        auto object_instance = std::get<std::shared_ptr<ObjectInstance>>(object_value);

        if (!object_instance->hasProperty(member_name_)) {
            throw EvaluationError("Object has no property '" + member_name_ + "'", context);
        }

        return object_instance->getProperty(member_name_);
    }

    throw EvaluationError(
        "Cannot access member '" + member_name_ + "' on value of type " + getTypeName(object_value),
        context);
}

std::string MemberAccessNode::toString() const {
    return object_expr_->toString() + "." + member_name_;
}

}  // namespace o2l