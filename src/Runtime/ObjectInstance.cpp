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

#include "ObjectInstance.hpp"

#include "../Common/Exceptions.hpp"
#include "Context.hpp"

namespace o2l {

ObjectInstance::ObjectInstance(const std::string& name) : object_name_(name) {}

ObjectInstance::ObjectInstance(const ObjectInstance& other)
    : object_name_(other.object_name_),
      methods_(other.methods_),
      method_visibility_(other.method_visibility_),
      method_signatures_(other.method_signatures_),
      properties_(other.properties_) {}

void ObjectInstance::addMethod(const std::string& method_name, Method method, bool is_external) {
    methods_[method_name] = std::move(method);
    method_visibility_[method_name] = is_external;
}

void ObjectInstance::addMethod(const std::string& method_name, Method method,
                               const std::vector<Parameter>& parameters,
                               const std::string& return_type, bool is_external) {
    methods_[method_name] = std::move(method);
    method_visibility_[method_name] = is_external;
    method_signatures_[method_name] =
        MethodSignature(method_name, parameters, return_type, is_external);
}

Value ObjectInstance::callMethod(const std::string& method_name, const std::vector<Value>& args,
                                 Context& context, bool external_call) {
    auto it = methods_.find(method_name);
    if (it == methods_.end()) {
        throw UnresolvedReferenceError("Method '" + method_name + "' not found in object '" +
                                       object_name_ + "'");
    }

    // Check method visibility
    auto vis_it = method_visibility_.find(method_name);
    bool is_external = (vis_it != method_visibility_.end()) ? vis_it->second : false;

    if (external_call && !is_external) {
        throw EvaluationError("Method '" + method_name +
                                  "' is not externally accessible on object '" + object_name_ + "'",
                              context);
    }

    // Push call information for stack trace
    context.pushCall(object_name_ + "." + method_name);

    // Push this object for property access - need to get shared_ptr to this
    auto shared_this = shared_from_this();
    context.pushThisObject(shared_this);

    try {
        Value result = it->second(args, context);
        context.popThisObject();
        context.popCall();
        return result;
    } catch (...) {
        context.popThisObject();
        context.popCall();
        throw;
    }
}

bool ObjectInstance::hasMethod(const std::string& method_name) const {
    return methods_.find(method_name) != methods_.end();
}

std::vector<std::string> ObjectInstance::getMethodNames() const {
    std::vector<std::string> names;
    names.reserve(methods_.size());

    for (const auto& pair : methods_) {
        names.push_back(pair.first);
    }

    return names;
}

bool ObjectInstance::isMethodExternal(const std::string& method_name) const {
    auto it = method_visibility_.find(method_name);
    return (it != method_visibility_.end()) ? it->second : false;
}

void ObjectInstance::setProperty(const std::string& property_name, const Value& value) {
    properties_[property_name] = value;
}

Value ObjectInstance::getProperty(const std::string& property_name) const {
    auto it = properties_.find(property_name);
    if (it == properties_.end()) {
        throw UnresolvedReferenceError("Property '" + property_name + "' not found in object '" +
                                       object_name_ + "'");
    }
    return it->second;
}

bool ObjectInstance::hasProperty(const std::string& property_name) const {
    return properties_.find(property_name) != properties_.end();
}

bool ObjectInstance::hasMethodSignature(const std::string& method_name) const {
    return method_signatures_.find(method_name) != method_signatures_.end();
}

const MethodSignature* ObjectInstance::getMethodSignature(const std::string& method_name) const {
    auto it = method_signatures_.find(method_name);
    return (it != method_signatures_.end()) ? &it->second : nullptr;
}

}  // namespace o2l