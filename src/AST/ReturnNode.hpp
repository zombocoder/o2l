#pragma once

#include "Node.hpp"

namespace o2l {

class ReturnNode : public ASTNode {
   private:
    ASTNodePtr expression_;

   public:
    explicit ReturnNode(ASTNodePtr expression);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getExpression() const {
        return expression_;
    }
};

}  // namespace o2l