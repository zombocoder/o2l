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

// Built-in immutable types
using Text = std::string;
using Int = long long;
#ifdef __SIZEOF_INT128__
// Suppress pedantic warning for __int128 which is a widely supported extension
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
using Long = __int128;
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#else
using Long = long long;  // Fallback to 64-bit if 128-bit not available
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

// The main Value variant that represents all possible O²L values
struct Value
    : public std::variant<Int, Long, Float, Double, Text, Bool, Char,
                          std::shared_ptr<ObjectInstance>, std::shared_ptr<EnumInstance>,
                          std::shared_ptr<RecordType>, std::shared_ptr<RecordInstance>,
                          std::shared_ptr<ProtocolInstance>, std::shared_ptr<ListInstance>,
                          std::shared_ptr<ListIterator>, std::shared_ptr<RepeatIterator>,
                          std::shared_ptr<MapInstance>, std::shared_ptr<MapIterator>,
                          std::shared_ptr<MapObject>, std::shared_ptr<SetInstance>,
                          std::shared_ptr<SetIterator>, std::shared_ptr<ErrorInstance>,
                          std::shared_ptr<ResultInstance>, ValueList, ValueMap, ValueOptional> {
    using variant::variant;
};

// Utility functions for Value operations
std::string valueToString(const Value& value);
std::string getTypeName(const Value& value);
bool valuesEqual(const Value& a, const Value& b);
bool valuesLess(const Value& a, const Value& b);

}  // namespace o2l