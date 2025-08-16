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

#include "FunctionCallNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ObjectInstance.hpp"
#include "../Runtime/ResultInstance.hpp"

namespace o2l {

FunctionCallNode::FunctionCallNode(std::string function_name, std::vector<ASTNodePtr> arguments)
    : function_name_(std::move(function_name)), arguments_(std::move(arguments)) {}

Value FunctionCallNode::evaluate(Context& context) {
    // Try to get the function from the context (imported functions)
    if (context.hasVariable(function_name_)) {
        Value function_value = context.getVariable(function_name_);

        // If it's an object instance, call it as a function
        if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(function_value)) {
            auto function_obj = std::get<std::shared_ptr<ObjectInstance>>(function_value);

            // Evaluate arguments
            std::vector<Value> arg_values;
            for (const auto& arg : arguments_) {
                arg_values.push_back(arg->evaluate(context));
            }

            // Call the function object (assumed to have a "call" method or similar)
            // For now, we'll assume imported functions are objects with a single method
            auto method_names = function_obj->getMethodNames();
            if (!method_names.empty()) {
                return function_obj->callMethod(method_names[0], arg_values, context);
            }
        }
    }

    // Handle special Result static methods
    if (function_name_ == "Result.success") {
        if (arguments_.size() != 1) {
            throw EvaluationError("Result.success requires exactly one argument");
        }

        Value success_value = arguments_[0]->evaluate(context);

        // Create a successful Result instance
        auto result_instance = std::make_shared<ResultInstance>(success_value, "T", "E");
        return std::static_pointer_cast<ResultInstance>(result_instance);
    }

    if (function_name_ == "Result.error") {
        if (arguments_.size() != 1) {
            throw EvaluationError("Result.error requires exactly one argument");
        }

        Value error_value = arguments_[0]->evaluate(context);

        // Create an error Result instance
        auto result_instance = ResultInstance::createError(error_value, "T", "E");
        return std::static_pointer_cast<ResultInstance>(result_instance);
    }

    throw UnresolvedReferenceError("Function '" + function_name_ + "' not found");
}

std::string FunctionCallNode::toString() const {
    std::string result = function_name_ + "(";
    for (size_t i = 0; i < arguments_.size(); ++i) {
        if (i > 0) result += ", ";
        result += arguments_[i]->toString();
    }
    result += ")";
    return result;
}

}  // namespace o2l