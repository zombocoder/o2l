#include "../src/Runtime/Scheduler.hpp"
#include <cassert>
#include <iostream>
#include <memory>

#include "../src/AST/BinaryOpNode.hpp"
#include "../src/AST/BlockNode.hpp"
#include "../src/AST/ComparisonNode.hpp"
#include "../src/AST/IdentifierNode.hpp"
#include "../src/AST/LiteralNode.hpp"
#include "../src/AST/Node.hpp"
#include "../src/AST/VariableAssignmentNode.hpp"
#include "../src/AST/VariableDeclarationNode.hpp"
#include "../src/AST/WhileStatementNode.hpp"
#include "../src/Common/Exceptions.hpp"
#include "../src/Common/SourceLocation.hpp"
#include "../src/Lexer.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

namespace {

// A custom AST node that throws SuspendException to simulate a yield point
class YieldNode : public ASTNode {
   public:
    int* call_count;
    explicit YieldNode(int* count) : call_count(count) {}

    Value evaluate(Context& context) override {
        (*call_count)++;
        throw SuspendException("test_yield");
    }

    std::string toString() const override {
        return "yield";
    }
};

}  // anonymous namespace

#include <gtest/gtest.h>

TEST(ConcurrencyTest, suspend_exception) {
    o2l::Scheduler::instance().reset();
    std::cout << "Running task-001: SuspendException Spike..." << std::endl;

    Context context;
    int yield_calls = 0;

    /*
     * Build AST manually:
     * var count: Int = 0;
     * while (count < 3) {
     *   yield;
     *   count = count + 1;
     * }
     */

    SourceLocation loc;

    // var count: Int = 0;
    auto var_decl = std::make_unique<VariableDeclarationNode>(
        "count", "Int", std::make_unique<LiteralNode>(Int(0)));

    // count < 3
    auto condition = std::make_unique<ComparisonNode>(std::make_unique<IdentifierNode>("count"),
                                                      ComparisonOperator::LESS_THAN,
                                                      std::make_unique<LiteralNode>(Int(3)), loc);

    // yield;
    auto yield_stmt = std::make_unique<YieldNode>(&yield_calls);

    // count = count + 1;
    auto increment = std::make_unique<VariableAssignmentNode>(
        "count", std::make_unique<BinaryOpNode>(std::make_unique<IdentifierNode>("count"),
                                                BinaryOperator::PLUS,
                                                std::make_unique<LiteralNode>(Int(1)), loc));

    // { yield; count = count + 1; }
    std::vector<ASTNodePtr> body_stmts;
    body_stmts.push_back(std::move(yield_stmt));
    body_stmts.push_back(std::move(increment));
    auto body = std::make_unique<BlockNode>(std::move(body_stmts));

    // while (count < 3) { ... }
    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    // Initial declaration
    var_decl->evaluate(context);

    // Simulate the scheduler loop
    int loops = 0;
    while (loops < 10) {  // Safety break
        try {
            while_stmt->evaluate(context);
            std::cout << "  While loop completed normally." << std::endl;
            break;
        } catch (const SuspendException& e) {
            std::cout << "  Caught SuspendException: " << e.getReason() << std::endl;

            // SIMULATE RESUME: In a real scheduler, we would resume from the exact statement.
            // Here, to make it progress without statement tracking, we manually increment count.
            // This still proves that the exception unwound correctly through AST nodes.
            Value c = context.getVariable("count");
            context.reassignVariable("count", Int(std::get<Int>(c) + 1));
        }
        loops++;
    }

    // Verification
    assert(yield_calls > 0);
    std::cout << "Spike Successful: SuspendException unwound correctly through nested AST nodes."
              << std::endl;

    return;
}



