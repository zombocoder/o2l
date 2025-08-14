#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

using namespace o2l;

class IntegrationTest : public ::testing::Test {
   protected:
    void SetUp() override {}

    struct TestResult {
        bool success;
        int exit_code;
        std::string output;
        std::string error;
    };

    std::string readFile(const std::string& filename) {
        // Try different possible paths since test runs from build directory
        std::vector<std::string> possible_paths = {
            "examples/" + filename, "../examples/" + filename, "../../examples/" + filename};

        for (const auto& path : possible_paths) {
            std::ifstream file(path);
            if (file.is_open()) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                return buffer.str();
            }
        }

        return "";
    }

    TestResult runO2LCode(const std::string& code) {
        try {
            // Tokenize
            Lexer lexer(code);
            auto tokens = lexer.tokenizeAll();

            // Parse
            Parser parser(std::move(tokens), "test_code.obq");
            auto ast_nodes = parser.parse();

            // Interpret
            Interpreter interpreter;
            Value result = interpreter.execute(ast_nodes);

            // Extract exit code if main() returned an Int
            int exit_code = 0;
            if (std::holds_alternative<Int>(result)) {
                exit_code = std::get<Int>(result);
            }

            return {true, exit_code, "", ""};
        } catch (const std::exception& e) {
            return {false, -1, "", e.what()};
        }
    }

    TestResult runO2LFile(const std::string& filename) {
        std::string code = readFile(filename);
        if (code.empty()) {
            return {false, -1, "", "Could not read file: " + filename};
        }
        return runO2LCode(code);
    }

    void expectSuccess(const std::string& filename, int expected_exit_code = 0) {
        auto result = runO2LFile(filename);
        EXPECT_TRUE(result.success) << "File: " << filename << "\nError: " << result.error;
        if (result.success) {
            EXPECT_EQ(result.exit_code, expected_exit_code) << "File: " << filename;
        }
    }

    void expectFailure(const std::string& filename, const std::string& expected_error = "") {
        auto result = runO2LFile(filename);
        EXPECT_FALSE(result.success) << "File: " << filename << " should have failed";
        if (!expected_error.empty()) {
            EXPECT_TRUE(result.error.find(expected_error) != std::string::npos)
                << "Expected '" << expected_error << "' in error: " << result.error;
        }
    }
};

// Test basic successful execution
TEST_F(IntegrationTest, BasicSuccessfulExecution) {
    expectSuccess("test_exit_zero.obq", 0);
}

// Test minimal test
TEST_F(IntegrationTest, MinimalTest) {
    expectSuccess("minimal_test.obq", 0);
}

// Test protocol functionality
TEST_F(IntegrationTest, ProtocolTest) {
    expectSuccess("basic_protocol_test.obq", 0);
}

// Test protocol compliance failure
TEST_F(IntegrationTest, ProtocolFailure) {
    expectFailure("protocol_failure_test.obq", "does not implement all methods");
}

// Test syntax issue - lowercase enum/record vs Enum/Record
TEST_F(IntegrationTest, WorkingDemoSyntaxError) {
    expectFailure("working_demo.obq", "Only object declarations");
}

// Test example with correct capitalized syntax
TEST_F(IntegrationTest, CorrectSyntaxTest) {
    expectSuccess("correct_syntax_test.obq", 0);
}

// Test that our integration framework handles multiple types of successful execution
TEST_F(IntegrationTest, IntegrationFrameworkValidation) {
    // Test that we can run a simple successful program
    auto result = runO2LCode(R"(
        Object Main {
            method main(): Int {
                return 42
            }
        }
    )");
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 42);

    // Test that we can catch expected errors
    auto error_result = runO2LCode("Invalid syntax here!");
    EXPECT_FALSE(error_result.success);
    EXPECT_FALSE(error_result.error.empty());
}

// Test syntax compatibility issues (should fail with specific errors)
TEST_F(IntegrationTest, SimpleProtocolTestSyntaxError) {
    expectFailure("simple_protocol_test.obq", "Unknown method 'greet' on Text type");
}

TEST_F(IntegrationTest, SimpleChainTestSyntaxError) {
    expectFailure("simple_chain_test.obq", "Only object declarations");
}

TEST_F(IntegrationTest, DemoAccessError) {
    expectFailure("demo.obq", "not externally accessible");
}

TEST_F(IntegrationTest, MultilineTestSyntaxError) {
    expectFailure("multiline_test.obq", "Only object declarations");
}

TEST_F(IntegrationTest, ConstructorSuccess) {
    expectSuccess("test_constructor.obq", 0);
}

TEST_F(IntegrationTest, LiteralsTopLevelError) {
    expectFailure("literals.obq", "Only object declarations");
}

TEST_F(IntegrationTest, GreetWithParamSyntaxError) {
    expectFailure("greet_with_param.obq", "Only object declarations");
}

TEST_F(IntegrationTest, FinalDemoSuccess) {
    expectSuccess("test_final_demo.obq", 0);
}

// Test error cases
TEST_F(IntegrationTest, ErrorHandling) {
    expectFailure("error_test.obq");
}

// Test files that should not have main method (known to exist)
TEST_F(IntegrationTest, NoMainError) {
    expectFailure("no_main.obq", "Main");
}

// Test void exit (should succeed but with different handling)
TEST_F(IntegrationTest, VoidExit) {
    expectSuccess("test_exit_void.obq", 0);
}

// Test List iterator functionality
TEST_F(IntegrationTest, ListIteratorSuccess) {
    expectSuccess("test_iterator.obq", 0);
}

// Test while loop functionality with iterators
TEST_F(IntegrationTest, WhileLoopSuccess) {
    expectSuccess("test_while.obq", 0);
}

// Test repeat iterator functionality
TEST_F(IntegrationTest, RepeatIteratorSuccess) {
    expectSuccess("test_repeat.obq", 0);
}

// Test Map functionality
TEST_F(IntegrationTest, MapBasicSuccess) {
    expectSuccess("test_map_basic.obq", 0);
}

TEST_F(IntegrationTest, MapMethodsSuccess) {
    expectSuccess("test_map_methods.obq", 0);
}

TEST_F(IntegrationTest, MapIteratorSuccess) {
    expectSuccess("test_map_iterator.obq", 0);
}

TEST_F(IntegrationTest, MapComprehensiveSuccess) {
    expectSuccess("test_map_comprehensive.obq", 0);
}

TEST_F(IntegrationTest, UserMapExampleSuccess) {
    expectSuccess("test_user_map_example.obq", 0);
}

// Test Set functionality
TEST_F(IntegrationTest, SetLiteralSuccess) {
    expectSuccess("test_set_literal.obq", 0);
}

TEST_F(IntegrationTest, UserSetExampleSuccess) {
    expectSuccess("test_user_set_example.obq", 0);
}

// Test string comparison functionality - This will expose the bug!
TEST_F(IntegrationTest, StringComparisonTest) {
    auto result = runO2LCode(R"(
        Object Main {
            method main(): Int {
                text1: Text = "hello"
                text2: Text = "hello"
                text3: Text = "world"
                
                # These should return true/false but currently all return false due to bug
                if (text1 == text2) {
                    return 1  # Should happen - strings are equal
                }
                if (text1 == text3) {
                    return 2  # Should not happen - strings are different  
                }
                if ("test" == "test") {
                    return 3  # Should happen - literal strings are equal
                }
                
                return 0  # Should not reach here if string comparison works
            }
        }
    )");

    // This test should pass (return 1) but will fail due to the string comparison bug
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 1);  // Should return 1 when text1 == text2 is true
}

// Test string comparison in conditional expressions
TEST_F(IntegrationTest, StringComparisonInConditionalsTest) {
    auto result = runO2LCode(R"(
        Object Calculator {
            @external method compute(operation: Text): Int {
                if (operation == "add") {
                    return 10
                }
                if (operation == "subtract") {
                    return 20  
                }
                return 0  # default case
            }
        }
        
        Object Main {
            method main(): Int {
                calc: Calculator = new Calculator()
                result: Int = calc.compute("add")
                return result
            }
        }
    )");

    // This should return 10 when operation == "add" matches, but will return 0 due to bug
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 10);
}

// Test Result<T,E> success case integration
TEST_F(IntegrationTest, ResultTypeSuccessCase) {
    auto result = runO2LCode(R"(
        import system.io
        
        Object SafeMath {
            @external method divide(a: Int, b: Int): Result<Int, Error> {
                if (b == 0) {
                    error: Error = new Error("Division by zero")
                    return Result.error(error)
                }
                return Result.success(a / b) 
            }
            
            @external method processResult(): Int {
                result: Result<Int, Error> = this.divide(10, 2)
                
                if (result.isSuccess()) {
                    value: Int = result.getResult()
                    return value
                } else {
                    return -1
                }
            }
        }
        
        Object Main {
            method main(): Int {
                math: SafeMath = new SafeMath()
                return math.processResult()
            }
        }
    )");

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 5);  // 10 / 2 = 5
}

// Test Result<T,E> error case integration
TEST_F(IntegrationTest, ResultTypeErrorCase) {
    auto result = runO2LCode(R"(
        import system.io
        
        Object SafeMath {
            @external method divide(a: Int, b: Int): Result<Int, Error> {
                if (b == 0) {
                    error: Error = new Error("Division by zero")
                    return Result.error(error)
                }
                return Result.success(a / b)
            }
            
            @external method processResult(): Int {
                result: Result<Int, Error> = this.divide(10, 0)
                
                if (result.isSuccess()) {
                    value: Int = result.getResult()
                    return value
                } else {
                    error: Error = result.getError()
                    message: Text = error.getMessage()
                    return -1
                }
            }
        }
        
        Object Main {
            method main(): Int {
                math: SafeMath = new SafeMath()
                return math.processResult()
            }
        }
    )");

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, -1);  // Error case returns -1
}

// Test Result with different types (simplified)
TEST_F(IntegrationTest, ResultTypeDifferentTypes) {
    auto result = runO2LCode(R"(
        import system.io
        
        Object TypeTest {
            @external method processResults(): Int {
                # Just test that Result.success works with Int
                int_result: Result<Int, Error> = Result.success(42)
                
                if (int_result.isSuccess()) {
                    return 1
                } else {
                    return 0
                }
            }
        }
        
        Object Main {
            method main(): Int {
                test: TypeTest = new TypeTest()
                return test.processResults()
            }
        }
    )");

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 1);  // Result should be successful
}

// Test Error type methods
TEST_F(IntegrationTest, ErrorTypeMethods) {
    auto result = runO2LCode(R"(
        import system.io
        
        Object ErrorTest {
            @external method testErrorMethods(): Int {
                error: Error = new Error("Test error message")
                message: Text = error.getMessage()
                code: Text = error.getCode()
                
                # Simple validation - if we can call methods, return 1
                return 1
            }
        }
        
        Object Main {
            method main(): Int {
                test: ErrorTest = new ErrorTest()
                return test.testErrorMethods()
            }
        }
    )");

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 1);
}

// Test Result.error with proper Error instance
TEST_F(IntegrationTest, ResultErrorWithErrorInstance) {
    auto result = runO2LCode(R"(
        import system.io
        
        Object ErrorHandling {
            @external method createErrorResult(): Result<Int, Error> {
                error: Error = new Error("Custom error message")
                return Result.error(error)
            }
            
            @external method processErrorResult(): Int {
                result: Result<Int, Error> = this.createErrorResult()
                
                if (result.isSuccess()) {
                    return 0
                } else {
                    error: Error = result.getError()
                    message: Text = error.getMessage()
                    # Return success to indicate we properly handled the error
                    return 1
                }
            }
        }
        
        Object Main {
            method main(): Int {
                handler: ErrorHandling = new ErrorHandling()
                return handler.processErrorResult()
            }
        }
    )");

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 1);  // Successfully processed error
}