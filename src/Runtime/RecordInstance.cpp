#include "RecordInstance.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

RecordInstance::RecordInstance(std::string type_name, std::unordered_map<std::string, Value> values)
    : record_type_name_(std::move(type_name)), field_values_(std::move(values)) {}

Value RecordInstance::getFieldValue(const std::string& field_name) const {
    auto it = field_values_.find(field_name);
    if (it == field_values_.end()) {
        throw EvaluationError("Record instance of type '" + record_type_name_ + "' has no field '" +
                              field_name + "'");
    }
    return it->second;
}

bool RecordInstance::hasField(const std::string& field_name) const {
    return field_values_.find(field_name) != field_values_.end();
}

std::vector<std::string> RecordInstance::getFieldNames() const {
    std::vector<std::string> names;
    for (const auto& [name, value] : field_values_) {
        names.push_back(name);
    }
    return names;
}

std::string RecordInstance::toString() const {
    std::string result = record_type_name_ + " { ";
    bool first = true;
    for (const auto& [name, value] : field_values_) {
        if (!first) result += ", ";
        result += name + " = " + valueToString(value);
        first = false;
    }
    result += " }";
    return result;
}

bool RecordInstance::equals(const RecordInstance& other) const {
    // First check if they're the same type
    if (record_type_name_ != other.record_type_name_) {
        return false;
    }

    // Check if they have the same number of fields
    if (field_values_.size() != other.field_values_.size()) {
        return false;
    }

    // Check if all field values are equal
    for (const auto& [field_name, value] : field_values_) {
        auto other_it = other.field_values_.find(field_name);
        if (other_it == other.field_values_.end()) {
            return false;  // Field doesn't exist in other record
        }

        // Use Value equality comparison
        if (!valuesEqual(value, other_it->second)) {
            return false;
        }
    }

    return true;
}

}  // namespace o2l