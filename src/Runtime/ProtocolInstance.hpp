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