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
#include <string>

#include "Node.hpp"

namespace o2l {

class ConstDeclarationNode : public ASTNode {
   private:
    std::string const_name_;
    std::string type_name_;  // Optional type annotation
    ASTNodePtr initializer_;

   public:
    ConstDeclarationNode(std::string const_name, std::string type_name, ASTNodePtr initializer);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getConstName() const {
        return const_name_;
    }
    const std::string& getTypeName() const {
        return type_name_;
    }
    const ASTNodePtr& getInitializer() const {
        return initializer_;
    }
};

}  // namespace o2l