#include "MapInstance.hpp"

#include <sstream>

#include "../Common/Exceptions.hpp"
#include "Value.hpp"

namespace o2l {

MapInstance::MapInstance(const std::string& key_type, const std::string& value_type)
    : key_type_name_(key_type), value_type_name_(value_type) {}

void MapInstance::put(const Value& key, const Value& value) {
    entries_[key] = value;
}

Value MapInstance::get(const Value& key) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) {
        throw EvaluationError("Key not found in map");
    }
    return it->second;
}

bool MapInstance::contains(const Value& key) const {
    return entries_.find(key) != entries_.end();
}

void MapInstance::remove(const Value& key) {
    auto it = entries_.find(key);
    if (it == entries_.end()) {
        throw EvaluationError("Key not found in map");
    }
    entries_.erase(it);
}

void MapInstance::clear() {
    entries_.clear();
}

std::vector<Value> MapInstance::keys() const {
    std::vector<Value> result;
    result.reserve(entries_.size());
    for (const auto& pair : entries_) {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<Value> MapInstance::values() const {
    std::vector<Value> result;
    result.reserve(entries_.size());
    for (const auto& pair : entries_) {
        result.push_back(pair.second);
    }
    return result;
}

size_t MapInstance::size() const {
    return entries_.size();
}

bool MapInstance::empty() const {
    return entries_.empty();
}

std::string MapInstance::toString() const {
    if (entries_.empty()) {
        return "{}";
    }

    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& pair : entries_) {
        if (!first) {
            oss << ", ";
        }
        oss << valueToString(pair.first) << ": " << valueToString(pair.second);
        first = false;
    }
    oss << "}";
    return oss.str();
}

const std::string& MapInstance::getKeyTypeName() const {
    return key_type_name_;
}

const std::string& MapInstance::getValueTypeName() const {
    return value_type_name_;
}

const std::map<Value, Value>& MapInstance::getEntries() const {
    return entries_;
}

std::map<Value, Value>& MapInstance::getEntries() {
    return entries_;
}

}  // namespace o2l