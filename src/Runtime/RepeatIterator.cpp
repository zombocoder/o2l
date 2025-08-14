#include "RepeatIterator.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

RepeatIterator::RepeatIterator(Int count) : total_count_(count), current_count_(0) {
    if (count < 0) {
        throw EvaluationError("RepeatIterator count cannot be negative: " + std::to_string(count));
    }
}

bool RepeatIterator::hasNext() const {
    return current_count_ < total_count_;
}

Value RepeatIterator::next() {
    if (!hasNext()) {
        throw EvaluationError("RepeatIterator.next() called when hasNext() is false");
    }
    Value result = Value(current_count_);
    current_count_++;
    return result;
}

void RepeatIterator::reset() {
    current_count_ = 0;
}

Int RepeatIterator::getCurrentCount() const {
    return current_count_;
}

Int RepeatIterator::getTotalCount() const {
    return total_count_;
}

std::string RepeatIterator::toString() const {
    return "RepeatIterator(count=" + std::to_string(current_count_) + "/" +
           std::to_string(total_count_) + ", hasNext=" + (hasNext() ? "true" : "false") + ")";
}

}  // namespace o2l