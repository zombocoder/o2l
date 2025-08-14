#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class PropertyAccessNode : public ASTNode {
   private:
    std::string property_name_;

   public:
    explicit PropertyAccessNode(std::string property_name);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getPropertyName() const {
        return property_name_;
    }
};

}  // namespace o2l