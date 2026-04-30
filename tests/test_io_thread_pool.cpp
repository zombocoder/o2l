#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "../src/AST/Node.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Scheduler.hpp"

#include <gtest/gtest.h>

using namespace o2l;

namespace {

// A node that suspends for IO, returning the IO result on resume
class IONode : public ASTNode {
   public:
    std::function<Value()> work;
    Value* result_out;

    IONode(std::function<Value()> w, Value* out)
        : work(std::move(w)), result_out(out) {}

    Value evaluate(Context& context) override {
        (void)context;
        auto& sched = Scheduler::instance();
        if (sched.hasResumeValue()) {
            Value v = sched.consumeResumeValue();
            if (result_out) *result_out = v;
            return v;
        }
        sched.suspendForIO(work);
        return Value(static_cast<Int>(0));  // unreachable
    }
    std::string toString() const override { return "io_node"; }
};

}  // anonymous namespace

// AC#1: suspendForIO delivers lambda result to coroutine on resume
TEST(ConcurrencyTest, suspend_for_io_basic) {
    auto& sched = Scheduler::instance();
    sched.reset();

    Value result;
    auto node = std::make_unique<IONode>(
        []() -> Value { return Value(static_cast<Int>(42)); },
        &result);

    sched.spawn(node.get(), Context());
    sched.run();

    EXPECT_EQ(sched.getRootException(), nullptr);
    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 42);
}

// AC#2: IO work runs on a different thread
TEST(ConcurrencyTest, suspend_for_io_different_thread) {
    auto& sched = Scheduler::instance();
    sched.reset();

    auto main_tid = std::this_thread::get_id();
    std::thread::id io_tid;

    Value result;
    auto node = std::make_unique<IONode>(
        [&io_tid]() -> Value {
            io_tid = std::this_thread::get_id();
            return Value(static_cast<Int>(1));
        },
        &result);

    sched.spawn(node.get(), Context());
    sched.run();

    EXPECT_EQ(sched.getRootException(), nullptr);
    EXPECT_NE(io_tid, std::thread::id{});  // was actually set
    EXPECT_NE(io_tid, main_tid);           // ran on a different thread
}

// AC#3: multiple concurrent IO suspensions all complete
TEST(ConcurrencyTest, suspend_for_io_multiple) {
    auto& sched = Scheduler::instance();
    sched.reset();

    Value r1, r2, r3;
    auto n1 = std::make_unique<IONode>(
        []() -> Value { return Value(static_cast<Int>(10)); }, &r1);
    auto n2 = std::make_unique<IONode>(
        []() -> Value { return Value(static_cast<Int>(20)); }, &r2);
    auto n3 = std::make_unique<IONode>(
        []() -> Value { return Value(static_cast<Int>(30)); }, &r3);

    sched.spawn(n1.get(), Context());
    sched.spawn(n2.get(), Context());
    sched.spawn(n3.get(), Context());
    sched.run();

    EXPECT_EQ(sched.getRootException(), nullptr);
    ASSERT_TRUE(std::holds_alternative<Int>(r1));
    ASSERT_TRUE(std::holds_alternative<Int>(r2));
    ASSERT_TRUE(std::holds_alternative<Int>(r3));
    EXPECT_EQ(std::get<Int>(r1), 10);
    EXPECT_EQ(std::get<Int>(r2), 20);
    EXPECT_EQ(std::get<Int>(r3), 30);
}
