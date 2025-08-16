/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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