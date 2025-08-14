#include "RecordDeclarationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/RecordType.hpp"

namespace o2l {

RecordDeclarationNode::RecordDeclarationNode(std::string name, std::vector<RecordField> fields)
    : record_name_(std::move(name)), fields_(std::move(fields)) {}

Value RecordDeclarationNode::evaluate(Context& context) {
    // Create a record type that will be used to create record instances
    auto record_type = std::make_shared<RecordType>(record_name_, fields_);

    // Register the record type in the context so it can be accessed later
    context.defineVariable(record_name_, record_type);

    return record_type;
}

std::string RecordDeclarationNode::toString() const {
    std::string result = "record " + record_name_ + " { ";
    for (size_t i = 0; i < fields_.size(); ++i) {
        if (i > 0) result += ", ";
        result += fields_[i].name + ": " + fields_[i].type;
    }
    result += " }";
    return result;
}

}  // namespace o2l