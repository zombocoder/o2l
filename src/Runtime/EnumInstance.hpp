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

#include "../AST/EnumDeclarationNode.hpp"
#include "Value.hpp"

namespace o2l {

class EnumInstance {
   private:
    std::string enum_name_;
    std::vector<EnumMember> members_;
    std::unordered_map<std::string, int> name_to_value_;

   public:
    EnumInstance(std::string name, std::vector<EnumMember> members);

    // Get enum member value by name
    int getMemberValue(const std::string& member_name) const;

    // Check if member exists
    bool hasMember(const std::string& member_name) const;

    // Get all member names
    std::vector<std::string> getMemberNames() const;

    // Get enum name
    const std::string& getEnumName() const {
        return enum_name_;
    }

    // String representation
    std::string toString() const;
};

}  // namespace o2l