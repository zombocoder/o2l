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

#include <memory>
#include <vector>
#include "Value.hpp"
#include "ObjectInstance.hpp"
#include "../AST/Node.hpp"

namespace o2l {

class CompilerLibrary {
public:
    static std::shared_ptr<ObjectInstance> createCompilerObject();
    
    // Native method implementations
    static Value nativeParse(const std::vector<Value>& args, Context& context);
    
    // ASTNodeHandle creation
    static std::shared_ptr<ObjectInstance> createASTNodeHandle(ASTNode* node);
    };

    /**
    * A specialized ObjectInstance that holds a pointer to a C++ AST node.
    */
    class ASTNodeHandleInstance : public ObjectInstance {
    private:
    ASTNode* node_;
    public:
    explicit ASTNodeHandleInstance(ASTNode* node);

    ASTNode* getNode() const { return node_; }
    
    // Handle-specific methods
    static Value nativeGetType(const std::vector<Value>& args, Context& context);
    static Value nativeGetChildren(const std::vector<Value>& args, Context& context);
    static Value nativeGetProperty(const std::vector<Value>& args, Context& context);
    static Value nativeGetLocation(const std::vector<Value>& args, Context& context);
};

} // namespace o2l
