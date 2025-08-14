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