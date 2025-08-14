#pragma once

#include <memory>
#include <string>

#include "Value.hpp"

namespace o2l {

// Error object type for structured error handling
class ErrorInstance : public std::enable_shared_from_this<ErrorInstance> {
   private:
    std::string message_;
    std::string code_;
    Value cause_;  // Optional nested error cause

   public:
    explicit ErrorInstance(const std::string& message)
        : message_(message), code_("ERROR"), cause_(Int(0)) {}

    ErrorInstance(const std::string& message, const std::string& code)
        : message_(message), code_(code), cause_(Int(0)) {}

    ErrorInstance(const std::string& message, const std::string& code, const Value& cause)
        : message_(message), code_(code), cause_(cause) {}

    // Getter methods
    const std::string& getMessage() const {
        return message_;
    }
    const std::string& getCode() const {
        return code_;
    }
    const Value& getCause() const {
        return cause_;
    }

    // Setter methods
    void setMessage(const std::string& message) {
        message_ = message;
    }
    void setCode(const std::string& code) {
        code_ = code;
    }
    void setCause(const Value& cause) {
        cause_ = cause;
    }

    // Convert to string representation
    std::string toString() const {
        std::string result = code_ + ": " + message_;
        // TODO: Add cause chain if needed
        return result;
    }
};

}  // namespace o2l