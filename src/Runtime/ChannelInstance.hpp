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

#include <deque>
#include <memory>
#include "Value.hpp"

namespace o2l {

class ChannelInstance {
public:
    explicit ChannelInstance(size_t capacity = 0);
    virtual ~ChannelInstance() = default;

    void send(const Value& value);
    Value receive();
    void close();
    bool isOpen() const { return !closed_; }
    size_t getCapacity() const { return capacity_; }

private:
    size_t capacity_;
    bool closed_ = false;
    std::deque<Value> buffer_;
    
    // Coroutines waiting to send (for unbuffered or full buffered channels)
    struct WaitingSender {
        uint64_t coroutine_id;
        Value value;
    };
    std::deque<WaitingSender> waiting_senders_;
    
    // Coroutines waiting to receive (when buffer is empty)
    std::deque<uint64_t> waiting_receivers_;
};

} // namespace o2l
