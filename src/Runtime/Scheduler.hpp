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
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <mutex>
#include <exception>
#include "Coroutine.hpp"

namespace o2l {

class Scheduler {
public:
    static Scheduler& instance();

    // Core lifecycle
    uint64_t spawn(ASTNode* body, Context cloned_context);
    void run(); // Main loop
    void reset(); // Clear state
    
    // Suspend operations (throw SuspendException)
    void yield();
    void suspendForSleep(uint64_t ms);
    void suspendCurrent(const std::string& reason);
    void suspendForIO(std::function<Value()> work);
    
    // Resume operations (thread-safe)
    void resumeCoroutine(uint64_t id, Value resume_value = Int(0));
    void enqueueReady(uint64_t id, Value result);

    bool hasResumeValue() const;
    Value consumeResumeValue();
    
    // State queries
    bool isActive() const { return active_; }
    Coroutine* currentCoroutine() { return current_; }
    
    // Root result and exception (from the first coroutine spawned, usually Main)
    Value getRootResult() const;
    std::exception_ptr getRootException() const { return root_exception_; }

private:
    Scheduler() = default;
    
    std::deque<std::unique_ptr<Coroutine>> ready_queue_;
    std::vector<std::unique_ptr<Coroutine>> timer_queue_;
    std::map<uint64_t, std::unique_ptr<Coroutine>> suspended_;
    
    // Thread-safe completion queue
    std::mutex completion_mutex_;
    std::deque<std::pair<uint64_t, Value>> io_completed_;

    std::unique_ptr<Coroutine> running_coro_;
    Coroutine* current_ = nullptr;
    uint64_t next_id_ = 0;
    bool active_ = false;
    Value root_result_ = Int(0);
    std::exception_ptr root_exception_;

    void wakeTimers();
    void processCompletions();
    void scheduleNext();
};

} // namespace o2l
