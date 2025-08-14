#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class ThisMethodCallNode : public ASTNode {
   private:
    std::string method_name_;
    std::vector<ASTNodePtr> arguments_;

   public:
    ThisMethodCallNode(std::string method_name, std::vector<ASTNodePtr> arguments);

    Value evaluate(Context& context) override;
    std::string toString() const override;
};

}  // namespace o2l