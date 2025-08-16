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