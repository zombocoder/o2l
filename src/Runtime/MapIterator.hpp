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
#include <string>

#include "MapInstance.hpp"
#include "Value.hpp"

// Forward declaration to avoid circular dependency
namespace o2l {
class MapObject;
}

namespace o2l {

class MapIterator {
   private:
    std::shared_ptr<MapInstance> map_instance_;
    std::map<Value, Value>::const_iterator current_iterator_;
    std::map<Value, Value>::const_iterator end_iterator_;

   public:
    MapIterator(std::shared_ptr<MapInstance> map_instance);

    // Iterator operations
    bool hasNext() const;
    Value nextKey();
    Value nextValue();
    Value nextEntry();  // Returns a Record with key and value fields
    Value MapItem();    // Returns a MapObject with key and value

    // Utility methods
    void reset();
    size_t getCurrentIndex() const;
    size_t getTotalSize() const;

    // String representation
    std::string toString() const;
};

}  // namespace o2l