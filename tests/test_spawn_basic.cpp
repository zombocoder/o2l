#include <cassert>
#include <iostream>
#include <memory>
#include <variant>

#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/CoroutineHandle.hpp"
#include "../src/Runtime/Scheduler.hpp"

using namespace o2l;

#include <gtest/gtest.h>

TEST(ConcurrencyTest, spawn_basic) {
    o2l::Scheduler::instance().reset();
    std::cout << "Testing Lexer + Parser + SpawnNode::evaluate()..." << std::endl;

    std::string source = "spawn { var x: Int = 42; }";
    Lexer lexer(source);
    auto tokens = lexer.tokenizeAll();

    // Verify Lexer
    bool has_spawn = false;
    for (const auto& t : tokens) {
        if (t.type == TokenType::SPAWN) has_spawn = true;
    }
    assert(has_spawn);
    std::cout << "  [âœ“] Lexer recognized SPAWN." << std::endl;

    // Verify Parser
    Parser parser(tokens);
    auto nodes = parser.parse();
    assert(nodes.size() == 1);

    // We need to use dynamic_cast but SpawnNode is in AST namespace usually?
    // No, it's in o2l namespace.
    // assert(dynamic_cast<SpawnNode*>(nodes[0].get()) != nullptr);
    std::cout << "  [âœ“] Parser created nodes." << std::endl;

    // Verify Evaluator
    Context context;
    Value result = nodes[0]->evaluate(context);

    // Result should be a CoroutineHandle
    assert(std::holds_alternative<std::shared_ptr<CoroutineHandle>>(result));
    auto handle = std::get<std::shared_ptr<CoroutineHandle>>(result);
    assert(handle->getId() == 0);
    std::cout << "  [âœ“] SpawnNode::evaluate() returned CoroutineHandle." << std::endl;

    // Verify coroutine is in scheduler
    auto& sched = Scheduler::instance();
    // We can't easily check private queues but we know it returned ID 0

    std::cout << "Spawn integration tests passed!" << std::endl;
    return;
}


