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