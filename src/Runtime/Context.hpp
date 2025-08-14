#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Value.hpp"

// Forward declarations
namespace o2l {
class ObjectInstance;
}

// Include SourceLocation
#include "../Common/SourceLocation.hpp"

namespace o2l {

class Context {
   private:
    // Stack of variable scopes (for method calls, object contexts)
    std::vector<std::map<std::string, Value>> scopes_;

    // Track constants for immutability (parallel to scopes_)
    std::vector<std::set<std::string>> const_scopes_;

    // Stack trace for error reporting
    std::vector<std::string> call_stack_;

    // Stack frame information with source locations
    struct StackFrame {
        std::string function_name;
        std::string object_name;
        SourceLocation location;

        StackFrame(const std::string& func, const std::string& obj, const SourceLocation& loc)
            : function_name(func), object_name(obj), location(loc) {}

        std::string toString() const {
            std::string result = "at ";
            if (!object_name.empty()) {
                result += object_name + ".";
            }
            result += function_name;
            if (location.line_number > 0) {
                result += " (" + location.toString() + ")";
            }
            return result;
        }
    };

    std::vector<StackFrame> execution_stack_;

    // Stack of 'this' objects for property access
    std::vector<std::shared_ptr<ObjectInstance>> this_stack_;

   public:
    Context();

    // Scope management
    void pushScope();
    void popScope();

    // Variable operations
    void defineVariable(const std::string& name, const Value& value);
    void defineConstant(const std::string& name, const Value& value);
    void reassignVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name) const;
    bool hasVariable(const std::string& name) const;
    bool isConstant(const std::string& name) const;
    std::vector<std::string> getVariableNames() const;

    // Call stack management for error reporting
    void pushCall(const std::string& call_description);
    void popCall();
    std::vector<std::string> getCallStack() const;

    // Enhanced stack frame management with source locations
    void pushStackFrame(const std::string& function_name, const std::string& object_name,
                        const SourceLocation& location);
    void popStackFrame();
    std::vector<std::string> getStackTrace() const;
    std::vector<StackFrame> getExecutionStack() const;

    // Get current scope depth (for debugging)
    size_t getScopeDepth() const {
        return scopes_.size();
    }

    // This object management
    void pushThisObject(std::shared_ptr<ObjectInstance> this_obj);
    void popThisObject();
    std::shared_ptr<ObjectInstance> getThisObject() const;
    bool hasThisObject() const;
};

}  // namespace o2l