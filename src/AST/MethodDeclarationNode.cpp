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

#include "MethodDeclarationNode.hpp"

namespace o2l {

MethodDeclarationNode::MethodDeclarationNode(std::string name, std::vector<Parameter> parameters,
                                             std::string return_type, ASTNodePtr body,
                                             bool is_external, const SourceLocation& location)
    : ASTNode(location),
      method_name_(std::move(name)),
      parameters_(std::move(parameters)),
      return_type_(std::move(return_type)),
      body_(std::move(body)),
      is_external_(is_external) {}

Value MethodDeclarationNode::evaluate(Context& context) {
    // Method declarations don't evaluate to values directly
    // They are processed when creating objects
    return Int(0);
}

std::string MethodDeclarationNode::toString() const {
    std::string params = "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) params += ", ";
        params += parameters_[i].name + ": " + parameters_[i].type;
    }
    params += ")";

    std::string modifier = is_external_ ? "@external " : "";
    return modifier + "Method(" + method_name_ + params + ": " + return_type_ + ")";
}

}  // namespace o2l