#pragma once

#include <string>
#include <vector>

#include "Node.hpp"

namespace o2l {

struct RecordField {
    std::string name;
    std::string type;  // Type name for the field

    RecordField(std::string n, std::string t) : name(std::move(n)), type(std::move(t)) {}
};

class RecordDeclarationNode : public ASTNode {
   private:
    std::string record_name_;
    std::vector<RecordField> fields_;

   public:
    RecordDeclarationNode(std::string name, std::vector<RecordField> fields);

    Value evaluate(Context& context) override;
    std::string toString() const override;

    const std::string& getRecordName() const {
        return record_name_;
    }
    const std::vector<RecordField>& getFields() const {
        return fields_;
    }
};

}  // namespace o2l