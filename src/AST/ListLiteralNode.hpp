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