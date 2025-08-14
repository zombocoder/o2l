#pragma once

#include <string>

#include "../Common/SourceLocation.hpp"
#include "../Runtime/Context.hpp"

// Forward declaration to avoid circular dependency
namespace o2l {
class ASTNode;
}

namespace o2l {

// RAII helper class for automatic stack frame management
class StackFrameGuard {
   private:
    Context& context_;
    bool pushed_;

   public:
    StackFrameGuard(Context& context, const std::string& function_name,
                    const std::string& object_name, const SourceLocation& location)
        : context_(context), pushed_(true) {
        context_.pushStackFrame(function_name, object_name, location);
    }

    // Constructor that extracts location from AST node
    StackFrameGuard(Context& context, const std::string& function_name,
                    const std::string& object_name, const ASTNode& node)
        : context_(context), pushed_(true) {
        context_.pushStackFrame(function_name, object_name, node.getSourceLocation());
    }

    ~StackFrameGuard() {
        if (pushed_) {
            context_.popStackFrame();
        }
    }

    // Non-copyable but movable
    StackFrameGuard(const StackFrameGuard&) = delete;
    StackFrameGuard& operator=(const StackFrameGuard&) = delete;

    StackFrameGuard(StackFrameGuard&& other) noexcept
        : context_(other.context_), pushed_(other.pushed_) {
        other.pushed_ = false;
    }

    StackFrameGuard& operator=(StackFrameGuard&& other) noexcept {
        if (this != &other) {
            if (pushed_) {
                context_.popStackFrame();
            }
            context_ = other.context_;
            pushed_ = other.pushed_;
            other.pushed_ = false;
        }
        return *this;
    }
};

// Convenience macro for creating stack frame guards
#define STACK_FRAME_GUARD(context, function_name, object_name, node) \
    StackFrameGuard _guard(context, function_name, object_name, node)

}  // namespace o2l