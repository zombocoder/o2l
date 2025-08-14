#pragma once

#include "../Runtime/Value.hpp"
#include "Node.hpp"

namespace o2l {

class TryCatchFinallyNode : public ASTNode {
   private:
    ASTNodePtr try_block_;        // The try block to execute
    ASTNodePtr catch_block_;      // Optional catch block (can be null)
    ASTNodePtr finally_block_;    // Optional finally block (can be null)
    std::string catch_variable_;  // Variable name to bind the caught exception to

   public:
    TryCatchFinallyNode(ASTNodePtr try_block, ASTNodePtr catch_block, ASTNodePtr finally_block,
                        std::string catch_variable,
                        const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    // Getters
    const ASTNodePtr& getTryBlock() const {
        return try_block_;
    }
    const ASTNodePtr& getCatchBlock() const {
        return catch_block_;
    }
    const ASTNodePtr& getFinallyBlock() const {
        return finally_block_;
    }
    const std::string& getCatchVariable() const {
        return catch_variable_;
    }
};

}  // namespace o2l