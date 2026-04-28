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

#include "BitLibrary.hpp"
#include "Context.hpp"
#include <bit>
#include <algorithm>
#include "../Common/Exceptions.hpp"

namespace o2l {

std::shared_ptr<ObjectInstance> BitLibrary::createBitObject() {
    auto bit_object = std::make_shared<ObjectInstance>("bit");
    
    bit_object->addMethod("clz", nativeCLZ, true);
    bit_object->addMethod("ctz", nativeCTZ, true);
    bit_object->addMethod("popcount", nativePopcount, true);
    bit_object->addMethod("bswap", nativeBswap, true);
    bit_object->addMethod("lshr", nativeLSHR, true);
    bit_object->addMethod("ashr", nativeASHR, true);
    bit_object->addMethod("rotl", nativeROTL, true);
    bit_object->addMethod("rotr", nativeROTR, true);
    
    return bit_object;
}

Value BitLibrary::nativeCLZ(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Int>(args[0])) throw EvaluationError("bit.clz() requires an Int");
    uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
    return Int(val == 0 ? 32 : std::countl_zero(val));
}

Value BitLibrary::nativeCTZ(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Int>(args[0])) throw EvaluationError("bit.ctz() requires an Int");
    uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
    return Int(val == 0 ? 32 : std::countr_zero(val));
}

Value BitLibrary::nativePopcount(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Int>(args[0])) throw EvaluationError("bit.popcount() requires an Int");
    return Int(std::popcount(static_cast<uint32_t>(std::get<Int>(args[0]))));
}

Value BitLibrary::nativeBswap(const std::vector<Value>& args, Context& context) {
    if (args.empty() || !std::holds_alternative<Int>(args[0])) throw EvaluationError("bit.bswap() requires an Int");
    uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
    return Int(std::byteswap(val));
}

Value BitLibrary::nativeLSHR(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || !std::holds_alternative<Int>(args[0]) || !std::holds_alternative<Int>(args[1])) 
        throw EvaluationError("bit.lshr() requires (Int value, Int shift)");
    
    uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
    uint32_t shift = static_cast<uint32_t>(std::get<Int>(args[1]));
    return Int(static_cast<int32_t>(val >> shift));
}

Value BitLibrary::nativeASHR(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || !std::holds_alternative<Int>(args[0]) || !std::holds_alternative<Int>(args[1])) 
        throw EvaluationError("bit.ashr() requires (Int value, Int shift)");
    
    int32_t val = std::get<Int>(args[0]);
    uint32_t shift = static_cast<uint32_t>(std::get<Int>(args[1]));
    return Int(val >> shift);
}

Value BitLibrary::nativeROTL(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || !std::holds_alternative<Int>(args[0]) || !std::holds_alternative<Int>(args[1])) 
        throw EvaluationError("bit.rotl() requires (Int value, Int shift)");
    
    uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
    int shift = std::get<Int>(args[1]);
    return Int(static_cast<int32_t>(std::rotl(val, shift)));
}

Value BitLibrary::nativeROTR(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || !std::holds_alternative<Int>(args[0]) || !std::holds_alternative<Int>(args[1])) 
        throw EvaluationError("bit.rotr() requires (Int value, Int shift)");
    
    uint32_t val = static_cast<uint32_t>(std::get<Int>(args[0]));
    int shift = std::get<Int>(args[1]);
    return Int(static_cast<int32_t>(std::rotr(val, shift)));
}

} // namespace o2l
