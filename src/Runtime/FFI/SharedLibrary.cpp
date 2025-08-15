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

#include "SharedLibrary.hpp"
#include <filesystem>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace o2l::ffi {

std::expected<SharedLibrary, FFILoadError> SharedLibrary::open(std::string_view path) {
    if (path.empty()) {
        return std::unexpected(FFILoadError{FFILoadError::NotFound, "Empty library path"});
    }

    std::filesystem::path lib_path(path);
    
    // Resolve to absolute path and check existence
    std::error_code ec;
    std::filesystem::path canonical_path = std::filesystem::canonical(lib_path, ec);
    if (ec) {
        return std::unexpected(FFILoadError{FFILoadError::NotFound, 
                             "Library not found: " + std::string(path)});
    }

    SharedLibrary lib;
    
#ifdef _WIN32
    lib.handle_ = LoadLibraryW(canonical_path.wstring().c_str());
    if (!lib.handle_) {
        DWORD error = GetLastError();
        return std::unexpected(FFILoadError{FFILoadError::OpenFailed, 
                             "Failed to load library: " + std::string(path) + 
                             " (Windows error: " + std::to_string(error) + ")"});
    }
#else
    lib.handle_ = dlopen(canonical_path.c_str(), RTLD_GLOBAL | RTLD_NOW);
    if (!lib.handle_) {
        const char* error = dlerror();
        return std::unexpected(FFILoadError{FFILoadError::OpenFailed, 
                             "Failed to load library: " + std::string(path) + 
                             " (" + (error ? error : "unknown error") + ")"});
    }
#endif

    return lib;
}

void* SharedLibrary::symbol(std::string_view name) const noexcept {
    if (!handle_) {
        return nullptr;
    }

#ifdef _WIN32
    return GetProcAddress(handle_, name.data());
#else
    // Clear any previous dlerror
    dlerror();
    
    // First try the symbol as provided
    void* result = dlsym(handle_, name.data());
    if (result) {
        return result;
    }
    
    // On macOS, try without underscore prefix if the name starts with one
    if (name.length() > 1 && name[0] == '_') {
        dlerror(); // clear error
        std::string name_without_underscore(name.substr(1));
        result = dlsym(handle_, name_without_underscore.c_str());
        if (result) {
            return result;
        }
    }
    
    // If the name doesn't start with underscore, try adding one (for other platforms)
    if (name.length() > 0 && name[0] != '_') {
        dlerror(); // clear error
        std::string name_with_underscore = std::string("_") + std::string(name);
        result = dlsym(handle_, name_with_underscore.c_str());
        if (result) {
            return result;
        }
    }
    
    // If we get here, symbol wasn't found with any variant
    const char* error = dlerror();
    // For debugging: This would normally not be done in production
    if (error) {
        fprintf(stderr, "dlsym failed for symbol '%.*s' (tried variants): %s\n", 
                (int)name.length(), name.data(), error);
    }
    
    return nullptr;
#endif
}

SharedLibrary::~SharedLibrary() {
    if (handle_) {
#ifdef _WIN32
        FreeLibrary(handle_);
#else
        dlclose(handle_);
#endif
    }
}

SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept 
    : handle_(other.handle_) {
    other.handle_ = nullptr;
}

SharedLibrary& SharedLibrary::operator=(SharedLibrary&& other) noexcept {
    if (this != &other) {
        if (handle_) {
#ifdef _WIN32
            FreeLibrary(handle_);
#else
            dlclose(handle_);
#endif
        }
        handle_ = other.handle_;
        other.handle_ = nullptr;
    }
    return *this;
}

}  // namespace o2l::ffi