#pragma once

#include "Node.hpp"

namespace o2l {

class ThisNode : public ASTNode {
   public:
    ThisNode() = default;

    Value evaluate(Context& context) override;
    std::string toString() const override;
};

}  // namespace o2l