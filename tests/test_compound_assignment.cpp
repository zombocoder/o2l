/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Tests for compound assignment operators (+=, -=, *=, /=, %=).
//
// Compound assignment desugars at parse time:
//     x op= expr          ->  x = x op (expr)
//     this.p op= expr     ->  this.p = this.p op (expr)
//
// The right-hand side is parsed as one complete expression and used as-is as the
// right operand, which keeps O2L's left-to-right evaluation semantics intact.

#include <gtest/gtest.h>

#include <memory>
#include <variant>

#include "AST/BinaryOpNode.hpp"
#include "AST/IdentifierNode.hpp"
#include "AST/MemberAccessNode.hpp"
#include "AST/PropertyAssignmentNode.hpp"
#include "AST/ThisNode.hpp"
#include "AST/VariableAssignmentNode.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

using namespace o2l;

class CompoundAssignmentTest : public ::testing::Test {
   protected:
    std::unique_ptr<Parser> createParser(const std::string& source) {
        Lexer lexer(source);
        auto tokens = lexer.tokenizeAll();
        return std::make_unique<Parser>(std::move(tokens), "test_compound.obq");
    }

    // Run a full O2L program and return the exit code from main() (or -1 on error).
    int runProgram(const std::string& code) {
        try {
            Lexer lexer(code);
            auto tokens = lexer.tokenizeAll();
            Parser parser(std::move(tokens), "test_compound.obq");
            auto ast_nodes = parser.parse();
            Interpreter interpreter;
            Value result = interpreter.execute(ast_nodes);
            if (std::holds_alternative<Int>(result)) {
                return static_cast<int>(std::get<Int>(result));
            }
            return 0;
        } catch (const std::exception&) {
            return -1;
        }
    }

    std::string wrapMain(const std::string& body) {
        return "Object Main {\n    method main(): Int {\n" + body + "\n    }\n}\n";
    }
};

// --- Lexer: compound-assignment tokens ---

TEST_F(CompoundAssignmentTest, LexerProducesCompoundTokens) {
    Lexer lexer("+= -= *= /= %=");
    auto tokens = lexer.tokenizeAll();
    ASSERT_GE(tokens.size(), 5u);
    EXPECT_EQ(tokens[0].type, TokenType::PLUS_ASSIGN);
    EXPECT_EQ(tokens[1].type, TokenType::MINUS_ASSIGN);
    EXPECT_EQ(tokens[2].type, TokenType::MULTIPLY_ASSIGN);
    EXPECT_EQ(tokens[3].type, TokenType::DIVIDE_ASSIGN);
    EXPECT_EQ(tokens[4].type, TokenType::MODULO_ASSIGN);
}

TEST_F(CompoundAssignmentTest, LexerDistinguishesPlusFromPlusAssign) {
    Lexer lexer("+ +=");
    auto tokens = lexer.tokenizeAll();
    ASSERT_GE(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[1].type, TokenType::PLUS_ASSIGN);
}

// --- Parser: desugaring shape for local variables ---

TEST_F(CompoundAssignmentTest, VariableDesugarsToBinaryOp) {
    auto parser = createParser("x += 5");
    auto node = parser->parseStatement();
    ASSERT_NE(node, nullptr);

    auto* assign = dynamic_cast<VariableAssignmentNode*>(node.get());
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->getVariableName(), "x");

    auto* binary = dynamic_cast<const BinaryOpNode*>(assign->getValueExpression());
    ASSERT_NE(binary, nullptr) << "x += 5 should desugar to x = (x + 5)";
    EXPECT_EQ(binary->getOperator(), BinaryOperator::PLUS);

    auto* lhs = dynamic_cast<const IdentifierNode*>(binary->getLeft().get());
    ASSERT_NE(lhs, nullptr) << "left operand of the desugared expression must read the variable";
    EXPECT_EQ(lhs->getName(), "x");
}

TEST_F(CompoundAssignmentTest, EachOperatorMapsToCorrectBinaryOperator) {
    struct Case {
        std::string src;
        BinaryOperator expected;
    };
    const Case cases[] = {
        {"x += 1", BinaryOperator::PLUS},     {"x -= 1", BinaryOperator::MINUS},
        {"x *= 1", BinaryOperator::MULTIPLY}, {"x /= 1", BinaryOperator::DIVIDE},
        {"x %= 1", BinaryOperator::MODULO},
    };
    for (const auto& c : cases) {
        auto parser = createParser(c.src);
        auto node = parser->parseStatement();
        auto* assign = dynamic_cast<VariableAssignmentNode*>(node.get());
        ASSERT_NE(assign, nullptr) << c.src;
        auto* binary = dynamic_cast<const BinaryOpNode*>(assign->getValueExpression());
        ASSERT_NE(binary, nullptr) << c.src;
        EXPECT_EQ(binary->getOperator(), c.expected) << c.src;
    }
}

// --- Parser: desugaring shape for this.property ---

TEST_F(CompoundAssignmentTest, ThisPropertyDesugarsToBinaryOp) {
    auto parser = createParser("this.total += n");
    auto node = parser->parseStatement();
    ASSERT_NE(node, nullptr);

    auto* assign = dynamic_cast<PropertyAssignmentNode*>(node.get());
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->getPropertyName(), "total");

    auto* binary = dynamic_cast<const BinaryOpNode*>(assign->getValueExpression().get());
    ASSERT_NE(binary, nullptr) << "this.total += n should desugar to this.total = (this.total + n)";
    EXPECT_EQ(binary->getOperator(), BinaryOperator::PLUS);

    auto* member = dynamic_cast<const MemberAccessNode*>(binary->getLeft().get());
    ASSERT_NE(member, nullptr) << "left operand must read this.total";
    EXPECT_EQ(member->getMemberName(), "total");
    EXPECT_NE(dynamic_cast<const ThisNode*>(member->getObjectExpression().get()), nullptr);
}

// --- End-to-end: arithmetic semantics ---

TEST_F(CompoundAssignmentTest, PlusAssign) {
    EXPECT_EQ(runProgram(wrapMain("        x: Int = 10\n        x += 5\n        return x")), 15);
}

TEST_F(CompoundAssignmentTest, MinusAssign) {
    EXPECT_EQ(runProgram(wrapMain("        x: Int = 10\n        x -= 3\n        return x")), 7);
}

TEST_F(CompoundAssignmentTest, MultiplyAssign) {
    EXPECT_EQ(runProgram(wrapMain("        x: Int = 4\n        x *= 5\n        return x")), 20);
}

TEST_F(CompoundAssignmentTest, DivideAssign) {
    EXPECT_EQ(runProgram(wrapMain("        x: Int = 20\n        x /= 4\n        return x")), 5);
}

TEST_F(CompoundAssignmentTest, ModuloAssign) {
    EXPECT_EQ(runProgram(wrapMain("        x: Int = 17\n        x %= 5\n        return x")), 2);
}

// `x op= EXPR` uses the full right-hand expression as the right operand:
// a += 2 + 3  ==  a = a + (2 + 3)  ==  105.
TEST_F(CompoundAssignmentTest, RightHandSideIsWholeExpression) {
    EXPECT_EQ(runProgram(wrapMain("        a: Int = 100\n        a += 2 + 3\n        return a")),
              105);
}

TEST_F(CompoundAssignmentTest, AccumulationInLoop) {
    const std::string body =
        "        sum: Int = 0\n"
        "        i: Int = 1\n"
        "        while (i <= 5) {\n"
        "            sum += i\n"
        "            i += 1\n"
        "        }\n"
        "        return sum";
    EXPECT_EQ(runProgram(wrapMain(body)), 15);
}

// End-to-end: compound assignment on this.property.
TEST_F(CompoundAssignmentTest, ThisPropertyAccumulator) {
    const std::string code =
        "Object Counter {\n"
        "    property total: Int\n"
        "    constructor() { this.total = 0 }\n"
        "    @external method add(n: Int): Int {\n"
        "        this.total += n\n"
        "        return this.total\n"
        "    }\n"
        "}\n"
        "Object Main {\n"
        "    method main(): Int {\n"
        "        c: Counter = new Counter()\n"
        "        c.add(10)\n"
        "        c.add(32)\n"
        "        return c.add(0)\n"
        "    }\n"
        "}\n";
    EXPECT_EQ(runProgram(code), 42);
}

// Plain assignment must remain unaffected by the new tokens.
TEST_F(CompoundAssignmentTest, PlainAssignmentStillWorks) {
    EXPECT_EQ(runProgram(wrapMain("        x: Int = 3\n        x = 9\n        return x")), 9);
}
