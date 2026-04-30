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

#include "ConcurrencyLibrary.hpp"
#include "ObjectInstance.hpp"
#include "ChannelInstance.hpp"
#include "Scheduler.hpp"
#include "../Common/Exceptions.hpp"
#include <thread>
#include <chrono>

namespace o2l {

std::shared_ptr<ObjectInstance> ConcurrencyLibrary::createConcurrencyObject() {
    auto obj = std::make_shared<ObjectInstance>("concurrency");
    
    obj->addMethod("sleep", nativeSleep, true);
    obj->addMethod("Channel", [](const std::vector<Value>&, Context&) {
        return Value(createChannelClassObject());
    }, true);

    return obj;
}

std::shared_ptr<ObjectInstance> ConcurrencyLibrary::createChannelClassObject() {
    auto obj = std::make_shared<ObjectInstance>("ChannelClass");
    obj->addMethod("new", nativeChannelNew, true);
    return obj;
}

Value ConcurrencyLibrary::nativeSleep(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1 || !std::holds_alternative<Int>(args[0])) {
        throw EvaluationError("sleep() requires one Integer argument (milliseconds)");
    }

    Int ms = std::get<Int>(args[0]);
    auto& scheduler = Scheduler::instance();

    if (scheduler.isActive()) {
        if (scheduler.hasResumeValue()) {
            scheduler.consumeResumeValue();
            return Value(Int(0));
        }
        scheduler.suspendForSleep(static_cast<uint64_t>(ms));
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    return Value(Int(0));
}

Value ConcurrencyLibrary::nativeChannelNew(const std::vector<Value>& args, Context& context) {
    size_t capacity = 0;
    if (args.size() >= 1 && std::holds_alternative<Int>(args[0])) {
        capacity = static_cast<size_t>(std::get<Int>(args[0]));
    }

    auto channel = std::make_shared<ChannelInstance>(capacity);
    auto channel_obj = std::make_shared<ObjectInstance>("Channel");
    
    channel_obj->addMethod("send", [channel](const std::vector<Value>& args, Context&) {
        if (args.size() != 1) throw EvaluationError("send() requires 1 argument");
        channel->send(args[0]);
        return Value(Int(0));
    }, true);

    channel_obj->addMethod("receive", [channel](const std::vector<Value>&, Context&) {
        return channel->receive();
    }, true);

    channel_obj->addMethod("close", [channel](const std::vector<Value>&, Context&) {
        channel->close();
        return Value(Int(0));
    }, true);

    channel_obj->addMethod("isOpen", [channel](const std::vector<Value>&, Context&) {
        return Value(channel->isOpen());
    }, true);

    return Value(channel_obj);
}

} // namespace o2l
