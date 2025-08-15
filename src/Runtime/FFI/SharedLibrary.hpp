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

#include <expected>
#include <string>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace o2l::ffi {

struct FFILoadError {
    enum Kind { NotFound, OpenFailed } kind;
    std::string msg;
};

class SharedLibrary {
public:
    static std::expected<SharedLibrary, FFILoadError> open(std::string_view path);
    
    void* symbol(std::string_view name) const noexcept;
    
    ~SharedLibrary();
    
    // Move-only semantics
    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;
    SharedLibrary(SharedLibrary&& other) noexcept;
    SharedLibrary& operator=(SharedLibrary&& other) noexcept;

private:
    SharedLibrary() = default;
    
#ifdef _WIN32
    HMODULE handle_{nullptr};
#else
    void* handle_{nullptr};
#endif
};

}  // namespace o2l::ffi