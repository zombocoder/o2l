#include "ListIterator.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

ListIterator::ListIterator(std::shared_ptr<ListInstance> list) : list_(list), current_index_(0) {}

bool ListIterator::hasNext() const {
    return current_index_ < list_->size();
}

Value ListIterator::next() {
    if (!hasNext()) {
        throw EvaluationError("ListIterator.next() called when hasNext() is false");
    }
    Value value = list_->get(current_index_);
    current_index_++;
    return value;
}

void ListIterator::reset() {
    current_index_ = 0;
}

size_t ListIterator::getCurrentIndex() const {
    return current_index_;
}

std::string ListIterator::toString() const {
    return "ListIterator(index=" + std::to_string(current_index_) +
           ", hasNext=" + (hasNext() ? "true" : "false") + ")";
}

}  // namespace o2l