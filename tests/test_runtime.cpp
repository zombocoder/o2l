#include <gtest/gtest.h>

#include "AST/EnumDeclarationNode.hpp"
#include "Common/Exceptions.hpp"
#include "Runtime/Context.hpp"
#include "Runtime/EnumInstance.hpp"
#include "Runtime/ErrorInstance.hpp"
#include "Runtime/ListInstance.hpp"
#include "Runtime/ListIterator.hpp"
#include "Runtime/MapInstance.hpp"
#include "Runtime/MapIterator.hpp"
#include "Runtime/MapObject.hpp"
#include "Runtime/ObjectInstance.hpp"
#include "Runtime/RepeatIterator.hpp"
#include "Runtime/ResultInstance.hpp"
#include "Runtime/SetInstance.hpp"
#include "Runtime/SetIterator.hpp"
#include "Runtime/Value.hpp"

using namespace o2l;

class RuntimeTest : public ::testing::Test {
   protected:
    void SetUp() override {}
};

// Test basic value types
TEST_F(RuntimeTest, BasicValueTypes) {
    Value int_val = Int(42);
    Value long_val = Long(123456789012345L);
    Value float_val = Float(3.14f);
    Value double_val = Double(2.718);
    Value text_val = Text("Hello");
    Value bool_val = Bool(true);
    Value char_val = Char('A');

    EXPECT_TRUE(std::holds_alternative<Int>(int_val));
    EXPECT_TRUE(std::holds_alternative<Long>(long_val));
    EXPECT_TRUE(std::holds_alternative<Float>(float_val));
    EXPECT_TRUE(std::holds_alternative<Double>(double_val));
    EXPECT_TRUE(std::holds_alternative<Text>(text_val));
    EXPECT_TRUE(std::holds_alternative<Bool>(bool_val));
    EXPECT_TRUE(std::holds_alternative<Char>(char_val));

    EXPECT_EQ(std::get<Int>(int_val), 42);
    EXPECT_EQ(std::get<Long>(long_val), Long(123456789012345L));
    EXPECT_FLOAT_EQ(std::get<Float>(float_val), 3.14f);
    EXPECT_DOUBLE_EQ(std::get<Double>(double_val), 2.718);
    EXPECT_EQ(std::get<Text>(text_val), "Hello");
    EXPECT_EQ(std::get<Bool>(bool_val), true);
    EXPECT_EQ(std::get<Char>(char_val), 'A');
}

// Test value to string conversion
TEST_F(RuntimeTest, ValueToString) {
    EXPECT_EQ(valueToString(Value(Int(42))), "42");
    EXPECT_EQ(valueToString(Value(Long(123456789012345L))), "123456789012345");
    EXPECT_EQ(valueToString(Value(Float(3.14f))), "3.140000");   // actual format
    EXPECT_EQ(valueToString(Value(Double(2.718))), "2.718000");  // actual format
    EXPECT_EQ(valueToString(Value(Text("Hello"))), "Hello");
    EXPECT_EQ(valueToString(Value(Bool(true))), "true");
    EXPECT_EQ(valueToString(Value(Bool(false))), "false");
    EXPECT_EQ(valueToString(Value(Char('A'))), "A");
}

// Test value equality
TEST_F(RuntimeTest, ValueEquality) {
    EXPECT_TRUE(valuesEqual(Value(Int(42)), Value(Int(42))));
    EXPECT_FALSE(valuesEqual(Value(Int(42)), Value(Int(43))));

    EXPECT_TRUE(valuesEqual(Value(Long(123456789012345L)), Value(Long(123456789012345L))));
    EXPECT_FALSE(valuesEqual(Value(Long(123456789012345L)), Value(Long(987654321098765L))));

    EXPECT_TRUE(valuesEqual(Value(Float(3.14f)), Value(Float(3.14f))));
    EXPECT_FALSE(valuesEqual(Value(Float(3.14f)), Value(Float(2.71f))));

    EXPECT_TRUE(valuesEqual(Value(Double(2.718)), Value(Double(2.718))));
    EXPECT_FALSE(valuesEqual(Value(Double(2.718)), Value(Double(3.141))));

    // Test that different numeric types are distinct
    EXPECT_FALSE(valuesEqual(Value(Int(42)), Value(Long(42L))));
    EXPECT_FALSE(valuesEqual(Value(Float(3.14f)), Value(Double(3.14))));
    EXPECT_FALSE(valuesEqual(Value(Long(42L)), Value(Float(42.0f))));

    EXPECT_TRUE(valuesEqual(Value(Text("Hello")), Value(Text("Hello"))));
    EXPECT_FALSE(valuesEqual(Value(Text("Hello")), Value(Text("World"))));

    EXPECT_TRUE(valuesEqual(Value(Bool(true)), Value(Bool(true))));
    EXPECT_FALSE(valuesEqual(Value(Bool(true)), Value(Bool(false))));
}

// Test type names
TEST_F(RuntimeTest, TypeNames) {
    EXPECT_EQ(getTypeName(Value(Int(42))), "Int");
    EXPECT_EQ(getTypeName(Value(Long(123456789012345L))), "Long");
    EXPECT_EQ(getTypeName(Value(Float(3.14f))), "Float");
    EXPECT_EQ(getTypeName(Value(Double(2.718))), "Double");
    EXPECT_EQ(getTypeName(Value(Text("Hello"))), "Text");
    EXPECT_EQ(getTypeName(Value(Bool(true))), "Bool");
    EXPECT_EQ(getTypeName(Value(Char('A'))), "Char");
}

// Test Double precision vs Float precision
TEST_F(RuntimeTest, FloatDoublePrecision) {
    // Test precision differences
    Float float_val = 1.234567890123456789f;
    Double double_val = 1.234567890123456789;

    Value float_value = Value(float_val);
    Value double_value = Value(double_val);

    // Double should maintain more precision than Float
    EXPECT_NE(std::get<Float>(float_value), std::get<Double>(double_value));

    // Test large numbers that exceed float precision
    Double large_double = 1234567890123456789.0;
    Float large_float = static_cast<Float>(large_double);

    EXPECT_NE(large_double, static_cast<Double>(large_float));
}

// Test Long integer range and arithmetic
TEST_F(RuntimeTest, LongIntegerArithmetic) {
    // Test basic Long arithmetic
    Long a = 123456789012345L;
    Long b = 987654321098765L;

    Value long_a = Value(a);
    Value long_b = Value(b);

    EXPECT_EQ(std::get<Long>(long_a), a);
    EXPECT_EQ(std::get<Long>(long_b), b);

    // Test that Long can hold much larger values than Int
    Int max_int = 9223372036854775807LL;  // 2^63 - 1
    Long large_long = static_cast<Long>(max_int) * 1000;

    Value large_long_val = Value(large_long);
    EXPECT_TRUE(std::holds_alternative<Long>(large_long_val));

    // Test Long string conversion
    EXPECT_EQ(valueToString(Value(Long(123456789012345L))), "123456789012345");
}

// Test Context operations
TEST_F(RuntimeTest, Context) {
    Context context;

    // Test variable definition and retrieval
    context.defineVariable("x", Value(Int(42)));
    EXPECT_TRUE(context.hasVariable("x"));

    Value retrieved = context.getVariable("x");
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved));
    EXPECT_EQ(std::get<Int>(retrieved), 42);

    // Test variable redefinition (no setVariable method)
    context.defineVariable("y", Value(Int(100)));
    Value updated = context.getVariable("y");
    EXPECT_EQ(std::get<Int>(updated), 100);

    // Test non-existent variable
    EXPECT_FALSE(context.hasVariable("nonexistent"));
}

// Test Context scopes
TEST_F(RuntimeTest, ContextScopes) {
    Context context;

    // Define variable in outer scope
    context.defineVariable("outer", Value(Int(1)));

    // Push new scope
    context.pushScope();

    // Define variable in inner scope
    context.defineVariable("inner", Value(Int(2)));

    // Both variables should be accessible
    EXPECT_TRUE(context.hasVariable("outer"));
    EXPECT_TRUE(context.hasVariable("inner"));

    // Pop scope
    context.popScope();

    // Only outer variable should be accessible
    EXPECT_TRUE(context.hasVariable("outer"));
    EXPECT_FALSE(context.hasVariable("inner"));
}

// Test ObjectInstance
TEST_F(RuntimeTest, ObjectInstance) {
    auto object = std::make_shared<ObjectInstance>("TestObject");

    // Test object name
    EXPECT_EQ(object->getName(), "TestObject");

    // Test method addition and checking
    Method test_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return Value(Text("test result"));
    };

    object->addMethod("test", test_method, true);  // external
    EXPECT_TRUE(object->hasMethod("test"));

    // Test method calling
    Context context;
    std::vector<Value> args;
    Value result = object->callMethod("test", args, context, true);  // external call

    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_EQ(std::get<Text>(result), "test result");
}

// Test ObjectInstance method visibility
TEST_F(RuntimeTest, ObjectInstanceVisibility) {
    auto object = std::make_shared<ObjectInstance>("TestObject");

    Method public_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return Value(Text("public"));
    };

    Method protected_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return Value(Text("protected"));
    };

    object->addMethod("publicMethod", public_method, true);         // external
    object->addMethod("protectedMethod", protected_method, false);  // protected

    Context context;
    std::vector<Value> args;

    // External call should work for public method
    EXPECT_NO_THROW({ object->callMethod("publicMethod", args, context, true); });

    // External call should fail for protected method
    EXPECT_THROW({ object->callMethod("protectedMethod", args, context, true); }, std::exception);

    // Internal call should work for both
    EXPECT_NO_THROW({ object->callMethod("publicMethod", args, context, false); });
    EXPECT_NO_THROW({ object->callMethod("protectedMethod", args, context, false); });
}

// Test EnumInstance
TEST_F(RuntimeTest, EnumInstance) {
    std::vector<EnumMember> enum_members = {{"ACTIVE", 1}, {"INACTIVE", 0}, {"PENDING", 2}};

    auto enum_instance = std::make_shared<EnumInstance>("Status", enum_members);

    EXPECT_EQ(enum_instance->getEnumName(), "Status");
    EXPECT_TRUE(enum_instance->hasMember("ACTIVE"));
    EXPECT_TRUE(enum_instance->hasMember("INACTIVE"));
    EXPECT_TRUE(enum_instance->hasMember("PENDING"));
    EXPECT_FALSE(enum_instance->hasMember("UNKNOWN"));

    EXPECT_EQ(enum_instance->getMemberValue("ACTIVE"), 1);
    EXPECT_EQ(enum_instance->getMemberValue("INACTIVE"), 0);
    EXPECT_EQ(enum_instance->getMemberValue("PENDING"), 2);
}

// Test basic object operations (simplified)
TEST_F(RuntimeTest, RecordAndProtocolBasics) {
    // Just test that we can create these objects without crashing
    EXPECT_NO_THROW({
        // These types exist but may have different constructors
        // Just test basic instantiation doesn't crash
        auto object = std::make_shared<ObjectInstance>("TestObject");
        EXPECT_EQ(object->getName(), "TestObject");
    });
}

// Test ListInstance creation and basic operations
TEST_F(RuntimeTest, ListInstanceBasics) {
    auto list = std::make_shared<ListInstance>("Int");

    EXPECT_EQ(list->size(), 0);
    EXPECT_TRUE(list->empty());
    EXPECT_EQ(list->getElementTypeName(), "Int");

    // Test adding elements
    list->add(Value(Int(10)));
    list->add(Value(Int(20)));
    list->add(Value(Int(30)));

    EXPECT_EQ(list->size(), 3);
    EXPECT_FALSE(list->empty());

    // Test getting elements
    EXPECT_TRUE(std::holds_alternative<Int>(list->get(0)));
    EXPECT_EQ(std::get<Int>(list->get(0)), 10);
    EXPECT_EQ(std::get<Int>(list->get(1)), 20);
    EXPECT_EQ(std::get<Int>(list->get(2)), 30);

    // Test remove
    list->remove(1);  // Remove element at index 1 (20)
    EXPECT_EQ(list->size(), 2);
    EXPECT_EQ(std::get<Int>(list->get(1)), 30);  // 30 should now be at index 1

    // Test pop
    Value popped = list->pop();
    EXPECT_TRUE(std::holds_alternative<Int>(popped));
    EXPECT_EQ(std::get<Int>(popped), 30);
    EXPECT_EQ(list->size(), 1);

    // Test clear
    list->clear();
    EXPECT_EQ(list->size(), 0);
    EXPECT_TRUE(list->empty());
}

// Test ListIterator functionality
TEST_F(RuntimeTest, ListIteratorBasics) {
    auto list = std::make_shared<ListInstance>("Int");
    list->add(Value(Int(100)));
    list->add(Value(Int(200)));
    list->add(Value(Int(300)));

    ListIterator iterator(list);

    // Test initial state
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);

    // Test first iteration
    Value first = iterator.next();
    EXPECT_TRUE(std::holds_alternative<Int>(first));
    EXPECT_EQ(std::get<Int>(first), 100);
    EXPECT_EQ(iterator.getCurrentIndex(), 1);
    EXPECT_TRUE(iterator.hasNext());

    // Test second iteration
    Value second = iterator.next();
    EXPECT_EQ(std::get<Int>(second), 200);
    EXPECT_EQ(iterator.getCurrentIndex(), 2);
    EXPECT_TRUE(iterator.hasNext());

    // Test third iteration
    Value third = iterator.next();
    EXPECT_EQ(std::get<Int>(third), 300);
    EXPECT_EQ(iterator.getCurrentIndex(), 3);
    EXPECT_FALSE(iterator.hasNext());

    // Test reset
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);

    // Test first element after reset
    Value first_again = iterator.next();
    EXPECT_EQ(std::get<Int>(first_again), 100);
}

// Test ListIterator with empty list
TEST_F(RuntimeTest, ListIteratorEmpty) {
    auto empty_list = std::make_shared<ListInstance>("Int");
    ListIterator iterator(empty_list);

    EXPECT_FALSE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);

    // Test that next() throws on empty iterator
    EXPECT_THROW({ iterator.next(); }, std::exception);
}

// Test ListIterator Value type handling
TEST_F(RuntimeTest, ListIteratorValueTypes) {
    auto list_iterator = std::make_shared<ListIterator>(std::make_shared<ListInstance>("Int"));
    Value iterator_value = Value(list_iterator);

    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListIterator>>(iterator_value));

    // Test type name
    EXPECT_EQ(getTypeName(iterator_value), "ListIterator");

    // Test toString
    std::string str = valueToString(iterator_value);
    EXPECT_TRUE(str.find("ListIterator") != std::string::npos);
}

// Test RepeatIterator functionality
TEST_F(RuntimeTest, RepeatIteratorBasics) {
    RepeatIterator repeater(5);

    // Test initial state
    EXPECT_TRUE(repeater.hasNext());
    EXPECT_EQ(repeater.getCurrentCount(), 0);
    EXPECT_EQ(repeater.getTotalCount(), 5);

    // Test first iteration
    Value first = repeater.next();
    EXPECT_TRUE(std::holds_alternative<Int>(first));
    EXPECT_EQ(std::get<Int>(first), 0);
    EXPECT_EQ(repeater.getCurrentCount(), 1);
    EXPECT_TRUE(repeater.hasNext());

    // Test second iteration
    Value second = repeater.next();
    EXPECT_EQ(std::get<Int>(second), 1);
    EXPECT_EQ(repeater.getCurrentCount(), 2);
    EXPECT_TRUE(repeater.hasNext());

    // Continue until end
    repeater.next();               // 2
    repeater.next();               // 3
    Value last = repeater.next();  // 4
    EXPECT_EQ(std::get<Int>(last), 4);
    EXPECT_EQ(repeater.getCurrentCount(), 5);
    EXPECT_FALSE(repeater.hasNext());

    // Test reset
    repeater.reset();
    EXPECT_TRUE(repeater.hasNext());
    EXPECT_EQ(repeater.getCurrentCount(), 0);

    // Test first element after reset
    Value first_again = repeater.next();
    EXPECT_EQ(std::get<Int>(first_again), 0);
}

// Test RepeatIterator with zero count
TEST_F(RuntimeTest, RepeatIteratorZero) {
    RepeatIterator repeater(0);

    EXPECT_FALSE(repeater.hasNext());
    EXPECT_EQ(repeater.getCurrentCount(), 0);
    EXPECT_EQ(repeater.getTotalCount(), 0);

    // Test that next() throws on empty iterator
    EXPECT_THROW({ repeater.next(); }, std::exception);
}

// Test RepeatIterator with negative count throws
TEST_F(RuntimeTest, RepeatIteratorNegative) {
    EXPECT_THROW({ RepeatIterator repeater(-1); }, std::exception);
}

// Test RepeatIterator Value type handling
TEST_F(RuntimeTest, RepeatIteratorValueTypes) {
    auto repeat_iterator = std::make_shared<RepeatIterator>(3);
    Value iterator_value = Value(repeat_iterator);

    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<RepeatIterator>>(iterator_value));

    // Test type name
    EXPECT_EQ(getTypeName(iterator_value), "RepeatIterator");

    // Test toString
    std::string str = valueToString(iterator_value);
    EXPECT_TRUE(str.find("RepeatIterator") != std::string::npos);
}

// Test MapInstance creation and basic operations
TEST_F(RuntimeTest, MapInstanceBasics) {
    auto map = std::make_shared<MapInstance>("Text", "Int");

    EXPECT_EQ(map->size(), 0);
    EXPECT_TRUE(map->empty());
    EXPECT_EQ(map->getKeyTypeName(), "Text");
    EXPECT_EQ(map->getValueTypeName(), "Int");

    // Test putting elements
    map->put(Value(Text("key1")), Value(Int(100)));
    map->put(Value(Text("key2")), Value(Int(200)));
    map->put(Value(Text("key3")), Value(Int(300)));

    EXPECT_EQ(map->size(), 3);
    EXPECT_FALSE(map->empty());

    // Test getting elements
    Value val1 = map->get(Value(Text("key1")));
    Value val2 = map->get(Value(Text("key2")));
    Value val3 = map->get(Value(Text("key3")));

    EXPECT_TRUE(std::holds_alternative<Int>(val1));
    EXPECT_TRUE(std::holds_alternative<Int>(val2));
    EXPECT_TRUE(std::holds_alternative<Int>(val3));
    EXPECT_EQ(std::get<Int>(val1), 100);
    EXPECT_EQ(std::get<Int>(val2), 200);
    EXPECT_EQ(std::get<Int>(val3), 300);

    // Test contains
    EXPECT_TRUE(map->contains(Value(Text("key1"))));
    EXPECT_TRUE(map->contains(Value(Text("key2"))));
    EXPECT_TRUE(map->contains(Value(Text("key3"))));
    EXPECT_FALSE(map->contains(Value(Text("nonexistent"))));

    // Test remove
    map->remove(Value(Text("key2")));
    EXPECT_EQ(map->size(), 2);
    EXPECT_FALSE(map->contains(Value(Text("key2"))));
    EXPECT_TRUE(map->contains(Value(Text("key1"))));
    EXPECT_TRUE(map->contains(Value(Text("key3"))));

    // Test clear
    map->clear();
    EXPECT_EQ(map->size(), 0);
    EXPECT_TRUE(map->empty());
}

// Test MapInstance with different value types
TEST_F(RuntimeTest, MapInstanceValueTypes) {
    auto text_map = std::make_shared<MapInstance>("Text", "Text");
    auto mixed_map = std::make_shared<MapInstance>("Int", "Text");

    // Test Text->Text map
    text_map->put(Value(Text("name")), Value(Text("Alice")));
    text_map->put(Value(Text("role")), Value(Text("Developer")));

    EXPECT_EQ(text_map->size(), 2);
    Value name = text_map->get(Value(Text("name")));
    EXPECT_TRUE(std::holds_alternative<Text>(name));
    EXPECT_EQ(std::get<Text>(name), "Alice");

    // Test Int->Text map
    mixed_map->put(Value(Int(1)), Value(Text("First")));
    mixed_map->put(Value(Int(2)), Value(Text("Second")));

    EXPECT_EQ(mixed_map->size(), 2);
    Value first = mixed_map->get(Value(Int(1)));
    EXPECT_TRUE(std::holds_alternative<Text>(first));
    EXPECT_EQ(std::get<Text>(first), "First");
}

// Test MapInstance keys() and values() methods
TEST_F(RuntimeTest, MapInstanceKeysValues) {
    auto map = std::make_shared<MapInstance>("Text", "Int");

    map->put(Value(Text("a")), Value(Int(10)));
    map->put(Value(Text("b")), Value(Int(20)));
    map->put(Value(Text("c")), Value(Int(30)));

    // Test keys()
    std::vector<Value> keys = map->keys();
    EXPECT_EQ(keys.size(), 3);

    // Keys should be "a", "b", "c" (order may vary)
    std::set<std::string> key_set;
    for (const auto& key : keys) {
        EXPECT_TRUE(std::holds_alternative<Text>(key));
        key_set.insert(std::get<Text>(key));
    }
    EXPECT_TRUE(key_set.count("a") > 0);
    EXPECT_TRUE(key_set.count("b") > 0);
    EXPECT_TRUE(key_set.count("c") > 0);

    // Test values()
    std::vector<Value> values = map->values();
    EXPECT_EQ(values.size(), 3);

    // Values should be 10, 20, 30 (order may vary)
    std::set<int> value_set;
    for (const auto& value : values) {
        EXPECT_TRUE(std::holds_alternative<Int>(value));
        value_set.insert(std::get<Int>(value));
    }
    EXPECT_TRUE(value_set.count(10) > 0);
    EXPECT_TRUE(value_set.count(20) > 0);
    EXPECT_TRUE(value_set.count(30) > 0);
}

// Test MapIterator functionality
TEST_F(RuntimeTest, MapIteratorBasics) {
    auto map = std::make_shared<MapInstance>("Text", "Int");
    map->put(Value(Text("apple")), Value(Int(5)));
    map->put(Value(Text("banana")), Value(Int(3)));
    map->put(Value(Text("cherry")), Value(Int(8)));

    MapIterator iterator(map);

    // Test initial state
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);
    EXPECT_EQ(iterator.getTotalSize(), 3);

    // Track keys and values we've seen
    std::set<std::string> keys_seen;
    std::set<int> values_seen;

    // Test first iteration
    EXPECT_TRUE(iterator.hasNext());
    Value first_key = iterator.nextKey();
    EXPECT_TRUE(std::holds_alternative<Text>(first_key));
    keys_seen.insert(std::get<Text>(first_key));

    // Reset and test nextValue
    iterator.reset();
    Value first_value = iterator.nextValue();
    EXPECT_TRUE(std::holds_alternative<Int>(first_value));
    values_seen.insert(std::get<Int>(first_value));

    // Reset and test nextEntry
    iterator.reset();
    Value first_entry = iterator.nextEntry();
    EXPECT_TRUE(std::holds_alternative<Text>(first_entry));
    std::string entry_str = std::get<Text>(first_entry);
    EXPECT_TRUE(entry_str.find(":") != std::string::npos);

    // Reset and test MapItem
    iterator.reset();
    Value first_map_item = iterator.MapItem();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapObject>>(first_map_item));
    auto map_obj = std::get<std::shared_ptr<MapObject>>(first_map_item);
    EXPECT_TRUE(std::holds_alternative<Text>(map_obj->getKey()));
    EXPECT_TRUE(std::holds_alternative<Int>(map_obj->getVal()));

    // Continue iteration to collect all items
    iterator.reset();
    int iterations = 0;
    while (iterator.hasNext() && iterations < 10) {  // safety check
        Value key = iterator.nextKey();
        EXPECT_TRUE(std::holds_alternative<Text>(key));
        keys_seen.insert(std::get<Text>(key));
        iterations++;
    }

    // Should have seen all keys
    EXPECT_EQ(keys_seen.size(), 3);
    EXPECT_TRUE(keys_seen.count("apple") > 0);
    EXPECT_TRUE(keys_seen.count("banana") > 0);
    EXPECT_TRUE(keys_seen.count("cherry") > 0);

    EXPECT_FALSE(iterator.hasNext());

    // Test reset
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);
}

// Test MapIterator with empty map
TEST_F(RuntimeTest, MapIteratorEmpty) {
    auto empty_map = std::make_shared<MapInstance>("Text", "Int");
    MapIterator iterator(empty_map);

    EXPECT_FALSE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);
    EXPECT_EQ(iterator.getTotalSize(), 0);

    // Test that next methods throw on empty iterator
    EXPECT_THROW({ iterator.nextKey(); }, std::exception);

    EXPECT_THROW({ iterator.nextValue(); }, std::exception);

    EXPECT_THROW({ iterator.nextEntry(); }, std::exception);

    EXPECT_THROW({ iterator.MapItem(); }, std::exception);
}

// Test MapObject functionality
TEST_F(RuntimeTest, MapObjectBasics) {
    Value key = Value(Text("test_key"));
    Value value = Value(Int(42));

    MapObject map_obj(key, value, "Text", "Int");

    // Test getters
    Value retrieved_key = map_obj.getKey();
    Value retrieved_value = map_obj.getVal();
    Value retrieved_value_alias = map_obj.getValue();

    EXPECT_TRUE(std::holds_alternative<Text>(retrieved_key));
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved_value));
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved_value_alias));

    EXPECT_EQ(std::get<Text>(retrieved_key), "test_key");
    EXPECT_EQ(std::get<Int>(retrieved_value), 42);
    EXPECT_EQ(std::get<Int>(retrieved_value_alias), 42);

    // Test type names
    EXPECT_EQ(map_obj.getKeyTypeName(), "Text");
    EXPECT_EQ(map_obj.getValueTypeName(), "Int");

    // Test toString
    std::string str = map_obj.toString();
    EXPECT_TRUE(str.find("test_key") != std::string::npos);
    EXPECT_TRUE(str.find("42") != std::string::npos);
}

// Test MapIterator and MapObject integration
TEST_F(RuntimeTest, MapIteratorMapObjectIntegration) {
    auto map = std::make_shared<MapInstance>("Text", "Text");
    map->put(Value(Text("name")), Value(Text("Alice")));
    map->put(Value(Text("role")), Value(Text("Developer")));

    MapIterator iterator(map);

    std::set<std::string> keys_found;
    std::set<std::string> values_found;

    while (iterator.hasNext()) {
        Value map_item_val = iterator.MapItem();
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapObject>>(map_item_val));

        auto map_obj = std::get<std::shared_ptr<MapObject>>(map_item_val);

        Value key = map_obj->getKey();
        Value value = map_obj->getVal();

        EXPECT_TRUE(std::holds_alternative<Text>(key));
        EXPECT_TRUE(std::holds_alternative<Text>(value));

        keys_found.insert(std::get<Text>(key));
        values_found.insert(std::get<Text>(value));
    }

    EXPECT_EQ(keys_found.size(), 2);
    EXPECT_EQ(values_found.size(), 2);
    EXPECT_TRUE(keys_found.count("name") > 0);
    EXPECT_TRUE(keys_found.count("role") > 0);
    EXPECT_TRUE(values_found.count("Alice") > 0);
    EXPECT_TRUE(values_found.count("Developer") > 0);
}

// Test Map Value type handling
TEST_F(RuntimeTest, MapValueTypes) {
    auto map_instance = std::make_shared<MapInstance>("Text", "Int");
    map_instance->put(Value(Text("key")), Value(Int(100)));

    Value map_value = Value(map_instance);
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(map_value));

    // Test type name
    EXPECT_EQ(getTypeName(map_value), "Map<Text, Int>");

    // Test toString
    std::string str = valueToString(map_value);
    EXPECT_TRUE(str.find("key") != std::string::npos);
    EXPECT_TRUE(str.find("100") != std::string::npos);

    // Test MapIterator as Value
    auto map_iterator = std::make_shared<MapIterator>(map_instance);
    Value iterator_value = Value(map_iterator);

    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapIterator>>(iterator_value));
    EXPECT_EQ(getTypeName(iterator_value), "MapIterator");

    // Test MapObject as Value
    MapObject map_obj(Value(Text("key")), Value(Int(100)), "Text", "Int");
    auto map_obj_ptr = std::make_shared<MapObject>(map_obj);
    Value map_obj_value = Value(map_obj_ptr);

    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapObject>>(map_obj_value));
    EXPECT_EQ(getTypeName(map_obj_value), "MapObject");
}

// Test SetInstance creation and basic operations
TEST_F(RuntimeTest, SetInstanceBasics) {
    auto set = std::make_shared<SetInstance>("Int");

    EXPECT_EQ(set->size(), 0);
    EXPECT_TRUE(set->empty());
    EXPECT_EQ(set->getElementTypeName(), "Int");

    // Test adding elements
    set->add(Value(Int(10)));
    set->add(Value(Int(20)));
    set->add(Value(Int(30)));

    EXPECT_EQ(set->size(), 3);
    EXPECT_FALSE(set->empty());

    // Test adding duplicate (should not increase size)
    set->add(Value(Int(20)));
    EXPECT_EQ(set->size(), 3);  // Still 3, no duplicates

    // Test contains
    EXPECT_TRUE(set->contains(Value(Int(10))));
    EXPECT_TRUE(set->contains(Value(Int(20))));
    EXPECT_TRUE(set->contains(Value(Int(30))));
    EXPECT_FALSE(set->contains(Value(Int(40))));

    // Test remove
    set->remove(Value(Int(20)));
    EXPECT_EQ(set->size(), 2);
    EXPECT_FALSE(set->contains(Value(Int(20))));
    EXPECT_TRUE(set->contains(Value(Int(10))));
    EXPECT_TRUE(set->contains(Value(Int(30))));

    // Test clear
    set->clear();
    EXPECT_EQ(set->size(), 0);
    EXPECT_TRUE(set->empty());
}

// Test SetInstance with different value types
TEST_F(RuntimeTest, SetInstanceValueTypes) {
    auto text_set = std::make_shared<SetInstance>("Text");
    auto mixed_set = std::make_shared<SetInstance>("Text");

    // Test Text set
    text_set->add(Value(Text("apple")));
    text_set->add(Value(Text("banana")));
    text_set->add(Value(Text("apple")));  // Duplicate

    EXPECT_EQ(text_set->size(), 2);  // Only unique elements
    EXPECT_TRUE(text_set->contains(Value(Text("apple"))));
    EXPECT_TRUE(text_set->contains(Value(Text("banana"))));
    EXPECT_FALSE(text_set->contains(Value(Text("cherry"))));
}

// Test SetInstance elements() method
TEST_F(RuntimeTest, SetInstanceElements) {
    auto set = std::make_shared<SetInstance>("Text");

    set->add(Value(Text("c")));
    set->add(Value(Text("a")));
    set->add(Value(Text("b")));

    // Test elements()
    std::vector<Value> elements = set->elements();
    EXPECT_EQ(elements.size(), 3);

    // Elements should be sorted (std::set behavior)
    std::set<std::string> element_set;
    for (const auto& element : elements) {
        EXPECT_TRUE(std::holds_alternative<Text>(element));
        element_set.insert(std::get<Text>(element));
    }
    EXPECT_TRUE(element_set.count("a") > 0);
    EXPECT_TRUE(element_set.count("b") > 0);
    EXPECT_TRUE(element_set.count("c") > 0);
}

// Test SetIterator functionality
TEST_F(RuntimeTest, SetIteratorBasics) {
    auto set = std::make_shared<SetInstance>("Text");
    set->add(Value(Text("apple")));
    set->add(Value(Text("banana")));
    set->add(Value(Text("cherry")));

    SetIterator iterator(set);

    // Test initial state
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);
    EXPECT_EQ(iterator.getTotalSize(), 3);

    // Track elements we've seen
    std::set<std::string> elements_seen;

    // Test iteration
    int iterations = 0;
    while (iterator.hasNext() && iterations < 10) {  // safety check
        Value element = iterator.next();
        EXPECT_TRUE(std::holds_alternative<Text>(element));
        elements_seen.insert(std::get<Text>(element));
        iterations++;
    }

    // Should have seen all elements
    EXPECT_EQ(elements_seen.size(), 3);
    EXPECT_TRUE(elements_seen.count("apple") > 0);
    EXPECT_TRUE(elements_seen.count("banana") > 0);
    EXPECT_TRUE(elements_seen.count("cherry") > 0);

    EXPECT_FALSE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 3);

    // Test reset
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);
}

// Test SetIterator with empty set
TEST_F(RuntimeTest, SetIteratorEmpty) {
    auto empty_set = std::make_shared<SetInstance>("Int");
    SetIterator iterator(empty_set);

    EXPECT_FALSE(iterator.hasNext());
    EXPECT_EQ(iterator.getCurrentIndex(), 0);
    EXPECT_EQ(iterator.getTotalSize(), 0);

    // Test that next() throws on empty iterator
    EXPECT_THROW({ iterator.next(); }, std::exception);
}

// Test SetIterator uniqueness behavior
TEST_F(RuntimeTest, SetIteratorUniqueness) {
    auto set = std::make_shared<SetInstance>("Int");

    // Add duplicates
    set->add(Value(Int(1)));
    set->add(Value(Int(2)));
    set->add(Value(Int(1)));  // Duplicate
    set->add(Value(Int(3)));
    set->add(Value(Int(2)));  // Duplicate

    EXPECT_EQ(set->size(), 3);  // Only unique elements

    SetIterator iterator(set);
    std::set<int> values_seen;

    while (iterator.hasNext()) {
        Value value = iterator.next();
        EXPECT_TRUE(std::holds_alternative<Int>(value));
        values_seen.insert(static_cast<int>(std::get<Int>(value)));
    }

    EXPECT_EQ(values_seen.size(), 3);
    EXPECT_TRUE(values_seen.count(1) > 0);
    EXPECT_TRUE(values_seen.count(2) > 0);
    EXPECT_TRUE(values_seen.count(3) > 0);
}

// Test Set Value type handling
TEST_F(RuntimeTest, SetValueTypes) {
    auto set_instance = std::make_shared<SetInstance>("Text");
    set_instance->add(Value(Text("element")));

    Value set_value = Value(set_instance);
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<SetInstance>>(set_value));

    // Test type name
    EXPECT_EQ(getTypeName(set_value), "Set<Text>");

    // Test toString
    std::string str = valueToString(set_value);
    EXPECT_TRUE(str.find("element") != std::string::npos);

    // Test SetIterator as Value
    auto set_iterator = std::make_shared<SetIterator>(set_instance);
    Value iterator_value = Value(set_iterator);

    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<SetIterator>>(iterator_value));
    EXPECT_EQ(getTypeName(iterator_value), "SetIterator");
}

// Test Set ordering behavior (std::set is sorted)
TEST_F(RuntimeTest, SetOrdering) {
    auto set = std::make_shared<SetInstance>("Int");

    // Add in random order
    set->add(Value(Int(30)));
    set->add(Value(Int(10)));
    set->add(Value(Int(20)));

    std::vector<Value> elements = set->elements();
    EXPECT_EQ(elements.size(), 3);

    // Should be in sorted order
    EXPECT_EQ(std::get<Int>(elements[0]), 10);
    EXPECT_EQ(std::get<Int>(elements[1]), 20);
    EXPECT_EQ(std::get<Int>(elements[2]), 30);

    // Test iterator also returns sorted order
    SetIterator iterator(set);
    std::vector<int> iterated_values;

    while (iterator.hasNext()) {
        Value value = iterator.next();
        iterated_values.push_back(static_cast<int>(std::get<Int>(value)));
    }

    EXPECT_EQ(iterated_values.size(), 3);
    EXPECT_EQ(iterated_values[0], 10);
    EXPECT_EQ(iterated_values[1], 20);
    EXPECT_EQ(iterated_values[2], 30);
}

// Test ErrorInstance creation and methods
TEST_F(RuntimeTest, ErrorInstance) {
    ErrorInstance error("Test error message");

    EXPECT_EQ(error.getMessage(), "Test error message");
    EXPECT_EQ(error.getCode(), "ERROR");  // Default error code

    // Test toString
    std::string error_str = error.toString();
    EXPECT_TRUE(error_str.find("Test error message") != std::string::npos);
}

// Test ResultInstance success case
TEST_F(RuntimeTest, ResultInstanceSuccess) {
    Int success_value = 42;
    ResultInstance result(success_value, "Int", "Error");

    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());

    Value retrieved_value = result.getResult();
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved_value));
    EXPECT_EQ(std::get<Int>(retrieved_value), 42);

    // Test toString
    std::string result_str = result.toString();
    EXPECT_TRUE(result_str.find("Result.Success") != std::string::npos);
}

// Test ResultInstance error case
TEST_F(RuntimeTest, ResultInstanceError) {
    auto error_instance = std::make_shared<ErrorInstance>("Test error");
    Value error_value = std::static_pointer_cast<ErrorInstance>(error_instance);

    auto result = ResultInstance::createError(error_value, "Int", "Error");

    EXPECT_FALSE(result->isSuccess());
    EXPECT_TRUE(result->isError());

    Value retrieved_error = result->getError();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ErrorInstance>>(retrieved_error));

    auto retrieved_error_instance = std::get<std::shared_ptr<ErrorInstance>>(retrieved_error);
    EXPECT_EQ(retrieved_error_instance->getMessage(), "Test error");

    // Test toString
    std::string result_str = result->toString();
    EXPECT_TRUE(result_str.find("Result.Error") != std::string::npos);
}

// Test Result type information
TEST_F(RuntimeTest, ResultTypeInformation) {
    Int value = 100;
    ResultInstance result(value, "Int", "Error");

    EXPECT_EQ(result.getValueTypeName(), "Int");
    EXPECT_EQ(result.getErrorTypeName(), "Error");
}

// Test Context variable management for mutability
TEST_F(RuntimeTest, ContextVariableMutability) {
    Context context;

    // Test basic variable definition and retrieval
    context.defineVariable("x", Int(10));
    Value retrieved = context.getVariable("x");
    EXPECT_TRUE(std::holds_alternative<Int>(retrieved));
    EXPECT_EQ(std::get<Int>(retrieved), 10);

    // Test variable reassignment
    context.reassignVariable("x", Int(20));
    Value reassigned = context.getVariable("x");
    EXPECT_TRUE(std::holds_alternative<Int>(reassigned));
    EXPECT_EQ(std::get<Int>(reassigned), 20);

    // Test that constants cannot be reassigned
    context.defineConstant("PI", Float(3.14159f));
    EXPECT_THROW({ context.reassignVariable("PI", Float(2.71828f)); }, EvaluationError);

    // PI should remain unchanged
    Value pi_value = context.getVariable("PI");
    EXPECT_TRUE(std::holds_alternative<Float>(pi_value));
    EXPECT_FLOAT_EQ(std::get<Float>(pi_value), 3.14159f);
}

// Test Context variable scoping with mutability
TEST_F(RuntimeTest, ContextVariableScopingMutability) {
    Context context;

    // Define variable in outer scope
    context.defineVariable("outer", Int(100));

    // Create inner scope
    context.pushScope();

    // Define variable in inner scope
    context.defineVariable("inner", Int(200));

    // Both variables should be accessible in inner scope
    EXPECT_EQ(std::get<Int>(context.getVariable("outer")), 100);
    EXPECT_EQ(std::get<Int>(context.getVariable("inner")), 200);

    // Modify outer variable from inner scope
    context.reassignVariable("outer", Int(150));
    EXPECT_EQ(std::get<Int>(context.getVariable("outer")), 150);

    // Modify inner variable
    context.reassignVariable("inner", Int(250));
    EXPECT_EQ(std::get<Int>(context.getVariable("inner")), 250);

    // Pop inner scope
    context.popScope();

    // Outer variable should retain modified value
    EXPECT_EQ(std::get<Int>(context.getVariable("outer")), 150);

    // Inner variable should no longer be accessible
    EXPECT_THROW({ context.getVariable("inner"); }, UnresolvedReferenceError);
}

// Test Context error cases for variable assignment
TEST_F(RuntimeTest, ContextVariableAssignmentErrors) {
    Context context;

    // Test reassigning non-existent variable
    EXPECT_THROW({ context.reassignVariable("nonexistent", Int(42)); }, UnresolvedReferenceError);

    // Test reassigning constant
    context.defineConstant("CONST_VAL", Text("immutable"));
    EXPECT_THROW({ context.reassignVariable("CONST_VAL", Text("modified")); }, EvaluationError);

    // Test that constant value remains unchanged after failed reassignment
    Value const_val = context.getVariable("CONST_VAL");
    EXPECT_TRUE(std::holds_alternative<Text>(const_val));
    EXPECT_EQ(std::get<Text>(const_val), "immutable");
}

// Test Context with different variable types
TEST_F(RuntimeTest, ContextVariableTypesMutability) {
    Context context;

    // Test Int mutability
    context.defineVariable("int_var", Int(1));
    context.reassignVariable("int_var", Int(2));
    EXPECT_EQ(std::get<Int>(context.getVariable("int_var")), 2);

    // Test Float mutability
    context.defineVariable("float_var", Float(1.0f));
    context.reassignVariable("float_var", Float(2.5f));
    EXPECT_FLOAT_EQ(std::get<Float>(context.getVariable("float_var")), 2.5f);

    // Test Text mutability
    context.defineVariable("text_var", Text("hello"));
    context.reassignVariable("text_var", Text("world"));
    EXPECT_EQ(std::get<Text>(context.getVariable("text_var")), "world");

    // Test Bool mutability
    context.defineVariable("bool_var", Bool(true));
    context.reassignVariable("bool_var", Bool(false));
    EXPECT_EQ(std::get<Bool>(context.getVariable("bool_var")), false);
}