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

#include "BlockNode.hpp"

#include <iostream>
#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/Scheduler.hpp"
#include "../Runtime/Coroutine.hpp"

namespace o2l {

BlockNode::BlockNode(std::vector<ASTNodePtr> statements) : statements_(std::move(statements)) {}

Value BlockNode::evaluate(Context& context) {
    Value result = Int(0);  // Default return value
    
    size_t start_index = 0;
    auto& scheduler = Scheduler::instance();
    auto current_coro = scheduler.currentCoroutine();
    
    // Concurrency support: resume from last suspended statement
    if (current_coro && !current_coro->block_resume_stack.empty()) {
        start_index = current_coro->block_resume_stack.back();
        current_coro->block_resume_stack.pop_back();
        if (start_index > 0) {
            std::cerr << "  [BlockNode] Resuming at index " << start_index << std::endl;
        }
    }

    for (size_t i = start_index; i < statements_.size(); ++i) {
        try {
            result = statements_[i]->evaluate(context);
        } catch (const ReturnException& e) {
            throw;
        } catch (const BreakException& e) {
            throw;
        } catch (const ContinueException& e) {
            throw;
        } catch (const SuspendException& e) {
            // Save current index for resumption
            if (current_coro) {
                current_coro->block_resume_stack.push_back(i);
                std::cerr << "  [BlockNode] Suspending at index " << i << std::endl;
            }
            throw;
        }
    }

    // Block finished successfully
    return result;
}

std::string BlockNode::toString() const {
    std::string result = "Block {";
    for (size_t i = 0; i < statements_.size(); ++i) {
        if (i > 0) result += "; ";
        result += statements_[i]->toString();
    }
    result += "}";
    return result;
}

}  // namespace o2l
