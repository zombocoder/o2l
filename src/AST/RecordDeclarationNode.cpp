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

#include "RecordDeclarationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/RecordType.hpp"

namespace o2l {

RecordDeclarationNode::RecordDeclarationNode(std::string name, std::vector<RecordField> fields)
    : record_name_(std::move(name)), fields_(std::move(fields)) {}

Value RecordDeclarationNode::evaluate(Context& context) {
    // Create a record type that will be used to create record instances
    auto record_type = std::make_shared<RecordType>(record_name_, fields_);

    // Register the record type in the context so it can be accessed later
    context.defineVariable(record_name_, record_type);

    return record_type;
}

std::string RecordDeclarationNode::toString() const {
    std::string result = "record " + record_name_ + " { ";
    for (size_t i = 0; i < fields_.size(); ++i) {
        if (i > 0) result += ", ";
        result += fields_[i].name + ": " + fields_[i].type;
    }
    result += " }";
    return result;
}

}  // namespace o2l