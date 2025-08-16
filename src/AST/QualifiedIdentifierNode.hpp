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