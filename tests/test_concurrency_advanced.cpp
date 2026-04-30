#include "../src/Runtime/Scheduler.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "../src/Interpreter.hpp"
#include "../src/Runtime/Value.hpp"
#include "../src/Common/Exceptions.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"

using namespace o2l;

Value runScript(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.tokenizeAll());
    auto ast = parser.parse();
    Interpreter interpreter;
    return interpreter.execute(ast);
}

void testNestedSpawnAndChannelCommunication() {
    std::cout << "Running testNestedSpawnAndChannelCommunication..." << std::endl;
    std::string source = R"(
        Object Worker {
            method innerWorker(id: Int, ch: Channel): Unit {
                ch.send(id * 10)
            }
            
            @external method outerWorker(ch: Channel): Unit {
                innerCh: Channel = Channel.new(2)
                spawn { this.innerWorker(1, innerCh) }
                spawn { this.innerWorker(2, innerCh) }
                
                v1: Int = innerCh.receive()
                v2: Int = innerCh.receive()
                
                ch.send(v1 + v2)
            }
        }
        
        Object Main {
            method main(): Int {
                results: Channel = Channel.new(1)
                w: Worker = new Worker()
                spawn { w.outerWorker(results) }
                return results.receive()
            }
        }
    )";

    Value result = runScript(source);
    assert(std::holds_alternative<Int>(result));
    assert(std::get<Int>(result) == 30);
    std::cout << "  Passed!" << std::endl;
}

void testClosedChannelSendThrows() {
    std::cout << "Running testClosedChannelSendThrows..." << std::endl;
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new(1)
                ch.close()
                try {
                    ch.send(42)
                    return 0
                } catch (e) {
                    return 1
                }
            }
        }
    )";

    Value result = runScript(source);
    assert(std::holds_alternative<Int>(result));
    assert(std::get<Int>(result) == 1);
    std::cout << "  Passed!" << std::endl;
}

void testClosedChannelReceiveEmptyThrows() {
    std::cout << "Running testClosedChannelReceiveEmptyThrows..." << std::endl;
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new(1)
                ch.close()
                try {
                    val: Int = ch.receive()
                    return 0
                } catch (e) {
                    return 1
                }
            }
        }
    )";

    Value result = runScript(source);
    assert(std::holds_alternative<Int>(result));
    assert(std::get<Int>(result) == 1);
    std::cout << "  Passed!" << std::endl;
}

void testClosedChannelReceiveBufferedSucceeds() {
    std::cout << "Running testClosedChannelReceiveBufferedSucceeds..." << std::endl;
    std::string source = R"(
        Object Main {
            method main(): Int {
                ch: Channel = Channel.new(2)
                ch.send(10)
                ch.send(20)
                ch.close()
                
                v1: Int = ch.receive()
                v2: Int = ch.receive()
                
                try {
                    v3: Int = ch.receive()
                    return -1
                } catch (e) {
                    return v1 + v2
                }
            }
        }
    )";

    Value result = runScript(source);
    assert(std::holds_alternative<Int>(result));
    assert(std::get<Int>(result) == 30);
    std::cout << "  Passed!" << std::endl;
}

#include <gtest/gtest.h>

TEST(ConcurrencyTest, concurrency_advanced) {
    o2l::Scheduler::instance().reset();
    std::cout << "--- Concurrency Advanced Tests ---" << std::endl;
    testNestedSpawnAndChannelCommunication();
    testClosedChannelSendThrows();
    testClosedChannelReceiveEmptyThrows();
    testClosedChannelReceiveBufferedSucceeds();
    std::cout << "All advanced concurrency tests passed!" << std::endl;
    return;
}



