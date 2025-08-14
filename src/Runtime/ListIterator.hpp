#pragma once

#include <memory>

#include "ListInstance.hpp"
#include "Value.hpp"

namespace o2l {

class ListIterator {
   private:
    std::shared_ptr<ListInstance> list_;
    size_t current_index_;

   public:
    ListIterator(std::shared_ptr<ListInstance> list);

    // Iterator methods
    bool hasNext() const;
    Value next();
    void reset();

    // Current state
    size_t getCurrentIndex() const;

    // Utility methods
    std::string toString() const;
};

}  // namespace o2l