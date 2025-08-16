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

#include "EnumInstance.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

EnumInstance::EnumInstance(std::string name, std::vector<EnumMember> members)
    : enum_name_(std::move(name)), members_(std::move(members)) {
    // Build the name-to-value mapping for fast lookup
    for (const auto& member : members_) {
        name_to_value_[member.name] = member.value;
    }
}

int EnumInstance::getMemberValue(const std::string& member_name) const {
    auto it = name_to_value_.find(member_name);
    if (it == name_to_value_.end()) {
        throw EvaluationError("Enum '" + enum_name_ + "' has no member '" + member_name + "'");
    }
    return it->second;
}

bool EnumInstance::hasMember(const std::string& member_name) const {
    return name_to_value_.find(member_name) != name_to_value_.end();
}

std::vector<std::string> EnumInstance::getMemberNames() const {
    std::vector<std::string> names;
    for (const auto& member : members_) {
        names.push_back(member.name);
    }
    return names;
}

std::string EnumInstance::toString() const {
    std::string result = "enum " + enum_name_ + " { ";
    for (size_t i = 0; i < members_.size(); ++i) {
        if (i > 0) result += ", ";
        result += members_[i].name + " = " + std::to_string(members_[i].value);
    }
    result += " }";
    return result;
}

}  // namespace o2l