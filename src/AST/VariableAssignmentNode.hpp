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

class VariableAssignmentNode : public ASTNode {
   private:
    std::string variable_name_;
    ASTNodePtr value_expr_;

   public:
    VariableAssignmentNode(const std::string& variable_name, ASTNodePtr value_expr)
        : variable_name_(variable_name), value_expr_(std::move(value_expr)) {}

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getVariableName() const {
        return variable_name_;
    }
    const ASTNode* getValueExpression() const {
        return value_expr_.get();
    }
    const ASTNodePtr& getValueExpressionPtr() const {
        return value_expr_;
    }
};

}  // namespace o2l