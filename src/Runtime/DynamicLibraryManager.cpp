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

#include "DynamicLibraryManager.hpp"

#include <filesystem>
#include <iostream>

#include "../Common/Exceptions.hpp"

// Platform-specific includes
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace o2l {

DynamicLibraryManager::DynamicLibraryManager() {
    // Add default search paths
    namespace fs = std::filesystem;

    // Current directory .o2l/lib/native
    auto current_dir = fs::current_path();
    addSearchPath(current_dir / ".o2l" / "lib" / "native");

    // System-wide library paths
#ifdef __APPLE__
    addSearchPath("/usr/local/lib/o2l");
    addSearchPath("/opt/homebrew/lib/o2l");
#elif __linux__
    addSearchPath("/usr/local/lib/o2l");
    addSearchPath("/usr/lib/o2l");
#elif _WIN32
    // Windows paths
    char* programFiles = nullptr;
    size_t len = 0;
    if (_dupenv_s(&programFiles, &len, "PROGRAMFILES") == 0 && programFiles) {
        addSearchPath(fs::path(programFiles) / "O2L" / "lib");
        free(programFiles);
    }
#endif
}

DynamicLibraryManager::~DynamicLibraryManager() {
    cleanup();
}

void DynamicLibraryManager::addSearchPath(const std::filesystem::path& path) {
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        library_search_paths_.push_back(path);
    }
}

std::string DynamicLibraryManager::getLibraryExtension() {
#ifdef _WIN32
    return ".dll";
#elif __APPLE__
    return ".dylib";
#else
    return ".so";
#endif
}

std::string DynamicLibraryManager::getLibraryPrefix() {
#ifdef _WIN32
    return "";
#else
    return "lib";
#endif
}

std::filesystem::path DynamicLibraryManager::findLibraryFile(const std::string& library_name) {
    std::string prefix = getLibraryPrefix();
    std::string extension = getLibraryExtension();
    std::string full_name = prefix + library_name + extension;

    for (const auto& search_path : library_search_paths_) {
        auto candidate = search_path / full_name;
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    throw EvaluationError("Native library '" + library_name + "' not found in search paths");
}

void* DynamicLibraryManager::loadDynamicLibrary(const std::filesystem::path& library_path) {
#ifdef _WIN32
    HMODULE handle = LoadLibraryA(library_path.string().c_str());
    if (!handle) {
        DWORD error = GetLastError();
        throw EvaluationError("Failed to load library '" + library_path.string() +
                              "', error: " + std::to_string(error));
    }
    return static_cast<void*>(handle);
#else
    void* handle = dlopen(library_path.c_str(), RTLD_LAZY);
    if (!handle) {
        throw EvaluationError("Failed to load library '" + library_path.string() +
                              "': " + dlerror());
    }
    return handle;
#endif
}

void DynamicLibraryManager::unloadDynamicLibrary(void* handle) {
    if (!handle) return;

#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif
}

void* DynamicLibraryManager::getSymbol(void* handle, const std::string& symbol_name) {
#ifdef _WIN32
    void* symbol = GetProcAddress(static_cast<HMODULE>(handle), symbol_name.c_str());
#else
    void* symbol = dlsym(handle, symbol_name.c_str());
#endif
    return symbol;
}

bool DynamicLibraryManager::validateABI(void* handle) {
    // Try to get ABI version function
    using GetABIVersionFunc = const char* (*)();
    auto get_abi_version =
        reinterpret_cast<GetABIVersionFunc>(getSymbol(handle, "get_abi_version"));

    if (!get_abi_version) {
        // No ABI version function - assume compatible for now
        std::cerr << "Warning: Native library does not export ABI version" << std::endl;
        return true;
    }

    const char* lib_abi = get_abi_version();
    if (std::string(lib_abi) != O2L_ABI_VERSION) {
        std::cerr << "Warning: ABI version mismatch. Expected: " << O2L_ABI_VERSION
                  << ", Got: " << lib_abi << std::endl;
        // For now, continue anyway - in future could be strict
        return true;
    }

    return true;
}

bool DynamicLibraryManager::loadLibrary(const std::string& library_name) {
    if (isLibraryLoaded(library_name)) {
        return true;  // Already loaded
    }

    try {
        auto library_path = findLibraryFile(library_name);
        return loadLibraryFromPath(library_name, library_path);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load library '" << library_name << "': " << e.what() << std::endl;
        return false;
    }
}

bool DynamicLibraryManager::loadLibraryFromPath(const std::string& library_name,
                                                const std::filesystem::path& library_path) {
    if (isLibraryLoaded(library_name)) {
        return true;  // Already loaded
    }

    try {
        // Load the dynamic library
        void* handle = loadDynamicLibrary(library_path);

        // Validate ABI compatibility
        if (!validateABI(handle)) {
            unloadDynamicLibrary(handle);
            return false;
        }

        // Get required entry points
        using CreateLibraryFunc = NativeLibrary* (*)();
        using DestroyLibraryFunc = void (*)(NativeLibrary*);

        auto create_func = reinterpret_cast<CreateLibraryFunc>(getSymbol(handle, "create_library"));
        auto destroy_func =
            reinterpret_cast<DestroyLibraryFunc>(getSymbol(handle, "destroy_library"));

        if (!create_func || !destroy_func) {
            unloadDynamicLibrary(handle);
            throw EvaluationError("Library '" + library_name + "' missing required entry points");
        }

        // Create library instance
        auto library_instance = std::unique_ptr<NativeLibrary>(create_func());
        if (!library_instance) {
            unloadDynamicLibrary(handle);
            throw EvaluationError("Failed to create library instance for '" + library_name + "'");
        }

        // Initialize library
        Context global_context;  // TODO: Use proper global context
        library_instance->initialize(global_context);

        // Store everything
        loaded_handles_[library_name] = handle;
        library_instances_[library_name] = std::move(library_instance);
        entry_functions_[library_name] = {reinterpret_cast<void*>(create_func),
                                          reinterpret_cast<void*>(destroy_func)};

        std::cout << "Successfully loaded native library: " << library_name << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Failed to load library '" << library_name << "': " << e.what() << std::endl;
        return false;
    }
}

void DynamicLibraryManager::unloadLibrary(const std::string& library_name) {
    if (!isLibraryLoaded(library_name)) {
        return;
    }

    // Cleanup library instance
    if (library_instances_[library_name]) {
        library_instances_[library_name]->cleanup();

        // Call destroy function
        auto entry_funcs = entry_functions_[library_name];
        if (entry_funcs.second) {
            using DestroyLibraryFunc = void (*)(NativeLibrary*);
            auto destroy_func = reinterpret_cast<DestroyLibraryFunc>(entry_funcs.second);
            destroy_func(library_instances_[library_name].release());
        }
    }

    // Unload dynamic library
    if (loaded_handles_[library_name]) {
        unloadDynamicLibrary(loaded_handles_[library_name]);
    }

    // Remove from maps
    loaded_handles_.erase(library_name);
    library_instances_.erase(library_name);
    entry_functions_.erase(library_name);

    std::cout << "Unloaded native library: " << library_name << std::endl;
}

bool DynamicLibraryManager::isLibraryLoaded(const std::string& library_name) const {
    return library_instances_.find(library_name) != library_instances_.end();
}

NativeLibrary* DynamicLibraryManager::getLibrary(const std::string& library_name) {
    auto it = library_instances_.find(library_name);
    return (it != library_instances_.end()) ? it->second.get() : nullptr;
}

std::shared_ptr<ObjectInstance> DynamicLibraryManager::createNativeObject(
    const std::string& library_name) {
    auto* library = getLibrary(library_name);
    if (!library) {
        throw EvaluationError("Native library '" + library_name + "' not loaded");
    }

    // Create a new object instance
    auto obj_instance = std::make_shared<ObjectInstance>(library->getName());

    // Let the library register its methods
    library->registerMethods(obj_instance.get());

    return obj_instance;
}

std::vector<std::string> DynamicLibraryManager::getLoadedLibraries() const {
    std::vector<std::string> libraries;
    libraries.reserve(library_instances_.size());
    for (const auto& [name, instance] : library_instances_) {
        libraries.push_back(name);
    }
    return libraries;
}

std::map<std::string, std::string> DynamicLibraryManager::getLibraryInfo(
    const std::string& library_name) {
    auto* library = getLibrary(library_name);
    if (!library) {
        return {};
    }

    std::map<std::string, std::string> info;
    info["name"] = library->getName();
    info["version"] = library->getVersion();

    // Add metadata
    auto metadata = library->getMetadata();
    info.insert(metadata.begin(), metadata.end());

    return info;
}

void DynamicLibraryManager::cleanup() {
    // Unload all libraries in reverse order
    auto library_names = getLoadedLibraries();
    for (auto it = library_names.rbegin(); it != library_names.rend(); ++it) {
        unloadLibrary(*it);
    }
}

}  // namespace o2l