#include "EnumAccessNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/EnumInstance.hpp"

namespace o2l {

EnumAccessNode::EnumAccessNode(std::string enum_name, std::string member_name)
    : enum_name_(std::move(enum_name)), member_name_(std::move(member_name)) {}

Value EnumAccessNode::evaluate(Context& context) {
    // Get the enum instance from context
    Value enum_value = context.getVariable(enum_name_);

    if (!std::holds_alternative<std::shared_ptr<EnumInstance>>(enum_value)) {
        throw EvaluationError("'" + enum_name_ + "' is not an enum", context);
    }

    auto enum_instance = std::get<std::shared_ptr<EnumInstance>>(enum_value);

    // Get the member value
    if (!enum_instance->hasMember(member_name_)) {
        throw EvaluationError("Enum '" + enum_name_ + "' has no member '" + member_name_ + "'",
                              context);
    }

    int member_value = enum_instance->getMemberValue(member_name_);
    return Int(member_value);
}

std::string EnumAccessNode::toString() const {
    return enum_name_ + "." + member_name_;
}

}  // namespace o2l