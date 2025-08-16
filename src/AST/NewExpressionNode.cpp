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

#include "NewExpressionNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ErrorInstance.hpp"
#include "../Runtime/ObjectInstance.hpp"

namespace o2l {

NewExpressionNode::NewExpressionNode(std::string object_type_name,
                                     std::vector<ASTNodePtr> constructor_args)
    : object_type_name_(std::move(object_type_name)),
      constructor_args_(std::move(constructor_args)) {}

Value NewExpressionNode::evaluate(Context& context) {
    // Handle special built-in types
    if (object_type_name_ == "Error") {
        // Evaluate constructor arguments
        std::vector<Value> arg_values;
        arg_values.reserve(constructor_args_.size());

        for (const auto& arg : constructor_args_) {
            arg_values.push_back(arg->evaluate(context));
        }

        // Create Error instance
        if (arg_values.size() == 1 && std::holds_alternative<Text>(arg_values[0])) {
            std::string message = std::get<Text>(arg_values[0]);
            auto error_instance = std::make_shared<ErrorInstance>(message);
            return std::static_pointer_cast<ErrorInstance>(error_instance);
        } else {
            throw EvaluationError("Error constructor requires exactly one Text argument", context);
        }
    }

    // Look up the object class/template in the context
    Value object_class = context.getVariable(object_type_name_);

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(object_class)) {
        throw TypeMismatchError("Cannot instantiate non-object type: " + object_type_name_);
    }

    auto class_instance = std::get<std::shared_ptr<ObjectInstance>>(object_class);

    // Create a new instance by copying the class template
    auto new_instance = std::make_shared<ObjectInstance>(*class_instance);

    // Evaluate constructor arguments
    std::vector<Value> arg_values;
    arg_values.reserve(constructor_args_.size());

    for (const auto& arg : constructor_args_) {
        arg_values.push_back(arg->evaluate(context));
    }

    // Call constructor if it exists - this is an internal call (during object creation)
    if (new_instance->hasMethod("constructor")) {
        new_instance->callMethod("constructor", arg_values, context, false);
    } else if (!arg_values.empty()) {
        throw EvaluationError(
            "Object '" + object_type_name_ + "' has no constructor but arguments were provided",
            context);
    }

    return new_instance;
}

std::string NewExpressionNode::toString() const {
    std::string result = "New(" + object_type_name_;
    if (!constructor_args_.empty()) {
        result += ", args: " + std::to_string(constructor_args_.size());
    }
    result += ")";
    return result;
}

}  // namespace o2l