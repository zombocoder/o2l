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

#include "PropertyAccessNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ObjectInstance.hpp"

namespace o2l {

PropertyAccessNode::PropertyAccessNode(std::string property_name)
    : property_name_(std::move(property_name)) {}

Value PropertyAccessNode::evaluate(Context& context) {
    // Get the current 'this' object from context
    if (!context.hasThisObject()) {
        throw EvaluationError(
            "'this." + property_name_ + "' can only be used inside object methods", context);
    }

    auto this_object = context.getThisObject();
    return this_object->getProperty(property_name_);
}

std::string PropertyAccessNode::toString() const {
    return "this." + property_name_;
}

}  // namespace o2l