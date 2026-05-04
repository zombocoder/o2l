#include "../src/Runtime/Scheduler.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <variant>

#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/ObjectInstance.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

#include <gtest/gtest.h>

TEST(ConcurrencyTest, context_clone) {
    o2l::Scheduler::instance().reset();
    std::cout << "Testing Context::clone()..." << std::endl;

    Context original;
    original.defineVariable("x", Int(10));
    original.defineVariable("msg", Text("hello"));

    auto obj = std::make_shared<ObjectInstance>("TestObj");
    original.defineConstant("obj", obj);

    // Clone the context
    Context cloned = original.clone();

    // Verify values are preserved
    assert(std::get<Int>(cloned.getVariable("x")) == 10);
    assert(std::get<Text>(cloned.getVariable("msg")) == "hello");
    assert(std::get<std::shared_ptr<ObjectInstance>>(cloned.getVariable("obj")) == obj);

    // Verify isolation of variables
    cloned.reassignVariable("x", Int(20));
    assert(std::get<Int>(cloned.getVariable("x")) == 20);
    assert(std::get<Int>(original.getVariable("x")) == 10);

    // Verify sharing of objects
    auto obj_in_cloned = std::get<std::shared_ptr<ObjectInstance>>(cloned.getVariable("obj"));
    assert(obj_in_cloned == obj);

    // Verify isolation of scope stack
    cloned.pushScope();
    cloned.defineVariable("y", Int(30));
    assert(cloned.hasVariable("y"));
    assert(!original.hasVariable("y"));
    cloned.popScope();
    assert(!cloned.hasVariable("y"));

    std::cout << "Context::clone() test passed!" << std::endl;
    return;
}



