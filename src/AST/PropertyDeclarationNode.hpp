#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class PropertyDeclarationNode : public ASTNode {
   private:
    std::string property_name_;
    std::string type_name_;

   public:
    PropertyDeclarationNode(std::string property_name, std::string type_name);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getPropertyName() const {
        return property_name_;
    }
    const std::string& getTypeName() const {
        return type_name_;
    }
};

}  // namespace o2l