#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class MemberAccessNode : public ASTNode {
   private:
    ASTNodePtr object_expr_;  // Expression that evaluates to an object/enum/record
    std::string member_name_;

   public:
    MemberAccessNode(ASTNodePtr object_expr, std::string member_name);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getObjectExpression() const {
        return object_expr_;
    }
    const std::string& getMemberName() const {
        return member_name_;
    }
};

}  // namespace o2l