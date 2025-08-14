#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

enum class BinaryOperator { PLUS, MINUS, MULTIPLY, DIVIDE, MODULO };

class BinaryOpNode : public ASTNode {
   private:
    ASTNodePtr left_;
    BinaryOperator operator_;
    ASTNodePtr right_;

   public:
    BinaryOpNode(ASTNodePtr left, BinaryOperator op, ASTNodePtr right,
                 const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getLeft() const {
        return left_;
    }
    const ASTNodePtr& getRight() const {
        return right_;
    }
    BinaryOperator getOperator() const {
        return operator_;
    }
};

}  // namespace o2l