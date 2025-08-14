#pragma once

#include <memory>
#include <set>

#include "SetInstance.hpp"
#include "Value.hpp"

namespace o2l {

class SetIterator {
   private:
    std::shared_ptr<SetInstance> set_instance_;
    std::set<Value>::const_iterator current_iterator_;
    std::set<Value>::const_iterator end_iterator_;
    size_t current_index_;

   public:
    SetIterator(std::shared_ptr<SetInstance> set_instance);

    // Iterator operations
    bool hasNext() const;
    Value next();

    // Utility methods
    void reset();
    size_t getCurrentIndex() const;
    size_t getTotalSize() const;

    // String representation
    std::string toString() const;
};

}  // namespace o2l