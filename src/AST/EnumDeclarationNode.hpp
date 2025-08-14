#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

struct EnumMember {
    std::string name;
    int value;  // Constant value for this enum member

    EnumMember(std::string n, int v) : name(std::move(n)), value(v) {}
};

class EnumDeclarationNode : public ASTNode {
   private:
    std::string enum_name_;
    std::vector<EnumMember> members_;

   public:
    EnumDeclarationNode(std::string name, std::vector<EnumMember> members);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getEnumName() const {
        return enum_name_;
    }
    const std::vector<EnumMember>& getMembers() const {
        return members_;
    }
};

}  // namespace o2l