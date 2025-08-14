#include "MethodCallNode.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <sstream>

#include "../Common/Exceptions.hpp"
#include "../Common/StackFrameGuard.hpp"
#include "../Runtime/ErrorInstance.hpp"
#include "../Runtime/ListInstance.hpp"
#include "../Runtime/ListIterator.hpp"
#include "../Runtime/MapInstance.hpp"
#include "../Runtime/MapIterator.hpp"
#include "../Runtime/MapObject.hpp"
#include "../Runtime/ObjectInstance.hpp"
#include "../Runtime/RepeatIterator.hpp"
#include "../Runtime/ResultInstance.hpp"
#include "../Runtime/SetInstance.hpp"
#include "../Runtime/SetIterator.hpp"

namespace o2l {

MethodCallNode::MethodCallNode(ASTNodePtr object, std::string method_name,
                               std::vector<ASTNodePtr> arguments, const SourceLocation& location)
    : ASTNode(location),
      object_(std::move(object)),
      method_name_(std::move(method_name)),
      arguments_(std::move(arguments)) {}

Value MethodCallNode::evaluate(Context& context) {
    try {
        // Evaluate the object expression first to get the actual object name
        Value object_value = object_->evaluate(context);

        // Determine the actual object name for stack trace
        std::string actual_object_name = "object";  // Default fallback
        if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(object_value)) {
            auto obj_instance = std::get<std::shared_ptr<ObjectInstance>>(object_value);
            actual_object_name = obj_instance->getName();
        } else if (std::holds_alternative<std::shared_ptr<ListInstance>>(object_value)) {
            actual_object_name = "List";
        } else if (std::holds_alternative<std::shared_ptr<ListIterator>>(object_value)) {
            actual_object_name = "ListIterator";
        } else if (std::holds_alternative<std::shared_ptr<RepeatIterator>>(object_value)) {
            actual_object_name = "RepeatIterator";
        } else if (std::holds_alternative<std::shared_ptr<MapInstance>>(object_value)) {
            actual_object_name = "Map";
        } else if (std::holds_alternative<std::shared_ptr<MapIterator>>(object_value)) {
            actual_object_name = "MapIterator";
        } else if (std::holds_alternative<std::shared_ptr<MapObject>>(object_value)) {
            actual_object_name = "MapObject";
        } else if (std::holds_alternative<std::shared_ptr<SetInstance>>(object_value)) {
            actual_object_name = "Set";
        } else if (std::holds_alternative<std::shared_ptr<SetIterator>>(object_value)) {
            actual_object_name = "SetIterator";
        } else if (std::holds_alternative<std::shared_ptr<ResultInstance>>(object_value)) {
            actual_object_name = "Result";
        } else if (std::holds_alternative<std::shared_ptr<ErrorInstance>>(object_value)) {
            actual_object_name = "Error";
        }

        // Create stack frame for this method call with actual object name
        STACK_FRAME_GUARD(context, method_name_, actual_object_name, *this);

        // Evaluate arguments
        std::vector<Value> arg_values;
        arg_values.reserve(arguments_.size());

        for (const auto& arg : arguments_) {
            arg_values.push_back(arg->evaluate(context));
        }

        // Check if it's a ListInstance
        if (std::holds_alternative<std::shared_ptr<ListInstance>>(object_value)) {
            auto list_instance = std::get<std::shared_ptr<ListInstance>>(object_value);

            // Handle List methods
            if (method_name_ == "add") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("List.add() requires exactly one argument", context);
                }
                list_instance->add(arg_values[0]);
                return Value{};  // Return void/empty value
            } else if (method_name_ == "get") {
                if (arg_values.size() != 1 || !std::holds_alternative<Int>(arg_values[0])) {
                    throw EvaluationError("List.get() requires exactly one Int argument", context);
                }
                size_t index = static_cast<size_t>(std::get<Int>(arg_values[0]));
                return list_instance->get(index);
            } else if (method_name_ == "remove") {
                if (arg_values.size() != 1 || !std::holds_alternative<Int>(arg_values[0])) {
                    throw EvaluationError("List.remove() requires exactly one Int argument",
                                          context);
                }
                size_t index = static_cast<size_t>(std::get<Int>(arg_values[0]));
                list_instance->remove(index);
                return Value{};  // Return void/empty value
            } else if (method_name_ == "reverse") {
                if (!arg_values.empty()) {
                    throw EvaluationError("List.reverse() takes no arguments", context);
                }
                list_instance->reverse();
                return Value{};  // Return void/empty value
            } else if (method_name_ == "pop") {
                if (!arg_values.empty()) {
                    throw EvaluationError("List.pop() takes no arguments", context);
                }
                return list_instance->pop();
            } else if (method_name_ == "size") {
                if (!arg_values.empty()) {
                    throw EvaluationError("List.size() takes no arguments", context);
                }
                return Int(static_cast<Int>(list_instance->size()));
            } else if (method_name_ == "isEmpty") {
                if (!arg_values.empty()) {
                    throw EvaluationError("List.isEmpty() takes no arguments", context);
                }
                return Bool(list_instance->empty());
            } else if (method_name_ == "clear") {
                if (!arg_values.empty()) {
                    throw EvaluationError("List.clear() takes no arguments", context);
                }
                list_instance->clear();
                return Value{};  // Return void/empty value
            } else if (method_name_ == "contains") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("List.contains() requires exactly one argument", context);
                }
                const auto& elements = list_instance->getElements();
                for (const auto& element : elements) {
                    if (o2l::valuesEqual(element, arg_values[0])) {
                        return Bool(true);
                    }
                }
                return Bool(false);
            } else if (method_name_ == "indexOf") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("List.indexOf() requires exactly one argument", context);
                }
                const auto& elements = list_instance->getElements();
                for (size_t i = 0; i < elements.size(); ++i) {
                    if (o2l::valuesEqual(elements[i], arg_values[0])) {
                        return Int(static_cast<Int>(i));
                    }
                }
                return Int(-1);  // Not found
            } else if (method_name_ == "iterator") {
                if (!arg_values.empty()) {
                    throw EvaluationError("List.iterator() takes no arguments", context);
                }
                return Value(std::make_shared<ListIterator>(list_instance));
            } else if (method_name_ == "forEach") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("List.forEach() requires exactly one argument (function)",
                                          context);
                }
                // For now, we'll implement a simple forEach that applies a function to each element
                // This is a placeholder - in a full implementation, we'd need function objects
                throw EvaluationError(
                    "List.forEach() is not yet fully implemented - use iterator() for now",
                    context);
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on List type",
                                      context);
            }
        }

        // Check if it's a ListIterator
        if (std::holds_alternative<std::shared_ptr<ListIterator>>(object_value)) {
            auto list_iterator = std::get<std::shared_ptr<ListIterator>>(object_value);

            // Handle ListIterator methods
            if (method_name_ == "hasNext") {
                if (!arg_values.empty()) {
                    throw EvaluationError("ListIterator.hasNext() takes no arguments", context);
                }
                return Bool(list_iterator->hasNext());
            } else if (method_name_ == "next") {
                if (!arg_values.empty()) {
                    throw EvaluationError("ListIterator.next() takes no arguments", context);
                }
                return list_iterator->next();
            } else if (method_name_ == "reset") {
                if (!arg_values.empty()) {
                    throw EvaluationError("ListIterator.reset() takes no arguments", context);
                }
                list_iterator->reset();
                return Value{};  // Return void/empty value
            } else if (method_name_ == "getCurrentIndex") {
                if (!arg_values.empty()) {
                    throw EvaluationError("ListIterator.getCurrentIndex() takes no arguments",
                                          context);
                }
                return Int(static_cast<Int>(list_iterator->getCurrentIndex()));
            } else if (method_name_ == "index") {
                if (!arg_values.empty()) {
                    throw EvaluationError("ListIterator.index() takes no arguments", context);
                }
                return Int(static_cast<Int>(list_iterator->getCurrentIndex()));
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on ListIterator type",
                                      context);
            }
        }

        // Check if it's a RepeatIterator
        if (std::holds_alternative<std::shared_ptr<RepeatIterator>>(object_value)) {
            auto repeat_iterator = std::get<std::shared_ptr<RepeatIterator>>(object_value);

            // Handle RepeatIterator methods
            if (method_name_ == "hasNext") {
                if (!arg_values.empty()) {
                    throw EvaluationError("RepeatIterator.hasNext() takes no arguments", context);
                }
                return Bool(repeat_iterator->hasNext());
            } else if (method_name_ == "next") {
                if (!arg_values.empty()) {
                    throw EvaluationError("RepeatIterator.next() takes no arguments", context);
                }
                return repeat_iterator->next();
            } else if (method_name_ == "reset") {
                if (!arg_values.empty()) {
                    throw EvaluationError("RepeatIterator.reset() takes no arguments", context);
                }
                repeat_iterator->reset();
                return Value{};  // Return void/empty value
            } else if (method_name_ == "getCurrentCount") {
                if (!arg_values.empty()) {
                    throw EvaluationError("RepeatIterator.getCurrentCount() takes no arguments",
                                          context);
                }
                return Int(repeat_iterator->getCurrentCount());
            } else if (method_name_ == "getTotalCount") {
                if (!arg_values.empty()) {
                    throw EvaluationError("RepeatIterator.getTotalCount() takes no arguments",
                                          context);
                }
                return Int(repeat_iterator->getTotalCount());
            } else {
                throw EvaluationError(
                    "Unknown method '" + method_name_ + "' on RepeatIterator type", context);
            }
        }

        // Check if it's a MapInstance
        if (std::holds_alternative<std::shared_ptr<MapInstance>>(object_value)) {
            auto map_instance = std::get<std::shared_ptr<MapInstance>>(object_value);

            // Handle Map methods
            if (method_name_ == "put") {
                if (arg_values.size() != 2) {
                    throw EvaluationError("Map.put() requires exactly two arguments (key, value)",
                                          context);
                }
                map_instance->put(arg_values[0], arg_values[1]);
                return Value();  // Void return
            } else if (method_name_ == "get") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("Map.get() requires exactly one argument (key)", context);
                }
                return map_instance->get(arg_values[0]);
            } else if (method_name_ == "contains") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("Map.contains() requires exactly one argument (key)",
                                          context);
                }
                return Bool(map_instance->contains(arg_values[0]));
            } else if (method_name_ == "remove") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("Map.remove() requires exactly one argument (key)",
                                          context);
                }
                map_instance->remove(arg_values[0]);
                return Value();  // Void return
            } else if (method_name_ == "size") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Map.size() takes no arguments", context);
                }
                return Int(static_cast<Int>(map_instance->size()));
            } else if (method_name_ == "empty") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Map.empty() takes no arguments", context);
                }
                return Bool(map_instance->empty());
            } else if (method_name_ == "clear") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Map.clear() takes no arguments", context);
                }
                map_instance->clear();
                return Value();  // Void return
            } else if (method_name_ == "keys") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Map.keys() takes no arguments", context);
                }
                // Return keys as a List<K>
                auto keys = map_instance->keys();
                auto list_instance = std::make_shared<ListInstance>(map_instance->getKeyTypeName());
                for (const auto& key : keys) {
                    list_instance->add(key);
                }
                return Value(list_instance);
            } else if (method_name_ == "values") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Map.values() takes no arguments", context);
                }
                // Return values as a List<V>
                auto values = map_instance->values();
                auto list_instance =
                    std::make_shared<ListInstance>(map_instance->getValueTypeName());
                for (const auto& value : values) {
                    list_instance->add(value);
                }
                return Value(list_instance);
            } else if (method_name_ == "iterator") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Map.iterator() takes no arguments", context);
                }
                return Value(std::make_shared<MapIterator>(map_instance));
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Map type", context);
            }
        }

        // Check if it's a MapIterator
        if (std::holds_alternative<std::shared_ptr<MapIterator>>(object_value)) {
            auto map_iterator = std::get<std::shared_ptr<MapIterator>>(object_value);

            // Handle MapIterator methods
            if (method_name_ == "hasNext") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.hasNext() takes no arguments", context);
                }
                return Bool(map_iterator->hasNext());
            } else if (method_name_ == "nextKey") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.nextKey() takes no arguments", context);
                }
                return map_iterator->nextKey();
            } else if (method_name_ == "nextValue") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.nextValue() takes no arguments", context);
                }
                return map_iterator->nextValue();
            } else if (method_name_ == "nextEntry") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.nextEntry() takes no arguments", context);
                }
                return map_iterator->nextEntry();
            } else if (method_name_ == "reset") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.reset() takes no arguments", context);
                }
                map_iterator->reset();
                return Value();  // Void return
            } else if (method_name_ == "getCurrentIndex") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.getCurrentIndex() takes no arguments",
                                          context);
                }
                return Int(static_cast<Int>(map_iterator->getCurrentIndex()));
            } else if (method_name_ == "getTotalSize") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.getTotalSize() takes no arguments", context);
                }
                return Int(static_cast<Int>(map_iterator->getTotalSize()));
            } else if (method_name_ == "MapItem") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapIterator.MapItem() takes no arguments", context);
                }
                return map_iterator->MapItem();
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on MapIterator type",
                                      context);
            }
        }

        // Check if it's a MapObject
        if (std::holds_alternative<std::shared_ptr<MapObject>>(object_value)) {
            auto map_object = std::get<std::shared_ptr<MapObject>>(object_value);

            // Handle MapObject methods
            if (method_name_ == "getKey") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapObject.getKey() takes no arguments", context);
                }
                return map_object->getKey();
            } else if (method_name_ == "getVal") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapObject.getVal() takes no arguments", context);
                }
                return map_object->getVal();
            } else if (method_name_ == "getValue") {
                if (!arg_values.empty()) {
                    throw EvaluationError("MapObject.getValue() takes no arguments", context);
                }
                return map_object->getValue();
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on MapObject type",
                                      context);
            }
        }

        // Check if it's a SetInstance
        if (std::holds_alternative<std::shared_ptr<SetInstance>>(object_value)) {
            auto set_instance = std::get<std::shared_ptr<SetInstance>>(object_value);

            // Handle Set methods
            if (method_name_ == "add") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("Set.add() requires exactly one argument", context);
                }
                set_instance->add(arg_values[0]);
                return Value();  // Void return
            } else if (method_name_ == "contains") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("Set.contains() requires exactly one argument", context);
                }
                return Bool(set_instance->contains(arg_values[0]));
            } else if (method_name_ == "remove") {
                if (arg_values.size() != 1) {
                    throw EvaluationError("Set.remove() requires exactly one argument", context);
                }
                set_instance->remove(arg_values[0]);
                return Value();  // Void return
            } else if (method_name_ == "size") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Set.size() takes no arguments", context);
                }
                return Int(static_cast<Int>(set_instance->size()));
            } else if (method_name_ == "empty") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Set.empty() takes no arguments", context);
                }
                return Bool(set_instance->empty());
            } else if (method_name_ == "clear") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Set.clear() takes no arguments", context);
                }
                set_instance->clear();
                return Value();  // Void return
            } else if (method_name_ == "elements") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Set.elements() takes no arguments", context);
                }
                // Return elements as a List<T>
                auto elements = set_instance->elements();
                auto list_instance =
                    std::make_shared<ListInstance>(set_instance->getElementTypeName());
                for (const auto& element : elements) {
                    list_instance->add(element);
                }
                return Value(list_instance);
            } else if (method_name_ == "iterator") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Set.iterator() takes no arguments", context);
                }
                return Value(std::make_shared<SetIterator>(set_instance));
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Set type", context);
            }
        }

        // Check if it's a SetIterator
        if (std::holds_alternative<std::shared_ptr<SetIterator>>(object_value)) {
            auto set_iterator = std::get<std::shared_ptr<SetIterator>>(object_value);

            // Handle SetIterator methods
            if (method_name_ == "hasNext") {
                if (!arg_values.empty()) {
                    throw EvaluationError("SetIterator.hasNext() takes no arguments", context);
                }
                return Bool(set_iterator->hasNext());
            } else if (method_name_ == "next") {
                if (!arg_values.empty()) {
                    throw EvaluationError("SetIterator.next() takes no arguments", context);
                }
                return set_iterator->next();
            } else if (method_name_ == "reset") {
                if (!arg_values.empty()) {
                    throw EvaluationError("SetIterator.reset() takes no arguments", context);
                }
                set_iterator->reset();
                return Value();  // Void return
            } else if (method_name_ == "getCurrentIndex") {
                if (!arg_values.empty()) {
                    throw EvaluationError("SetIterator.getCurrentIndex() takes no arguments",
                                          context);
                }
                return Int(static_cast<Int>(set_iterator->getCurrentIndex()));
            } else if (method_name_ == "getTotalSize") {
                if (!arg_values.empty()) {
                    throw EvaluationError("SetIterator.getTotalSize() takes no arguments", context);
                }
                return Int(static_cast<Int>(set_iterator->getTotalSize()));
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on SetIterator type",
                                      context);
            }
        }

        // Check if it's a ResultInstance
        if (std::holds_alternative<std::shared_ptr<ResultInstance>>(object_value)) {
            auto result_instance = std::get<std::shared_ptr<ResultInstance>>(object_value);

            // Handle Result methods
            if (method_name_ == "isSuccess") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Result.isSuccess() takes no arguments", context);
                }
                return Bool(result_instance->isSuccess());
            } else if (method_name_ == "getResult") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Result.getResult() takes no arguments", context);
                }
                return result_instance->getResult();
            } else if (method_name_ == "getError") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Result.getError() takes no arguments", context);
                }
                return result_instance->getError();
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Result type",
                                      context);
            }
        }

        // Check if it's an ErrorInstance
        if (std::holds_alternative<std::shared_ptr<ErrorInstance>>(object_value)) {
            auto error_instance = std::get<std::shared_ptr<ErrorInstance>>(object_value);

            // Handle Error methods
            if (method_name_ == "getMessage") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Error.getMessage() takes no arguments", context);
                }
                return Text(error_instance->getMessage());
            } else if (method_name_ == "getCode") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Error.getCode() takes no arguments", context);
                }
                return Text(error_instance->getCode());
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Error type",
                                      context);
            }
        }

        // Check if it's a Text (string)
        if (std::holds_alternative<Text>(object_value)) {
            auto text_value = std::get<Text>(object_value);

            // Handle Text methods
            if (method_name_ == "capitalize") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.capitalize() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Text(text_value);
                }
                std::string result = text_value;
                result[0] = std::toupper(result[0]);
                for (size_t i = 1; i < result.length(); ++i) {
                    result[i] = std::tolower(result[i]);
                }
                return Text(result);
            } else if (method_name_ == "length") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.length() takes no arguments", context);
                }
                return Int(static_cast<Int>(text_value.length()));
            } else if (method_name_ == "caseFold") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.caseFold() takes no arguments", context);
                }
                std::string result = text_value;
                std::transform(result.begin(), result.end(), result.begin(), ::tolower);
                return Text(result);
            } else if (method_name_ == "lower") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.lower() takes no arguments", context);
                }
                std::string result = text_value;
                std::transform(result.begin(), result.end(), result.begin(), ::tolower);
                return Text(result);
            } else if (method_name_ == "upper") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.upper() takes no arguments", context);
                }
                std::string result = text_value;
                std::transform(result.begin(), result.end(), result.begin(), ::toupper);
                return Text(result);
            } else if (method_name_ == "swapCase") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.swapCase() takes no arguments", context);
                }
                std::string result = text_value;
                for (char& c : result) {
                    if (std::islower(c)) {
                        c = std::toupper(c);
                    } else if (std::isupper(c)) {
                        c = std::tolower(c);
                    }
                }
                return Text(result);
            } else if (method_name_ == "title") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.title() takes no arguments", context);
                }
                std::string result = text_value;
                bool capitalize_next = true;
                for (char& c : result) {
                    if (std::isalpha(c)) {
                        if (capitalize_next) {
                            c = std::toupper(c);
                            capitalize_next = false;
                        } else {
                            c = std::tolower(c);
                        }
                    } else {
                        capitalize_next = true;
                    }
                }
                return Text(result);
            } else if (method_name_ == "count") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.count() requires exactly one Text argument",
                                          context);
                }
                std::string substring = std::get<Text>(arg_values[0]);
                if (substring.empty()) {
                    return Int(0);
                }
                size_t count = 0;
                size_t pos = 0;
                while ((pos = text_value.find(substring, pos)) != std::string::npos) {
                    count++;
                    pos += substring.length();
                }
                return Int(static_cast<Int>(count));
            } else if (method_name_ == "endswith") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.endswith() requires exactly one Text argument",
                                          context);
                }
                std::string suffix = std::get<Text>(arg_values[0]);
                if (suffix.length() > text_value.length()) {
                    return Bool(false);
                }
                return Bool(text_value.compare(text_value.length() - suffix.length(),
                                               suffix.length(), suffix) == 0);
            } else if (method_name_ == "startswith") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.startswith() requires exactly one Text argument",
                                          context);
                }
                std::string prefix = std::get<Text>(arg_values[0]);
                return Bool(text_value.compare(0, prefix.length(), prefix) == 0);
            } else if (method_name_ == "find") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.find() requires exactly one Text argument",
                                          context);
                }
                std::string substring = std::get<Text>(arg_values[0]);
                size_t pos = text_value.find(substring);
                return Int(pos == std::string::npos ? -1 : static_cast<Int>(pos));
            } else if (method_name_ == "rfind") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.rfind() requires exactly one Text argument",
                                          context);
                }
                std::string substring = std::get<Text>(arg_values[0]);
                size_t pos = text_value.rfind(substring);
                return Int(pos == std::string::npos ? -1 : static_cast<Int>(pos));
            } else if (method_name_ == "index") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.index() requires exactly one Text argument",
                                          context);
                }
                std::string substring = std::get<Text>(arg_values[0]);
                size_t pos = text_value.find(substring);
                if (pos == std::string::npos) {
                    throw EvaluationError("Text.index(): substring not found", context);
                }
                return Int(static_cast<Int>(pos));
            } else if (method_name_ == "rindex") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.rindex() requires exactly one Text argument",
                                          context);
                }
                std::string substring = std::get<Text>(arg_values[0]);
                size_t pos = text_value.rfind(substring);
                if (pos == std::string::npos) {
                    throw EvaluationError("Text.rindex(): substring not found", context);
                }
                return Int(static_cast<Int>(pos));
            } else if (method_name_ == "isAlnum") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isAlnum() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                for (char c : text_value) {
                    if (!std::isalnum(static_cast<unsigned char>(c))) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isAlpha") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isAlpha() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                for (char c : text_value) {
                    if (!std::isalpha(static_cast<unsigned char>(c))) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isAscii") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isAscii() takes no arguments", context);
                }
                for (char c : text_value) {
                    if (static_cast<unsigned char>(c) > 127) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isDecimal") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isDecimal() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                for (char c : text_value) {
                    if (!std::isdigit(static_cast<unsigned char>(c))) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isDigit") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isDigit() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                for (char c : text_value) {
                    if (!std::isdigit(static_cast<unsigned char>(c))) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isIdentifier") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isIdentifier() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                // Valid identifier: starts with letter or underscore, followed by letters, digits,
                // or underscores
                if (!std::isalpha(static_cast<unsigned char>(text_value[0])) &&
                    text_value[0] != '_') {
                    return Bool(false);
                }
                for (size_t i = 1; i < text_value.length(); ++i) {
                    char c = text_value[i];
                    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isLower") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isLower() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                bool has_cased_char = false;
                for (char c : text_value) {
                    if (std::isalpha(static_cast<unsigned char>(c))) {
                        has_cased_char = true;
                        if (!std::islower(static_cast<unsigned char>(c))) {
                            return Bool(false);
                        }
                    }
                }
                return Bool(has_cased_char);
            } else if (method_name_ == "isNumeric") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isNumeric() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                // Accept digits, decimal point, plus/minus signs
                for (char c : text_value) {
                    if (!std::isdigit(static_cast<unsigned char>(c)) && c != '.' && c != '+' &&
                        c != '-') {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isPrintable") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isPrintable() takes no arguments", context);
                }
                for (char c : text_value) {
                    if (!std::isprint(static_cast<unsigned char>(c))) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isSpace") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isSpace() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                for (char c : text_value) {
                    if (!std::isspace(static_cast<unsigned char>(c))) {
                        return Bool(false);
                    }
                }
                return Bool(true);
            } else if (method_name_ == "isTitle") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isTitle() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                bool word_start = true;
                bool has_title_case = false;
                for (char c : text_value) {
                    if (std::isalpha(static_cast<unsigned char>(c))) {
                        if (word_start) {
                            if (!std::isupper(static_cast<unsigned char>(c))) {
                                return Bool(false);
                            }
                            has_title_case = true;
                            word_start = false;
                        } else {
                            if (!std::islower(static_cast<unsigned char>(c))) {
                                return Bool(false);
                            }
                        }
                    } else {
                        word_start = true;
                    }
                }
                return Bool(has_title_case);
            } else if (method_name_ == "isUpper") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.isUpper() takes no arguments", context);
                }
                if (text_value.empty()) {
                    return Bool(false);
                }
                bool has_cased_char = false;
                for (char c : text_value) {
                    if (std::isalpha(static_cast<unsigned char>(c))) {
                        has_cased_char = true;
                        if (!std::isupper(static_cast<unsigned char>(c))) {
                            return Bool(false);
                        }
                    }
                }
                return Bool(has_cased_char);
            } else if (method_name_ == "strip") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.strip() takes no arguments", context);
                }
                std::string result = text_value;
                // Left trim
                result.erase(result.begin(),
                             std::find_if(result.begin(), result.end(),
                                          [](unsigned char ch) { return !std::isspace(ch); }));
                // Right trim
                result.erase(std::find_if(result.rbegin(), result.rend(),
                                          [](unsigned char ch) { return !std::isspace(ch); })
                                 .base(),
                             result.end());
                return Text(result);
            } else if (method_name_ == "lstrip") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.lstrip() takes no arguments", context);
                }
                std::string result = text_value;
                result.erase(result.begin(),
                             std::find_if(result.begin(), result.end(),
                                          [](unsigned char ch) { return !std::isspace(ch); }));
                return Text(result);
            } else if (method_name_ == "rstrip") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.rstrip() takes no arguments", context);
                }
                std::string result = text_value;
                result.erase(std::find_if(result.rbegin(), result.rend(),
                                          [](unsigned char ch) { return !std::isspace(ch); })
                                 .base(),
                             result.end());
                return Text(result);
            } else if (method_name_ == "replace") {
                if (arg_values.size() != 2 || !std::holds_alternative<Text>(arg_values[0]) ||
                    !std::holds_alternative<Text>(arg_values[1])) {
                    throw EvaluationError("Text.replace() requires exactly two Text arguments",
                                          context);
                }
                std::string old_str = std::get<Text>(arg_values[0]);
                std::string new_str = std::get<Text>(arg_values[1]);
                std::string result = text_value;

                if (!old_str.empty()) {
                    size_t pos = 0;
                    while ((pos = result.find(old_str, pos)) != std::string::npos) {
                        result.replace(pos, old_str.length(), new_str);
                        pos += new_str.length();
                    }
                }
                return Text(result);
            } else if (method_name_ == "split") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.split() requires exactly one Text argument",
                                          context);
                }
                std::string delimiter = std::get<Text>(arg_values[0]);

                auto list_instance = std::make_shared<ListInstance>("Text");

                if (delimiter.empty()) {
                    // Split by whitespace
                    std::istringstream iss(text_value);
                    std::string token;
                    while (iss >> token) {
                        list_instance->add(Text(token));
                    }
                } else {
                    // Split by delimiter
                    size_t start = 0;
                    size_t end = text_value.find(delimiter);

                    while (end != std::string::npos) {
                        list_instance->add(Text(text_value.substr(start, end - start)));
                        start = end + delimiter.length();
                        end = text_value.find(delimiter, start);
                    }
                    list_instance->add(Text(text_value.substr(start)));
                }

                return Value(list_instance);
            } else if (method_name_ == "rsplit") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.rsplit() requires exactly one Text argument",
                                          context);
                }
                std::string delimiter = std::get<Text>(arg_values[0]);

                auto list_instance = std::make_shared<ListInstance>("Text");

                if (delimiter.empty()) {
                    // Split by whitespace (same as split for simplicity)
                    std::istringstream iss(text_value);
                    std::string token;
                    while (iss >> token) {
                        list_instance->add(Text(token));
                    }
                } else {
                    // Split by delimiter from right
                    std::vector<std::string> tokens;
                    size_t end = text_value.length();
                    size_t start = text_value.rfind(delimiter);

                    tokens.push_back(text_value.substr(
                        start == std::string::npos ? 0 : start + delimiter.length(), end));

                    while (start != std::string::npos) {
                        end = start;
                        start = text_value.rfind(delimiter, start - 1);

                        size_t token_start =
                            (start == std::string::npos) ? 0 : start + delimiter.length();
                        tokens.push_back(text_value.substr(token_start, end - token_start));
                    }

                    // Add tokens in reverse order
                    for (auto it = tokens.rbegin(); it != tokens.rend(); ++it) {
                        list_instance->add(Text(*it));
                    }
                }

                return Value(list_instance);
            } else if (method_name_ == "splitlines") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.splitlines() takes no arguments", context);
                }

                auto list_instance = std::make_shared<ListInstance>("Text");
                std::istringstream iss(text_value);
                std::string line;

                while (std::getline(iss, line)) {
                    list_instance->add(Text(line));
                }

                // Handle case where string doesn't end with newline but has content
                if (!text_value.empty() && text_value.back() != '\n') {
                    // Already handled by getline
                }

                return Value(list_instance);
            } else if (method_name_ == "center") {
                if (arg_values.size() != 1 || !std::holds_alternative<Int>(arg_values[0])) {
                    throw EvaluationError("Text.center() requires exactly one Int argument",
                                          context);
                }
                int width = std::get<Int>(arg_values[0]);
                if (width <= static_cast<int>(text_value.length())) {
                    return Text(text_value);
                }

                int padding = width - static_cast<int>(text_value.length());
                int left_padding = padding / 2;
                int right_padding = padding - left_padding;

                std::string result =
                    std::string(left_padding, ' ') + text_value + std::string(right_padding, ' ');
                return Text(result);
            } else if (method_name_ == "ljust") {
                if (arg_values.size() != 1 || !std::holds_alternative<Int>(arg_values[0])) {
                    throw EvaluationError("Text.ljust() requires exactly one Int argument",
                                          context);
                }
                int width = std::get<Int>(arg_values[0]);
                if (width <= static_cast<int>(text_value.length())) {
                    return Text(text_value);
                }

                int padding = width - static_cast<int>(text_value.length());
                std::string result = text_value + std::string(padding, ' ');
                return Text(result);
            } else if (method_name_ == "rjust") {
                if (arg_values.size() != 1 || !std::holds_alternative<Int>(arg_values[0])) {
                    throw EvaluationError("Text.rjust() requires exactly one Int argument",
                                          context);
                }
                int width = std::get<Int>(arg_values[0]);
                if (width <= static_cast<int>(text_value.length())) {
                    return Text(text_value);
                }

                int padding = width - static_cast<int>(text_value.length());
                std::string result = std::string(padding, ' ') + text_value;
                return Text(result);
            } else if (method_name_ == "zfill") {
                if (arg_values.size() != 1 || !std::holds_alternative<Int>(arg_values[0])) {
                    throw EvaluationError("Text.zfill() requires exactly one Int argument",
                                          context);
                }
                int width = std::get<Int>(arg_values[0]);
                if (width <= static_cast<int>(text_value.length())) {
                    return Text(text_value);
                }

                std::string result = text_value;

                // Handle leading sign
                size_t sign_pos = 0;
                if (!result.empty() && (result[0] == '+' || result[0] == '-')) {
                    sign_pos = 1;
                }

                int padding = width - static_cast<int>(text_value.length());
                result.insert(sign_pos, padding, '0');

                return Text(result);
            } else if (method_name_ == "join") {
                if (arg_values.size() != 1 ||
                    !std::holds_alternative<std::shared_ptr<ListInstance>>(arg_values[0])) {
                    throw EvaluationError("Text.join() requires exactly one List argument",
                                          context);
                }
                auto list = std::get<std::shared_ptr<ListInstance>>(arg_values[0]);
                const auto& elements = list->getElements();

                std::string result;
                for (size_t i = 0; i < elements.size(); ++i) {
                    if (i > 0) {
                        result += text_value;  // separator
                    }
                    // Convert element to string
                    if (std::holds_alternative<Text>(elements[i])) {
                        result += std::get<Text>(elements[i]);
                    } else if (std::holds_alternative<Int>(elements[i])) {
                        result += std::to_string(std::get<Int>(elements[i]));
                    } else if (std::holds_alternative<Float>(elements[i])) {
                        result += std::to_string(std::get<Float>(elements[i]));
                    } else if (std::holds_alternative<Bool>(elements[i])) {
                        result += std::get<Bool>(elements[i]) ? "true" : "false";
                    } else {
                        result += "[object]";  // fallback for complex objects
                    }
                }
                return Text(result);
            } else if (method_name_ == "partition") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.partition() requires exactly one Text argument",
                                          context);
                }
                std::string separator = std::get<Text>(arg_values[0]);

                auto list_instance = std::make_shared<ListInstance>("Text");

                size_t pos = text_value.find(separator);
                if (pos == std::string::npos) {
                    // Separator not found
                    list_instance->add(Text(text_value));
                    list_instance->add(Text(""));
                    list_instance->add(Text(""));
                } else {
                    // Separator found
                    list_instance->add(Text(text_value.substr(0, pos)));
                    list_instance->add(Text(separator));
                    list_instance->add(Text(text_value.substr(pos + separator.length())));
                }

                return Value(list_instance);
            } else if (method_name_ == "rpartition") {
                if (arg_values.size() != 1 || !std::holds_alternative<Text>(arg_values[0])) {
                    throw EvaluationError("Text.rpartition() requires exactly one Text argument",
                                          context);
                }
                std::string separator = std::get<Text>(arg_values[0]);

                auto list_instance = std::make_shared<ListInstance>("Text");

                size_t pos = text_value.rfind(separator);
                if (pos == std::string::npos) {
                    // Separator not found
                    list_instance->add(Text(""));
                    list_instance->add(Text(""));
                    list_instance->add(Text(text_value));
                } else {
                    // Separator found
                    list_instance->add(Text(text_value.substr(0, pos)));
                    list_instance->add(Text(separator));
                    list_instance->add(Text(text_value.substr(pos + separator.length())));
                }

                return Value(list_instance);
            } else if (method_name_ == "format") {
                // Simple format method - this would need more complex implementation for full
                // Python-style formatting For now, implement basic placeholder substitution
                std::string result = text_value;

                for (size_t i = 0; i < arg_values.size(); ++i) {
                    std::string placeholder = "{" + std::to_string(i) + "}";
                    std::string replacement;

                    if (std::holds_alternative<Text>(arg_values[i])) {
                        replacement = std::get<Text>(arg_values[i]);
                    } else if (std::holds_alternative<Int>(arg_values[i])) {
                        replacement = std::to_string(std::get<Int>(arg_values[i]));
                    } else if (std::holds_alternative<Float>(arg_values[i])) {
                        replacement = std::to_string(std::get<Float>(arg_values[i]));
                    } else if (std::holds_alternative<Bool>(arg_values[i])) {
                        replacement = std::get<Bool>(arg_values[i]) ? "true" : "false";
                    } else {
                        replacement = "[object]";
                    }

                    size_t pos = 0;
                    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                        result.replace(pos, placeholder.length(), replacement);
                        pos += replacement.length();
                    }
                }

                return Text(result);
            } else if (method_name_ == "formatMap") {
                if (arg_values.size() != 1 ||
                    !std::holds_alternative<std::shared_ptr<MapInstance>>(arg_values[0])) {
                    throw EvaluationError("Text.formatMap() requires exactly one Map argument",
                                          context);
                }

                auto map = std::get<std::shared_ptr<MapInstance>>(arg_values[0]);
                std::string result = text_value;

                // This is a simplified implementation - would need more complex parsing for full
                // functionality For now, replace {key} with map values
                for (const auto& [key, value] : map->getEntries()) {
                    if (std::holds_alternative<Text>(key)) {
                        std::string placeholder = "{" + std::get<Text>(key) + "}";
                        std::string replacement;

                        if (std::holds_alternative<Text>(value)) {
                            replacement = std::get<Text>(value);
                        } else if (std::holds_alternative<Int>(value)) {
                            replacement = std::to_string(std::get<Int>(value));
                        } else if (std::holds_alternative<Float>(value)) {
                            replacement = std::to_string(std::get<Float>(value));
                        } else if (std::holds_alternative<Bool>(value)) {
                            replacement = std::get<Bool>(value) ? "true" : "false";
                        } else {
                            replacement = "[object]";
                        }

                        size_t pos = 0;
                        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                            result.replace(pos, placeholder.length(), replacement);
                            pos += replacement.length();
                        }
                    }
                }

                return Text(result);
            } else if (method_name_ == "makeTrans") {
                // Simplified implementation - returns identity translation table
                if (arg_values.size() != 2 || !std::holds_alternative<Text>(arg_values[0]) ||
                    !std::holds_alternative<Text>(arg_values[1])) {
                    throw EvaluationError("Text.makeTrans() requires exactly two Text arguments",
                                          context);
                }

                // For simplicity, return a simple map representation
                // In a full implementation, this would create a translation table
                auto map_instance = std::make_shared<MapInstance>("Text", "Text");
                std::string from = std::get<Text>(arg_values[0]);
                std::string to = std::get<Text>(arg_values[1]);

                size_t min_len = std::min(from.length(), to.length());
                for (size_t i = 0; i < min_len; ++i) {
                    map_instance->put(Text(std::string(1, from[i])), Text(std::string(1, to[i])));
                }

                return Value(map_instance);
            } else if (method_name_ == "translate") {
                if (arg_values.size() != 1 ||
                    !std::holds_alternative<std::shared_ptr<MapInstance>>(arg_values[0])) {
                    throw EvaluationError("Text.translate() requires exactly one Map argument",
                                          context);
                }

                auto translation_table = std::get<std::shared_ptr<MapInstance>>(arg_values[0]);
                std::string result = text_value;

                // Apply character translations
                for (char& c : result) {
                    Text char_key = Text(std::string(1, c));
                    Value key_val(char_key);
                    if (translation_table->contains(key_val)) {
                        Value translated = translation_table->get(key_val);
                        if (std::holds_alternative<Text>(translated)) {
                            std::string trans_str = std::get<Text>(translated);
                            if (!trans_str.empty()) {
                                c = trans_str[0];
                            }
                        }
                    }
                }

                return Text(result);
            } else if (method_name_ == "toInt") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.toInt() takes no arguments", context);
                }
                try {
                    // Remove whitespace and convert to integer
                    std::string trimmed = text_value;
                    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
                    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

                    if (trimmed.empty()) {
                        throw std::invalid_argument("Empty string");
                    }

                    int result = std::stoi(trimmed);
                    return Int(result);
                } catch (const std::exception&) {
                    throw EvaluationError("Cannot convert '" + text_value + "' to Int", context);
                }
            } else if (method_name_ == "toLong") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.toLong() takes no arguments", context);
                }
                try {
                    std::string trimmed = text_value;
                    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
                    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

                    if (trimmed.empty()) {
                        throw std::invalid_argument("Empty string");
                    }

                    long result = std::stol(trimmed);
                    return Long(result);
                } catch (const std::exception&) {
                    throw EvaluationError("Cannot convert '" + text_value + "' to Long", context);
                }
            } else if (method_name_ == "toDouble") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.toDouble() takes no arguments", context);
                }
                try {
                    std::string trimmed = text_value;
                    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
                    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

                    if (trimmed.empty()) {
                        throw std::invalid_argument("Empty string");
                    }

                    double result = std::stod(trimmed);
                    return Float(result);
                } catch (const std::exception&) {
                    throw EvaluationError("Cannot convert '" + text_value + "' to Double", context);
                }
            } else if (method_name_ == "toFloat") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.toFloat() takes no arguments", context);
                }
                try {
                    std::string trimmed = text_value;
                    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
                    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

                    if (trimmed.empty()) {
                        throw std::invalid_argument("Empty string");
                    }

                    float result = std::stof(trimmed);
                    return Float(result);
                } catch (const std::exception&) {
                    throw EvaluationError("Cannot convert '" + text_value + "' to Float", context);
                }
            } else if (method_name_ == "toBool") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Text.toBool() takes no arguments", context);
                }

                std::string trimmed = text_value;
                trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
                trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

                // Convert to lowercase for comparison
                std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);

                if (trimmed == "true" || trimmed == "1" || trimmed == "yes" || trimmed == "on") {
                    return Bool(true);
                } else if (trimmed == "false" || trimmed == "0" || trimmed == "no" ||
                           trimmed == "off" || trimmed.empty()) {
                    return Bool(false);
                } else {
                    throw EvaluationError(
                        "Cannot convert '" + text_value +
                            "' to Bool. Expected: true/false, 1/0, yes/no, on/off",
                        context);
                }
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Text type",
                                      context);
            }
        }

        // Check if it's an Int
        if (std::holds_alternative<Int>(object_value)) {
            auto int_value = std::get<Int>(object_value);

            if (method_name_ == "toString") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Int.toString() takes no arguments", context);
                }
                return Text(std::to_string(int_value));
            } else if (method_name_ == "toDouble") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Int.toDouble() takes no arguments", context);
                }
                return Float(static_cast<double>(int_value));
            } else if (method_name_ == "toFloat") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Int.toFloat() takes no arguments", context);
                }
                return Float(static_cast<float>(int_value));
            } else if (method_name_ == "toLong") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Int.toLong() takes no arguments", context);
                }
                return Long(static_cast<long>(int_value));
            } else if (method_name_ == "toBool") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Int.toBool() takes no arguments", context);
                }
                return Bool(int_value != 0);
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Int type", context);
            }
        }

        // Check if it's a Long
        if (std::holds_alternative<Long>(object_value)) {
            auto long_value = std::get<Long>(object_value);

            if (method_name_ == "toString") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Long.toString() takes no arguments", context);
                }
                return Text(std::to_string(static_cast<long long>(long_value)));
            } else if (method_name_ == "toInt") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Long.toInt() takes no arguments", context);
                }
                if (long_value > std::numeric_limits<int>::max() ||
                    long_value < std::numeric_limits<int>::min()) {
                    throw EvaluationError("Long value " +
                                              std::to_string(static_cast<long long>(long_value)) +
                                              " out of Int range",
                                          context);
                }
                return Int(static_cast<int>(long_value));
            } else if (method_name_ == "toDouble") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Long.toDouble() takes no arguments", context);
                }
                return Float(static_cast<double>(long_value));
            } else if (method_name_ == "toFloat") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Long.toFloat() takes no arguments", context);
                }
                return Float(static_cast<float>(long_value));
            } else if (method_name_ == "toBool") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Long.toBool() takes no arguments", context);
                }
                return Bool(long_value != 0);
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Long type",
                                      context);
            }
        }

        // Check if it's a Float (double)
        if (std::holds_alternative<Float>(object_value)) {
            auto float_value = std::get<Float>(object_value);

            if (method_name_ == "toString") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.toString() takes no arguments", context);
                }
                return Text(std::to_string(float_value));
            } else if (method_name_ == "toInt") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.toInt() takes no arguments", context);
                }
                if (std::isnan(float_value) || std::isinf(float_value)) {
                    throw EvaluationError("Cannot convert NaN or Infinity to Int", context);
                }
                if (float_value > std::numeric_limits<int>::max() ||
                    float_value < std::numeric_limits<int>::min()) {
                    throw EvaluationError(
                        "Float value " + std::to_string(float_value) + " out of Int range",
                        context);
                }
                return Int(static_cast<int>(float_value));
            } else if (method_name_ == "toLong") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.toLong() takes no arguments", context);
                }
                if (std::isnan(float_value) || std::isinf(float_value)) {
                    throw EvaluationError("Cannot convert NaN or Infinity to Long", context);
                }
                if (float_value > std::numeric_limits<long>::max() ||
                    float_value < std::numeric_limits<long>::min()) {
                    throw EvaluationError(
                        "Float value " + std::to_string(float_value) + " out of Long range",
                        context);
                }
                return Long(static_cast<long>(float_value));
            } else if (method_name_ == "toBool") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.toBool() takes no arguments", context);
                }
                return Bool(float_value != 0.0);
            } else if (method_name_ == "isNaN") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.isNaN() takes no arguments", context);
                }
                return Bool(std::isnan(float_value));
            } else if (method_name_ == "isInfinite") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.isInfinite() takes no arguments", context);
                }
                return Bool(std::isinf(float_value));
            } else if (method_name_ == "isFinite") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Float.isFinite() takes no arguments", context);
                }
                return Bool(std::isfinite(float_value));
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Float type",
                                      context);
            }
        }

        // Check if it's a Bool
        if (std::holds_alternative<Bool>(object_value)) {
            auto bool_value = std::get<Bool>(object_value);

            if (method_name_ == "toString") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Bool.toString() takes no arguments", context);
                }
                return Text(bool_value ? "true" : "false");
            } else if (method_name_ == "toInt") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Bool.toInt() takes no arguments", context);
                }
                return Int(bool_value ? 1 : 0);
            } else if (method_name_ == "toLong") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Bool.toLong() takes no arguments", context);
                }
                return Long(bool_value ? 1L : 0L);
            } else if (method_name_ == "toDouble") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Bool.toDouble() takes no arguments", context);
                }
                return Float(bool_value ? 1.0 : 0.0);
            } else if (method_name_ == "toFloat") {
                if (!arg_values.empty()) {
                    throw EvaluationError("Bool.toFloat() takes no arguments", context);
                }
                return Float(bool_value ? 1.0f : 0.0f);
            } else {
                throw EvaluationError("Unknown method '" + method_name_ + "' on Bool type",
                                      context);
            }
        }

        // Check if it's an object instance
        if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(object_value)) {
            throw TypeMismatchError("Cannot call method '" + method_name_ + "' on non-object type");
        }

        auto object_instance = std::get<std::shared_ptr<ObjectInstance>>(object_value);

        // Determine if this is an internal call (this.method()) or external call
        bool is_external_call = true;

        // Check if the object expression is a ThisNode by checking if the object_value
        // is the same as the current 'this' object in the context
        if (context.hasThisObject()) {
            auto this_object = context.getThisObject();
            if (object_instance == this_object) {
                is_external_call = false;  // Internal call from within the same object
            }
        }

        return object_instance->callMethod(method_name_, arg_values, context, is_external_call);

    } catch (const o2lException& e) {
        // Re-throw with current context if it doesn't already have a stack trace
        if (e.getStackTrace().empty()) {
            throw EvaluationError(e.getMessage(), context);
        } else {
            throw;  // Re-throw as-is if it already has stack trace
        }
    } catch (const std::exception& e) {
        // Convert standard exceptions to our exception type with stack trace
        throw EvaluationError(std::string("Standard exception in method call: ") + e.what(),
                              context);
    }
}

std::string MethodCallNode::toString() const {
    return "MethodCall(" + object_->toString() + "." + method_name_ + ")";
}

}  // namespace o2l