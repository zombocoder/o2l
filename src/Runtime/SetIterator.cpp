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

#include "SetIterator.hpp"

#include <sstream>
#include <stdexcept>

namespace o2l {

SetIterator::SetIterator(std::shared_ptr<SetInstance> set_instance)
    : set_instance_(set_instance), current_index_(0) {
    if (!set_instance_) {
        throw std::invalid_argument("SetIterator: set_instance cannot be null");
    }

    current_iterator_ = set_instance_->getElements().begin();
    end_iterator_ = set_instance_->getElements().end();
}

bool SetIterator::hasNext() const {
    return current_iterator_ != end_iterator_;
}

Value SetIterator::next() {
    if (!hasNext()) {
        throw std::runtime_error("SetIterator: No more elements to iterate");
    }

    Value result = *current_iterator_;
    ++current_iterator_;
    ++current_index_;

    return result;
}

void SetIterator::reset() {
    current_iterator_ = set_instance_->getElements().begin();
    end_iterator_ = set_instance_->getElements().end();
    current_index_ = 0;
}

size_t SetIterator::getCurrentIndex() const {
    return current_index_;
}

size_t SetIterator::getTotalSize() const {
    return set_instance_->size();
}

std::string SetIterator::toString() const {
    std::ostringstream oss;
    oss << "SetIterator<" << set_instance_->getElementTypeName() << ">(index=" << current_index_
        << ", total=" << getTotalSize() << ")";
    return oss.str();
}

}  // namespace o2l