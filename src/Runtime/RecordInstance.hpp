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