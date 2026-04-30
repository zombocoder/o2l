#include <cassert>
#include <iostream>
#include <memory>
#include <variant>

#include "../src/AST/BinaryOpNode.hpp"
#include "../src/AST/BlockNode.hpp"
#include "../src/AST/ComparisonNode.hpp"
#include "../src/AST/IdentifierNode.hpp"
#include "../src/AST/LiteralNode.hpp"
#include "../src/AST/Node.hpp"
#include "../src/AST/VariableAssignmentNode.hpp"
#include "../src/AST/VariableDeclarationNode.hpp"
#include "../src/AST/WhileStatementNode.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Coroutine.hpp"
#include "../src/Runtime/Scheduler.hpp"

#include <gtest/gtest.h>

using namespace o2l;

namespace {

// Global counter for yields
static int g_yield_count = 0;

class YieldNode : public ASTNode {
   public:
    Value evaluate(Context& context) override {
        (void)context;
        auto& sched = Scheduler::instance();
        if (sched.hasResumeValue()) {
            return sched.consumeResumeValue();
        }
        ++g_yield_count;
        sched.yield();
        return Value(static_cast<Int>(0));
    }
    std::string toString() const override {
        return "yield";
    }
};

}  // anonymous namespace

TEST(ConcurrencyTest, resume_logic) {
    o2l::Scheduler::instance().reset();
    std::cout << "Testing statement-level resumption..." << std::endl;

    auto& sched = Scheduler::instance();
    Context context;

    /*
     * var i: Int = 0;
     * while (i < 2) {
     *   yield;
     *   i = i + 1;
     * }
     */

    SourceLocation loc;
    auto var_decl = std::make_unique<VariableDeclarationNode>(
        "i", "Int", std::make_unique<LiteralNode>(Int(0)));

    auto condition = std::make_unique<ComparisonNode>(std::make_unique<IdentifierNode>("i"),
                                                      ComparisonOperator::LESS_THAN,
                                                      std::make_unique<LiteralNode>(Int(2)), loc);

    auto yield_stmt = std::make_unique<YieldNode>();
    auto increment = std::make_unique<VariableAssignmentNode>(
        "i",
        std::make_unique<BinaryOpNode>(std::make_unique<IdentifierNode>("i"), BinaryOperator::PLUS,
                                       std::make_unique<LiteralNode>(Int(1)), loc));

    std::vector<ASTNodePtr> body_stmts;
    body_stmts.push_back(std::move(yield_stmt));
    body_stmts.push_back(std::move(increment));
    auto body = std::make_unique<BlockNode>(std::move(body_stmts));

    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    // 1. Initial declaration
    var_decl->evaluate(context);

    // 2. Spawn coroutine
    sched.spawn(while_stmt.get(), std::move(context));

    // 3. Run
    sched.run();

    // EXPECTATION:
    // Iteration 0: Hits yield (stmt 0 of block), suspends. g_yield_count = 1, i = 0.
    // Iteration 0 resumed: Resumes at stmt 0, completes it, executes stmt 1 (i=1).
    // Iteration 1: Hits condition (1 < 2), enters body, hits yield (stmt 0), suspends.
    // g_yield_count = 2, i = 1. Iteration 1 resumed: Resumes at stmt 0, completes it, executes stmt
    // 1 (i=2). Iteration 2: Condition (2 < 2) is false, loop terminates.

    std::cout << "  Yield count: " << g_yield_count << std::endl;
    EXPECT_EQ(g_yield_count, 2);

    std::cout << "Resumption tests passed!" << std::endl;
}
