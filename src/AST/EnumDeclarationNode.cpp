#include "EnumDeclarationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/EnumInstance.hpp"

namespace o2l {

EnumDeclarationNode::EnumDeclarationNode(std::string name, std::vector<EnumMember> members)
    : enum_name_(std::move(name)), members_(std::move(members)) {}

Value EnumDeclarationNode::evaluate(Context& context) {
    // Create an enum instance that will hold all the constant values
    auto enum_instance = std::make_shared<EnumInstance>(enum_name_, members_);

    // Register the enum in the context so it can be accessed later
    context.defineVariable(enum_name_, enum_instance);

    return enum_instance;
}

std::string EnumDeclarationNode::toString() const {
    std::string result = "enum " + enum_name_ + " { ";
    for (size_t i = 0; i < members_.size(); ++i) {
        if (i > 0) result += ", ";
        result += members_[i].name + " = " + std::to_string(members_[i].value);
    }
    result += " }";
    return result;
}

}  // namespace o2l