#pragma once

#include <memory>

#include "Value.hpp"

namespace o2l {

class RepeatIterator {
   private:
    Int total_count_;
    Int current_count_;

   public:
    RepeatIterator(Int count);

    // Iterator methods
    bool hasNext() const;
    Value next();  // Returns the current iteration number (0-based)
    void reset();

    // Current state
    Int getCurrentCount() const;
    Int getTotalCount() const;

    // Utility methods
    std::string toString() const;
};

}  // namespace o2l