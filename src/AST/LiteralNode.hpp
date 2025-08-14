#pragma once

#include "Node.hpp"

namespace o2l {

class LiteralNode : public ASTNode {
   private:
    Value value_;

   public:
    explicit LiteralNode(Value value) : value_(std::move(value)) {}

    Value evaluate(Context& context) override {
        return value_;
    }

    std::string toString() const override {
        return "Literal(" + valueToString(value_) + ")";
    }

    const Value& getValue() const {
        return value_;
    }
};

}  // namespace o2l