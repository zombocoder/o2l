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
#include "ObjectInstance.hpp"

namespace o2l {

class BitLibrary {
public:
    static std::shared_ptr<ObjectInstance> createBitObject();
    
    // LLVM-equivalent Intrinsics
    static Value nativeCLZ(const std::vector<Value>& args, Context& context);      // countLeadingZeros
    static Value nativeCTZ(const std::vector<Value>& args, Context& context);      // countTrailingZeros
    static Value nativePopcount(const std::vector<Value>& args, Context& context); // populationCount
    static Value nativeBswap(const std::vector<Value>& args, Context& context);    // byteSwap
    static Value nativeLSHR(const std::vector<Value>& args, Context& context);     // logicalShiftRight
    static Value nativeASHR(const std::vector<Value>& args, Context& context);     // arithmeticShiftRight
    static Value nativeROTL(const std::vector<Value>& args, Context& context);     // rotateLeft
    static Value nativeROTR(const std::vector<Value>& args, Context& context);     // rotateRight
};

} // namespace o2l
