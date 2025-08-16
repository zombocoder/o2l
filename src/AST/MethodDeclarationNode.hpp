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

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

struct Parameter {
    std::string name;
    std::string type;

    Parameter(std::string n, std::string t) : name(std::move(n)), type(std::move(t)) {}
};

class MethodDeclarationNode : public ASTNode {
   private:
    std::string method_name_;
    std::vector<Parameter> parameters_;
    std::string return_type_;
    ASTNodePtr body_;
    bool is_external_;

   public:
    MethodDeclarationNode(std::string name, std::vector<Parameter> parameters,
                          std::string return_type, ASTNodePtr body, bool is_external = false,
                          const SourceLocation& location = SourceLocation());

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getName() const {
        return method_name_;
    }
    const std::vector<Parameter>& getParameters() const {
        return parameters_;
    }
    const std::string& getReturnType() const {
        return return_type_;
    }
    const ASTNodePtr& getBody() const {
        return body_;
    }
    bool isExternal() const {
        return is_external_;
    }
};

}  // namespace o2l