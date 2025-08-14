#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class IdentifierNode : public ASTNode {
   private:
    std::string name_;

   public:
    explicit IdentifierNode(std::string name) : name_(std::move(name)) {}

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getName() const {
        return name_;
    }
};

}  // namespace o2l