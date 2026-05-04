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

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "Value.hpp"

namespace o2l {

class IOThreadPool {
public:
    static IOThreadPool& instance();

    void submit(std::function<Value()> work, uint64_t coroutine_id);
    void shutdown();

private:
    IOThreadPool(size_t threads = 4);
    ~IOThreadPool();

    std::vector<std::thread> workers_;
    std::queue<std::pair<std::function<Value()>, uint64_t>> tasks_;

    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;

    void workerThread();
};

} // namespace o2l
