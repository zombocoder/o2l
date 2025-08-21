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

#include <exception>
#include <string>
#include <vector>
#include <sstream>
#include "../Runtime/Value.hpp"

namespace o2l {

// Forward declaration
class Context;

class o2lException : public std::exception {
protected:
    std::string message_;
    std::vector<std::string> stack_trace_;
    std::string full_message_; // Cached full message with stack trace

public:
    explicit o2lException(const std::string& message) 
        : message_(message) {}
    
    o2lException(const std::string& message, const std::vector<std::string>& stack_trace)
        : message_(message), stack_trace_(stack_trace) {
        buildFullMessage();
    }
    
    // Constructor that takes a Context to extract stack trace
    o2lException(const std::string& message, const Context& context);

    const char* what() const noexcept override {
        if (full_message_.empty()) {
            return message_.c_str();
        }
        return full_message_.c_str();
    }

    const std::vector<std::string>& getStackTrace() const {
        return stack_trace_;
    }
    
    // Get the original message without stack trace formatting
    const std::string& getMessage() const {
        return message_;
    }
    
    // Get formatted error message with stack trace
    std::string getFormattedMessage() const {
        if (stack_trace_.empty()) {
            return message_;
        }
        
        std::ostringstream oss;
        oss << message_ << "\nStack trace:\n";
        for (const auto& frame : stack_trace_) {
            oss << "  " << frame << "\n";
        }
        return oss.str();
    }

private:
    void buildFullMessage() {
        if (!stack_trace_.empty()) {
            full_message_ = getFormattedMessage();
        } else {
            full_message_ = message_;
        }
    }
};

class SyntaxError : public o2lException {
public:
    explicit SyntaxError(const std::string& message) 
        : o2lException("Syntax Error: " + message) {}
    
    SyntaxError(const std::string& message, const std::vector<std::string>& stack_trace)
        : o2lException("Syntax Error: " + message, stack_trace) {}
        
    SyntaxError(const std::string& message, const Context& context)
        : o2lException("Syntax Error: " + message, context) {}
};

class EvaluationError : public o2lException {
public:
    explicit EvaluationError(const std::string& message) 
        : o2lException("Evaluation Error: " + message) {}
    
    EvaluationError(const std::string& message, const std::vector<std::string>& stack_trace)
        : o2lException("Evaluation Error: " + message, stack_trace) {}
        
    EvaluationError(const std::string& message, const Context& context)
        : o2lException("Evaluation Error: " + message, context) {}
};

class TypeMismatchError : public o2lException {
public:
    explicit TypeMismatchError(const std::string& message) 
        : o2lException("Type Mismatch Error: " + message) {}
    
    TypeMismatchError(const std::string& message, const std::vector<std::string>& stack_trace)
        : o2lException("Type Mismatch Error: " + message, stack_trace) {}
        
    TypeMismatchError(const std::string& message, const Context& context)
        : o2lException("Type Mismatch Error: " + message, context) {}
};

class UnresolvedReferenceError : public o2lException {
public:
    explicit UnresolvedReferenceError(const std::string& message) 
        : o2lException("Unresolved Reference Error: " + message) {}
    
    UnresolvedReferenceError(const std::string& message, const std::vector<std::string>& stack_trace)
        : o2lException("Unresolved Reference Error: " + message, stack_trace) {}
        
    UnresolvedReferenceError(const std::string& message, const Context& context)
        : o2lException("Unresolved Reference Error: " + message, context) {}
};

// Special exception for handling return statements - not an error, but control flow
class ReturnException : public std::exception {
private:
    Value return_value_;

public:
    explicit ReturnException(const Value& value) : return_value_(value) {}
    
    const Value& getValue() const { return return_value_; }
    
    const char* what() const noexcept override {
        return "Return statement executed (not an error)";
    }
};

// Special exception for handling break statements - not an error, but control flow
class BreakException : public std::exception {
public:
    BreakException() = default;
    
    const char* what() const noexcept override {
        return "Break statement executed (not an error)";
    }
};

// Exception for user-thrown errors via throw statements
class UserException : public o2lException {
private:
    Value thrown_value_;

public:
    explicit UserException(const Value& value) 
        : o2lException("User thrown error"), thrown_value_(value) {}
    
    UserException(const Value& value, const Context& context)
        : o2lException("User thrown error", context), thrown_value_(value) {}
    
    const Value& getThrownValue() const { return thrown_value_; }
    
    std::string getFormattedMessage() const {
        std::string message = "Thrown: " + valueToString(thrown_value_);
        
        if (!stack_trace_.empty()) {
            std::ostringstream oss;
            oss << message << "\nStack trace:\n";
            for (const auto& frame : stack_trace_) {
                oss << "  " << frame << "\n";
            }
            return oss.str();
        }
        return message;
    }
};

// Utility macros for easy error throwing with stack traces
#define THROW_ERROR_WITH_CONTEXT(ErrorType, message, context) \
    throw ErrorType(message, context)

#define THROW_EVALUATION_ERROR(message, context) \
    THROW_ERROR_WITH_CONTEXT(EvaluationError, message, context)

#define THROW_TYPE_MISMATCH_ERROR(message, context) \
    THROW_ERROR_WITH_CONTEXT(TypeMismatchError, message, context)

#define THROW_UNRESOLVED_REFERENCE_ERROR(message, context) \
    THROW_ERROR_WITH_CONTEXT(UnresolvedReferenceError, message, context)

} // namespace o2l