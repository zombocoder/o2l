#include "Context.hpp"

#include "../Common/Exceptions.hpp"
#include "ObjectInstance.hpp"

namespace o2l {

Context::Context() {
    // Start with global scope
    pushScope();
}

void Context::pushScope() {
    scopes_.emplace_back();
    const_scopes_.emplace_back();
}

void Context::popScope() {
    if (scopes_.empty()) {
        throw EvaluationError("Cannot pop scope: no scopes available");
    }
    scopes_.pop_back();
    const_scopes_.pop_back();
}

void Context::defineVariable(const std::string& name, const Value& value) {
    if (scopes_.empty()) {
        throw EvaluationError("Cannot define variable: no active scope");
    }

    // Check if trying to redefine a constant
    if (isConstant(name)) {
        throw EvaluationError("Cannot redefine constant '" + name + "'");
    }

    scopes_.back()[name] = value;
}

void Context::defineConstant(const std::string& name, const Value& value) {
    if (scopes_.empty()) {
        throw EvaluationError("Cannot define constant: no active scope");
    }

    // Check if name already exists as variable or constant
    if (hasVariable(name)) {
        throw EvaluationError("Cannot define constant '" + name + "': name already exists");
    }

    scopes_.back()[name] = value;
    const_scopes_.back().insert(name);
}

void Context::reassignVariable(const std::string& name, const Value& value) {
    // Check if variable exists
    if (!hasVariable(name)) {
        throw UnresolvedReferenceError("Cannot reassign undefined variable '" + name + "'");
    }

    // Check if variable is a constant (object properties are constants)
    if (isConstant(name)) {
        throw EvaluationError("Cannot reassign constant variable '" + name +
                              "'. Object properties are immutable.");
    }

    // Find the variable in the scope stack and update it
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto var_it = it->find(name);
        if (var_it != it->end()) {
            var_it->second = value;
            return;
        }
    }

    // This should never happen since we checked hasVariable above
    throw UnresolvedReferenceError("Variable '" + name + "' not found during reassignment");
}

Value Context::getVariable(const std::string& name) const {
    // Search from innermost to outermost scope
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto var_it = it->find(name);
        if (var_it != it->end()) {
            return var_it->second;
        }
    }

    throw UnresolvedReferenceError("Variable '" + name + "' not found");
}

bool Context::hasVariable(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->find(name) != it->end()) {
            return true;
        }
    }
    return false;
}

void Context::pushCall(const std::string& call_description) {
    call_stack_.push_back(call_description);
}

void Context::popCall() {
    if (!call_stack_.empty()) {
        call_stack_.pop_back();
    }
}

std::vector<std::string> Context::getCallStack() const {
    return call_stack_;
}

void Context::pushStackFrame(const std::string& function_name, const std::string& object_name,
                             const SourceLocation& location) {
    execution_stack_.emplace_back(function_name, object_name, location);
}

void Context::popStackFrame() {
    if (!execution_stack_.empty()) {
        execution_stack_.pop_back();
    }
}

std::vector<std::string> Context::getStackTrace() const {
    std::vector<std::string> trace;
    trace.reserve(execution_stack_.size());

    for (const auto& frame : execution_stack_) {
        trace.push_back(frame.toString());
    }

    return trace;
}

std::vector<Context::StackFrame> Context::getExecutionStack() const {
    return execution_stack_;
}

void Context::pushThisObject(std::shared_ptr<ObjectInstance> this_obj) {
    this_stack_.push_back(this_obj);
}

void Context::popThisObject() {
    if (!this_stack_.empty()) {
        this_stack_.pop_back();
    }
}

std::shared_ptr<ObjectInstance> Context::getThisObject() const {
    if (this_stack_.empty()) {
        throw EvaluationError("No 'this' object available in current context");
    }
    return this_stack_.back();
}

bool Context::hasThisObject() const {
    return !this_stack_.empty();
}

bool Context::isConstant(const std::string& name) const {
    // Search from innermost to outermost scope
    for (auto it = const_scopes_.rbegin(); it != const_scopes_.rend(); ++it) {
        if (it->find(name) != it->end()) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> Context::getVariableNames() const {
    std::vector<std::string> names;
    std::set<std::string> unique_names;  // To avoid duplicates from different scopes

    // Collect variable names from all scopes
    for (const auto& scope : scopes_) {
        for (const auto& [name, value] : scope) {
            if (unique_names.find(name) == unique_names.end()) {
                names.push_back(name);
                unique_names.insert(name);
            }
        }
    }

    return names;
}

}  // namespace o2l