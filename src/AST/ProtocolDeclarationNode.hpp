#pragma once

#include <string>
#include <vector>

#include "MethodDeclarationNode.hpp"
#include "Node.hpp"

namespace o2l {

// Protocol method signature (no implementation, just signature)
struct ProtocolMethodSignature {
    std::string name;
    std::vector<Parameter> parameters;
    std::string return_type;

    ProtocolMethodSignature(std::string n, std::vector<Parameter> params, std::string ret_type)
        : name(std::move(n)), parameters(std::move(params)), return_type(std::move(ret_type)) {}
};

class ProtocolDeclarationNode : public ASTNode {
   private:
    std::string protocol_name_;
    std::vector<ProtocolMethodSignature> method_signatures_;

   public:
    ProtocolDeclarationNode(std::string name, std::vector<ProtocolMethodSignature> signatures);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getProtocolName() const {
        return protocol_name_;
    }
    const std::vector<ProtocolMethodSignature>& getMethodSignatures() const {
        return method_signatures_;
    }
};

}  // namespace o2l