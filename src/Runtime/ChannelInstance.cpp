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

#include "ChannelInstance.hpp"
#include "Scheduler.hpp"
#include "../Common/Exceptions.hpp"

namespace o2l {

ChannelInstance::ChannelInstance(size_t capacity) : capacity_(capacity) {}

void ChannelInstance::send(const Value& value) {
    if (closed_) {
        throw EvaluationError("Cannot send on closed channel");
    }

    auto& scheduler = Scheduler::instance();

    // If we just resumed from a send suspension, we're done
    if (scheduler.hasResumeValue()) {
        scheduler.consumeResumeValue();
        return;
    }

    // 1. If there's a waiting receiver, hand over the value immediately
    if (!waiting_receivers_.empty()) {
        uint64_t receiver_id = waiting_receivers_.front();
        waiting_receivers_.pop_front();
        
        scheduler.resumeCoroutine(receiver_id, value);
        return;
    }

    // 2. If it's a buffered channel and has space, buffer it
    if (capacity_ > 0 && buffer_.size() < capacity_) {
        buffer_.push_back(value);
        return;
    }

    // 3. Otherwise, suspend the sender
    if (scheduler.isActive()) {
        uint64_t coro_id = scheduler.currentCoroutine()->id;
        waiting_senders_.push_back({coro_id, value});
        
        scheduler.suspendCurrent("channel_send");
        // suspendCurrent throws SuspendException, so we never reach here
    } else {
        throw EvaluationError("Blocking channel send without active scheduler not supported");
    }
}

Value ChannelInstance::receive() {
    auto& scheduler = Scheduler::instance();

    // If we just resumed from a receive suspension, return the resumed value
    if (scheduler.hasResumeValue()) {
        return scheduler.consumeResumeValue();
    }

    // 1. If there's data in the buffer, take it
    if (!buffer_.empty()) {
        Value value = buffer_.front();
        buffer_.pop_front();
        
        // If there are waiting senders, move one into the buffer
        if (!waiting_senders_.empty()) {
            auto sender = waiting_senders_.front();
            waiting_senders_.pop_front();
            
            buffer_.push_back(sender.value);
            scheduler.resumeCoroutine(sender.coroutine_id, Value(Int(0)));
        }
        
        return value;
    }

    // 2. If it's unbuffered and there's a waiting sender, take their value
    if (capacity_ == 0 && !waiting_senders_.empty()) {
        auto sender = waiting_senders_.front();
        waiting_senders_.pop_front();
        
        Value value = sender.value;
        scheduler.resumeCoroutine(sender.coroutine_id, Value(Int(0)));
        return value;
    }

    // 3. If closed and empty, error
    if (closed_) {
        throw EvaluationError("Cannot receive from closed empty channel");
    }

    // 4. Otherwise, suspend the receiver
    if (scheduler.isActive()) {
        uint64_t coro_id = scheduler.currentCoroutine()->id;
        waiting_receivers_.push_back(coro_id);
        
        scheduler.suspendCurrent("channel_receive");
        // suspendCurrent throws SuspendException, so we never reach here
    } else {
        throw EvaluationError("Blocking channel receive without active scheduler not supported");
    }

    return Value(Int(0)); // Unreachable
}

void ChannelInstance::close() {
    if (closed_) return;
    closed_ = true;

    auto& scheduler = Scheduler::instance();

    // Wake all waiting receivers with an error (or they might need a sentinel value)
    // For now, let's just wake them; they will re-check closed_ and throw if buffer empty
    while (!waiting_receivers_.empty()) {
        uint64_t receiver_id = waiting_receivers_.front();
        waiting_receivers_.pop_front();
        scheduler.resumeCoroutine(receiver_id, Value(Int(0)));
    }

    // Wake all waiting senders with an error
    while (!waiting_senders_.empty()) {
        auto sender = waiting_senders_.front();
        waiting_senders_.pop_front();
        // We might want to resume them with an exception or just let them throw on next attempt
        scheduler.resumeCoroutine(sender.coroutine_id, Value(Int(0)));
    }
}

} // namespace o2l
