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
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// Forward declarations
class Context;
class ObjectInstance;

/**
 * Native function signature that all native methods must implement
 * @param args - Array of Value arguments passed from O²L
 * @param context - Current execution context
 * @return Value - Return value to O²L
 */
using NativeFunction = std::function<Value(const std::vector<Value>&, Context&)>;

/**
 * Abstract base class for native libraries that can be loaded dynamically
 * Native libraries must inherit from this and implement all virtual methods
 */
class NativeLibrary {
   public:
    virtual ~NativeLibrary() = default;

    /**
     * Get the library name (e.g., "math", "collections")
     */
    virtual std::string getName() const = 0;

    /**
     * Get the library version (e.g., "1.0.0")
     */
    virtual std::string getVersion() const = 0;

    /**
     * Initialize the library with the given context
     * Called once when the library is first loaded
     */
    virtual void initialize(Context& context) = 0;

    /**
     * Register all native methods with the given object instance
     * This is called when creating a new instance of the native object
     */
    virtual void registerMethods(ObjectInstance* obj) = 0;

    /**
     * Cleanup resources before library unload
     * Called once when the library is being unloaded
     */
    virtual void cleanup() = 0;

    /**
     * Get metadata about the library (optional)
     * Can include author, description, dependencies, etc.
     */
    virtual std::map<std::string, std::string> getMetadata() const {
        return {};
    }
};

/**
 * Registry for native method bindings
 * Maps method names to native function implementations
 */
class NativeMethodRegistry {
   private:
    std::map<std::string, NativeFunction> methods_;

   public:
    void registerMethod(const std::string& method_name, NativeFunction func);
    bool hasMethod(const std::string& method_name) const;
    NativeFunction getMethod(const std::string& method_name) const;
    std::vector<std::string> getMethodNames() const;
    void clear();
};

}  // namespace o2l

/**
 * Required C-style entry points for dynamic libraries
 * Every native library .so/.dll must export these functions
 */
extern "C" {
/**
 * Create and return a new instance of the native library
 * This is the main entry point called by the dynamic loader
 */
o2l::NativeLibrary* create_library();

/**
 * Destroy the given library instance
 * Called during library cleanup/unload
 */
void destroy_library(o2l::NativeLibrary* lib);

/**
 * Optional: Get library ABI version for compatibility checking
 */
const char* get_abi_version();
}