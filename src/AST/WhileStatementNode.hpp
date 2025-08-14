#pragma once

#include "Node.hpp"

namespace o2l {

class WhileStatementNode : public ASTNode {
   private:
    ASTNodePtr condition_;
    ASTNodePtr body_;

   public:
    WhileStatementNode(ASTNodePtr condition, ASTNodePtr body);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    // Getters for testing
    const ASTNodePtr& getCondition() const {
        return condition_;
    }
    const ASTNodePtr& getBody() const {
        return body_;
    }
};

}  // namespace o2l