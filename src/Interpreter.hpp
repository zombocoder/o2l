#pragma once

#include <vector>

#include "AST/Node.hpp"
#include "Runtime/Context.hpp"
#include "Runtime/ModuleLoader.hpp"

namespace o2l {

class Interpreter {
   private:
    Context global_context_;
    ModuleLoader module_loader_;
    std::string source_filename_;

   public:
    Interpreter();
    explicit Interpreter(const std::string& filename);

    // Execute a list of AST nodes
    Value execute(const std::vector<ASTNodePtr>& nodes);

    // Execute a single node
    Value execute(const ASTNodePtr& node);

    // Get the global context (for debugging/inspection)
    const Context& getGlobalContext() const {
        return global_context_;
    }
    Context& getGlobalContext() {
        return global_context_;
    }

    // Get the module loader
    ModuleLoader& getModuleLoader() {
        return module_loader_;
    }

    // Set program arguments for system.os.args
    void setProgramArguments(const std::vector<std::string>& args);

   private:
    // Generate main namespace from filename
    std::string generateMainNamespace() const;
};

}  // namespace o2l