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

#include "SetInstance.hpp"
#include "Value.hpp"

namespace o2l {

class SetIterator {
   private:
    std::shared_ptr<SetInstance> set_instance_;
    std::set<Value>::const_iterator current_iterator_;
    std::set<Value>::const_iterator end_iterator_;
    size_t current_index_;

   public:
    SetIterator(std::shared_ptr<SetInstance> set_instance);

    // Iterator operations
    bool hasNext() const;
    Value next();

    // Utility methods
    void reset();
    size_t getCurrentIndex() const;
    size_t getTotalSize() const;

    // String representation
    std::string toString() const;
};

}  // namespace o2l