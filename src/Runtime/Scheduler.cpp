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

#include "Scheduler.hpp"
#include "IOThreadPool.hpp"
#include "../Common/Exceptions.hpp"
#include <iostream>
#include <thread>
#include <algorithm>

namespace o2l {

Scheduler& Scheduler::instance() {
    static Scheduler instance;
    return instance;
}

uint64_t Scheduler::spawn(ASTNode* body, Context cloned_context) {
    uint64_t id = next_id_++;
    auto coro = std::make_unique<Coroutine>(id, body, std::move(cloned_context));
    ready_queue_.push_back(std::move(coro));
    return id;
}

void Scheduler::reset() {
    ready_queue_.clear();
    timer_queue_.clear();
    suspended_.clear();
    {
        std::unique_lock<std::mutex> lock(completion_mutex_);
        io_completed_.clear();
    }
    running_coro_.reset();
    current_ = nullptr;
    next_id_ = 0;
    active_ = false;
    root_result_ = Int(0);
    root_exception_ = nullptr;
}

void Scheduler::run() {
    active_ = true;
    while (!ready_queue_.empty() || !timer_queue_.empty() || !suspended_.empty()) {
        wakeTimers();
        processCompletions();
        
        if (ready_queue_.empty()) {
            if (!timer_queue_.empty() || !suspended_.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            break;
        }

        running_coro_ = std::move(ready_queue_.front());
        ready_queue_.pop_front();
        
        current_ = running_coro_.get();
        current_->state = Coroutine::State::Running;

        try {
            Value result = current_->body->evaluate(current_->context);
            current_->state = Coroutine::State::Completed;
            current_->result = result;
            if (current_->id == 0) {
                root_result_ = result;
            }
            running_coro_.reset();
            current_ = nullptr;
        } catch (const SuspendException& e) {
            // Coroutine was moved to another queue by the suspend method
            // which reset running_coro_ and current_
        } catch (const std::exception& e) {
            current_->state = Coroutine::State::Failed;
            current_->error_message = e.what();
            if (current_->id == 0) {
                root_exception_ = std::current_exception();
            }
            std::cerr << "Error in coroutine #" << current_->id << ": " << e.what() << std::endl;
            running_coro_.reset();
            current_ = nullptr;
        }
    }
    active_ = false;
}

void Scheduler::yield() {
    if (!running_coro_) return;

    running_coro_->state = Coroutine::State::Ready;
    running_coro_->has_resume_value = true;
    running_coro_->resume_value = Int(0);
    ready_queue_.push_back(std::move(running_coro_));
    current_ = nullptr;
    throw SuspendException("yield");
}

void Scheduler::suspendForSleep(uint64_t ms) {
    if (!running_coro_) return;
    
    running_coro_->state = Coroutine::State::Suspended;
    running_coro_->suspend_reason = "sleep";
    running_coro_->wake_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
    running_coro_->has_resume_value = true;
    running_coro_->resume_value = Int(0);
    
    timer_queue_.push_back(std::move(running_coro_));
    current_ = nullptr;
    throw SuspendException("sleep");
}

void Scheduler::suspendCurrent(const std::string& reason) {
    if (!running_coro_) return;

    uint64_t id = running_coro_->id;
    running_coro_->state = Coroutine::State::Suspended;
    running_coro_->suspend_reason = reason;

    suspended_[id] = std::move(running_coro_);
    current_ = nullptr;
    throw SuspendException(reason);
}

void Scheduler::suspendForIO(std::function<Value()> work) {
    if (!running_coro_) return;

    uint64_t id = running_coro_->id;
    running_coro_->state = Coroutine::State::Suspended;
    running_coro_->suspend_reason = "io";

    suspended_[id] = std::move(running_coro_);
    current_ = nullptr;

    IOThreadPool::instance().submit(std::move(work), id);

    throw SuspendException("io");
}

void Scheduler::resumeCoroutine(uint64_t id, Value resume_value) {
    auto it = suspended_.find(id);
    if (it != suspended_.end()) {
        auto coro = std::move(it->second);
        suspended_.erase(it);
        
        coro->state = Coroutine::State::Ready;
        coro->resume_value = resume_value;
        coro->has_resume_value = true;
        ready_queue_.push_back(std::move(coro));
    }
}

void Scheduler::enqueueReady(uint64_t id, Value result) {
    std::unique_lock<std::mutex> lock(completion_mutex_);
    io_completed_.emplace_back(id, result);
}

bool Scheduler::hasResumeValue() const {
    return current_ && current_->has_resume_value;
}

Value Scheduler::consumeResumeValue() {
    if (!current_ || !current_->has_resume_value) {
        return Int(0);
    }
    Value val = current_->resume_value;
    current_->resume_value = Int(0);
    current_->has_resume_value = false;
    return val;
}

Value Scheduler::getRootResult() const {
    return root_result_;
}

void Scheduler::wakeTimers() {
    auto now = std::chrono::steady_clock::now();
    for (auto it = timer_queue_.begin(); it != timer_queue_.end(); ) {
        if ((*it)->wake_time <= now) {
            auto coro = std::move(*it);
            coro->state = Coroutine::State::Ready;
            ready_queue_.push_back(std::move(coro));
            it = timer_queue_.erase(it);
        } else {
            ++it;
        }
    }
}

void Scheduler::processCompletions() {
    std::deque<std::pair<uint64_t, Value>> completions;
    {
        std::unique_lock<std::mutex> lock(completion_mutex_);
        completions = std::move(io_completed_);
    }

    for (const auto& comp : completions) {
        resumeCoroutine(comp.first, comp.second);
    }
}

} // namespace o2l
