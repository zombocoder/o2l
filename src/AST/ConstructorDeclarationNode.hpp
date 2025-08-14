#pragma once

#include <string>
#include <vector>

#include "MethodDeclarationNode.hpp"
#include "Node.hpp"

namespace o2l {

class ConstructorDeclarationNode : public ASTNode {
   private:
    std::vector<Parameter> parameters_;
    ASTNodePtr body_;

   public:
    ConstructorDeclarationNode(std::vector<Parameter> parameters, ASTNodePtr body);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::vector<Parameter>& getParameters() const {
        return parameters_;
    }
    const ASTNodePtr& getBody() const {
        return body_;
    }
};

}  // namespace o2l