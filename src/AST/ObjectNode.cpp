#include "ObjectNode.hpp"

#include <iostream>

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ObjectInstance.hpp"
#include "../Runtime/ProtocolInstance.hpp"
#include "ConstructorDeclarationNode.hpp"
#include "MethodDeclarationNode.hpp"

namespace o2l {

ObjectNode::ObjectNode(std::string name, std::vector<ASTNodePtr> methods,
                       std::vector<ASTNodePtr> properties, ASTNodePtr constructor,
                       const std::string& protocol_name)
    : object_name_(std::move(name)),
      methods_(std::move(methods)),
      properties_(std::move(properties)),
      constructor_(std::move(constructor)),
      protocol_name_(protocol_name) {}

Value ObjectNode::evaluate(Context& context) {
    // Create new object instance
    auto object_instance = std::make_shared<ObjectInstance>(object_name_);

    // Process constructor if present
    if (constructor_) {
        auto constructor_decl = dynamic_cast<ConstructorDeclarationNode*>(constructor_.get());
        if (constructor_decl) {
            // Create constructor method
            Method constructor_impl = [constructor_decl](const std::vector<Value>& args,
                                                         Context& ctx) -> Value {
                // Create new scope for constructor execution
                ctx.pushScope();

                // Bind parameters to arguments
                const auto& params = constructor_decl->getParameters();
                if (args.size() != params.size()) {
                    ctx.popScope();
                    throw EvaluationError("Constructor expects " + std::to_string(params.size()) +
                                              " arguments, got " + std::to_string(args.size()),
                                          ctx);
                }

                for (size_t i = 0; i < params.size(); ++i) {
                    ctx.defineVariable(params[i].name, args[i]);
                }

                // Execute constructor body
                Value result = constructor_decl->getBody()->evaluate(ctx);

                ctx.popScope();
                return result;
            };

            object_instance->addMethod("constructor", constructor_impl);
        }
    }

    // Process methods and add them to the object instance
    for (const auto& method_node : methods_) {
        // Cast to MethodDeclarationNode to access method details
        auto method_decl = dynamic_cast<MethodDeclarationNode*>(method_node.get());
        if (method_decl) {
            const std::string& method_name = method_decl->getName();

            // Create a lambda that captures the method body and copies module variables
            // Only capture variables that were defined at module load time (imports), not inherited
            // variables
            std::map<std::string, Value> module_variables;

            // We should only capture variables that belong to this specific module's context
            // For now, we'll capture all variables, but this needs to be more selective
            // TODO: Implement proper module-scoped variable tracking
            for (const auto& var_name : context.getVariableNames()) {
                module_variables[var_name] = context.getVariable(var_name);
            }

            Method method_impl = [method_decl, module_variables](const std::vector<Value>& args,
                                                                 Context& ctx) -> Value {
                // Create new scope for method execution
                ctx.pushScope();

                // Copy module-level variables (like imports) into the method execution context
                for (const auto& [var_name, var_value] : module_variables) {
                    if (!ctx.hasVariable(var_name)) {
                        ctx.defineVariable(var_name, var_value);
                    }
                }

                // Bind parameters to arguments
                const auto& params = method_decl->getParameters();
                if (args.size() != params.size()) {
                    ctx.popScope();
                    throw EvaluationError("Method '" + method_decl->getName() + "' expects " +
                                              std::to_string(params.size()) + " arguments, got " +
                                              std::to_string(args.size()),
                                          ctx);
                }

                for (size_t i = 0; i < params.size(); ++i) {
                    ctx.defineVariable(params[i].name, args[i]);
                }

                // Execute method body
                Value result;
                try {
                    result = method_decl->getBody()->evaluate(ctx);
                } catch (const ReturnException& e) {
                    // Return statement encountered - use its value
                    ctx.popScope();
                    return e.getValue();
                }

                ctx.popScope();
                return result;
            };

            object_instance->addMethod(method_name, method_impl, method_decl->isExternal());
        }
    }

    // Check protocol compliance if protocol is specified
    if (!protocol_name_.empty()) {
        // Look up the protocol in the context
        if (context.hasVariable(protocol_name_)) {
            Value protocol_value = context.getVariable(protocol_name_);

            // Check if it's a protocol instance
            if (std::holds_alternative<std::shared_ptr<ProtocolInstance>>(protocol_value)) {
                auto protocol = std::get<std::shared_ptr<ProtocolInstance>>(protocol_value);

                // Validate that the object implements all required methods
                if (!protocol->isImplementedBy(object_instance)) {
                    throw EvaluationError(
                        "Object '" + object_name_ +
                            "' does not implement all methods required by protocol '" +
                            protocol_name_ + "'",
                        context);
                }
            } else {
                throw EvaluationError("'" + protocol_name_ + "' is not a protocol", context);
            }
        } else {
            throw EvaluationError("Protocol '" + protocol_name_ + "' not found", context);
        }
    }

    return object_instance;
}

std::string ObjectNode::toString() const {
    return "Object(" + object_name_ + ")";
}

}  // namespace o2l