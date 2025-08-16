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

#include "QualifiedIdentifierNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

Value QualifiedIdentifierNode::evaluate(Context& context) {
    std::string full_name = getFullQualifiedName();

    // First try to resolve the full qualified name
    if (context.hasVariable(full_name)) {
        return context.getVariable(full_name);
    }

    // If not found, try to resolve step by step
    // Start with the first part and see if it exists
    if (parts_.empty()) {
        throw EvaluationError("Empty qualified identifier");
    }

    // Try just the last part (for simple access within same namespace)
    std::string last_part = getLastPart();
    if (context.hasVariable(last_part)) {
        return context.getVariable(last_part);
    }

    // If neither worked, throw an error
    throw UnresolvedReferenceError("Qualified identifier '" + full_name +
                                   "' not found in current context");
}

std::string QualifiedIdentifierNode::toString() const {
    return "QualifiedIdentifier(" + getFullQualifiedName() + ")";
}

}  // namespace o2l