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