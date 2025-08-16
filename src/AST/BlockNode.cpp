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

#include "BlockNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

BlockNode::BlockNode(std::vector<ASTNodePtr> statements) : statements_(std::move(statements)) {}

Value BlockNode::evaluate(Context& context) {
    Value result = Int(0);  // Default return value

    // Execute all statements in sequence
    for (const auto& statement : statements_) {
        try {
            result = statement->evaluate(context);
        } catch (const ReturnException& e) {
            // Return statement encountered - propagate it up to method level
            throw;
        }
    }

    return result;
}

std::string BlockNode::toString() const {
    std::string result = "Block {";
    for (size_t i = 0; i < statements_.size(); ++i) {
        if (i > 0) result += "; ";
        result += statements_[i]->toString();
    }
    result += "}";
    return result;
}

}  // namespace o2l