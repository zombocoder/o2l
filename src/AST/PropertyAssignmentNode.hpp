#pragma once

#include <memory>
#include <string>

#include "Node.hpp"

namespace o2l {

class PropertyAssignmentNode : public ASTNode {
   private:
    std::string property_name_;
    ASTNodePtr value_expr_;

   public:
    PropertyAssignmentNode(std::string property_name, ASTNodePtr value_expr);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getPropertyName() const {
        return property_name_;
    }
    const ASTNodePtr& getValueExpression() const {
        return value_expr_;
    }
};

}  // namespace o2l