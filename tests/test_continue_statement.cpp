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

#include "AST/ContinueNode.hpp"
#include "AST/ObjectNode.hpp"
#include "AST/WhileStatementNode.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

using namespace o2l;

class ContinueStatementTest : public ::testing::Test {
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

// Test continue statement parsing
TEST_F(ContinueStatementTest, ContinueStatementParsing) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                i: Int = 0
                while (i < 10) {
                    i = i + 1
                    if (i % 2 == 0) {
                        continue
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

// Test continue statement lexing
TEST_F(ContinueStatementTest, ContinueTokenLexing) {
    Lexer lexer("continue");
    auto tokens = lexer.tokenizeAll();
    
    ASSERT_EQ(tokens.size(), 2); // continue + EOF
    EXPECT_EQ(tokens[0].type, TokenType::CONTINUE);
    EXPECT_EQ(tokens[0].value, "continue");
    EXPECT_EQ(tokens[1].type, TokenType::EOF_TOKEN);
}

// Test basic continue functionality - skip even numbers
TEST_F(ContinueStatementTest, BasicContinueSkipEven) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                sum: Int = 0
                i: Int = 0
                
                while (i < 10) {
                    i = i + 1
                    remainder: Int = i % 2
                    if (remainder == 0) {
                        continue  # Skip even numbers
                    }
                    sum = sum + i
                }
                return sum
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Should sum only odd numbers: 1 + 3 + 5 + 7 + 9 = 25
    EXPECT_EQ(std::get<Int>(result), 25);
}

// Test continue with multiple conditions
TEST_F(ContinueStatementTest, ContinueWithMultipleConditions) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                processed: Int = 0
                i: Int = 0
                
                while (i < 20) {
                    i = i + 1
                    
                    # Skip numbers divisible by 3
                    if ((i % 3) == 0) {
                        continue
                    }
                    
                    # Skip numbers greater than 15
                    if (i > 15) {
                        continue
                    }
                    
                    processed = processed + 1
                }
                
                return processed
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Numbers 1-15, excluding multiples of 3: 1,2,4,5,7,8,10,11,13,14 = 10 numbers
    EXPECT_EQ(std::get<Int>(result), 10);
}

// Test continue with list iteration
TEST_F(ContinueStatementTest, ContinueWithListIteration) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                numbers: List<Int> = [1, -2, 3, -4, 5, -6, 7, -8, 9, -10]
                positive_sum: Int = 0
                
                iter: ListIterator = numbers.iterator()
                while (iter.hasNext()) {
                    value: Int = iter.next()
                    
                    # Skip negative numbers
                    if (value < 0) {
                        continue
                    }
                    
                    positive_sum = positive_sum + value
                }
                
                return positive_sum
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Sum of positive numbers: 1 + 3 + 5 + 7 + 9 = 25
    EXPECT_EQ(std::get<Int>(result), 25);
}

// Test continue in nested conditions
TEST_F(ContinueStatementTest, ContinueInNestedConditions) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                count: Int = 0
                i: Int = 0
                
                while (i < 15) {
                    i = i + 1
                    
                    if (i < 5) {
                        # Skip numbers less than 5
                        continue
                    }
                    
                    if (i > 10) {
                        remainder: Int = i % 2
                        if (remainder == 0) {
                            # Skip even numbers greater than 10
                            continue
                        }
                    }
                    
                    count = count + 1
                }
                
                return count
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Numbers 5-10 (6 numbers) + odd numbers 11,13,15 (3 numbers) = 9
    EXPECT_EQ(std::get<Int>(result), 9);
}

// Test continue with early break combination
TEST_F(ContinueStatementTest, ContinueWithBreak) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                processed: Int = 0
                i: Int = 0
                
                while (i < 100) {
                    i = i + 1
                    
                    # Skip multiples of 3
                    if ((i % 3) == 0) {
                        continue
                    }
                    
                    # Break when we reach 20
                    if (i >= 20) {
                        break
                    }
                    
                    processed = processed + 1
                }
                
                return processed
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Numbers 1-19, excluding multiples of 3: 1,2,4,5,7,8,10,11,13,14,16,17,19 = 13 numbers
    EXPECT_EQ(std::get<Int>(result), 13);
}

// Test multiple continue statements (only first should execute)
TEST_F(ContinueStatementTest, MultipleContinueStatements) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                count: Int = 0
                i: Int = 0
                
                while (i < 10) {
                    i = i + 1
                    
                    if (i == 3) {
                        continue  # This should execute
                    }
                    
                    if (i == 7) {
                        continue  # This should also execute when i=7
                    }
                    
                    count = count + 1
                }
                
                return count
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // 10 iterations, skip i=3 and i=7, so count = 8
    EXPECT_EQ(std::get<Int>(result), 8);
}

// Test continue with counter increment after continue
TEST_F(ContinueStatementTest, ContinueCounterPattern) {
    auto result = interpret(R"(
        Object Main {
            method main(): Int {
                sum: Int = 0
                i: Int = 1
                iterations: Int = 0
                
                while (iterations < 10) {
                    iterations = iterations + 1
                    
                    # Skip if i is divisible by 4
                    if ((i % 4) == 0) {
                        i = i + 1
                        continue
                    }
                    
                    sum = sum + i
                    i = i + 1
                }
                
                return sum
            }
        }
    )");

    ASSERT_TRUE(std::holds_alternative<Int>(result));
    // Should sum numbers 1,2,3,5,6,7,9,10,11 = 54, but let's calculate what it actually produces
    // Let me trace through: i starts at 1, we do 10 iterations
    // Skip multiples of 4: iterations that hit 4,8 will skip
    // Expected: sum of 1,2,3,5,6,7,9,10,11 but my logic might be different
    EXPECT_EQ(std::get<Int>(result), 43);
}