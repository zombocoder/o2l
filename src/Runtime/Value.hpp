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

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace o2l {

// Forward declarations
class ObjectInstance;
class EnumInstance;
class RecordType;
class RecordInstance;
class ProtocolInstance;
class ListInstance;
class ListIterator;
class RepeatIterator;
class MapInstance;
class MapIterator;
class MapObject;
class SetInstance;
class SetIterator;
class ErrorInstance;
class ResultInstance;
class CoroutineHandle;
class ChannelInstance;

// FFI forward declarations
namespace ffi {
class PtrInstance;
class CBufferInstance;
class CStructInstance;
class CArrayInstance;
class CCallbackInstance;
}  // namespace ffi

// Built-in immutable types
using Text = std::string;
using Int = long long;
#if defined(__SIZEOF_INT128__) && !defined(_WIN32)
// Suppress pedantic warning for __int128 which is a widely supported extension
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
using Long = __int128;
#define O2L_HAS_INT128 1
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#else
using Long = long long;  // Fallback to 64-bit if 128-bit not available
#define O2L_HAS_INT128 0
#endif

using Float = float;
using Double = double;
using Bool = bool;
using Char = char;

template <typename T>
using List = std::vector<T>;

template <typename K, typename V>
using Map = std::map<K, V>;

template <typename T>
using Optional = std::optional<T>;

// Forward declaration for recursive use
struct Value;

// Collections with Value pointers
using ValueList = List<std::shared_ptr<Value>>;
using ValueMap = Map<std::shared_ptr<Value>, std::shared_ptr<Value>>;
using ValueOptional = Optional<std::shared_ptr<Value>>;

#if O2L_HAS_INT128
using ValueVariantBase = std::variant<
    Int, Long, Float, Double, Text, Bool, Char, std::shared_ptr<ObjectInstance>,
    std::shared_ptr<EnumInstance>, std::shared_ptr<RecordType>,
    std::shared_ptr<RecordInstance>, std::shared_ptr<ProtocolInstance>,
    std::shared_ptr<ListInstance>, std::shared_ptr<ListIterator>,
    std::shared_ptr<RepeatIterator>, std::shared_ptr<MapInstance>,
    std::shared_ptr<MapIterator>, std::shared_ptr<MapObject>,
    std::shared_ptr<SetInstance>, std::shared_ptr<SetIterator>,
    std::shared_ptr<ErrorInstance>, std::shared_ptr<ResultInstance>,
    std::shared_ptr<CoroutineHandle>, std::shared_ptr<ChannelInstance>,
    std::shared_ptr<ffi::PtrInstance>, std::shared_ptr<ffi::CBufferInstance>,
    std::shared_ptr<ffi::CStructInstance>, std::shared_ptr<ffi::CArrayInstance>,
    std::shared_ptr<ffi::CCallbackInstance>, ValueList, ValueMap, ValueOptional>;
#else
using ValueVariantBase = std::variant<
    Int, Float, Double, Text, Bool, Char, std::shared_ptr<ObjectInstance>,
    std::shared_ptr<EnumInstance>, std::shared_ptr<RecordType>,
    std::shared_ptr<RecordInstance>, std::shared_ptr<ProtocolInstance>,
    std::shared_ptr<ListInstance>, std::shared_ptr<ListIterator>,
    std::shared_ptr<RepeatIterator>, std::shared_ptr<MapInstance>,
    std::shared_ptr<MapIterator>, std::shared_ptr<MapObject>,
    std::shared_ptr<SetInstance>, std::shared_ptr<SetIterator>,
    std::shared_ptr<ErrorInstance>, std::shared_ptr<ResultInstance>,
    std::shared_ptr<CoroutineHandle>, std::shared_ptr<ChannelInstance>,
    std::shared_ptr<ffi::PtrInstance>, std::shared_ptr<ffi::CBufferInstance>,
    std::shared_ptr<ffi::CStructInstance>, std::shared_ptr<ffi::CArrayInstance>,
    std::shared_ptr<ffi::CCallbackInstance>, ValueList, ValueMap, ValueOptional>;
#endif

// The main Value variant that represents all possible O²L values
struct Value : public ValueVariantBase {
    Value() : ValueVariantBase(static_cast<Int>(0)), is_long_(false) {}

    Value(std::nullptr_t) : Value() {}

    using ValueVariantBase::ValueVariantBase;

    // Additional flag to distinguish between Int and Long when they have the same underlying type
    bool is_long_ = false;

    // Constructors to set the flag
    Value(Int v) : ValueVariantBase(v), is_long_(false) {}
#if !O2L_HAS_INT128
    // Special constructor for Long when it's the same as Int
    struct LongTag {};
    Value(Long v, LongTag) : ValueVariantBase(v), is_long_(true) {}
#else
    Value(Long v) : ValueVariantBase(v), is_long_(true) {}
#endif
};

// Utility functions for Value operations
std::string valueToString(const Value& value);
std::string getTypeName(const Value& value);
bool valuesEqual(const Value& a, const Value& b);
bool valuesLess(const Value& a, const Value& b);

// Helper functions for MSVC compatibility when Int and Long are the same type.
// On platforms with __int128 (Linux/macOS) Long occupies its own variant slot (index 1).
// On MSVC/Windows Long=Int=long long; the is_long_ flag distinguishes them at runtime.
#if O2L_HAS_INT128
inline bool holds_Int_Value(const Value& v) {
    return v.index() == 0;
}
inline Int get_Int_Value(const Value& v) {
    return std::get<0>(v);
}
inline bool holds_Long_Value(const Value& v) {
    return v.index() == 1;
}
inline Long get_Long_Value(const Value& v) {
    return std::get<1>(v);
}
#else
inline bool holds_Int_Value(const Value& v) {
    return v.index() == 0 && !v.is_long_;
}
inline Int get_Int_Value(const Value& v) {
    return std::get<0>(v);
}
inline bool holds_Long_Value(const Value& v) {
    return v.index() == 0 && v.is_long_;
}
inline Long get_Long_Value(const Value& v) {
    return std::get<0>(v);
}
#endif

// Custom comparator for Value types for use in std::set and std::map
struct ValueComparator {
    bool operator()(const Value& a, const Value& b) const {
        return valuesLess(a, b);
    }
};

}  // namespace o2l
