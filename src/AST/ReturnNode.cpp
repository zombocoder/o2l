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

#include "ReturnNode.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

ReturnNode::ReturnNode(ASTNodePtr expression) : expression_(std::move(expression)) {}

Value ReturnNode::evaluate(Context& context) {
    Value return_value;

    if (expression_) {
        return_value = expression_->evaluate(context);
    } else {
        // Return default value if no expression
        return_value = Int(0);
    }

    // Throw ReturnException to cause early exit from method execution
    throw ReturnException(return_value);
}

std::string ReturnNode::toString() const {
    if (expression_) {
        return "Return(" + expression_->toString() + ")";
    }
    return "Return()";
}

}  // namespace o2l