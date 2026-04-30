#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
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

// Shared counters reset per test
static int g_yield_count = 0;
static int g_post_yield_count = 0;

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

class ErrorNode : public ASTNode {
   public:
    Value evaluate(Context& context) override {
        (void)context;
        throw std::runtime_error("coroutine error");
    }
    std::string toString() const override { return "error"; }
};

// A node that yields on first call, then throws on resume
class YieldThenErrorNode : public ASTNode {
   public:
    Value evaluate(Context& context) override {
        (void)context;
        auto& sched = Scheduler::instance();
        if (sched.hasResumeValue()) {
            sched.consumeResumeValue();
            throw std::runtime_error("error after resume");
        }
        sched.yield();
        return Value(static_cast<Int>(0));
    }
    std::string toString() const override { return "yield_then_error"; }
};

// A condition node that succeeds N times then throws
class CountdownConditionNode : public ASTNode {
   public:
    int* remaining;
    explicit CountdownConditionNode(int* r) : remaining(r) {}
    Value evaluate(Context& context) override {
        (void)context;
        if (*remaining <= 0) {
            throw std::runtime_error("condition error");
        }
        (*remaining)--;
        return Value(Bool(true));
    }
    std::string toString() const override { return "countdown_cond"; }
};

// A node that records post-yield execution (proves resume happened before error)
class PostYieldCounterNode : public ASTNode {
   public:
    Value evaluate(Context& context) override {
        (void)context;
        ++g_post_yield_count;
        return Value(static_cast<Int>(0));
    }
    std::string toString() const override { return "post_yield_counter"; }
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

// AC#1: yield inside while(false) — coroutine completes without ever yielding
TEST(ConcurrencyTest, yield_in_false_loop) {
    auto& sched = Scheduler::instance();
    sched.reset();
    g_yield_count = 0;

    // while (false) { yield; }
    SourceLocation loc;
    auto condition = std::make_unique<LiteralNode>(Bool(false));

    std::vector<ASTNodePtr> body_stmts;
    body_stmts.push_back(std::make_unique<YieldNode>());
    auto body = std::make_unique<BlockNode>(std::move(body_stmts));

    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    Context context;
    sched.spawn(while_stmt.get(), std::move(context));
    sched.run();

    EXPECT_EQ(g_yield_count, 0);
}

// AC#2: yield where condition becomes false on the resume iteration
TEST(ConcurrencyTest, yield_single_iteration) {
    auto& sched = Scheduler::instance();
    sched.reset();
    g_yield_count = 0;

    // var i: Int = 0;
    // while (i < 1) { yield; i = i + 1; }
    SourceLocation loc;
    auto var_decl = std::make_unique<VariableDeclarationNode>(
        "i", "Int", std::make_unique<LiteralNode>(Int(0)));

    auto condition = std::make_unique<ComparisonNode>(
        std::make_unique<IdentifierNode>("i"), ComparisonOperator::LESS_THAN,
        std::make_unique<LiteralNode>(Int(1)), loc);

    auto increment = std::make_unique<VariableAssignmentNode>(
        "i", std::make_unique<BinaryOpNode>(
                 std::make_unique<IdentifierNode>("i"), BinaryOperator::PLUS,
                 std::make_unique<LiteralNode>(Int(1)), loc));

    std::vector<ASTNodePtr> body_stmts;
    body_stmts.push_back(std::make_unique<YieldNode>());
    body_stmts.push_back(std::move(increment));
    auto body = std::make_unique<BlockNode>(std::move(body_stmts));

    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    Context context;
    var_decl->evaluate(context);
    sched.spawn(while_stmt.get(), std::move(context));
    sched.run();

    // Yields once (i=0 < 1), resumes, increments i to 1, condition 1<1 false, done.
    EXPECT_EQ(g_yield_count, 1);
}

// AC#3: yield in deeply nested blocks (3 levels of BlockNode)
TEST(ConcurrencyTest, yield_deeply_nested) {
    auto& sched = Scheduler::instance();
    sched.reset();
    g_yield_count = 0;

    // var i: Int = 0;
    // while (i < 1) {
    //   {              // outer block
    //     {            // middle block
    //       {          // inner block
    //         yield;
    //         i = i + 1;
    //       }
    //     }
    //   }
    // }
    SourceLocation loc;
    auto var_decl = std::make_unique<VariableDeclarationNode>(
        "i", "Int", std::make_unique<LiteralNode>(Int(0)));

    auto condition = std::make_unique<ComparisonNode>(
        std::make_unique<IdentifierNode>("i"), ComparisonOperator::LESS_THAN,
        std::make_unique<LiteralNode>(Int(1)), loc);

    auto increment = std::make_unique<VariableAssignmentNode>(
        "i", std::make_unique<BinaryOpNode>(
                 std::make_unique<IdentifierNode>("i"), BinaryOperator::PLUS,
                 std::make_unique<LiteralNode>(Int(1)), loc));

    // Inner block: { yield; i = i + 1; }
    std::vector<ASTNodePtr> inner_stmts;
    inner_stmts.push_back(std::make_unique<YieldNode>());
    inner_stmts.push_back(std::move(increment));
    auto inner_block = std::make_unique<BlockNode>(std::move(inner_stmts));

    // Middle block: { inner_block }
    std::vector<ASTNodePtr> middle_stmts;
    middle_stmts.push_back(std::move(inner_block));
    auto middle_block = std::make_unique<BlockNode>(std::move(middle_stmts));

    // Outer block (while body): { middle_block }
    std::vector<ASTNodePtr> outer_stmts;
    outer_stmts.push_back(std::move(middle_block));
    auto outer_block = std::make_unique<BlockNode>(std::move(outer_stmts));

    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(outer_block));

    Context context;
    var_decl->evaluate(context);
    sched.spawn(while_stmt.get(), std::move(context));
    sched.run();

    // block_resume_stack should handle 3 levels of BlockNode + WhileStatementNode
    EXPECT_EQ(g_yield_count, 1);
}

// AC#1 (task-004): error thrown after yield propagates to scheduler
TEST(ConcurrencyTest, error_after_resume) {
    auto& sched = Scheduler::instance();
    sched.reset();

    // while (true) { yield_then_error; }
    // First run: yields. Resume: throws runtime_error.
    auto condition = std::make_unique<LiteralNode>(Bool(true));

    std::vector<ASTNodePtr> body_stmts;
    body_stmts.push_back(std::make_unique<YieldThenErrorNode>());
    auto body = std::make_unique<BlockNode>(std::move(body_stmts));

    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    Context context;
    sched.spawn(while_stmt.get(), std::move(context));  // id=0 (root)
    sched.run();

    // Root coroutine should have failed with our error
    ASSERT_NE(sched.getRootException(), nullptr);
    try {
        std::rethrow_exception(sched.getRootException());
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "error after resume");
    }
}

// AC#2 (task-004): scheduler queues are drained after both success and failure
TEST(ConcurrencyTest, scheduler_clean_after_run) {
    auto& sched = Scheduler::instance();

    // Case 1: successful completion
    sched.reset();
    auto success = std::make_unique<LiteralNode>(Int(1));
    sched.spawn(success.get(), Context());
    sched.run();
    EXPECT_FALSE(sched.isActive());
    EXPECT_EQ(sched.getRootException(), nullptr);

    // Case 2: error completion
    sched.reset();
    auto error = std::make_unique<ErrorNode>();
    sched.spawn(error.get(), Context());
    sched.run();
    EXPECT_FALSE(sched.isActive());
    EXPECT_NE(sched.getRootException(), nullptr);

    // Case 3: yield then error — scheduler should still drain
    sched.reset();
    auto cond = std::make_unique<LiteralNode>(Bool(true));
    std::vector<ASTNodePtr> stmts;
    stmts.push_back(std::make_unique<YieldThenErrorNode>());
    auto blk = std::make_unique<BlockNode>(std::move(stmts));
    auto ws = std::make_unique<WhileStatementNode>(std::move(cond), std::move(blk));
    sched.spawn(ws.get(), Context());
    sched.run();
    EXPECT_FALSE(sched.isActive());
}

// AC#3 (task-004): error during condition evaluation doesn't leave stale state
TEST(ConcurrencyTest, error_in_condition_after_resume) {
    auto& sched = Scheduler::instance();
    sched.reset();
    g_yield_count = 0;
    g_post_yield_count = 0;

    // countdown starts at 1: first condition eval succeeds (returns true),
    // body yields, resume completes, second condition eval throws.
    int countdown = 1;
    auto condition = std::make_unique<CountdownConditionNode>(&countdown);

    std::vector<ASTNodePtr> body_stmts;
    body_stmts.push_back(std::make_unique<YieldNode>());
    body_stmts.push_back(std::make_unique<PostYieldCounterNode>());
    auto body = std::make_unique<BlockNode>(std::move(body_stmts));

    auto while_stmt = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    Context context;
    sched.spawn(while_stmt.get(), std::move(context));
    sched.run();

    // Yield happened once, then resume ran post-yield counter, then condition threw
    EXPECT_EQ(g_yield_count, 1);
    EXPECT_EQ(g_post_yield_count, 1);
    ASSERT_NE(sched.getRootException(), nullptr);
    EXPECT_FALSE(sched.isActive());
}
