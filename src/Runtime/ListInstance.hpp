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
#include <string>
#include <vector>

#include "Value.hpp"

namespace o2l {

class ListInstance {
   private:
    std::vector<Value> elements_;
    std::string element_type_name_;

   public:
    ListInstance(const std::string& element_type = "Value");

    // List operations
    void add(const Value& element);
    Value get(size_t index) const;
    void remove(size_t index);
    void reverse();
    Value pop();

    // Utility methods
    size_t size() const;
    bool empty() const;
    void clear();

    // String representation
    std::string toString() const;

    // Element type information
    const std::string& getElementTypeName() const;

    // Iterator access for internal use
    const std::vector<Value>& getElements() const;
    std::vector<Value>& getElements();
};

}  // namespace o2l