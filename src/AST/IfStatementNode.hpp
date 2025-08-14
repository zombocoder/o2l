#pragma once

#include <memory>

#include "Node.hpp"

namespace o2l {

class IfStatementNode : public ASTNode {
   private:
    ASTNodePtr condition_;
    ASTNodePtr then_branch_;
    ASTNodePtr else_branch_;  // Optional

   public:
    IfStatementNode(ASTNodePtr condition, ASTNodePtr then_branch, ASTNodePtr else_branch = nullptr);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getCondition() const {
        return condition_;
    }
    const ASTNodePtr& getThenBranch() const {
        return then_branch_;
    }
    const ASTNodePtr& getElseBranch() const {
        return else_branch_;
    }
    bool hasElseBranch() const {
        return else_branch_ != nullptr;
    }
};

}  // namespace o2l