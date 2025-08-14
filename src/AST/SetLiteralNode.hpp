#pragma once

#include <vector>

#include "Node.hpp"
#include "Runtime/SetInstance.hpp"

namespace o2l {

class SetLiteralNode : public ASTNode {
   private:
    std::vector<ASTNodePtr> elements_;
    std::string element_type_name_;

   public:
    SetLiteralNode(std::vector<ASTNodePtr> elements, std::string element_type_name = "")
        : elements_(std::move(elements)), element_type_name_(std::move(element_type_name)) {}

    Value evaluate(Context& context) override {
        // Infer element type from first element if not specified
        std::string actual_element_type = element_type_name_;
        if (actual_element_type.empty() && !elements_.empty()) {
            Value first_element = elements_[0]->evaluate(context);
            actual_element_type = getTypeName(first_element);
        }

        auto set_instance = std::make_shared<SetInstance>(actual_element_type);

        // Evaluate each element and add to the set
        // Note: std::set automatically handles duplicates
        for (const auto& element : elements_) {
            Value element_value = element->evaluate(context);
            set_instance->add(element_value);
        }

        return set_instance;
    }

    std::string toString() const override {
        std::string result = "Set(";
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (i > 0) result += ", ";
            result += elements_[i]->toString();
        }
        result += ")";
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