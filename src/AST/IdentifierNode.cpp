#include "IdentifierNode.hpp"

#include "../Runtime/Context.hpp"

namespace o2l {

Value IdentifierNode::evaluate(Context& context) {
    return context.getVariable(name_);
}

std::string IdentifierNode::toString() const {
    return "Identifier(" + name_ + ")";
}

}  // namespace o2l