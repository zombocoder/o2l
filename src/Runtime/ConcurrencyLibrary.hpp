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

#pragma once

#include <memory>
#include <vector>
#include "Value.hpp"

namespace o2l {

class ObjectInstance;
class Context;

class ConcurrencyLibrary {
public:
    static std::shared_ptr<ObjectInstance> createConcurrencyObject();
    static std::shared_ptr<ObjectInstance> createChannelClassObject();

    // Native methods
    static Value nativeSleep(const std::vector<Value>& args, Context& context);
    static Value nativeChannelNew(const std::vector<Value>& args, Context& context);
    static Value nativeChannelSend(const std::vector<Value>& args, Context& context);
    static Value nativeChannelReceive(const std::vector<Value>& args, Context& context);
    static Value nativeChannelClose(const std::vector<Value>& args, Context& context);
    static Value nativeChannelIsOpen(const std::vector<Value>& args, Context& context);
};

} // namespace o2l
