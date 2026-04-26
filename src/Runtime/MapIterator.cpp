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

#include "MapIterator.hpp"

#include <sstream>

#include "../Common/Exceptions.hpp"
#include "MapObject.hpp"

namespace o2l {

MapIterator::MapIterator(std::shared_ptr<MapInstance> map_instance) : map_instance_(map_instance) {
    const auto& entries = map_instance_->getEntries();
    current_iterator_ = entries.begin();
    end_iterator_ = entries.end();
}

bool MapIterator::hasNext() const {
    return current_iterator_ != end_iterator_;
}

Value MapIterator::nextKey() {
    if (!hasNext()) {
        throw EvaluationError("MapIterator has no more keys");
    }
    // Advance and cache the current entry so nextValue() can return it without re-advancing.
    cached_key_ = current_iterator_->first;
    cached_value_ = current_iterator_->second;
    has_cached_entry_ = true;
    ++current_iterator_;
    return cached_key_;
}

Value MapIterator::nextValue() {
    if (has_cached_entry_) {
        // Return the value cached by the preceding nextKey() call.
        has_cached_entry_ = false;
        return cached_value_;
    }
    // Standalone nextValue() without a preceding nextKey() — advance normally.
    if (!hasNext()) {
        throw EvaluationError("MapIterator has no more values");
    }
    Value value = current_iterator_->second;
    ++current_iterator_;
    return value;
}

Value MapIterator::nextEntry() {
    if (!hasNext()) {
        throw EvaluationError("MapIterator has no more entries");
    }
    Value key = current_iterator_->first;
    Value value = current_iterator_->second;
    ++current_iterator_;
    has_cached_entry_ = false;
    std::string entry_str = valueToString(key) + ":" + valueToString(value);
    return Text(entry_str);
}

Value MapIterator::MapItem() {
    if (!hasNext()) {
        throw EvaluationError("MapIterator has no more items");
    }
    Value key = current_iterator_->first;
    Value value = current_iterator_->second;
    ++current_iterator_;
    has_cached_entry_ = false;
    auto map_object = std::make_shared<MapObject>(key, value, map_instance_->getKeyTypeName(),
                                                  map_instance_->getValueTypeName());
    return Value(map_object);
}

void MapIterator::reset() {
    const auto& entries = map_instance_->getEntries();
    current_iterator_ = entries.begin();
    end_iterator_ = entries.end();
    has_cached_entry_ = false;
}

size_t MapIterator::getCurrentIndex() const {
    const auto& entries = map_instance_->getEntries();
    return std::distance(entries.begin(), current_iterator_);
}

size_t MapIterator::getTotalSize() const {
    return map_instance_->size();
}

std::string MapIterator::toString() const {
    std::ostringstream oss;
    oss << "MapIterator(index=" << getCurrentIndex() << ", total=" << getTotalSize()
        << ", hasNext=" << (hasNext() ? "true" : "false") << ")";
    return oss.str();
}

}  // namespace o2l
