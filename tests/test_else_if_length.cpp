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

#include <memory>

#include "../src/Common/Exceptions.hpp"
#include "../src/Interpreter.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/Context.hpp"

using namespace o2l;

class ElseIfAndLengthTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to evaluate O²L code
    Value evaluateCode(const std::string& code) {
        Lexer lexer(code);
        auto tokens = lexer.tokenizeAll();

        Parser parser(tokens);
        auto ast = parser.parse();

        Interpreter interpreter;
        return interpreter.execute(ast);
    }

    // Helper to check integer results
    void expectInt(const Value& result, Int expected) {
        ASSERT_TRUE(std::holds_alternative<Int>(result));
        EXPECT_EQ(std::get<Int>(result), expected);
    }

    // Helper to check text results
    void expectText(const Value& result, const std::string& expected) {
        ASSERT_TRUE(std::holds_alternative<Text>(result));
        EXPECT_EQ(std::get<Text>(result), expected);
    }
};

// Test else if functionality
TEST_F(ElseIfAndLengthTest, BasicElseIfChain) {
    std::string code = R"(
        Object Test {
            @external method gradeTest(score: Int): Text {
                if (score >= 90) {
                    return "A"
                } else if (score >= 80) {
                    return "B"
                } else if (score >= 70) {
                    return "C"
                } else if (score >= 60) {
                    return "D"
                } else {
                    return "F"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                return test.gradeTest(85)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "B");
}

TEST_F(ElseIfAndLengthTest, ElseIfWithMultipleConditions) {
    std::string code = R"(
        Object Test {
            @external method weatherTest(temp: Int, humidity: Int): Text {
                if (temp > 80 && humidity > 70) {
                    return "hot-humid"
                } else if (temp > 80) {
                    return "hot-dry"
                } else if (temp < 60) {
                    return "cold"
                } else if (humidity > 80) {
                    return "humid"
                } else {
                    return "pleasant"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                return test.weatherTest(75, 60)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "pleasant");
}

TEST_F(ElseIfAndLengthTest, ElseIfLastCondition) {
    std::string code = R"(
        Object Test {
            @external method dayTest(day: Int): Text {
                if (day == 1) {
                    return "Monday"
                } else if (day == 2) {
                    return "Tuesday"
                } else if (day == 3) {
                    return "Wednesday"
                } else if (day == 4) {
                    return "Thursday"
                } else if (day == 5) {
                    return "Friday"
                } else if (day == 6) {
                    return "Saturday"
                } else if (day == 7) {
                    return "Sunday"
                } else {
                    return "Invalid"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                return test.dayTest(6)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "Saturday");
}

TEST_F(ElseIfAndLengthTest, ElseIfFallThrough) {
    std::string code = R"(
        Object Test {
            @external method fallThroughTest(value: Int): Text {
                if (value < 10) {
                    return "small"
                } else if (value < 50) {
                    return "medium"
                } else if (value < 100) {
                    return "large"
                } else {
                    return "huge"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                return test.fallThroughTest(150)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "huge");
}

TEST_F(ElseIfAndLengthTest, NestedElseIf) {
    std::string code = R"(
        Object Test {
            @external method nestedTest(x: Int, y: Int): Text {
                if (x > 0) {
                    if (y > 0) {
                        return "positive-positive"
                    } else if (y < 0) {
                        return "positive-negative"
                    } else {
                        return "positive-zero"
                    }
                } else if (x < 0) {
                    if (y > 0) {
                        return "negative-positive"
                    } else if (y < 0) {
                        return "negative-negative"
                    } else {
                        return "negative-zero"
                    }
                } else {
                    return "zero"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                return test.nestedTest(-5, 10)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "negative-positive");
}

// Test Text.length() method
TEST_F(ElseIfAndLengthTest, BasicTextLength) {
    std::string code = R"(
        Object Main {
            method main(): Int {
                text: Text = "Hello World"
                return text.length()
            }
        }
    )";

    Value result = evaluateCode(code);
    expectInt(result, 11);
}

TEST_F(ElseIfAndLengthTest, EmptyTextLength) {
    std::string code = R"(
        Object Main {
            method main(): Int {
                empty: Text = ""
                return empty.length()
            }
        }
    )";

    Value result = evaluateCode(code);
    expectInt(result, 0);
}

TEST_F(ElseIfAndLengthTest, LongTextLength) {
    std::string code = R"(
        Object Main {
            method main(): Int {
                long_text: Text = "This is a much longer text string with multiple words and punctuation!"
                return long_text.length()
            }
        }
    )";

    Value result = evaluateCode(code);
    expectInt(result, 70);
}

TEST_F(ElseIfAndLengthTest, TextLengthInCondition) {
    std::string code = R"(
        Object Test {
            @external method lengthTest(text: Text): Text {
                len: Int = text.length()
                if (len == 0) {
                    return "empty"
                } else if (len <= 5) {
                    return "short"
                } else if (len < 20) {
                    return "medium"
                } else {
                    return "long"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                return test.lengthTest("Hello")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "short");
}

TEST_F(ElseIfAndLengthTest, TextLengthWithSpecialCharacters) {
    std::string code = R"(
        Object Main {
            method main(): Int {
                special: Text = "Hello\nWorld\t!"
                return special.length()
            }
        }
    )";

    Value result = evaluateCode(code);
    expectInt(result, 13);  // H-e-l-l-o-\-n-W-o-r-l-d-\-t-!
}

TEST_F(ElseIfAndLengthTest, TextLengthComparison) {
    std::string code = R"(
        Object Main {
            method main(): Text {
                text1: Text = "short"
                text2: Text = "longer text"
                
                len1: Int = text1.length()
                len2: Int = text2.length()
                
                if (len1 > len2) {
                    return "text1 longer"
                } else if (len1 < len2) {
                    return "text2 longer"
                } else {
                    return "same length"
                }
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "text2 longer");
}

TEST_F(ElseIfAndLengthTest, CombinedElseIfAndLength) {
    std::string code = R"(
        Object Test {
            @external method classifyText(text: Text): Text {
                len: Int = text.length()
                
                if (len == 0) {
                    return "empty"
                } else if (len <= 3) {
                    return "tiny"
                } else if (len <= 11) {
                    return "small"
                } else if (len <= 50) {
                    return "medium"
                } else if (len <= 100) {
                    return "large"
                } else {
                    return "huge"
                }
            }
        }
        
        Object Main {
            method main(): Text {
                test: Test = new Test()
                result1: Text = test.classifyText("")
                result2: Text = test.classifyText("Hi") 
                result3: Text = test.classifyText("Hello World")
                result4: Text = test.classifyText("This is a medium sized text")
                
                # Check each result individually
                if (result1 != "empty") {
                    return "result1 wrong"
                } else if (result2 != "tiny") {
                    return "result2 wrong"
                } else if (result3 != "small") {
                    return "result3 wrong"
                } else if (result4 != "medium") {
                    return "result4 wrong"
                } else {
                    return "all correct"
                }
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "all correct");
}

// Test error cases
TEST_F(ElseIfAndLengthTest, TextLengthWithArguments) {
    std::string code = R"(
        Object Main {
            method main(): Int {
                text: Text = "test"
                return text.length(5)
            }
        }
    )";

    EXPECT_THROW(evaluateCode(code), EvaluationError);
}

TEST_F(ElseIfAndLengthTest, ElseIfWithoutParentheses) {
    std::string code = R"(
        Object Main {
            method main(): Text {
                x: Int = 5
                if x > 0 {
                    return "positive"
                } else if x < 0 {
                    return "negative"  
                } else {
                    return "zero"
                }
            }
        }
    )";

    // This should fail because conditions need parentheses
    EXPECT_THROW(evaluateCode(code), std::exception);
}