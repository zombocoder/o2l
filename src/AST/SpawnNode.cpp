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

#include "SpawnNode.hpp"
#include "../Runtime/Scheduler.hpp"
#include "../Runtime/CoroutineHandle.hpp"

namespace o2l {

SpawnNode::SpawnNode(ASTNodePtr body) : body_(std::move(body)) {}

Value SpawnNode::evaluate(Context& context) {
    auto& scheduler = Scheduler::instance();
    
    // 1. Clone context
    Context cloned = context.clone();
    
    // 2. Spawn coroutine
    uint64_t id = scheduler.spawn(body_.get(), std::move(cloned));
    
    // 3. Return handle
    return Value(std::make_shared<CoroutineHandle>(id));
}

std::string SpawnNode::toString() const {
    return "spawn " + body_->toString();
}

}  // namespace o2l
