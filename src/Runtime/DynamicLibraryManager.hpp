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

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "NativeLibrary.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

/**
 * Manages loading, unloading, and lifecycle of dynamic native libraries (.so/.dll)
 * Provides a safe interface for interacting with native code from O²L
 */
class DynamicLibraryManager {
   private:
    // Map from library name to loaded library handle (void* = dlopen handle)
    std::map<std::string, void*> loaded_handles_;

    // Map from library name to library instance
    std::map<std::string, std::unique_ptr<NativeLibrary>> library_instances_;

    // Map from library name to creation/destruction functions
    std::map<std::string, std::pair<void*, void*>> entry_functions_;

    // Search paths for native libraries
    std::vector<std::filesystem::path> library_search_paths_;

    // Current ABI version for compatibility checking
    static constexpr const char* O2L_ABI_VERSION = "1.0.0";

    /**
     * Platform-specific library loading
     */
    void* loadDynamicLibrary(const std::filesystem::path& library_path);
    void unloadDynamicLibrary(void* handle);
    void* getSymbol(void* handle, const std::string& symbol_name);

    /**
     * Find library file in search paths
     */
    std::filesystem::path findLibraryFile(const std::string& library_name);

    /**
     * Validate library ABI compatibility
     */
    bool validateABI(void* handle);

   public:
    DynamicLibraryManager();
    ~DynamicLibraryManager();

    /**
     * Add a directory to the library search path
     */
    void addSearchPath(const std::filesystem::path& path);

    /**
     * Load a native library by name
     * Searches in: .o2l/lib/native/ (O²L project dir), /usr/local/lib/o2l/, system paths
     * @param library_name - Name without extension (e.g., "math" for "libmath.so")
     * @return true if loaded successfully
     */
    bool loadLibrary(const std::string& library_name);

    /**
     * Load a native library from specific path
     * @param library_name - Internal name for the library
     * @param library_path - Full path to the library file
     * @return true if loaded successfully
     */
    bool loadLibraryFromPath(const std::string& library_name,
                             const std::filesystem::path& library_path);

    /**
     * Unload a previously loaded library
     * @param library_name - Name of the library to unload
     */
    void unloadLibrary(const std::string& library_name);

    /**
     * Check if a library is currently loaded
     */
    bool isLibraryLoaded(const std::string& library_name) const;

    /**
     * Get a loaded library instance
     * @param library_name - Name of the library
     * @return Pointer to library instance or nullptr if not loaded
     */
    NativeLibrary* getLibrary(const std::string& library_name);

    /**
     * Create a new object instance backed by a native library
     * @param library_name - Name of the loaded native library
     * @return Shared pointer to ObjectInstance with native methods
     */
    std::shared_ptr<ObjectInstance> createNativeObject(const std::string& library_name);

    /**
     * Get list of all loaded libraries
     */
    std::vector<std::string> getLoadedLibraries() const;

    /**
     * Get library metadata (name, version, etc.)
     */
    std::map<std::string, std::string> getLibraryInfo(const std::string& library_name);

    /**
     * Unload all libraries and cleanup
     */
    void cleanup();

    /**
     * Get default library file extension for current platform
     */
    static std::string getLibraryExtension();

    /**
     * Get default library prefix for current platform
     */
    static std::string getLibraryPrefix();
};

}  // namespace o2l