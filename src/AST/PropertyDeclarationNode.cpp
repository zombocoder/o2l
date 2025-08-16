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

#include "PropertyDeclarationNode.hpp"

#include "../Runtime/Context.hpp"

namespace o2l {

PropertyDeclarationNode::PropertyDeclarationNode(std::string property_name, std::string type_name)
    : property_name_(std::move(property_name)), type_name_(std::move(type_name)) {}

Value PropertyDeclarationNode::evaluate(Context& context) {
    // Property declarations are processed during object creation
    // This node doesn't need to do anything during evaluation
    return Int(0);
}

std::string PropertyDeclarationNode::toString() const {
    return "property " + property_name_ + ": " + type_name_;
}

}  // namespace o2l