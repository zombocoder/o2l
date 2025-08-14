#include "Interpreter.hpp"

#include <filesystem>
#include <iostream>

#include "AST/EnumDeclarationNode.hpp"
#include "AST/ImportNode.hpp"
#include "AST/NamespaceNode.hpp"
#include "AST/ObjectNode.hpp"
#include "AST/ProtocolDeclarationNode.hpp"
#include "AST/RecordDeclarationNode.hpp"
#include "Common/Exceptions.hpp"
#include "Runtime/ListInstance.hpp"
#include "Runtime/ObjectInstance.hpp"

namespace o2l {

Interpreter::Interpreter() {
    // Initialize global context with built-in objects/methods if needed
}

Interpreter::Interpreter(const std::string& filename) : source_filename_(filename) {
    // Initialize global context with built-in objects/methods if needed
}

Value Interpreter::execute(const std::vector<ASTNodePtr>& nodes) {
    // First pass: Register all objects
    bool has_main = false;

    for (const auto& node : nodes) {
        auto object_node = dynamic_cast<ObjectNode*>(node.get());
        auto import_node = dynamic_cast<ImportNode*>(node.get());
        auto enum_node = dynamic_cast<EnumDeclarationNode*>(node.get());
        auto record_node = dynamic_cast<RecordDeclarationNode*>(node.get());
        auto protocol_node = dynamic_cast<ProtocolDeclarationNode*>(node.get());
        auto namespace_node = dynamic_cast<NamespaceNode*>(node.get());

        if (namespace_node) {
            // Process namespace declarations FIRST (before imports)
            node->evaluate(global_context_);
        } else if (object_node) {
            Value object_value = node->evaluate(global_context_);
            global_context_.defineVariable(object_node->getName(), object_value);

            if (object_node->getName() == "Main") {
                has_main = true;
            }
        } else if (import_node) {
            // Process import
            const auto& import_path = import_node->getImportPath();

            if (import_path.is_wildcard()) {
                // Import all methods from the object
                auto all_methods = module_loader_.loadAllMethods(import_path, global_context_);
                for (const auto& [name, value] : all_methods) {
                    global_context_.defineVariable(name, value);
                }
            } else {
                // Import specific method or object
                Value imported_value = module_loader_.loadMethod(import_path, global_context_);

                // Always register the object with its original name
                global_context_.defineVariable(import_path.object_name, imported_value);

                if (!import_path.method_name.empty() && import_path.method_name != "*") {
                    // Also create an alias for specific method imports
                    std::string alias_name =
                        import_path.object_name + "_" + import_path.method_name;
                    global_context_.defineVariable(alias_name, imported_value);
                }
            }
        } else if (enum_node) {
            // Process enum declaration
            Value enum_value = node->evaluate(global_context_);
            global_context_.defineVariable(enum_node->getEnumName(), enum_value);
        } else if (record_node) {
            // Process record declaration
            Value record_value = node->evaluate(global_context_);
            global_context_.defineVariable(record_node->getRecordName(), record_value);
        } else if (protocol_node) {
            // Process protocol declaration
            Value protocol_value = node->evaluate(global_context_);
            global_context_.defineVariable(protocol_node->getProtocolName(), protocol_value);
        } else {
            throw EvaluationError(
                "Only object declarations, enum declarations, record declarations, protocol "
                "declarations, namespace declarations, and import statements are allowed at the "
                "top level.");
        }
    }

    if (!has_main) {
        throw EvaluationError("Program must contain a 'Main' object as entry point");
    }

    // Second pass: Execute Main.main()
    try {
        Value main_object = global_context_.getVariable("Main");

        if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(main_object)) {
            throw EvaluationError("Main is not an object instance");
        }

        auto main_instance = std::get<std::shared_ptr<ObjectInstance>>(main_object);

        if (!main_instance->hasMethod("main")) {
            throw EvaluationError("Main object must have a 'main()' method");
        }

        // Call Main.main() with no arguments
        std::vector<Value> no_args;
        return main_instance->callMethod("main", no_args, global_context_);

    } catch (const UnresolvedReferenceError& e) {
        // Re-throw the original error instead of masking it
        throw;
    }
}

Value Interpreter::execute(const ASTNodePtr& node) {
    return node->evaluate(global_context_);
}

std::string Interpreter::generateMainNamespace() const {
    if (source_filename_.empty()) {
        return "__main__";
    }

    // Extract filename without extension and path
    std::filesystem::path file_path(source_filename_);
    std::string base_name = file_path.stem().string();

    // Convert to valid namespace (replace non-alphanumeric with underscore)
    std::string namespace_name = "main_";
    for (char c : base_name) {
        if (std::isalnum(c)) {
            namespace_name += c;
        } else {
            namespace_name += '_';
        }
    }

    return namespace_name;
}

void Interpreter::setProgramArguments(const std::vector<std::string>& args) {
    // Store arguments in the global context as a special variable
    // that can be accessed by system.os.args
    std::vector<Value> arg_values;
    for (const auto& arg : args) {
        arg_values.push_back(Text(arg));
    }

    // Create a List<Text> from the arguments
    auto args_list = std::make_shared<ListInstance>("Text");
    for (const auto& arg_value : arg_values) {
        args_list->add(arg_value);
    }

    // Store in global context with a special name
    global_context_.defineVariable("__program_args__", Value(args_list));
}

}  // namespace o2l
