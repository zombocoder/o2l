#include "ListInstance.hpp"

#include <algorithm>
#include <sstream>

#include "../Common/Exceptions.hpp"

namespace o2l {

ListInstance::ListInstance(const std::string& element_type) : element_type_name_(element_type) {}

void ListInstance::add(const Value& element) {
    elements_.push_back(element);
}

Value ListInstance::get(size_t index) const {
    if (index >= elements_.size()) {
        throw EvaluationError("List index " + std::to_string(index) +
                              " out of bounds (size: " + std::to_string(elements_.size()) + ")");
    }
    return elements_[index];
}

void ListInstance::remove(size_t index) {
    if (index >= elements_.size()) {
        throw EvaluationError("List index " + std::to_string(index) +
                              " out of bounds (size: " + std::to_string(elements_.size()) + ")");
    }
    elements_.erase(elements_.begin() + index);
}

void ListInstance::reverse() {
    std::reverse(elements_.begin(), elements_.end());
}

Value ListInstance::pop() {
    if (elements_.empty()) {
        throw EvaluationError("Cannot pop from empty list");
    }
    Value last_element = elements_.back();
    elements_.pop_back();
    return last_element;
}

size_t ListInstance::size() const {
    return elements_.size();
}

bool ListInstance::empty() const {
    return elements_.empty();
}

void ListInstance::clear() {
    elements_.clear();
}

std::string ListInstance::toString() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << valueToString(elements_[i]);
    }
    oss << "]";
    return oss.str();
}

const std::string& ListInstance::getElementTypeName() const {
    return element_type_name_;
}

const std::vector<Value>& ListInstance::getElements() const {
    return elements_;
}

std::vector<Value>& ListInstance::getElements() {
    return elements_;
}

}  // namespace o2l