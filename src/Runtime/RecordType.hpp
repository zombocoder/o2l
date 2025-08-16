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
#include <vector>

#include "../AST/RecordDeclarationNode.hpp"
#include "Value.hpp"

namespace o2l {

// Forward declaration
class RecordInstance;

class RecordType {
   private:
    std::string record_name_;
    std::vector<RecordField> fields_;
    std::unordered_map<std::string, std::string> field_types_;

   public:
    RecordType(std::string name, std::vector<RecordField> fields);

    // Create a new record instance with field values
    std::shared_ptr<RecordInstance> createInstance(
        const std::unordered_map<std::string, Value>& field_values) const;

    // Check if field exists
    bool hasField(const std::string& field_name) const;

    // Get field type
    std::string getFieldType(const std::string& field_name) const;

    // Get all field names
    std::vector<std::string> getFieldNames() const;

    // Get record name
    const std::string& getRecordName() const {
        return record_name_;
    }

    // String representation
    std::string toString() const;
};

}  // namespace o2l