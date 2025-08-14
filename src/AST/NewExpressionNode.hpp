#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class NewExpressionNode : public ASTNode {
   private:
    std::string object_type_name_;
    std::vector<ASTNodePtr> constructor_args_;

   public:
    NewExpressionNode(std::string object_type_name, std::vector<ASTNodePtr> constructor_args);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getObjectTypeName() const {
        return object_type_name_;
    }
    const std::vector<ASTNodePtr>& getConstructorArgs() const {
        return constructor_args_;
    }
};

}  // namespace o2l