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

#include "WhileStatementNode.hpp"

#include <cstddef>

#include "../Common/Exceptions.hpp"
#include "../Runtime/Scheduler.hpp"
#include "../Runtime/Coroutine.hpp"

namespace o2l {

namespace {
// Resume markers pushed onto Coroutine::block_resume_stack when a coroutine
// suspends inside a while-loop. The while node is treated as a two-position
// sequence: position 0 re-evaluates the loop condition, position 1 resumes
// directly into the loop body (skipping the condition re-check). Only a
// body-suspend is ever recorded today, so the value acts as a flag, but naming
// both positions documents how the marker is interpreted on resume and removes
// the bare magic number.
constexpr std::size_t kResumeAtCondition = 0;
constexpr std::size_t kResumeInBody = 1;
}  // namespace

WhileStatementNode::WhileStatementNode(ASTNodePtr condition, ASTNodePtr body)
    : condition_(std::move(condition)), body_(std::move(body)) {}

Value WhileStatementNode::evaluate(Context& context) {
    Value result = Value{};  // Default empty value

    auto& scheduler = Scheduler::instance();
    auto current_coro = scheduler.currentCoroutine();

    while (true) {
        // A recorded marker other than kResumeAtCondition means we suspended
        // INSIDE the body, so we skip the condition check for this first
        // "resume" iteration and re-enter the body directly.
        bool skip_condition = false;
        if (current_coro && !current_coro->block_resume_stack.empty()) {
            skip_condition = current_coro->block_resume_stack.back() != kResumeAtCondition;
            current_coro->block_resume_stack.pop_back();
        }

        if (!skip_condition) {
            // Evaluate the condition
            Value condition_value = condition_->evaluate(context);

            // Check if condition is boolean
            if (!std::holds_alternative<Bool>(condition_value)) {
                throw TypeMismatchError("While condition must evaluate to Bool, got " +
                                        o2l::getTypeName(condition_value));
            }

            // Check if condition is true
            bool condition_bool = std::get<Bool>(condition_value);
            if (!condition_bool) {
                break;  // Exit loop if condition is false
            }
        }

        // Execute the body
        try {
            result = body_->evaluate(context);
            // If body completes without suspension, loop continues normally.
        } catch (const BreakException&) {
            break;
        } catch (const ContinueException&) {
            continue;
        } catch (const SuspendException&) {
            if (current_coro) {
                // Suspended inside the body: on resume, re-enter the body and
                // skip the condition re-check.
                current_coro->block_resume_stack.push_back(kResumeInBody);
            }
            throw;
        }
    }

    return result;  // Return the last result from the body
}

std::string WhileStatementNode::toString() const {
    return "While(" + condition_->toString() + ", " + body_->toString() + ")";
}

}  // namespace o2l
