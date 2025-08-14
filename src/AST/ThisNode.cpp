#include "ThisNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

Value ThisNode::evaluate(Context& context) {
    if (!context.hasThisObject()) {
        throw EvaluationError("'this' can only be used inside object methods", context);
    }

    // Return the 'this' object as a value
    auto this_object = context.getThisObject();
    return Value(this_object);
}

std::string ThisNode::toString() const {
    return "this";
}

}  // namespace o2l