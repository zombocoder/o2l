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

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../AST/MethodDeclarationNode.hpp"  // For Parameter struct
#include "Value.hpp"

namespace o2l {

// Forward declarations
struct ProtocolMethodSignature;
class ObjectInstance;

class ProtocolInstance {
   private:
    std::string protocol_name_;
    std::vector<ProtocolMethodSignature> method_signatures_;

   public:
    explicit ProtocolInstance(const std::string& name,
                              const std::vector<ProtocolMethodSignature>& signatures);

    const std::string& getName() const {
        return protocol_name_;
    }
    const std::vector<ProtocolMethodSignature>& getMethodSignatures() const {
        return method_signatures_;
    }

    // Check if an object implements this protocol
    bool isImplementedBy(const std::shared_ptr<ObjectInstance>& object) const;

    // Validate method signature compatibility
    bool validateMethodSignature(const std::string& method_name,
                                 const std::vector<Parameter>& params,
                                 const std::string& return_type) const;

    // Get detailed validation errors (for debugging)
    std::vector<std::string> getValidationErrors(
        const std::shared_ptr<ObjectInstance>& object) const;
};

}  // namespace o2l