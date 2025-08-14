#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class EnumAccessNode : public ASTNode {
   private:
    std::string enum_name_;
    std::string member_name_;

   public:
    EnumAccessNode(std::string enum_name, std::string member_name);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getEnumName() const {
        return enum_name_;
    }
    const std::string& getMemberName() const {
        return member_name_;
    }
};

}  // namespace o2l