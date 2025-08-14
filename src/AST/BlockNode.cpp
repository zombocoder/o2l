#include "BlockNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

BlockNode::BlockNode(std::vector<ASTNodePtr> statements) : statements_(std::move(statements)) {}

Value BlockNode::evaluate(Context& context) {
    Value result = Int(0);  // Default return value

    // Execute all statements in sequence
    for (const auto& statement : statements_) {
        try {
            result = statement->evaluate(context);
        } catch (const ReturnException& e) {
            // Return statement encountered - propagate it up to method level
            throw;
        }
    }

    return result;
}

std::string BlockNode::toString() const {
    std::string result = "Block {";
    for (size_t i = 0; i < statements_.size(); ++i) {
        if (i > 0) result += "; ";
        result += statements_[i]->toString();
    }
    result += "}";
    return result;
}

}  // namespace o2l