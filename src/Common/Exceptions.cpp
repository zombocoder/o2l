#include "Exceptions.hpp"
#include "../Runtime/Context.hpp"

namespace o2l {

o2lException::o2lException(const std::string& message, const Context& context)
    : message_(message), stack_trace_(context.getStackTrace()) {
    buildFullMessage();
}

} // namespace o2l