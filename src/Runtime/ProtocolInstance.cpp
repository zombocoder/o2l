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

#include "ProtocolInstance.hpp"

#include "../AST/ProtocolDeclarationNode.hpp"
#include "../Common/Exceptions.hpp"
#include "ObjectInstance.hpp"

namespace o2l {

ProtocolInstance::ProtocolInstance(const std::string& name,
                                   const std::vector<ProtocolMethodSignature>& signatures)
    : protocol_name_(name), method_signatures_(signatures) {}

bool ProtocolInstance::isImplementedBy(const std::shared_ptr<ObjectInstance>& object) const {
    // Check for null object
    if (!object) {
        return false;
    }

    // Check that the object has all required methods with correct signatures
    for (const auto& protocol_signature : method_signatures_) {
        // 1. Check method existence
        if (!object->hasMethod(protocol_signature.name)) {
            return false;
        }

        // 2. Deep signature validation - check if object has signature information
        if (!object->hasMethodSignature(protocol_signature.name)) {
            // If no signature information is available, we can't validate deeply
            // This maintains backward compatibility with methods added without signature info
            continue;
        }

        const MethodSignature* object_signature =
            object->getMethodSignature(protocol_signature.name);
        if (!object_signature) {
            continue;  // Should not happen, but be defensive
        }

        // 3. Validate parameter count
        if (object_signature->parameters.size() != protocol_signature.parameters.size()) {
            return false;
        }

        // 4. Validate parameter types
        for (size_t i = 0; i < protocol_signature.parameters.size(); ++i) {
            if (object_signature->parameters[i].type != protocol_signature.parameters[i].type) {
                return false;
            }
            // Parameter names don't need to match, only types
        }

        // 5. Validate return type
        if (object_signature->return_type != protocol_signature.return_type) {
            return false;
        }

        // Note: External visibility is not enforced for protocol compliance
        // A protocol method can be implemented as internal and still satisfy the protocol
    }
    return true;
}

bool ProtocolInstance::validateMethodSignature(const std::string& method_name,
                                               const std::vector<Parameter>& params,
                                               const std::string& return_type) const {
    // Find the method signature in the protocol
    for (const auto& signature : method_signatures_) {
        if (signature.name == method_name) {
            // Check parameter count
            if (signature.parameters.size() != params.size()) {
                return false;
            }

            // Check parameter types
            for (size_t i = 0; i < params.size(); ++i) {
                if (signature.parameters[i].type != params[i].type) {
                    return false;
                }
            }

            // Check return type
            if (signature.return_type != return_type) {
                return false;
            }

            return true;
        }
    }

    // Method not found in protocol
    return false;
}

std::vector<std::string> ProtocolInstance::getValidationErrors(
    const std::shared_ptr<ObjectInstance>& object) const {
    std::vector<std::string> errors;

    // Check for null object
    if (!object) {
        errors.push_back("Object is null - cannot validate protocol implementation");
        return errors;
    }

    for (const auto& protocol_signature : method_signatures_) {
        std::string method_name = protocol_signature.name;

        // 1. Check method existence
        if (!object->hasMethod(method_name)) {
            errors.push_back("Missing method: " + method_name);
            continue;
        }

        // 2. Check if signature information is available
        if (!object->hasMethodSignature(method_name)) {
            errors.push_back("Method '" + method_name +
                             "' lacks signature information for validation");
            continue;
        }

        const MethodSignature* object_signature = object->getMethodSignature(method_name);
        if (!object_signature) {
            errors.push_back("Method '" + method_name + "' signature could not be retrieved");
            continue;
        }

        // 3. Validate parameter count
        if (object_signature->parameters.size() != protocol_signature.parameters.size()) {
            errors.push_back("Method '" + method_name + "': parameter count mismatch (expected " +
                             std::to_string(protocol_signature.parameters.size()) + ", got " +
                             std::to_string(object_signature->parameters.size()) + ")");
            continue;
        }

        // 4. Validate parameter types
        for (size_t i = 0; i < protocol_signature.parameters.size(); ++i) {
            if (object_signature->parameters[i].type != protocol_signature.parameters[i].type) {
                errors.push_back("Method '" + method_name + "': parameter " + std::to_string(i) +
                                 " type mismatch (expected '" +
                                 protocol_signature.parameters[i].type + "', got '" +
                                 object_signature->parameters[i].type + "')");
            }
        }

        // 5. Validate return type
        if (object_signature->return_type != protocol_signature.return_type) {
            errors.push_back("Method '" + method_name + "': return type mismatch (expected '" +
                             protocol_signature.return_type + "', got '" +
                             object_signature->return_type + "')");
        }
    }

    return errors;
}

}  // namespace o2l