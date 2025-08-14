#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class MethodCallNode : public ASTNode {
   private:
    ASTNodePtr object_;
    std::string method_name_;
    std::vector<ASTNodePtr> arguments_;

   public:
    MethodCallNode(ASTNodePtr object, std::string method_name, std::vector<ASTNodePtr> arguments,
                   const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getObject() const {
        return object_;
    }
    const std::string& getMethodName() const {
        return method_name_;
    }
    const std::vector<ASTNodePtr>& getArguments() const {
        return arguments_;
    }
};

}  // namespace o2l