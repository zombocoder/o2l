#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

enum class UnaryOperator {
    NOT,   // !
    MINUS  // - (unary minus)
};

class UnaryNode : public ASTNode {
   private:
    UnaryOperator operator_;
    ASTNodePtr operand_;

   public:
    UnaryNode(UnaryOperator op, ASTNodePtr operand,
              const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getOperand() const {
        return operand_;
    }
    UnaryOperator getOperator() const {
        return operator_;
    }

   private:
    std::string operatorToString(UnaryOperator op) const;
};

}  // namespace o2l