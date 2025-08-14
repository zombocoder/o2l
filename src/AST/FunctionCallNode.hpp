#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class FunctionCallNode : public ASTNode {
   private:
    std::string function_name_;
    std::vector<ASTNodePtr> arguments_;

   public:
    FunctionCallNode(std::string function_name, std::vector<ASTNodePtr> arguments);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getFunctionName() const {
        return function_name_;
    }
    const std::vector<ASTNodePtr>& getArguments() const {
        return arguments_;
    }
};

}  // namespace o2l