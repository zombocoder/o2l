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

    // Enable/disable FFI
    void setFFIEnabled(bool enabled);

   private:
    // Generate main namespace from filename
    std::string generateMainNamespace() const;
};

}  // namespace o2l