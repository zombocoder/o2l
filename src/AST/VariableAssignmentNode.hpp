#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class VariableAssignmentNode : public ASTNode {
   private:
    std::string variable_name_;
    ASTNodePtr value_expr_;

   public:
    VariableAssignmentNode(const std::string& variable_name, ASTNodePtr value_expr)
        : variable_name_(variable_name), value_expr_(std::move(value_expr)) {}

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getVariableName() const {
        return variable_name_;
    }
    const ASTNode* getValueExpression() const {
        return value_expr_.get();
    }
    const ASTNodePtr& getValueExpressionPtr() const {
        return value_expr_;
    }
};

}  // namespace o2l