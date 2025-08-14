#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class ObjectNode : public ASTNode {
   private:
    std::string object_name_;
    std::vector<ASTNodePtr> methods_;
    std::vector<ASTNodePtr> properties_;
    ASTNodePtr constructor_;     // Optional constructor
    std::string protocol_name_;  // Optional protocol inheritance

   public:
    ObjectNode(std::string name, std::vector<ASTNodePtr> methods,
               std::vector<ASTNodePtr> properties = {}, ASTNodePtr constructor = nullptr,
               const std::string& protocol_name = "");

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getName() const {
        return object_name_;
    }
    const std::string& getProtocolName() const {
        return protocol_name_;
    }
    const std::vector<ASTNodePtr>& getMethods() const {
        return methods_;
    }
    const std::vector<ASTNodePtr>& getProperties() const {
        return properties_;
    }
    const ASTNodePtr& getConstructor() const {
        return constructor_;
    }
};

}  // namespace o2l