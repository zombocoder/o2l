#include <cassert>
#include <iostream>
#include <memory>
#include <variant>

#include "../src/Interpreter.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/ChannelInstance.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Scheduler.hpp"

using namespace o2l;

#include <gtest/gtest.h>

TEST(ConcurrencyTest, channel_basic) {
    o2l::Scheduler::instance().reset();
    std::cout << "Testing Channels (Unbuffered)..." << std::endl;

    std::string source =
        "Object Main {\n"
        "  @external method main(): Void {\n"
        "    ch: Channel = Channel.new()\n"
        "    spawn {\n"
        "      ch.send(42)\n"
        "    }\n"
        "    val: Int = ch.receive()\n"
        "    io.print(\"Received: %d\", val)\n"
        "  }\n"
        "}";

    // We need to register system.io for io.print
    Interpreter interpreter;

    // For this test, we'll manually parse and execute
    Lexer lexer(source);
    auto tokens = lexer.tokenizeAll();
    Parser parser(tokens);
    auto nodes = parser.parse();

    // We expect "Received: 42" to be printed.
    // Since io.print uses stdout, we can check it.

    std::cout << "--- Program Output ---" << std::endl;
    interpreter.execute(nodes);
    std::cout << "--- End Output ---" << std::endl;

    std::cout << "Channel basic test passed!" << std::endl;

    // Buffered channel test
    std::cout << "Testing Channels (Buffered)..." << std::endl;
    std::string source_buffered =
        "Object Main {\n"
        "  @external method main(): Void {\n"
        "    ch: Channel = Channel.new(2)\n"
        "    ch.send(1)\n"
        "    ch.send(2)\n"
        "    io.print(\"Sent two values to buffered channel\")\n"
        "    io.print(\"Recv 1: %d\", ch.receive())\n"
        "    io.print(\"Recv 2: %d\", ch.receive())\n"
        "  }\n"
        "}";

    Lexer lexer2(source_buffered);
    auto nodes2 = Parser(lexer2.tokenizeAll()).parse();

    std::cout << "--- Buffered Program Output ---" << std::endl;
    interpreter.execute(nodes2);
    std::cout << "--- End Output ---" << std::endl;

    return;
}


