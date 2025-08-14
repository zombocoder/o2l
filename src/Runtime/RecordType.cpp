#include "RecordType.hpp"

#include "../Common/Exceptions.hpp"
#include "RecordInstance.hpp"

namespace o2l {

RecordType::RecordType(std::string name, std::vector<RecordField> fields)
    : record_name_(std::move(name)), fields_(std::move(fields)) {
    // Build the field name to type mapping
    for (const auto& field : fields_) {
        field_types_[field.name] = field.type;
    }
}

std::shared_ptr<RecordInstance> RecordType::createInstance(
    const std::unordered_map<std::string, Value>& field_values) const {
    // Validate that all required fields are provided
    for (const auto& field : fields_) {
        if (field_values.find(field.name) == field_values.end()) {
            throw EvaluationError("Missing required field '" + field.name + "' for record type '" +
                                  record_name_ + "'");
        }
    }

    // Validate that no extra fields are provided
    for (const auto& [name, value] : field_values) {
        if (field_types_.find(name) == field_types_.end()) {
            throw EvaluationError("Unknown field '" + name + "' for record type '" + record_name_ +
                                  "'");
        }
    }

    return std::make_shared<RecordInstance>(record_name_, field_values);
}

bool RecordType::hasField(const std::string& field_name) const {
    return field_types_.find(field_name) != field_types_.end();
}

std::string RecordType::getFieldType(const std::string& field_name) const {
    auto it = field_types_.find(field_name);
    if (it == field_types_.end()) {
        throw EvaluationError("Record type '" + record_name_ + "' has no field '" + field_name +
                              "'");
    }
    return it->second;
}

std::vector<std::string> RecordType::getFieldNames() const {
    std::vector<std::string> names;
    for (const auto& field : fields_) {
        names.push_back(field.name);
    }
    return names;
}

std::string RecordType::toString() const {
    std::string result = "record " + record_name_ + " { ";
    for (size_t i = 0; i < fields_.size(); ++i) {
        if (i > 0) result += ", ";
        result += fields_[i].name + ": " + fields_[i].type;
    }
    result += " }";
    return result;
}

}  // namespace o2l