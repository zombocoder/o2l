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

#include "ProtocolDeclarationNode.hpp"

#include "../Runtime/ProtocolInstance.hpp"

namespace o2l {

ProtocolDeclarationNode::ProtocolDeclarationNode(std::string name,
                                                 std::vector<ProtocolMethodSignature> signatures)
    : protocol_name_(std::move(name)), method_signatures_(std::move(signatures)) {}

Value ProtocolDeclarationNode::evaluate(Context& context) {
    // Create a protocol instance that defines the interface
    auto protocol_instance = std::make_shared<ProtocolInstance>(protocol_name_, method_signatures_);
    return protocol_instance;
}

std::string ProtocolDeclarationNode::toString() const {
    std::string result = "Protocol(" + protocol_name_ + " with " +
                         std::to_string(method_signatures_.size()) + " methods)";
    return result;
}

}  // namespace o2l