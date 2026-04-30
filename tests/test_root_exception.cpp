#include <iostream>
#include <memory>
#include <stdexcept>

#include "../src/AST/LiteralNode.hpp"
#include "../src/AST/Node.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Scheduler.hpp"

#include <gtest/gtest.h>

using namespace o2l;

namespace {

class SuccessNode : public ASTNode {
   public:
    Value evaluate(Context& context) override {
        (void)context;
        return Value(static_cast<Int>(42));
    }
    std::string toString() const override { return "success"; }
};

class ErrorNode : public ASTNode {
   public:
    Value evaluate(Context& context) override {
        (void)context;
        throw std::runtime_error("coroutine error");
    }
    std::string toString() const override { return "error"; }
};

}  // anonymous namespace

// AC#1: root coroutine error is captured in root_exception_
TEST(ConcurrencyTest, root_exception_captured) {
    auto& sched = Scheduler::instance();
    sched.reset();

    auto error_node = std::make_unique<ErrorNode>();
    sched.spawn(error_node.get(), Context());  // id=0 (root)
    sched.run();

    ASSERT_NE(sched.getRootException(), nullptr);
    EXPECT_THROW(std::rethrow_exception(sched.getRootException()), std::runtime_error);
}

// AC#2: non-root coroutine error does NOT set root_exception_
TEST(ConcurrencyTest, non_root_exception_ignored) {
    auto& sched = Scheduler::instance();
    sched.reset();

    auto success_node = std::make_unique<SuccessNode>();
    auto error_node = std::make_unique<ErrorNode>();

    sched.spawn(success_node.get(), Context());  // id=0 (root) — succeeds
    sched.spawn(error_node.get(), Context());     // id=1 — fails

    sched.run();

    EXPECT_EQ(sched.getRootException(), nullptr);
    EXPECT_EQ(std::get<Int>(sched.getRootResult()), 42);
}

// AC#3: root_exception_ is null on successful completion
TEST(ConcurrencyTest, root_exception_null_on_success) {
    auto& sched = Scheduler::instance();
    sched.reset();

    auto success_node = std::make_unique<SuccessNode>();
    sched.spawn(success_node.get(), Context());  // id=0 (root)
    sched.run();

    EXPECT_EQ(sched.getRootException(), nullptr);
    EXPECT_EQ(std::get<Int>(sched.getRootResult()), 42);
}
