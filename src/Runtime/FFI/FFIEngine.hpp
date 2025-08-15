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
#include <unordered_map>
#include <memory>
#include "FFITypes.hpp"
#include "../Value.hpp"

// Include ffi.h if available, otherwise define minimal interface
#ifdef HAVE_FFI
#include <ffi.h>
#else
// Minimal libffi interface for platforms without it
extern "C" {
    typedef enum {
        FFI_OK = 0,
        FFI_BAD_TYPEDEF,
        FFI_BAD_ABI
    } ffi_status;
    
    typedef struct ffi_type ffi_type;
    typedef struct ffi_cif ffi_cif;
    
    struct ffi_type {
        size_t size;
        unsigned short alignment;
        unsigned short type;
        struct ffi_type **elements;
    };
    
    extern ffi_type ffi_type_void;
    extern ffi_type ffi_type_uint8;
    extern ffi_type ffi_type_sint32;
    extern ffi_type ffi_type_sint64;
    extern ffi_type ffi_type_float;
    extern ffi_type ffi_type_double;
    extern ffi_type ffi_type_pointer;
    
    ffi_status ffi_prep_cif(ffi_cif *cif, unsigned int abi, unsigned int nargs,
                           ffi_type *rtype, ffi_type **atypes);
    void ffi_call(ffi_cif *cif, void *fn, void *rvalue, void **avalue);
}
#endif

namespace o2l::ffi {

struct FFICallError {
    enum Kind { InvalidSignature, TypeMismatch, CallFailed, NullResult } kind;
    std::string msg;
};

// Forward declaration
struct PreparedCallImpl;

// Cached FFI call interface for performance
struct PreparedCall {
    std::unique_ptr<PreparedCallImpl> impl;
    Signature signature;
    
    PreparedCall(Signature sig);
    ~PreparedCall();
};

class FFIEngine {
public:
    FFIEngine();
    ~FFIEngine();
    
    // Main FFI call interface
    std::expected<Value, FFICallError> call(
        void* func_ptr, 
        const Signature& sig, 
        const std::vector<Value>& args
    );
    
    // Get last system errno after FFI call
    int getLastErrno() const { return last_errno_; }
    
#ifdef _WIN32
    // Get last Windows error after FFI call
    unsigned long getLastWinErr() const { return last_win_err_; }
#endif

private:
    std::unordered_map<std::string, std::unique_ptr<PreparedCall>> call_cache_;
    int last_errno_{0};
#ifdef _WIN32
    unsigned long last_win_err_{0};
#endif
    
    // Helper methods
    std::string signatureToKey(const Signature& sig);
    PreparedCall* getOrCreateCall(const Signature& sig);
    ffi_type* ctypeToFFIType(CType type);
    
    // Value marshaling
    std::expected<void*, FFICallError> marshalValue(const Value& value, CType expected_type, 
                                                   std::vector<std::unique_ptr<uint8_t[]>>& storage);
    std::expected<Value, FFICallError> unmarshalValue(void* result, CType type);
    
    // Memory management for temporary C strings
    std::vector<std::unique_ptr<char[]>> temp_cstrings_;
    void cleanup_temp_memory();
};

}  // namespace o2l::ffi