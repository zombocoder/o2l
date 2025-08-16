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

#include "ConstDeclarationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

ConstDeclarationNode::ConstDeclarationNode(std::string const_name, std::string type_name,
                                           ASTNodePtr initializer)
    : const_name_(std::move(const_name)),
      type_name_(std::move(type_name)),
      initializer_(std::move(initializer)) {}

Value ConstDeclarationNode::evaluate(Context& context) {
    // Evaluate the initializer expression
    Value value = initializer_->evaluate(context);

    // TODO: Add type checking if type_name_ is provided

    // Define the constant in the current scope with immutability protection
    context.defineConstant(const_name_, value);

    // Return the assigned value
    return value;
}

std::string ConstDeclarationNode::toString() const {
    std::string result = "const " + const_name_;
    if (!type_name_.empty()) {
        result += ": " + type_name_;
    }
    result += " = " + initializer_->toString();
    return result;
}

}  // namespace o2l