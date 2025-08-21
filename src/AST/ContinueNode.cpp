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

#include "ContinueNode.hpp"

#include "../Common/Exceptions.hpp"

namespace o2l {

Value ContinueNode::evaluate(Context& context) {
    throw ContinueException();
}

std::string ContinueNode::toString() const {
    return "Continue()";
}

}  // namespace o2l