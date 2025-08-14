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