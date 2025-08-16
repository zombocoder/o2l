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

#include "SetInstance.hpp"

#include <sstream>

namespace o2l {

SetInstance::SetInstance(const std::string& element_type) : element_type_name_(element_type) {}

void SetInstance::add(const Value& element) {
    elements_.insert(element);
}

bool SetInstance::contains(const Value& element) const {
    return elements_.find(element) != elements_.end();
}

void SetInstance::remove(const Value& element) {
    elements_.erase(element);
}

void SetInstance::clear() {
    elements_.clear();
}

std::vector<Value> SetInstance::elements() const {
    std::vector<Value> result;
    result.reserve(elements_.size());
    for (const auto& element : elements_) {
        result.push_back(element);
    }
    return result;
}

size_t SetInstance::size() const {
    return elements_.size();
}

bool SetInstance::empty() const {
    return elements_.empty();
}

std::string SetInstance::toString() const {
    if (elements_.empty()) {
        return "()";
    }

    std::ostringstream oss;
    oss << "(";

    bool first = true;
    for (const auto& element : elements_) {
        if (!first) {
            oss << ", ";
        }
        oss << valueToString(element);
        first = false;
    }

    oss << ")";
    return oss.str();
}

const std::string& SetInstance::getElementTypeName() const {
    return element_type_name_;
}

const std::set<Value, ValueComparator>& SetInstance::getElements() const {
    return elements_;
}

std::set<Value, ValueComparator>& SetInstance::getElements() {
    return elements_;
}

}  // namespace o2l