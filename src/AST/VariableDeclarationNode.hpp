#pragma once

#include <memory>
#include <string>

#include "Node.hpp"

namespace o2l {

class VariableDeclarationNode : public ASTNode {
   private:
    std::string variable_name_;
    std::string type_name_;
    ASTNodePtr initializer_;

   public:
    VariableDeclarationNode(std::string variable_name, std::string type_name,
                            ASTNodePtr initializer);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getVariableName() const {
        return variable_name_;
    }
    const std::string& getTypeName() const {
        return type_name_;
    }
    const ASTNodePtr& getInitializer() const {
        return initializer_;
    }
};

}  // namespace o2l