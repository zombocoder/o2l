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

#include <cstdint>
#include <string>
#include <chrono>
#include <vector>
#include "Context.hpp"
#include "../AST/Node.hpp"

namespace o2l {

struct Coroutine {
    enum class State { Ready, Running, Suspended, Completed, Failed };

    uint64_t id;
    State state;
    Context context;
    ASTNode* body; // Raw pointer: AST is owned by the parser output/Main nodes
    
    Value resume_value;
    bool has_resume_value = false;
    std::string suspend_reason;
    std::chrono::steady_clock::time_point wake_time;
    
    // Progress tracking for statement-level resumption
    std::vector<size_t> block_resume_stack;
    
    Value result;
    std::string error_message;

    Coroutine(uint64_t id, ASTNode* body, Context context);
};

} // namespace o2l
