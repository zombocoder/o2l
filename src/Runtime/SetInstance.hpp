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
#include <set>
#include <string>
#include <vector>

#include "Value.hpp"

namespace o2l {

// Custom comparator for Value types using string representation
struct ValueComparator {
    bool operator()(const Value& a, const Value& b) const {
        return valueToString(a) < valueToString(b);
    }
};

class SetInstance {
   private:
    std::set<Value, ValueComparator> elements_;
    std::string element_type_name_;

   public:
    SetInstance(const std::string& element_type = "Value");

    // Set operations
    void add(const Value& element);
    bool contains(const Value& element) const;
    void remove(const Value& element);
    void clear();

    // Collection operations
    std::vector<Value> elements() const;
    size_t size() const;
    bool empty() const;

    // String representation
    std::string toString() const;

    // Type information
    const std::string& getElementTypeName() const;

    // Iterator access for internal use
    const std::set<Value, ValueComparator>& getElements() const;
    std::set<Value, ValueComparator>& getElements();
};

}  // namespace o2l