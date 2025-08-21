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

#include <gtest/gtest.h>

#include "AST/BreakNode.hpp"
#include "AST/ObjectNode.hpp"
#include "AST/WhileStatementNode.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

using namespace o2l;

class BreakStatementTest : public ::testing::Test {
   protected:
    void SetUp() override {}

    std::vector<ASTNodePtr> parse(const std::string& input) {
        Lexer lexer(input);
        auto tokens = lexer.tokenizeAll();
        Parser parser(std::move(tokens));
        return parser.parse();
    }

    Value interpret(const std::string& input) {
        auto nodes = parse(input);
        Interpreter interpreter;
        return interpreter.execute(nodes);
    }
};

// Test break statement parsing
TEST_F(BreakStatementTest, BreakStatementParsing) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                i: Int = 0
                while (i < 10) {
                    i = i + 1
                    if (i == 5) {
                        break
                    }
                }
                return i
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test break statement lexing
TEST_F(BreakStatementTest, BreakTokenLexing) {
    Lexer lexer("break");
    auto tokens = lexer.tokenizeAll();
    
    ASSERT_EQ(tokens.size(), 2); // break + EOF
    EXPECT_EQ(tokens[0].type, TokenType::BREAK);
    EXPECT_EQ(tokens[0].value, "break");
    EXPECT_EQ(tokens[1].type, TokenType::EOF_TOKEN);
}

// Test basic break functionality in while loop
TEST_F(BreakStatementTest, BasicBreakInWhile) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                i: Int = 0
                while (i < 10) {
                    i = i + 1
                    if (i == 5) {
                        break
                    }
                }
                return i
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 5);
}

// Test break in nested conditions
TEST_F(BreakStatementTest, BreakInNestedCondition) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                count: Int = 0
                i: Int = 1
                while (i <= 20) {
                    if (i > 3) {
                        if (i < 8) {
                            count = count + 1
                            if (count == 3) {
                                break
                            }
                        }
                    }
                    i = i + 1
                }
                return i
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);
}

// Test break with list iteration
TEST_F(BreakStatementTest, BreakWithListIteration) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                numbers: List<Int> = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
                iter: ListIterator = numbers.iterator()
                count: Int = 0
                
                while (iter.hasNext()) {
                    value: Int = iter.next()
                    count = count + 1
                    if (value == 6) {
                        break
                    }
                }
                
                return count
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 6);
}

// Test break with early termination
TEST_F(BreakStatementTest, BreakEarlyTermination) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                sum: Int = 0
                i: Int = 1
                
                while (true) {
                    sum = sum + i
                    i = i + 1
                    if (sum > 50) {
                        break
                    }
                }
                
                return sum
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // 1+2+3+4+5+6+7+8+9+10 = 55
    EXPECT_EQ(std::get<Int>(result), 55);
}

// Test multiple break statements (only first should execute)
TEST_F(BreakStatementTest, MultipleBreakStatements) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                i: Int = 0
                while (i < 10) {
                    i = i + 1
                    if (i == 3) {
                        break
                    }
                    if (i == 7) {
                        break
                    }
                }
                return i
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    EXPECT_EQ(std::get<Int>(result), 3);
}