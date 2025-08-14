#include "RecordInstantiationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/RecordInstance.hpp"
#include "../Runtime/RecordType.hpp"

namespace o2l {

RecordInstantiationNode::RecordInstantiationNode(std::string type_name,
                                                 std::vector<RecordFieldAssignment> assignments)
    : record_type_name_(std::move(type_name)), field_assignments_(std::move(assignments)) {}

Value RecordInstantiationNode::evaluate(Context& context) {
    // Get the record type from context
    Value record_type_value = context.getVariable(record_type_name_);

    if (!std::holds_alternative<std::shared_ptr<RecordType>>(record_type_value)) {
        throw EvaluationError("'" + record_type_name_ + "' is not a record type", context);
    }

    auto record_type = std::get<std::shared_ptr<RecordType>>(record_type_value);

    // Evaluate all field assignments
    std::unordered_map<std::string, Value> field_values;

    for (const auto& assignment : field_assignments_) {
        Value field_value = assignment.value_expr->evaluate(context);
        field_values[assignment.field_name] = field_value;
    }

    // Create the record instance using the record type
    auto record_instance = record_type->createInstance(field_values);
    return record_instance;
}

std::string RecordInstantiationNode::toString() const {
    std::string result = record_type_name_ + "(";
    for (size_t i = 0; i < field_assignments_.size(); ++i) {
        if (i > 0) result += ", ";
        result +=
            field_assignments_[i].field_name + "=" + field_assignments_[i].value_expr->toString();
    }
    result += ")";
    return result;
}

}  // namespace o2l