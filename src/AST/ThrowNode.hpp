#pragma once

#include "../Runtime/Value.hpp"
#include "Node.hpp"

namespace o2l {

class ThrowNode : public ASTNode {
   private:
    ASTNodePtr expression_;  // The value/expression to throw

   public:
    explicit ThrowNode(ASTNodePtr expression, const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    // Getter for the thrown expression
    const ASTNodePtr& getExpression() const {
        return expression_;
    }
};

}  // namespace o2l