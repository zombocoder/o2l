#include <iostream>
#include <memory>

#include "../src/Interpreter.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/Scheduler.hpp"

#include <gtest/gtest.h>

using namespace o2l;

namespace {

Value runScript(const std::string& source) {
    Lexer lexer(source);
    auto nodes = Parser(lexer.tokenizeAll()).parse();
    Interpreter interpreter;
    return interpreter.execute(nodes);
}

}  // anonymous namespace

// AC#1: multiple producers sending to one channel
TEST(ConcurrencyTest, channel_multiple_producers) {
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new(4)
                spawn { ch.send(10); ch.send(20) }
                spawn { ch.send(30); ch.send(40) }
                v1: Int = ch.receive()
                v2: Int = ch.receive()
                v3: Int = ch.receive()
                v4: Int = ch.receive()
                return v1 + v2 + v3 + v4
            }
        }
    )";

    Value result = runScript(source);
    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 100);
}

// AC#2: multiple consumers receiving from one producer
TEST(ConcurrencyTest, channel_multiple_consumers) {
    std::string source = R"(
        Object Main {
            method main(): Int {
                src: Channel = Channel.new(4)
                results: Channel = Channel.new(2)

                src.send(10)
                src.send(20)
                src.send(30)
                src.send(40)

                spawn { results.send(src.receive() + src.receive()) }
                spawn { results.send(src.receive() + src.receive()) }

                r1: Int = results.receive()
                r2: Int = results.receive()
                return r1 + r2
            }
        }
    )";

    Value result = runScript(source);
    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Each consumer takes 2 values; total is always 10+20+30+40=100
    EXPECT_EQ(std::get<Int>(result), 100);
}

// AC#3: unbuffered channel send blocks until receiver is ready
TEST(ConcurrencyTest, channel_unbuffered_blocking) {
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new()
                spawn {
                    ch.send(99)
                }
                val: Int = ch.receive()
                return val
            }
        }
    )";

    Value result = runScript(source);
    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 99);
}

// AC#4: buffered channel at capacity, sender blocks until space available
TEST(ConcurrencyTest, channel_buffered_full) {
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new(2)
                ch.send(1)
                ch.send(2)
                spawn {
                    ch.send(3)
                }
                v1: Int = ch.receive()
                v2: Int = ch.receive()
                v3: Int = ch.receive()
                return v1 + v2 + v3
            }
        }
    )";

    Value result = runScript(source);
    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);
}
