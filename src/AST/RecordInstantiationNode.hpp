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

#include "Node.hpp"

namespace o2l {

struct RecordFieldAssignment {
    std::string field_name;
    ASTNodePtr value_expr;

    RecordFieldAssignment(std::string name, ASTNodePtr expr)
        : field_name(std::move(name)), value_expr(std::move(expr)) {}
};

class RecordInstantiationNode : public ASTNode {
   private:
    std::string record_type_name_;
    std::vector<RecordFieldAssignment> field_assignments_;

   public:
    RecordInstantiationNode(std::string type_name, std::vector<RecordFieldAssignment> assignments);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getRecordTypeName() const {
        return record_type_name_;
    }
    const std::vector<RecordFieldAssignment>& getFieldAssignments() const {
        return field_assignments_;
    }
};

}  // namespace o2l