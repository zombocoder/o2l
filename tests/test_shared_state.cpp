#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../src/AST/Node.hpp"
#include "../src/Interpreter.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Scheduler.hpp"

#include <gtest/gtest.h>

using namespace o2l;

namespace {

// A coroutine node that yields N times, writing a shared counter each time
class CounterNode : public ASTNode {
   public:
    std::string name;
    int yields;
    std::vector<std::string>* log;

    CounterNode(std::string n, int y, std::vector<std::string>* l)
        : name(std::move(n)), yields(y), log(l) {}

    Value evaluate(Context& context) override {
        (void)context;
        auto& sched = Scheduler::instance();
        if (sched.hasResumeValue()) {
            sched.consumeResumeValue();
        }
        while (yields > 0) {
            log->push_back(name + "_" + std::to_string(yields));
            yields--;
            sched.yield();
        }
        log->push_back(name + "_done");
        return Value(static_cast<Int>(0));
    }
    std::string toString() const override { return name; }
};

}  // anonymous namespace

// AC#1: two coroutines communicate via channel, verify correct values
TEST(ConcurrencyTest, channel_producer_consumer_values) {
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new(3)
                spawn {
                    ch.send(10)
                    ch.send(20)
                    ch.send(30)
                }
                v1: Int = ch.receive()
                v2: Int = ch.receive()
                v3: Int = ch.receive()
                return v1 + v2 + v3
            }
        }
    )";

    Interpreter interpreter;
    Lexer lexer(source);
    auto nodes = Parser(lexer.tokenizeAll()).parse();
    Value result = interpreter.execute(nodes);

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 60);
}

// AC#2: three coroutines yield in deterministic round-robin order with shared log
TEST(ConcurrencyTest, three_coroutine_round_robin) {
    auto& sched = Scheduler::instance();
    sched.reset();

    std::vector<std::string> log;
    auto a = std::make_unique<CounterNode>("A", 2, &log);
    auto b = std::make_unique<CounterNode>("B", 2, &log);
    auto c = std::make_unique<CounterNode>("C", 2, &log);

    sched.spawn(a.get(), Context());
    sched.spawn(b.get(), Context());
    sched.spawn(c.get(), Context());
    sched.run();

    // Round-robin: A yields, B yields, C yields, A yields, B yields, C yields,
    // then A done, B done, C done
    ASSERT_EQ(log.size(), 9u);
    EXPECT_EQ(log[0], "A_2");
    EXPECT_EQ(log[1], "B_2");
    EXPECT_EQ(log[2], "C_2");
    EXPECT_EQ(log[3], "A_1");
    EXPECT_EQ(log[4], "B_1");
    EXPECT_EQ(log[5], "C_1");
    EXPECT_EQ(log[6], "A_done");
    EXPECT_EQ(log[7], "B_done");
    EXPECT_EQ(log[8], "C_done");
}
