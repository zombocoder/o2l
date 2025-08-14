#pragma once

#include <string>

#include "Node.hpp"

namespace o2l {

class RecordFieldAccessNode : public ASTNode {
   private:
    ASTNodePtr record_expr_;  // Expression that evaluates to a record instance
    std::string field_name_;

   public:
    RecordFieldAccessNode(ASTNodePtr record_expr, std::string field_name);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ASTNodePtr& getRecordExpression() const {
        return record_expr_;
    }
    const std::string& getFieldName() const {
        return field_name_;
    }
};

}  // namespace o2l