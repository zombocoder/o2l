#pragma once

#include <string>

#include "Value.hpp"

namespace o2l {

/**
 * MapObject represents a key-value pair from a Map
 * This provides a better user experience for Map iteration
 */
class MapObject {
   private:
    Value key_;
    Value value_;
    std::string key_type_name_;
    std::string value_type_name_;

   public:
    MapObject(const Value& key, const Value& value, const std::string& key_type_name,
              const std::string& value_type_name);

    // Getter methods for accessing key and value
    Value getKey() const;
    Value getVal() const;
    Value getValue() const;  // Alias for getVal()

    // Type information
    std::string getKeyTypeName() const;
    std::string getValueTypeName() const;

    // String representation
    std::string toString() const;
};

}  // namespace o2l