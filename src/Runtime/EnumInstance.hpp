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