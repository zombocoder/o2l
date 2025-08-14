#include "ImportNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

Value ImportNode::evaluate(Context& context) {
    // Import evaluation will be handled by the interpreter
    // For now, just return a placeholder value
    return Int(0);
}

std::string ImportNode::toString() const {
    std::string result = "Import(" + import_path_.getFullPath();
    if (import_path_.is_wildcard()) {
        result += ".*";
    }
    result += ")";
    return result;
}

}  // namespace o2l