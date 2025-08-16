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

#include "TryCatchFinallyNode.hpp"

#include "../Common/Exceptions.hpp"
#include "../Runtime/Context.hpp"
#include "../Runtime/ErrorInstance.hpp"

namespace o2l {

TryCatchFinallyNode::TryCatchFinallyNode(ASTNodePtr try_block, ASTNodePtr catch_block,
                                         ASTNodePtr finally_block, std::string catch_variable,
                                         const SourceLocation& location)
    : ASTNode(location),
      try_block_(std::move(try_block)),
      catch_block_(std::move(catch_block)),
      finally_block_(std::move(finally_block)),
      catch_variable_(std::move(catch_variable)) {}

Value TryCatchFinallyNode::evaluate(Context& context) {
    Value result = Int(0);  // Default result
    bool exception_thrown = false;
    Value caught_exception;

    // Execute try block
    try {
        result = try_block_->evaluate(context);
    } catch (const UserException& e) {
        // User threw an exception with throw statement
        exception_thrown = true;
        caught_exception = e.getThrownValue();
    } catch (const o2lException& e) {
        // System exception - convert to Error object
        exception_thrown = true;
        auto error_instance = std::make_shared<ErrorInstance>(e.getMessage(), "SYSTEM_ERROR");
        caught_exception = Value(error_instance);
    } catch (const ReturnException& e) {
        // Return exceptions should not be caught - propagate them up
        if (finally_block_) {
            finally_block_->evaluate(context);
        }
        throw;
    }

    // Execute catch block if exception was thrown and catch block exists
    if (exception_thrown && catch_block_) {
        // Create new scope for catch block
        context.pushScope();

        try {
            // Bind the caught exception to the catch variable
            if (!catch_variable_.empty()) {
                context.defineVariable(catch_variable_, caught_exception);
            }

            // Execute catch block
            result = catch_block_->evaluate(context);

            context.popScope();
        } catch (...) {
            context.popScope();
            // Execute finally block before re-throwing
            if (finally_block_) {
                try {
                    finally_block_->evaluate(context);
                } catch (...) {
                    // Suppress finally exceptions when re-throwing catch exceptions
                }
            }
            throw;
        }
    } else if (exception_thrown) {
        // No catch block but exception was thrown - execute finally then re-throw
        if (finally_block_) {
            try {
                finally_block_->evaluate(context);
            } catch (...) {
                // Suppress finally exceptions when re-throwing original exception
            }
        }

        // Re-throw original exception
        if (std::holds_alternative<std::shared_ptr<ErrorInstance>>(caught_exception)) {
            // This was a system exception - throw as UserException
            throw UserException(caught_exception, context);
        } else {
            // This was a user exception - re-throw as UserException
            throw UserException(caught_exception, context);
        }
    }

    // Execute finally block (always executes if present)
    if (finally_block_) {
        try {
            finally_block_->evaluate(context);
        } catch (const ReturnException& e) {
            // Return in finally block overrides try/catch result
            throw;
        } catch (...) {
            // Finally block exceptions override try/catch results
            throw;
        }
    }

    return result;
}

std::string TryCatchFinallyNode::toString() const {
    std::string result = "Try { " + try_block_->toString() + " }";

    if (catch_block_) {
        result += " Catch(" + catch_variable_ + ") { " + catch_block_->toString() + " }";
    }

    if (finally_block_) {
        result += " Finally { " + finally_block_->toString() + " }";
    }

    return result;
}

}  // namespace o2l