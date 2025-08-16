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

#include "ThrowNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

ThrowNode::ThrowNode(ASTNodePtr expression, const SourceLocation& location)
    : ASTNode(location), expression_(std::move(expression)) {}

Value ThrowNode::evaluate(Context& context) {
    // Evaluate the expression to get the value to throw
    Value thrown_value = expression_->evaluate(context);

    // Throw a UserException with the evaluated value and current context
    throw UserException(thrown_value, context);
}

std::string ThrowNode::toString() const {
    return "Throw(" + expression_->toString() + ")";
}

}  // namespace o2l