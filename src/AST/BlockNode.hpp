#pragma once

#include <memory>
#include <vector>

#include "Node.hpp"

namespace o2l {

class BlockNode : public ASTNode {
   private:
    std::vector<ASTNodePtr> statements_;

   public:
    explicit BlockNode(std::vector<ASTNodePtr> statements);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::vector<ASTNodePtr>& getStatements() const {
        return statements_;
    }
};

}  // namespace o2l