#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

enum class LogicalOperator {
    AND,  // &&
    OR    // ||
};

class LogicalNode : public ASTNode {
   private:
    ASTNodePtr left_;
    LogicalOperator operator_;
    ASTNodePtr right_;

   public:
    LogicalNode(ASTNodePtr left, LogicalOperator op, ASTNodePtr right,
                const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getLeft() const {
        return left_;
    }
    const ASTNodePtr& getRight() const {
        return right_;
    }
    LogicalOperator getOperator() const {
        return operator_;
    }

   private:
    std::string operatorToString(LogicalOperator op) const;
};

}  // namespace o2l