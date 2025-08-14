#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Value.hpp"

namespace o2l {

class MapInstance {
   private:
    std::map<Value, Value> entries_;
    std::string key_type_name_;
    std::string value_type_name_;

   public:
    MapInstance(const std::string& key_type = "Value", const std::string& value_type = "Value");

    // Map operations
    void put(const Value& key, const Value& value);
    Value get(const Value& key) const;
    bool contains(const Value& key) const;
    void remove(const Value& key);
    void clear();

    // Collection operations
    std::vector<Value> keys() const;
    std::vector<Value> values() const;
    size_t size() const;
    bool empty() const;

    // String representation
    std::string toString() const;

    // Type information
    const std::string& getKeyTypeName() const;
    const std::string& getValueTypeName() const;

    // Iterator access for internal use
    const std::map<Value, Value>& getEntries() const;
    std::map<Value, Value>& getEntries();
};

}  // namespace o2l