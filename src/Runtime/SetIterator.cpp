#include "SetIterator.hpp"

#include <sstream>
#include <stdexcept>

namespace o2l {

SetIterator::SetIterator(std::shared_ptr<SetInstance> set_instance)
    : set_instance_(set_instance), current_index_(0) {
    if (!set_instance_) {
        throw std::invalid_argument("SetIterator: set_instance cannot be null");
    }

    current_iterator_ = set_instance_->getElements().begin();
    end_iterator_ = set_instance_->getElements().end();
}

bool SetIterator::hasNext() const {
    return current_iterator_ != end_iterator_;
}

Value SetIterator::next() {
    if (!hasNext()) {
        throw std::runtime_error("SetIterator: No more elements to iterate");
    }

    Value result = *current_iterator_;
    ++current_iterator_;
    ++current_index_;

    return result;
}

void SetIterator::reset() {
    current_iterator_ = set_instance_->getElements().begin();
    end_iterator_ = set_instance_->getElements().end();
    current_index_ = 0;
}

size_t SetIterator::getCurrentIndex() const {
    return current_index_;
}

size_t SetIterator::getTotalSize() const {
    return set_instance_->size();
}

std::string SetIterator::toString() const {
    std::ostringstream oss;
    oss << "SetIterator<" << set_instance_->getElementTypeName() << ">(index=" << current_index_
        << ", total=" << getTotalSize() << ")";
    return oss.str();
}

}  // namespace o2l