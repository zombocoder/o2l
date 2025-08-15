#include "ModuleLoader.hpp"

#include <fstream>
#include <iostream>

#include "../AST/ImportNode.hpp"
#include "../AST/ObjectNode.hpp"
#include "../Common/Exceptions.hpp"
#include "../Interpreter.hpp"
#include "../Lexer.hpp"
#include "../Parser.hpp"
#include "DateTimeLibrary.hpp"
#include "HttpClientLibrary.hpp"
#include "HttpServerLibrary.hpp"
#include "JsonLibrary.hpp"
#include "MathLibrary.hpp"
#include "ObjectInstance.hpp"
#include "RegexpLibrary.hpp"
#include "SystemLibrary.hpp"
#include "TestLibrary.hpp"
#include "UrlLibrary.hpp"
#include "FFILibrary.hpp"

namespace o2l {

ModuleLoader::ModuleLoader() {
    // Add default search paths
    addSearchPath(std::filesystem::current_path());
    addSearchPath(std::filesystem::current_path() / "modules");
    addSearchPath(std::filesystem::current_path() / "lib");

    // Add .o2l virtual environment paths (for user libraries managed by o2l-pkg)
    auto o2l_dir = std::filesystem::current_path() / ".o2l";
    if (std::filesystem::exists(o2l_dir)) {
        addSearchPath(o2l_dir / "lib");
    }
}

void ModuleLoader::addSearchPath(const std::filesystem::path& path) {
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        module_search_paths_.push_back(path);
    }
}

std::filesystem::path ModuleLoader::findModuleFile(const std::vector<std::string>& package_path,
                                                   const std::string& object_name,
                                                   bool is_user_import) {
    if (is_user_import) {
        // For user imports (@import), search only in local user paths (no libraries)
        std::vector<std::filesystem::path> user_paths;

        // Add traditional user paths for local files
        user_paths.push_back(std::filesystem::current_path());
        user_paths.push_back(std::filesystem::current_path() / "src");

        for (const auto& search_path : user_paths) {
            std::filesystem::path module_path = search_path;

            // Build path: search_path/package_path.../object_name.obq
            for (const auto& package_part : package_path) {
                module_path /= package_part;
            }
            module_path /= (object_name + ".obq");

            if (std::filesystem::exists(module_path)) {
                return module_path;
            }
        }

        throw EvaluationError("Local file not found: " + object_name +
                              " (use @import for local user files)");
    } else {
        // For library imports (import), search in library and system directories
        std::vector<std::filesystem::path> library_paths;

        // Add .o2l/lib paths for libraries managed by o2l-pkg
        auto o2l_lib = std::filesystem::current_path() / ".o2l" / "lib";
        if (std::filesystem::exists(o2l_lib)) {
            library_paths.push_back(o2l_lib);
        }

        // Add system library paths
        library_paths.push_back(std::filesystem::current_path() / "modules");
        library_paths.push_back(std::filesystem::current_path() / "lib");

        for (const auto& search_path : library_paths) {
            // For .o2l/lib, look for object_name/object_name.obq structure
            if (search_path.filename() == "lib" &&
                search_path.parent_path().filename() == ".o2l") {
                std::filesystem::path module_path = search_path / object_name;

                // Build path: search_path/object_name/package_path.../object_name.obq
                for (const auto& package_part : package_path) {
                    module_path /= package_part;
                }
                module_path /= (object_name + ".obq");

                if (std::filesystem::exists(module_path)) {
                    return module_path;
                }
            } else {
                // For traditional paths, look directly for object_name.obq
                std::filesystem::path module_path = search_path;

                // Build path: search_path/package_path.../object_name.obq
                for (const auto& package_part : package_path) {
                    module_path /= package_part;
                }
                module_path /= (object_name + ".obq");

                if (std::filesystem::exists(module_path)) {
                    return module_path;
                }
            }
        }

        throw EvaluationError("Library not found: " + object_name +
                              " (use import for installed libraries)");
    }
}

std::map<std::string, Value> ModuleLoader::loadModuleFromFile(
    const std::filesystem::path& file_path) {
    // Check if already loaded
    std::string module_key = file_path.string();
    if (loaded_modules_.find(module_key) != loaded_modules_.end()) {
        return loaded_modules_[module_key];
    }

    // Check for circular imports at module loading level
    for (const auto& loading_module : loading_chain_) {
        if (loading_module == module_key) {
            // Build a user-friendly circular import description
            std::string chain_description = "";
            for (const auto& module : loading_chain_) {
                if (!chain_description.empty()) {
                    chain_description += " -> ";
                }
                std::filesystem::path module_path(module);
                std::string filename = module_path.filename().string();
                // Remove .obq extension for cleaner display
                if (filename.ends_with(".obq")) {
                    filename = filename.substr(0, filename.length() - 4);
                }
                chain_description += filename;
            }
            std::filesystem::path current_module_path(module_key);
            std::string current_filename = current_module_path.filename().string();
            if (current_filename.ends_with(".obq")) {
                current_filename = current_filename.substr(0, current_filename.length() - 4);
            }
            chain_description += " -> " + current_filename;

            std::cerr << "Error: Circular @import detected: " << chain_description << std::endl;
            std::cerr << "This indicates a circular dependency in your @import statements."
                      << std::endl;
            std::cerr << "Consider restructuring your code to avoid circular dependencies."
                      << std::endl;
            throw EvaluationError("Circular @import detected: " + chain_description);
        }
    }

    // Add current module to loading chain
    loading_chain_.push_back(module_key);

    try {
        // Read and parse the module file
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw EvaluationError("Cannot open module file: " + file_path.string());
        }

        std::string source_code((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
        file.close();

        // Parse the module
        Lexer lexer(source_code);
        auto tokens = lexer.tokenizeAll();

        Parser parser(std::move(tokens), file_path.string());
        auto ast_nodes = parser.parse();

        // Store AST nodes to keep them alive (important for method lambdas)
        module_ast_storage_[module_key] = std::move(ast_nodes);

        // Create a fresh context for this module (no inherited variables)
        Context module_context;

        // Process @import statements in this module recursively
        for (const auto& node : module_ast_storage_[module_key]) {
            auto import_node = dynamic_cast<ImportNode*>(node.get());
            if (import_node) {
                const auto& import_path = import_node->getImportPath();

                // Only process user imports (@import) recursively
                if (import_path.is_user_import) {
                    if (import_path.is_wildcard()) {
                        auto all_methods = loadAllMethods(import_path, module_context);
                        for (const auto& [name, value] : all_methods) {
                            module_context.defineVariable(name, value);
                        }
                    } else {
                        Value imported_value =
                            resolveImportRecursively(import_path, module_context);
                        module_context.defineVariable(import_path.object_name, imported_value);

                        if (!import_path.method_name.empty() && import_path.method_name != "*") {
                            std::string alias_name =
                                import_path.object_name + "_" + import_path.method_name;
                            module_context.defineVariable(alias_name, imported_value);
                        }
                    }
                }
            }
        }

        // Execute the module to get objects
        std::map<std::string, Value> module_objects;

        for (const auto& node : module_ast_storage_[module_key]) {
            auto object_node = dynamic_cast<ObjectNode*>(node.get());
            if (object_node) {
                Value object_value = object_node->evaluate(module_context);
                module_objects[object_node->getName()] = object_value;
            }
        }

        // Cache the loaded module
        loaded_modules_[module_key] = module_objects;

        // Remove from loading chain before returning
        loading_chain_.pop_back();

        return module_objects;
    } catch (...) {
        // Remove from loading chain before re-throwing
        loading_chain_.pop_back();
        throw;
    }
}

Value ModuleLoader::loadMethod(const ImportPath& import_path, Context& context) {
    return resolveImportRecursively(import_path, context);
}

Value ModuleLoader::resolveImportRecursively(const ImportPath& import_path, Context& context) {
    // Check if this is a native system module first (only for system imports)
    if (!import_path.is_user_import && isNativeSystemModule(import_path)) {
        auto native_object = createNativeSystemModule(import_path.object_name);
        return native_object;
    }

    // For namespace-qualified imports, try to resolve from current context first
    if (!import_path.is_user_import && !import_path.package_path.empty()) {
        // Build the full qualified name (namespace.path.ObjectName)
        std::string qualified_name;
        for (const auto& part : import_path.package_path) {
            if (!qualified_name.empty()) qualified_name += ".";
            qualified_name += part;
        }
        if (!qualified_name.empty()) qualified_name += ".";
        qualified_name += import_path.object_name;

        // Try to find the qualified object in the current context
        if (context.hasVariable(qualified_name)) {
            return context.getVariable(qualified_name);
        }

        // Special handling for http namespace objects
        if (import_path.package_path.size() == 1 && import_path.package_path[0] == "http") {
            if (import_path.object_name == "client") {
                // Create http.client object
                auto client_obj = HttpClientLibrary::createHttpClientObject();
                context.defineVariable("http.client", Value(client_obj));
                return Value(client_obj);
            } else if (import_path.object_name == "server") {
                // Create http.server object
                auto server_obj = HttpServerLibrary::createHttpServerObject();
                context.defineVariable("http.server", Value(server_obj));
                return Value(server_obj);
            }
        }

        // If not found in context, this means the namespace doesn't exist
        // Don't fall through to file-based module loading for namespace imports
        throw EvaluationError("Namespace object '" + qualified_name +
                              "' not found. Make sure the namespace is declared in the same file "
                              "or imported from another module.");
    }

    // For user imports (@import), check for circular dependencies
    if (import_path.is_user_import) {
        std::string import_key = import_path.getFullPath();

        // Check for circular import resolution
        for (const auto& resolving_import : import_resolution_chain_) {
            if (resolving_import == import_key) {
                // Build circular import description
                std::string chain_description = "";
                bool found_start = false;
                for (const auto& imp : import_resolution_chain_) {
                    if (imp == import_key) {
                        found_start = true;
                    }
                    if (found_start) {
                        if (!chain_description.empty()) {
                            chain_description += " -> ";
                        }
                        chain_description += imp;
                    }
                }
                chain_description += " -> " + import_key;

                std::cerr << "Error: Circular @import detected: " << chain_description << std::endl;
                std::cerr << "This indicates a circular dependency in your @import statements."
                          << std::endl;
                std::cerr << "Consider restructuring your code to avoid circular dependencies."
                          << std::endl;
                throw EvaluationError("Circular @import detected: " + chain_description);
            }
        }

        // Add current import to resolution chain
        import_resolution_chain_.push_back(import_key);
    }

    try {
        auto module_file = findModuleFile(import_path.package_path, import_path.object_name,
                                          import_path.is_user_import);
        auto module_objects = loadModuleFromFile(module_file);

        auto object_it = module_objects.find(import_path.object_name);
        if (object_it == module_objects.end()) {
            throw EvaluationError("Object '" + import_path.object_name + "' not found in module");
        }

        // Remove from resolution chain on success
        if (import_path.is_user_import) {
            import_resolution_chain_.pop_back();
        }

        // If method_name is empty, return the entire object
        if (import_path.method_name.empty()) {
            return object_it->second;
        }

        // Otherwise, check for specific method
        auto object_instance = std::get<std::shared_ptr<ObjectInstance>>(object_it->second);
        if (!object_instance->hasMethod(import_path.method_name)) {
            throw EvaluationError("Method '" + import_path.method_name + "' not found in object '" +
                                  import_path.object_name + "'");
        }

        // Return the object instance (method calls will be handled by the object)
        return object_it->second;
    } catch (...) {
        // Remove from resolution chain on failure
        if (import_path.is_user_import) {
            import_resolution_chain_.pop_back();
        }
        throw;
    }
}

std::map<std::string, Value> ModuleLoader::loadAllMethods(const ImportPath& import_path,
                                                          Context& context) {
    // Check if this is a native system module first
    if (isNativeSystemModule(import_path)) {
        auto native_object = createNativeSystemModule(import_path.object_name);
        std::map<std::string, Value> all_methods;
        all_methods[import_path.object_name] = native_object;
        return all_methods;
    }

    auto module_file = findModuleFile(import_path.package_path, import_path.object_name,
                                      import_path.is_user_import);
    auto module_objects = loadModuleFromFile(module_file);

    auto object_it = module_objects.find(import_path.object_name);
    if (object_it == module_objects.end()) {
        throw EvaluationError("Object '" + import_path.object_name + "' not found in module");
    }

    // For wildcard imports, we'll need to expose all methods
    // This is a simplified implementation - return the object itself
    std::map<std::string, Value> all_methods;
    all_methods[import_path.object_name] = object_it->second;

    return all_methods;
}

bool ModuleLoader::moduleExists(const ImportPath& import_path) {
    // Check native system modules first
    if (isNativeSystemModule(import_path)) {
        return true;
    }

    try {
        findModuleFile(import_path.package_path, import_path.object_name,
                       import_path.is_user_import);
        return true;
    } catch (const EvaluationError&) {
        return false;
    }
}

bool ModuleLoader::isNativeSystemModule(const ImportPath& import_path) {
    // Check if this is a system module
    if (import_path.package_path.size() == 1 && import_path.package_path[0] == "system") {
        return import_path.object_name == "io" || import_path.object_name == "os" ||
               import_path.object_name == "utils" || import_path.object_name == "fs";
    }

    // Check if this is a direct math import
    if (import_path.package_path.empty() && import_path.object_name == "math") {
        return true;
    }

    // Check if this is a direct testing import
    if (import_path.package_path.empty() && import_path.object_name == "testing") {
        return true;
    }

    // Check if this is a direct datetime import
    if (import_path.package_path.empty() && import_path.object_name == "datetime") {
        return true;
    }

    // Check if this is a direct regexp import
    if (import_path.package_path.empty() && import_path.object_name == "regexp") {
        return true;
    }

    // Check if this is a direct url import
    if (import_path.package_path.empty() && import_path.object_name == "url") {
        return true;
    }

    // Check if this is a direct json import
    if (import_path.package_path.empty() && import_path.object_name == "json") {
        return true;
    }

    // Check if this is an http.client import
    if (import_path.package_path.size() == 1 && import_path.package_path[0] == "http" &&
        import_path.object_name == "client") {
        return true;
    }

    // Check if this is an http.server import
    if (import_path.package_path.size() == 1 && import_path.package_path[0] == "http" &&
        import_path.object_name == "server") {
        return true;
    }

    // Check if this is a direct ffi import
    if (import_path.package_path.empty() && import_path.object_name == "ffi") {
        return true;
    }

    return false;
}

std::shared_ptr<ObjectInstance> ModuleLoader::createNativeSystemModule(
    const std::string& module_name) {
    if (module_name == "io") {
        return SystemLibrary::createIOObject();
    } else if (module_name == "os") {
        return SystemLibrary::createOSObject();
    } else if (module_name == "utils") {
        return SystemLibrary::createUtilsObject();
    } else if (module_name == "fs") {
        return SystemLibrary::createFSObject();
    } else if (module_name == "math") {
        return MathLibrary::createMathObject();
    } else if (module_name == "testing") {
        return TestLibrary::createTestingObject();
    } else if (module_name == "datetime") {
        return DateTimeLibrary::createDateTimeObject();
    } else if (module_name == "regexp") {
        return RegexpLibrary::createRegexpObject();
    } else if (module_name == "url") {
        return UrlLibrary::createUrlObject();
    } else if (module_name == "json") {
        return JsonLibrary::createJsonObject();
    } else if (module_name == "client") {
        return HttpClientLibrary::createHttpClientObject();
    } else if (module_name == "server") {
        return HttpServerLibrary::createHttpServerObject();
    } else if (module_name == "ffi") {
        return FFILibrary::createFFIObject();
    }

    throw EvaluationError("Unknown native module: " + module_name);
}

}  // namespace o2l