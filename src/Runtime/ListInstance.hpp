#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Value.hpp"

namespace o2l {

class ListInstance {
   private:
    std::vector<Value> elements_;
    std::string element_type_name_;

   public:
    ListInstance(const std::string& element_type = "Value");

    // List operations
    void add(const Value& element);
    Value get(size_t index) const;
    void remove(size_t index);
    void reverse();
    Value pop();

    // Utility methods
    size_t size() const;
    bool empty() const;
    void clear();

    // String representation
    std::string toString() const;

    // Element type information
    const std::string& getElementTypeName() const;

    // Iterator access for internal use
    const std::vector<Value>& getElements() const;
    std::vector<Value>& getElements();
};

}  // namespace o2l