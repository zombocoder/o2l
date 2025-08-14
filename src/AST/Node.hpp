#pragma once

#include <memory>
#include <string>

#include "../Common/SourceLocation.hpp"
#include "../Runtime/Value.hpp"

// Forward declaration to avoid circular dependency
namespace o2l {
class Context;
}

namespace o2l {

// Base class for all AST nodes
class ASTNode {
   protected:
    SourceLocation source_location_;

   public:
    ASTNode(const SourceLocation& location = SourceLocation()) : source_location_(location) {}

    virtual ~ASTNode() = default;

    // Every node can be evaluated to produce a Value
    virtual Value evaluate(Context& context) = 0;

    // For debugging and error reporting
    virtual std::string toString() const = 0;

    // Get source location information
    const SourceLocation& getSourceLocation() const {
        return source_location_;
    }
    void setSourceLocation(const SourceLocation& location) {
        source_location_ = location;
    }
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

}  // namespace o2l