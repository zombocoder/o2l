#pragma once

#include <memory>
#include <string>

#include "Node.hpp"

namespace o2l {

class ConstDeclarationNode : public ASTNode {
   private:
    std::string const_name_;
    std::string type_name_;  // Optional type annotation
    ASTNodePtr initializer_;

   public:
    ConstDeclarationNode(std::string const_name, std::string type_name, ASTNodePtr initializer);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getConstName() const {
        return const_name_;
    }
    const std::string& getTypeName() const {
        return type_name_;
    }
    const ASTNodePtr& getInitializer() const {
        return initializer_;
    }
};

}  // namespace o2l