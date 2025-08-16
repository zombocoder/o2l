/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RecordFieldAccessNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/RecordInstance.hpp"

namespace o2l {

RecordFieldAccessNode::RecordFieldAccessNode(ASTNodePtr record_expr, std::string field_name)
    : record_expr_(std::move(record_expr)), field_name_(std::move(field_name)) {}

Value RecordFieldAccessNode::evaluate(Context& context) {
    // Evaluate the record expression
    Value record_value = record_expr_->evaluate(context);

    if (!std::holds_alternative<std::shared_ptr<RecordInstance>>(record_value)) {
        throw EvaluationError("Cannot access field '" + field_name_ + "' on non-record value",
                              context);
    }

    auto record_instance = std::get<std::shared_ptr<RecordInstance>>(record_value);

    // Get the field value
    if (!record_instance->hasField(field_name_)) {
        throw EvaluationError("Record instance has no field '" + field_name_ + "'", context);
    }

    return record_instance->getFieldValue(field_name_);
}

std::string RecordFieldAccessNode::toString() const {
    return record_expr_->toString() + "." + field_name_;
}

}  // namespace o2l