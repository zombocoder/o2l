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
#include <vector>

#include "Node.hpp"

namespace o2l {

struct RecordField {
    std::string name;
    std::string type;  // Type name for the field

    RecordField(std::string n, std::string t) : name(std::move(n)), type(std::move(t)) {}
};

class RecordDeclarationNode : public ASTNode {
   private:
    std::string record_name_;
    std::vector<RecordField> fields_;

   public:
    RecordDeclarationNode(std::string name, std::vector<RecordField> fields);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getRecordName() const {
        return record_name_;
    }
    const std::vector<RecordField>& getFields() const {
        return fields_;
    }
};

}  // namespace o2l