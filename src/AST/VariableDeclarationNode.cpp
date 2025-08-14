#include "VariableDeclarationNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ListInstance.hpp"
#include "../Runtime/Value.hpp"

namespace o2l {

VariableDeclarationNode::VariableDeclarationNode(std::string variable_name, std::string type_name,
                                                 ASTNodePtr initializer)
    : variable_name_(std::move(variable_name)),
      type_name_(std::move(type_name)),
      initializer_(std::move(initializer)) {}

Value VariableDeclarationNode::evaluate(Context& context) {
    // Evaluate the initializer expression
    Value value = initializer_->evaluate(context);

    // Add type checking for List types
    if (type_name_.find("List<") == 0) {
        // Extract the element type from List<ElementType>
        size_t start = type_name_.find('<') + 1;
        size_t end = type_name_.find('>', start);
        if (start != std::string::npos && end != std::string::npos) {
            std::string expected_element_type = type_name_.substr(start, end - start);

            // Check if the value is a ListInstance
            if (std::holds_alternative<std::shared_ptr<ListInstance>>(value)) {
                auto list_inst = std::get<std::shared_ptr<ListInstance>>(value);

                // Verify each element matches the expected type
                for (const auto& element : list_inst->getElements()) {
                    std::string actual_type = o2l::getTypeName(element);
                    if (actual_type != expected_element_type) {
                        throw EvaluationError("Type mismatch in " + variable_name_ + ": expected " +
                                                  expected_element_type + " but found " +
                                                  actual_type,
                                              context);
                    }
                }
            } else {
                throw EvaluationError("Type mismatch: " + variable_name_ + " declared as " +
                                          type_name_ + " but assigned non-List value",
                                      context);
            }
        }
    } else {
        // TODO: Add type checking for other types
        std::string actual_type = o2l::getTypeName(value);
        if (actual_type != type_name_) {
            // Allow some implicit conversions (e.g., Int to Long)
            bool conversion_allowed = false;
            if (type_name_ == "Long" && actual_type == "Int") {
                conversion_allowed = true;
            } else if (type_name_ == "Double" && (actual_type == "Float" || actual_type == "Int")) {
                conversion_allowed = true;
            } else if (type_name_ == "Float" && actual_type == "Int") {
                conversion_allowed = true;
            }

            if (!conversion_allowed) {
                // Be more lenient with certain types for compatibility
                // Skip type checking for complex types like Enums, Records, etc.
                if (type_name_ != "Int" && type_name_ != "Long" && type_name_ != "Float" &&
                    type_name_ != "Double" && type_name_ != "Text" && type_name_ != "Bool" &&
                    type_name_ != "Char" && type_name_.find("List<") != 0) {
                    // Skip type checking for complex types to maintain compatibility
                } else {
                    throw EvaluationError("Type mismatch: " + variable_name_ + " declared as " +
                                              type_name_ + " but assigned " + actual_type,
                                          context);
                }
            }
        }
    }

    // Define the variable in the current scope
    context.defineVariable(variable_name_, value);

    // Return the assigned value
    return value;
}

std::string VariableDeclarationNode::toString() const {
    return variable_name_ + ": " + type_name_ + " = " + initializer_->toString();
}

}  // namespace o2l