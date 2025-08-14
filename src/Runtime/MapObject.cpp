#include "MapObject.hpp"

namespace o2l {

MapObject::MapObject(const Value& key, const Value& value, const std::string& key_type_name,
                     const std::string& value_type_name)
    : key_(key), value_(value), key_type_name_(key_type_name), value_type_name_(value_type_name) {}

Value MapObject::getKey() const {
    return key_;
}

Value MapObject::getVal() const {
    return value_;
}

Value MapObject::getValue() const {
    return value_;
}

std::string MapObject::getKeyTypeName() const {
    return key_type_name_;
}

std::string MapObject::getValueTypeName() const {
    return value_type_name_;
}

std::string MapObject::toString() const {
    return "MapObject{key: " + valueToString(key_) + ", value: " + valueToString(value_) + "}";
}

}  // namespace o2l