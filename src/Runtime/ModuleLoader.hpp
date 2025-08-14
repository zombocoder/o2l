#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "../AST/ImportNode.hpp"
#include "Context.hpp"
#include "Value.hpp"

// Forward declaration
namespace o2l {
class ObjectInstance;
}

namespace o2l {

class ModuleLoader {
   private:
    std::vector<std::filesystem::path> module_search_paths_;
    std::map<std::string, std::map<std::string, Value>>
        loaded_modules_;  // package.object -> methods
    std::map<std::string, std::vector<ASTNodePtr>> module_ast_storage_;  // Keep AST nodes alive
    std::vector<std::string>
        loading_chain_;  // Track modules currently being loaded to detect circular imports
    std::vector<std::string>
        import_resolution_chain_;  // Track imports being resolved to detect circular @imports

    std::filesystem::path findModuleFile(const std::vector<std::string>& package_path,
                                         const std::string& object_name,
                                         bool is_user_import = false);
    std::map<std::string, Value> loadModuleFromFile(const std::filesystem::path& file_path);
    Value resolveImportRecursively(const ImportPath& import_path, Context& context);

    // Native system module support
    std::shared_ptr<ObjectInstance> createNativeSystemModule(const std::string& module_name);
    bool isNativeSystemModule(const ImportPath& import_path);

   public:
    ModuleLoader();

    // Add a directory to search for modules
    void addSearchPath(const std::filesystem::path& path);

    // Load and return specific method from a module
    Value loadMethod(const ImportPath& import_path, Context& context);

    // Load all methods from an object and return them as a map
    std::map<std::string, Value> loadAllMethods(const ImportPath& import_path, Context& context);

    // Check if a module exists
    bool moduleExists(const ImportPath& import_path);
};

}  // namespace o2l