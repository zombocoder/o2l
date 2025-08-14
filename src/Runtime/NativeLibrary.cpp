#include "NativeLibrary.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

void NativeMethodRegistry::registerMethod(const std::string& method_name, NativeFunction func) {
    if (methods_.find(method_name) != methods_.end()) {
        throw EvaluationError("Native method '" + method_name + "' is already registered");
    }
    methods_[method_name] = std::move(func);
}

bool NativeMethodRegistry::hasMethod(const std::string& method_name) const {
    return methods_.find(method_name) != methods_.end();
}

NativeFunction NativeMethodRegistry::getMethod(const std::string& method_name) const {
    auto it = methods_.find(method_name);
    if (it == methods_.end()) {
        throw EvaluationError("Native method '" + method_name + "' not found");
    }
    return it->second;
}

std::vector<std::string> NativeMethodRegistry::getMethodNames() const {
    std::vector<std::string> names;
    names.reserve(methods_.size());
    for (const auto& [name, func] : methods_) {
        names.push_back(name);
    }
    return names;
}

void NativeMethodRegistry::clear() {
    methods_.clear();
}

}  // namespace o2l