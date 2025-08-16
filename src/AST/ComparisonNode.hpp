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

#include "Node.hpp"

namespace o2l {

enum class ComparisonOperator {
    EQUAL,         // ==
    NOT_EQUAL,     // !=
    LESS_THAN,     // <
    GREATER_THAN,  // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL  // >=
};

class ComparisonNode : public ASTNode {
   private:
    ASTNodePtr left_;
    ComparisonOperator operator_;
    ASTNodePtr right_;

   public:
    ComparisonNode(ASTNodePtr left, ComparisonOperator op, ASTNodePtr right,
                   const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getLeft() const {
        return left_;
    }
    const ASTNodePtr& getRight() const {
        return right_;
    }
    ComparisonOperator getOperator() const {
        return operator_;
    }

   private:
    bool compareValues(const Value& left, const Value& right, ComparisonOperator op,
                       Context& context);
    std::string operatorToString(ComparisonOperator op) const;
};

}  // namespace o2l