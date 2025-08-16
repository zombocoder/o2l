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

struct ImportPath {
    std::vector<std::string> package_path;  // e.g., ["some_package"]
    std::string object_name;                // e.g., "some_test_obj"
    std::string method_name;                // e.g., "method" or "*" for all methods
    bool is_user_import = false;            // true for @import, false for import

    bool is_wildcard() const {
        return method_name == "*";
    }

    std::string getFullPath() const {
        std::string result;
        for (const auto& part : package_path) {
            if (!result.empty()) result += ".";
            result += part;
        }
        if (!result.empty()) result += ".";
        result += object_name;
        if (!method_name.empty() && method_name != "*") {
            result += "." + method_name;
        }
        return result;
    }
};

class ImportNode : public ASTNode {
   private:
    ImportPath import_path_;

   public:
    explicit ImportNode(ImportPath path) : import_path_(std::move(path)) {}

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const ImportPath& getImportPath() const {
        return import_path_;
    }
};

}  // namespace o2l