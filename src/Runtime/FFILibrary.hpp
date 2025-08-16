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
#include <unordered_map>
#include <vector>

#include "Value.hpp"
#include "FFI/SharedLibrary.hpp"
#include "FFI/FFIEngine.hpp"
#include "FFI/FFITypes.hpp"

namespace o2l {

class Context;

// FFI Library wrapper instance
class FFILibraryInstance {
private:
    std::unique_ptr<ffi::SharedLibrary> library_;
    std::string path_;

public:
    explicit FFILibraryInstance(std::unique_ptr<ffi::SharedLibrary> lib, std::string path)
        : library_(std::move(lib)), path_(std::move(path)) {}
    
    void* getSymbol(const std::string& name) { return library_->symbol(name); }
    const std::string& getPath() const { return path_; }
    
    std::string toString() const {
        return "Library(" + path_ + ")";
    }
};

// FFI Native Function wrapper
class FFINativeFnInstance {
private:
    void* func_ptr_;
    ffi::Signature signature_;
    std::shared_ptr<FFILibraryInstance> library_;

public:
    FFINativeFnInstance(void* func_ptr, ffi::Signature sig, std::shared_ptr<FFILibraryInstance> lib)
        : func_ptr_(func_ptr), signature_(std::move(sig)), library_(std::move(lib)) {}
    
    void* getFuncPtr() const { return func_ptr_; }
    const ffi::Signature& getSignature() const { return signature_; }
    
    std::string toString() const {
        return std::string("NativeFn(") + (func_ptr_ ? "loaded" : "null") + ")";
    }
};

// Main FFI Library that provides O²L API
class FFILibrary {
private:
    static bool ffi_enabled_;
    static std::vector<std::string> allowed_paths_;
    static std::vector<std::string> denied_symbols_;
    static size_t max_out_bytes_;
    static std::unique_ptr<ffi::FFIEngine> engine_;
    
public:
    // Create the ffi object with native methods
    static std::shared_ptr<ObjectInstance> createFFIObject();
    
    // Policy management
    static void setFFIEnabled(bool enabled) { ffi_enabled_ = enabled; }
    static bool isFFIEnabled() { return ffi_enabled_; }
    
    // Core FFI functions
    static Value ffi_load(const std::vector<Value>& args, Context& context);
    static Value ffi_getLastErrno(const std::vector<Value>& args, Context& context);
    static Value ffi_setPolicy(const std::vector<Value>& args, Context& context);
    static Value ffi_ptr(const std::vector<Value>& args, Context& context);
    static Value ffi_nullPtr(const std::vector<Value>& args, Context& context);
    
    // Enhanced FFI types
    static Value ffi_struct(const std::vector<Value>& args, Context& context);
    static Value ffi_array(const std::vector<Value>& args, Context& context);
    static Value ffi_callback(const std::vector<Value>& args, Context& context);
    static Value ffi_cstring(const std::vector<Value>& args, Context& context);
    static Value ffi_ptrToString(const std::vector<Value>& args, Context& context);
    static Value ffi_ptrToInt(const std::vector<Value>& args, Context& context);
    static Value ffi_ptrToDouble(const std::vector<Value>& args, Context& context);
    static Value ffi_ptrToFloat(const std::vector<Value>& args, Context& context);
    static Value ffi_ptrToBool(const std::vector<Value>& args, Context& context);
    
    // Library methods
    static Value library_symbol(const std::vector<Value>& args, Context& context);
    static Value library_close(const std::vector<Value>& args, Context& context);
    
    // NativeFn methods
    static Value nativefn_call(const std::vector<Value>& args, Context& context);
    static Value nativefn_call_impl(const std::vector<Value>& args, Context& context, 
                                    std::shared_ptr<FFINativeFnInstance> native_fn);

private:
    static bool isPathAllowed(const std::string& path);
    static bool isSymbolAllowed(const std::string& symbol);
    static void initializeEngine();
    static ffi::Signature parseSignature(const std::string& signature_str);
};

}  // namespace o2l