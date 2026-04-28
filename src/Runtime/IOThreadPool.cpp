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

#include "IOThreadPool.hpp"
#include "Scheduler.hpp"

namespace o2l {

IOThreadPool& IOThreadPool::instance() {
    static IOThreadPool instance;
    return instance;
}

IOThreadPool::IOThreadPool(size_t threads) {
    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this] { workerThread(); });
    }
}

IOThreadPool::~IOThreadPool() {
    shutdown();
}

void IOThreadPool::submit(std::function<Value()> work, uint64_t coroutine_id) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (stop_) return;
        tasks_.emplace(std::move(work), coroutine_id);
    }
    condition_.notify_one();
}

void IOThreadPool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (stop_) return;
        stop_ = true;
    }
    condition_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();
}

void IOThreadPool::workerThread() {
    while (true) {
        std::pair<std::function<Value()>, uint64_t> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) return;
            task = std::move(tasks_.front());
            tasks_.pop();
        }

        // Execute the work
        Value result = task.first();

        // Enqueue completion to scheduler
        Scheduler::instance().enqueueReady(task.second, result);
    }
}

} // namespace o2l
