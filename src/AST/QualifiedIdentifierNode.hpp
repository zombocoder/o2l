#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class QualifiedIdentifierNode : public ASTNode {
   private:
    std::vector<std::string> parts_;  // e.g., ["mylib", "collections", "List"]

   public:
    explicit QualifiedIdentifierNode(std::vector<std::string> parts) : parts_(std::move(parts)) {}

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::vector<std::string>& getParts() const {
        return parts_;
    }

    std::string getFullQualifiedName() const {
        std::string result;
        for (size_t i = 0; i < parts_.size(); ++i) {
            if (i > 0) result += ".";
            result += parts_[i];
        }
        return result;
    }

    std::string getLastPart() const {
        return parts_.empty() ? "" : parts_.back();
    }
};

}  // namespace o2l