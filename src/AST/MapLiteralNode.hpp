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

#include <utility>
#include <vector>

#include "Node.hpp"
#include "Runtime/MapInstance.hpp"

namespace o2l {

class MapLiteralNode : public ASTNode {
   private:
    std::vector<std::pair<ASTNodePtr, ASTNodePtr>> entries_;
    std::string key_type_name_;
    std::string value_type_name_;

   public:
    MapLiteralNode(std::vector<std::pair<ASTNodePtr, ASTNodePtr>> entries,
                   std::string key_type_name = "", std::string value_type_name = "")
        : entries_(std::move(entries)),
          key_type_name_(std::move(key_type_name)),
          value_type_name_(std::move(value_type_name)) {}

    Value evaluate(Context& context) override {
        // Infer types from first entry if not specified
        std::string actual_key_type = key_type_name_;
        std::string actual_value_type = value_type_name_;

        if ((actual_key_type.empty() || actual_value_type.empty()) && !entries_.empty()) {
            Value first_key = entries_[0].first->evaluate(context);
            Value first_value = entries_[0].second->evaluate(context);

            if (actual_key_type.empty()) {
                actual_key_type = getTypeName(first_key);
            }
            if (actual_value_type.empty()) {
                actual_value_type = getTypeName(first_value);
            }
        }

        auto map_instance = std::make_shared<MapInstance>(actual_key_type, actual_value_type);

        // Evaluate each key-value pair and add to the map
        for (const auto& entry : entries_) {
            Value key_value = entry.first->evaluate(context);
            Value value_value = entry.second->evaluate(context);
            map_instance->put(key_value, value_value);
        }

        return map_instance;
    }

    std::string toString() const override {
        std::string result = "Map{";
        for (size_t i = 0; i < entries_.size(); ++i) {
            if (i > 0) result += ", ";
            result += entries_[i].first->toString() + ": " + entries_[i].second->toString();
        }
        result += "}";
        return result;
    }

    const std::vector<std::pair<ASTNodePtr, ASTNodePtr>>& getEntries() const {
        return entries_;
    }
    const std::string& getKeyTypeName() const {
        return key_type_name_;
    }
    const std::string& getValueTypeName() const {
        return value_type_name_;
    }
};

}  // namespace o2l