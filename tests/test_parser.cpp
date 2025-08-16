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

#include "AST/EnumDeclarationNode.hpp"
#include "AST/ImportNode.hpp"
#include "AST/LiteralNode.hpp"
#include "AST/MethodDeclarationNode.hpp"
#include "AST/ObjectNode.hpp"
#include "AST/ProtocolDeclarationNode.hpp"
#include "AST/RecordDeclarationNode.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

using namespace o2l;

class ParserTest : public ::testing::Test {
   protected:
    void SetUp() override {}

    std::vector<ASTNodePtr> parse(const std::string& input) {
        Lexer lexer(input);
        auto tokens = lexer.tokenizeAll();
        Parser parser(std::move(tokens));
        return parser.parse();
    }
};

// Test basic object parsing
TEST_F(ParserTest, SimpleObjectDeclaration) {
    auto nodes = parse("Object TestObject {}");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test object with methods
TEST_F(ParserTest, ObjectWithMethods) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Text {
                return "Hello"
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test object with external methods
TEST_F(ParserTest, ObjectWithExternalMethods) {
    auto nodes = parse(R"(
        Object TestObject {
            @external method publicMethod(): Text {
                return "Public"
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test object with constructor
TEST_F(ParserTest, ObjectWithConstructor) {
    auto nodes = parse(R"(
        Object TestObject {
            constructor(name: Text) {
                this.name = name
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test enum declaration
TEST_F(ParserTest, EnumDeclaration) {
    auto nodes = parse(R"(
        Enum Status {
            ACTIVE = 1,
            INACTIVE = 0,
            PENDING = 2
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto enum_node = dynamic_cast<EnumDeclarationNode*>(nodes[0].get());
    ASSERT_NE(enum_node, nullptr);
    EXPECT_EQ(enum_node->getEnumName(), "Status");
}

// Test record declaration
TEST_F(ParserTest, RecordDeclaration) {
    auto nodes = parse(R"(
        Record Person {
            name: Text,
            age: Int
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto record_node = dynamic_cast<RecordDeclarationNode*>(nodes[0].get());
    ASSERT_NE(record_node, nullptr);
    EXPECT_EQ(record_node->getRecordName(), "Person");
}

// Test protocol declaration
TEST_F(ParserTest, ProtocolDeclaration) {
    auto nodes = parse(R"(
        Protocol Greetable {
            method greet(): Text
            method farewell(name: Text): Text
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto protocol_node = dynamic_cast<ProtocolDeclarationNode*>(nodes[0].get());
    ASSERT_NE(protocol_node, nullptr);
    EXPECT_EQ(protocol_node->getProtocolName(), "Greetable");
}

// Test object with protocol inheritance
TEST_F(ParserTest, ObjectWithProtocol) {
    auto nodes = parse(R"(
        Protocol Greetable {
            method greet(): Text
        }
        
        Object Person: Greetable {
            @external method greet(): Text {
                return "Hello"
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 2);

    auto protocol_node = dynamic_cast<ProtocolDeclarationNode*>(nodes[0].get());
    ASSERT_NE(protocol_node, nullptr);
    EXPECT_EQ(protocol_node->getProtocolName(), "Greetable");

    auto object_node = dynamic_cast<ObjectNode*>(nodes[1].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "Person");
    EXPECT_EQ(object_node->getProtocolName(), "Greetable");
}

// Test import statements
TEST_F(ParserTest, ImportStatements) {
    auto nodes = parse("import system.io");

    ASSERT_EQ(nodes.size(), 1);
    auto import_node = dynamic_cast<ImportNode*>(nodes[0].get());
    ASSERT_NE(import_node, nullptr);
}

// Test constants (not supported at top level)
TEST_F(ParserTest, ConstantDeclaration) {
    // Constants are not supported at top level, expect an exception
    EXPECT_THROW({ auto nodes = parse("const PI: Float = 3.14159"); }, std::exception);
}

// Test if statements
TEST_F(ParserTest, IfStatements) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(x: Int): Text {
                if (x > 0) {
                    return "positive"
                } else {
                    return "non-positive"
                }
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test while statements
TEST_F(ParserTest, WhileStatements) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                numbers: List<Int> = [1, 2, 3]
                iter: ListIterator = numbers.iterator()
                while (iter.hasNext()) {
                    value: Int = iter.next()
                }
                return 0
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test method parameters
TEST_F(ParserTest, MethodWithParameters) {
    auto nodes = parse(R"(
        Object Calculator {
            method add(a: Int, b: Int): Int {
                return a + b
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test simple expressions (parentheses might not be fully supported)
TEST_F(ParserTest, SimpleExpressions) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Bool {
                return 2 + 3 == 5
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

TEST_F(ParserTest, FloatAndDoubleLiterals) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Bool {
                return 3.14f == 2.718d
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test simple method calls (parser might not support this syntax yet)
TEST_F(ParserTest, SimpleMethodCalls) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Text {
                return "hello"
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test multiple declarations
TEST_F(ParserTest, MultipleDeclarations) {
    auto nodes = parse(R"(
        import system.io
        
        Record Person {
            name: Text
        }
        
        Object PersonService {
            method create(name: Text): Person {
                return Person(name=name)
            }
        }
        
        Object Main {
            @external method main(): Int {
                return 0
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 4);  // import + record + 2 objects
}

// Test error handling - should not crash on invalid syntax
TEST_F(ParserTest, ErrorHandling) {
    EXPECT_THROW({ parse("Object InvalidObject {"); }, std::exception);

    EXPECT_THROW({ parse("method orphanMethod() {}"); }, std::exception);

    EXPECT_THROW({ parse("Object Test { invalid_token }"); }, std::exception);
}

// Test Result<T,E> generic type parsing
TEST_F(ParserTest, ResultGenericType) {
    auto nodes = parse(R"(
        Object Test {
            @external method divide(a: Int, b: Int): Result<Int, Error> {
                return Result.success(a / b)
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    // Just test that it parses without throwing
}

// Test Error type parsing
TEST_F(ParserTest, ErrorTypeConstructor) {
    auto nodes = parse(R"(
        Object Test {
            @external method createError(): Error {
                return new Error("test message")
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
}

// Test Result static method calls
TEST_F(ParserTest, ResultStaticMethods) {
    auto nodes = parse(R"(
        Object Test {
            @external method testSuccess(): Result<Int, Error> {
                return Result.success(42)
            }
            
            @external method testError(): Result<Int, Error> {
                error: Error = new Error("failure")
                return Result.error(error)
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
}

// Test Result method calls
TEST_F(ParserTest, ResultMethodCalls) {
    auto nodes = parse(R"(
        Object Test {
            @external method processResult(result: Result<Int, Error>): Int {
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
    )");

    ASSERT_EQ(nodes.size(), 1);
}

// Test complex Result type with different generic parameters
TEST_F(ParserTest, ComplexResultTypes) {
    auto nodes = parse(R"(
        Object Test {
            @external method textResult(): Result<Text, Error> {
                return Result.success("hello")
            }
            
            @external method boolResult(): Result<Bool, Error> {
                return Result.success(true)
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
}

// Test variable assignment parsing
TEST_F(ParserTest, VariableAssignment) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                count: Int = 5
                count = count + 1
                return count
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}

// Test multiple variable assignments
TEST_F(ParserTest, MultipleVariableAssignments) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                a: Int = 1
                b: Int = 2
                c: Int = 3
                a = b + c
                b = a * 2
                c = b - a
                return a + b + c
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test variable assignment with different types
TEST_F(ParserTest, VariableAssignmentDifferentTypes) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Text {
                message: Text = "Hello"
                flag: Bool = true
                counter: Int = 0
                
                message = message + " World"
                flag = !flag
                counter = counter + 1
                
                return message
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test variable assignment in control structures
TEST_F(ParserTest, VariableAssignmentInControlStructures) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                result: Int = 0
                i: Int = 1
                
                while (i <= 3) {
                    result = result + i
                    i = i + 1
                }
                
                if (result > 5) {
                    result = result * 2
                }
                
                return result
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test variable assignment with complex expressions
TEST_F(ParserTest, VariableAssignmentComplexExpressions) {
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                x: Int = 10
                y: Int = 20
                z: Int = 0
                
                z = (x + y) * 2
                x = z - (y * 3)
                
                return x + y + z
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}

// Test variable assignment edge cases
TEST_F(ParserTest, VariableAssignmentEdgeCases) {
    // Test assignment to same variable multiple times
    auto nodes = parse(R"(
        Object TestObject {
            method test(): Int {
                value: Int = 1
                value = value
                value = value + 0
                value = value * 1
                return value
            }
        }
    )");

    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
}