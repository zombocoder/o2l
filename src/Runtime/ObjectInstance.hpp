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

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "../AST/MethodDeclarationNode.hpp"  // For Parameter struct
#include "Value.hpp"

namespace o2l {

// Forward declaration
class Context;

// Method signature: takes arguments and context, returns a Value
using Method = std::function<Value(const std::vector<Value>&, Context&)>;

// Method signature information for protocol validation
struct MethodSignature {
    std::string name;
    std::vector<Parameter> parameters;
    std::string return_type;
    bool is_external;

    // Default constructor for map storage
    MethodSignature() : is_external(false) {}

    MethodSignature(std::string n, std::vector<Parameter> params, std::string ret_type,
                    bool external = false)
        : name(std::move(n)),
          parameters(std::move(params)),
          return_type(std::move(ret_type)),
          is_external(external) {}
};

class ObjectInstance : public std::enable_shared_from_this<ObjectInstance> {
   private:
    std::string object_name_;
    std::map<std::string, Method> methods_;
    std::map<std::string, bool> method_visibility_;  // true = external, false = protected
    std::map<std::string, MethodSignature> method_signatures_;  // Method signature information
    std::map<std::string, Value> properties_;                   // Private properties

   public:
    explicit ObjectInstance(const std::string& name);

    // Copy constructor for creating instances from class templates
    ObjectInstance(const ObjectInstance& other);

    // Add a method to this object instance
    void addMethod(const std::string& method_name, Method method, bool is_external = false);

    // Add a method with signature information
    void addMethod(const std::string& method_name, Method method,
                   const std::vector<Parameter>& parameters, const std::string& return_type,
                   bool is_external = false);

    // Call a method on this object
    Value callMethod(const std::string& method_name, const std::vector<Value>& args,
                     Context& context, bool external_call = false);

    // Check if a method exists
    bool hasMethod(const std::string& method_name) const;

    // Check if a method is externally visible
    bool isMethodExternal(const std::string& method_name) const;

    // Get method signature information
    bool hasMethodSignature(const std::string& method_name) const;
    const MethodSignature* getMethodSignature(const std::string& method_name) const;

    // Get object name
    const std::string& getName() const {
        return object_name_;
    }

    // Get all method names (for debugging/introspection)
    std::vector<std::string> getMethodNames() const;

    // Property access methods (private properties)
    void setProperty(const std::string& property_name, const Value& value);
    Value getProperty(const std::string& property_name) const;
    bool hasProperty(const std::string& property_name) const;
};

}  // namespace o2l