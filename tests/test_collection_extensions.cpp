#include <gtest/gtest.h>
#include "Runtime/ListInstance.hpp"
#include "Runtime/MapInstance.hpp"
#include "Runtime/MapIterator.hpp"
#include "Runtime/SetInstance.hpp"
#include "Runtime/Value.hpp"
#include "Common/Exceptions.hpp"

using namespace o2l;

class CollectionExtensionsTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// --- List Extensions ---

TEST_F(CollectionExtensionsTest, ListSetAndCopy) {
    auto list = std::make_shared<ListInstance>("Int");
    list->add(Value(Int(10)));
    list->add(Value(Int(20)));
    
    Value old = list->set(0, Value(Int(15)));
    EXPECT_EQ(std::get<Int>(old), 10);
    EXPECT_EQ(std::get<Int>(list->get(0)), 15);
    
    auto copy = list->copy();
    EXPECT_EQ(copy->size(), 2);
    EXPECT_EQ(std::get<Int>(copy->get(0)), 15);
    
    copy->add(Value(Int(30)));
    EXPECT_EQ(list->size(), 2); // Original unchanged
    EXPECT_EQ(copy->size(), 3);
}

TEST_F(CollectionExtensionsTest, ListSort) {
    auto list = std::make_shared<ListInstance>("Int");
    list->add(Value(Int(30)));
    list->add(Value(Int(10)));
    list->add(Value(Int(20)));
    
    list->sort();
    EXPECT_EQ(std::get<Int>(list->get(0)), 10);
    EXPECT_EQ(std::get<Int>(list->get(1)), 20);
    EXPECT_EQ(std::get<Int>(list->get(2)), 30);
    
    list->sortDescending();
    EXPECT_EQ(std::get<Int>(list->get(0)), 30);
    EXPECT_EQ(std::get<Int>(list->get(1)), 20);
    EXPECT_EQ(std::get<Int>(list->get(2)), 10);
}

TEST_F(CollectionExtensionsTest, ListBulkOperations) {
    auto list1 = std::make_shared<ListInstance>("Int");
    list1->add(Value(Int(1)));
    list1->add(Value(Int(2)));
    
    auto list2 = std::make_shared<ListInstance>("Int");
    list2->add(Value(Int(3)));
    list2->add(Value(Int(4)));
    
    int new_size = list1->addAll(*list2);
    EXPECT_EQ(new_size, 4);
    EXPECT_EQ(list1->size(), 4);
    
    auto to_remove = std::make_shared<ListInstance>("Int");
    to_remove->add(Value(Int(2)));
    to_remove->add(Value(Int(3)));
    
    int removed = list1->removeAll(*to_remove);
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(list1->size(), 2);
    EXPECT_FALSE(list1->contains(Value(Int(2))));
    
    list1->add(Value(Int(5))); // [1, 4, 5]
    auto to_retain = std::make_shared<ListInstance>("Int");
    to_retain->add(Value(Int(1)));
    to_retain->add(Value(Int(5)));
    
    int retained = list1->retainAll(*to_retain);
    EXPECT_EQ(retained, 2);
    EXPECT_EQ(list1->size(), 2);
    EXPECT_TRUE(list1->contains(Value(Int(5))));
    EXPECT_FALSE(list1->contains(Value(Int(4))));
}

TEST_F(CollectionExtensionsTest, ListSliceAndSearch) {
    auto list = std::make_shared<ListInstance>("Int");
    for (int i = 0; i < 5; ++i) list->add(Value(Int(i))); // [0, 1, 2, 3, 4]
    list->add(Value(Int(2))); // [0, 1, 2, 3, 4, 2]
    
    EXPECT_EQ(list->lastIndexOf(Value(Int(2))), 5);
    EXPECT_TRUE(list->contains(Value(Int(4))));
    
    Value sliced = list->slice(1, 4);
    auto sliced_list = std::get<std::shared_ptr<ListInstance>>(sliced);
    EXPECT_EQ(sliced_list->size(), 3);
    EXPECT_EQ(std::get<Int>(sliced_list->get(0)), 1);
    EXPECT_EQ(std::get<Int>(sliced_list->get(2)), 3);
}

// --- Map Extensions ---

TEST_F(CollectionExtensionsTest, MapPutAndRemoveReturn) {
    auto map = std::make_shared<MapInstance>("Text", "Int");
    Value old = map->put(Value(Text("a")), Value(Int(1)));
    EXPECT_TRUE(std::holds_alternative<Int>(old));
    EXPECT_EQ(std::get<Int>(old), 0);
    
    old = map->put(Value(Text("a")), Value(Int(2)));
    EXPECT_EQ(std::get<Int>(old), 1);
    
    Value removed = map->remove(Value(Text("a")));
    EXPECT_EQ(std::get<Int>(removed), 2);
}

TEST_F(CollectionExtensionsTest, MapOrDefaultAndContainsValue) {
    auto map = std::make_shared<MapInstance>("Text", "Int");
    map->put(Value(Text("a")), Value(Int(1)));
    
    EXPECT_EQ(std::get<Int>(map->getOrDefault(Value(Text("a")), Value(Int(0)))), 1);
    EXPECT_EQ(std::get<Int>(map->getOrDefault(Value(Text("b")), Value(Int(0)))), 0);
    
    EXPECT_TRUE(map->containsValue(Value(Int(1))));
    EXPECT_FALSE(map->containsValue(Value(Int(2))));
}

TEST_F(CollectionExtensionsTest, MapMergeAndEntrySet) {
    auto map1 = std::make_shared<MapInstance>("Text", "Int");
    map1->put(Value(Text("a")), Value(Int(1)));
    
    auto map2 = std::make_shared<MapInstance>("Text", "Int");
    map2->put(Value(Text("b")), Value(Int(2)));
    
    map1->merge(*map2);
    EXPECT_EQ(map1->size(), 2);
    EXPECT_TRUE(map1->contains(Value(Text("b"))));
    
    auto entries = map1->entrySet();
    EXPECT_EQ(entries->size(), 2);
    // Since it returns Text placeholder "key:value"
    bool found = false;
    for (const auto& el : entries->elements()) {
        if (std::get<Text>(el) == "a:1") found = true;
    }
    EXPECT_TRUE(found);
}

// --- Set Extensions ---

TEST_F(CollectionExtensionsTest, SetOperations) {
    auto set1 = std::make_shared<SetInstance>("Int");
    set1->add(Value(Int(1)));
    set1->add(Value(Int(2)));
    set1->add(Value(Int(3)));
    
    auto set2 = std::make_shared<SetInstance>("Int");
    set2->add(Value(Int(3)));
    set2->add(Value(Int(4)));
    set2->add(Value(Int(5)));
    
    auto union_set = set1->setUnion(*set2);
    EXPECT_EQ(union_set->size(), 5);
    
    auto intersect_set = set1->setIntersection(*set2);
    EXPECT_EQ(intersect_set->size(), 1);
    EXPECT_TRUE(intersect_set->contains(Value(Int(3))));
    
    auto diff_set = set1->setDifference(*set2);
    EXPECT_EQ(diff_set->size(), 2);
    EXPECT_TRUE(diff_set->contains(Value(Int(1))));
    EXPECT_FALSE(diff_set->contains(Value(Int(3))));
    
    auto sym_diff = set1->setSymmetricDifference(*set2);
    EXPECT_EQ(sym_diff->size(), 4);
    EXPECT_FALSE(sym_diff->contains(Value(Int(3))));
}

TEST_F(CollectionExtensionsTest, SetRelationshipsAndBulk) {
    auto set1 = std::make_shared<SetInstance>("Int");
    set1->add(Value(Int(1)));
    set1->add(Value(Int(2)));
    
    auto set2 = std::make_shared<SetInstance>("Int");
    set2->add(Value(Int(1)));
    set2->add(Value(Int(2)));
    set2->add(Value(Int(3)));
    
    EXPECT_TRUE(set1->isSubsetOf(*set2));
    EXPECT_TRUE(set2->isSupersetOf(*set1));
    
    auto set3 = std::make_shared<SetInstance>("Int");
    set3->add(Value(Int(4)));
    EXPECT_TRUE(set1->isDisjointFrom(*set3));
    
    int added = set1->addAll(*set3);
    EXPECT_EQ(added, 1);
    EXPECT_EQ(set1->size(), 3);
    
    int removed = set1->removeAll(*set3);
    EXPECT_EQ(removed, 1);
    EXPECT_EQ(set1->size(), 2);
}

// --- MapIterator Fix ---

TEST_F(CollectionExtensionsTest, MapIteratorDoubleAdvanceFix) {
    auto map = std::make_shared<MapInstance>("Text", "Int");
    map->put(Value(Text("a")), Value(Int(1)));
    map->put(Value(Text("b")), Value(Int(2)));
    
    auto iter = std::make_shared<MapIterator>(map);
    int count = 0;
    while (iter->hasNext()) {
        Value k = iter->nextKey();
        Value v = iter->nextValue();
        count++;
    }
    EXPECT_EQ(count, 2);
    
    iter->reset();
    count = 0;
    while (iter->hasNext()) {
        iter->MapItem();
        count++;
    }
    EXPECT_EQ(count, 2);
}
