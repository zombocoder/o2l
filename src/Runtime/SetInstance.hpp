#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Value.hpp"

namespace o2l {

// Custom comparator for Value types using string representation
struct ValueComparator {
    bool operator()(const Value& a, const Value& b) const {
        return valueToString(a) < valueToString(b);
    }
};

class SetInstance {
   private:
    std::set<Value, ValueComparator> elements_;
    std::string element_type_name_;

   public:
    SetInstance(const std::string& element_type = "Value");

    // Set operations
    void add(const Value& element);
    bool contains(const Value& element) const;
    void remove(const Value& element);
    void clear();

    // Collection operations
    std::vector<Value> elements() const;
    size_t size() const;
    bool empty() const;

    // String representation
    std::string toString() const;

    // Type information
    const std::string& getElementTypeName() const;

    // Iterator access for internal use
    const std::set<Value, ValueComparator>& getElements() const;
    std::set<Value, ValueComparator>& getElements();
};

}  // namespace o2l