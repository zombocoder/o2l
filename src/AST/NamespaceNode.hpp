#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

class NamespaceNode : public ASTNode {
   private:
    std::vector<std::string> namespace_path_;  // e.g., ["mylib", "collections"]
    std::vector<ASTNodePtr> body_;             // Objects, enums, etc. inside namespace

   public:
    NamespaceNode(std::vector<std::string> namespace_path, std::vector<ASTNodePtr> body)
        : namespace_path_(std::move(namespace_path)), body_(std::move(body)) {}

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::vector<std::string>& getNamespacePath() const {
        return namespace_path_;
    }
    const std::vector<ASTNodePtr>& getBody() const {
        return body_;
    }

    std::string getFullNamespace() const {
        std::string result;
        for (size_t i = 0; i < namespace_path_.size(); ++i) {
            if (i > 0) result += ".";
            result += namespace_path_[i];
        }
        return result;
    }
};

}  // namespace o2l