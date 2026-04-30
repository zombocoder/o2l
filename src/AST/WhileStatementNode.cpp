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

#include "../Common/Exceptions.hpp"
#include "../Runtime/Scheduler.hpp"
#include "../Runtime/Coroutine.hpp"

namespace o2l {

WhileStatementNode::WhileStatementNode(ASTNodePtr condition, ASTNodePtr body)
    : condition_(std::move(condition)), body_(std::move(body)) {}

Value WhileStatementNode::evaluate(Context& context) {
    Value result = Value{};  // Default empty value

    auto& scheduler = Scheduler::instance();
    auto current_coro = scheduler.currentCoroutine();

    while (true) {
        // If we have a non-zero index, it means we suspended INSIDE the body.
        // So we skip the condition check for this first "resume" iteration.
        bool skip_condition = false;
        if (current_coro && !current_coro->block_resume_stack.empty()) {
            skip_condition = current_coro->block_resume_stack.back() != 0;
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
                current_coro->block_resume_stack.push_back(1); // 1 means resuming body
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
