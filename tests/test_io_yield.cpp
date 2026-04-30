#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <variant>

#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/CoroutineHandle.hpp"
#include "../src/Runtime/ModuleLoader.hpp"
#include "../src/Runtime/Scheduler.hpp"

using namespace o2l;

#include <gtest/gtest.h>

TEST(ConcurrencyTest, io_yield) {
    o2l::Scheduler::instance().reset();
    std::cout << "Testing Concurrency Yield Points (sleep)..." << std::endl;

    // We need to register native methods if we want to test sleep()
    // but sleep() is also a built-in recognized in FunctionCallNode::evaluate()

    std::string source =
        "spawn { "
        "  sleep(100); "
        "  var x: Int = 42; "
        "}";

    Lexer lexer(source);
    auto tokens = lexer.tokenizeAll();
    Parser parser(tokens);
    auto nodes = parser.parse();

    auto& sched = Scheduler::instance();
    sched.reset();

    Context context;
    // We need to make sure ModuleLoader is initialized if we use it,
    // but here we are using built-in sleep.

    std::cout << "  Spawning coroutine..." << std::endl;
    nodes[0]->evaluate(context);

    auto start = std::chrono::steady_clock::now();
    std::cout << "  Running scheduler..." << std::endl;
    sched.run();
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  Scheduler ran for " << duration << "ms." << std::endl;

    assert(duration >= 100);
    std::cout << "  [âœ“] Scheduler respected sleep() duration." << std::endl;

    std::cout << "Concurrency yield point tests passed!" << std::endl;
    return;
}


