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

#pragma once

#include <vector>

#include "Node.hpp"
#include "Runtime/ListInstance.hpp"

namespace o2l {

class ListLiteralNode : public ASTNode {
   private:
    std::vector<ASTNodePtr> elements_;
    std::string element_type_name_;

   public:
    ListLiteralNode(std::vector<ASTNodePtr> elements, std::string element_type_name = "")
        : elements_(std::move(elements)), element_type_name_(std::move(element_type_name)) {}

    Value evaluate(Context& context) override {
        // Infer element type from first element if not specified
        std::string actual_element_type = element_type_name_;
        if (actual_element_type.empty() && !elements_.empty()) {
            Value first_element = elements_[0]->evaluate(context);
            actual_element_type = getTypeName(first_element);
        }

        auto list_instance = std::make_shared<ListInstance>(actual_element_type);

        // Evaluate each element and add to the list
        for (const auto& element : elements_) {
            Value element_value = element->evaluate(context);
            list_instance->add(element_value);
        }

        return list_instance;
    }

    std::string toString() const override {
        std::string result = "List[";
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (i > 0) result += ", ";
            result += elements_[i]->toString();
        }
        result += "]";
        return result;
    }

    const std::vector<ASTNodePtr>& getElements() const {
        return elements_;
    }
    const std::string& getElementTypeName() const {
        return element_type_name_;
    }
};

}  // namespace o2l