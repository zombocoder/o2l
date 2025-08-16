/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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