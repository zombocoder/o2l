#pragma once

#include <string>
#include <unordered_map>

#include "Value.hpp"

namespace o2l {

class RecordInstance {
   private:
    std::string record_type_name_;
    std::unordered_map<std::string, Value> field_values_;

   public:
    RecordInstance(std::string type_name, std::unordered_map<std::string, Value> values);

    // Get field value by name
    Value getFieldValue(const std::string& field_name) const;

    // Check if field exists
    bool hasField(const std::string& field_name) const;

    // Get all field names
    std::vector<std::string> getFieldNames() const;

    // Get record type name
    const std::string& getTypeName() const {
        return record_type_name_;
    }

    // String representation
    std::string toString() const;

    // Equality comparison (structural equality based on field values)
    bool equals(const RecordInstance& other) const;
};

}  // namespace o2l