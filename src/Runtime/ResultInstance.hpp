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

#include <memory>
#include <string>

#include "Value.hpp"

namespace o2l {

// Result<T,E> type for functional error handling
class ResultInstance : public std::enable_shared_from_this<ResultInstance> {
   private:
    Value value_;                  // The success value (T)
    Value error_;                  // The error value (E)
    bool is_success_;              // Whether this represents success or error
    std::string value_type_name_;  // Type name for T
    std::string error_type_name_;  // Type name for E

   public:
    // Success constructor
    ResultInstance(const Value& value, const std::string& value_type, const std::string& error_type)
        : value_(value),
          error_(Int(0)),
          is_success_(true),
          value_type_name_(value_type),
          error_type_name_(error_type) {}

    // Error constructor
    static std::shared_ptr<ResultInstance> createError(const Value& error,
                                                       const std::string& value_type,
                                                       const std::string& error_type) {
        auto result = std::shared_ptr<ResultInstance>(new ResultInstance());
        result->value_ = Int(0);  // Default value for T
        result->error_ = error;
        result->is_success_ = false;
        result->value_type_name_ = value_type;
        result->error_type_name_ = error_type;
        return result;
    }

    // Query methods
    bool isSuccess() const {
        return is_success_;
    }
    bool isError() const {
        return !is_success_;
    }

    // Access methods (should check isSuccess/isError first)
    const Value& getResult() const {
        return value_;
    }
    const Value& getError() const {
        return error_;
    }

    // Type information
    const std::string& getValueTypeName() const {
        return value_type_name_;
    }
    const std::string& getErrorTypeName() const {
        return error_type_name_;
    }

    // String representation
    std::string toString() const {
        if (is_success_) {
            return "Result.Success(" + valueToString(value_) + ")";
        } else {
            return "Result.Error(" + valueToString(error_) + ")";
        }
    }

   private:
    // Private constructor for error case
    ResultInstance() = default;
};

}  // namespace o2l