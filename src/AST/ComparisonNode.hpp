#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

enum class ComparisonOperator {
    EQUAL,         // ==
    NOT_EQUAL,     // !=
    LESS_THAN,     // <
    GREATER_THAN,  // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL  // >=
};

class ComparisonNode : public ASTNode {
   private:
    ASTNodePtr left_;
    ComparisonOperator operator_;
    ASTNodePtr right_;

   public:
    ComparisonNode(ASTNodePtr left, ComparisonOperator op, ASTNodePtr right,
                   const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getLeft() const {
        return left_;
    }
    const ASTNodePtr& getRight() const {
        return right_;
    }
    ComparisonOperator getOperator() const {
        return operator_;
    }

   private:
    bool compareValues(const Value& left, const Value& right, ComparisonOperator op,
                       Context& context);
    std::string operatorToString(ComparisonOperator op) const;
};

}  // namespace o2l