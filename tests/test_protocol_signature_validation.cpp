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
#include <vector>

#include "../src/AST/MethodDeclarationNode.hpp"
#include "../src/AST/ProtocolDeclarationNode.hpp"
#include "../src/Common/Exceptions.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/ObjectInstance.hpp"
#include "../src/Runtime/ProtocolInstance.hpp"

using namespace o2l;

class ProtocolSignatureValidationTest : public ::testing::Test {
   protected:
    void SetUp() override {
        context = std::make_unique<Context>();

        // Create a sample protocol with various method signatures
        createSampleProtocol();

        // Create test objects
        createTestObjects();
    }

    void createSampleProtocol() {
        // Protocol Calculator {
        //     method add(a: Int, b: Int): Int
        //     method multiply(x: Float, y: Float): Float
        //     method getName(): Text
        //     method isReady(): Bool
        //     method reset(): Text
        // }

        std::vector<ProtocolMethodSignature> signatures;

        // add(a: Int, b: Int): Int
        std::vector<Parameter> add_params = {Parameter("a", "Int"), Parameter("b", "Int")};
        signatures.emplace_back("add", add_params, "Int");

        // multiply(x: Float, y: Float): Float
        std::vector<Parameter> multiply_params = {Parameter("x", "Float"), Parameter("y", "Float")};
        signatures.emplace_back("multiply", multiply_params, "Float");

        // getName(): Text
        std::vector<Parameter> getName_params;  // No parameters
        signatures.emplace_back("getName", getName_params, "Text");

        // isReady(): Bool
        std::vector<Parameter> isReady_params;  // No parameters
        signatures.emplace_back("isReady", isReady_params, "Bool");

        // reset(): Text
        std::vector<Parameter> reset_params;  // No parameters
        signatures.emplace_back("reset", reset_params, "Text");

        protocol = std::make_shared<ProtocolInstance>("Calculator", signatures);
    }

    void createTestObjects() {
        // Create correctly implementing object
        correct_object = std::make_shared<ObjectInstance>("CorrectCalculator");

        // Add methods with correct signatures
        std::vector<Parameter> add_params = {Parameter("a", "Int"), Parameter("b", "Int")};
        correct_object->addMethod("add", createDummyMethod(), add_params, "Int", true);

        std::vector<Parameter> multiply_params = {Parameter("x", "Float"), Parameter("y", "Float")};
        correct_object->addMethod("multiply", createDummyMethod(), multiply_params, "Float", true);

        std::vector<Parameter> getName_params;
        correct_object->addMethod("getName", createDummyMethod(), getName_params, "Text", true);

        std::vector<Parameter> isReady_params;
        correct_object->addMethod("isReady", createDummyMethod(), isReady_params, "Bool", true);

        std::vector<Parameter> reset_params;
        correct_object->addMethod("reset", createDummyMethod(), reset_params, "Text", true);

        // Create object missing methods
        missing_method_object = std::make_shared<ObjectInstance>("MissingMethodCalculator");
        missing_method_object->addMethod("add", createDummyMethod(), add_params, "Int", true);
        missing_method_object->addMethod("multiply", createDummyMethod(), multiply_params, "Float",
                                         true);
        // Missing getName, isReady, reset methods

        // Create object with wrong parameter count
        wrong_param_count_object = std::make_shared<ObjectInstance>("WrongParamCountCalculator");

        std::vector<Parameter> wrong_add_params = {Parameter("a", "Int")};  // Should have 2 params
        wrong_param_count_object->addMethod("add", createDummyMethod(), wrong_add_params, "Int",
                                            true);
        wrong_param_count_object->addMethod("multiply", createDummyMethod(), multiply_params,
                                            "Float", true);
        wrong_param_count_object->addMethod("getName", createDummyMethod(), getName_params, "Text",
                                            true);
        wrong_param_count_object->addMethod("isReady", createDummyMethod(), isReady_params, "Bool",
                                            true);
        wrong_param_count_object->addMethod("reset", createDummyMethod(), reset_params, "Text",
                                            true);

        // Create object with wrong parameter types
        wrong_param_type_object = std::make_shared<ObjectInstance>("WrongParamTypeCalculator");

        std::vector<Parameter> wrong_type_params = {Parameter("a", "Text"),
                                                    Parameter("b", "Text")};  // Should be Int, Int
        wrong_param_type_object->addMethod("add", createDummyMethod(), wrong_type_params, "Int",
                                           true);
        wrong_param_type_object->addMethod("multiply", createDummyMethod(), multiply_params,
                                           "Float", true);
        wrong_param_type_object->addMethod("getName", createDummyMethod(), getName_params, "Text",
                                           true);
        wrong_param_type_object->addMethod("isReady", createDummyMethod(), isReady_params, "Bool",
                                           true);
        wrong_param_type_object->addMethod("reset", createDummyMethod(), reset_params, "Text",
                                           true);

        // Create object with wrong return type
        wrong_return_type_object = std::make_shared<ObjectInstance>("WrongReturnTypeCalculator");

        wrong_return_type_object->addMethod("add", createDummyMethod(), add_params, "Text",
                                            true);  // Should return Int
        wrong_return_type_object->addMethod("multiply", createDummyMethod(), multiply_params,
                                            "Float", true);
        wrong_return_type_object->addMethod("getName", createDummyMethod(), getName_params, "Text",
                                            true);
        wrong_return_type_object->addMethod("isReady", createDummyMethod(), isReady_params, "Bool",
                                            true);
        wrong_return_type_object->addMethod("reset", createDummyMethod(), reset_params, "Text",
                                            true);

        // Create object without signature information (for backward compatibility test)
        no_signature_object = std::make_shared<ObjectInstance>("NoSignatureCalculator");
        no_signature_object->addMethod("add", createDummyMethod(),
                                       true);  // Old-style method addition
        no_signature_object->addMethod("multiply", createDummyMethod(), true);
        no_signature_object->addMethod("getName", createDummyMethod(), true);
        no_signature_object->addMethod("isReady", createDummyMethod(), true);
        no_signature_object->addMethod("reset", createDummyMethod(), true);
    }

    // Create a dummy method that just returns a default value
    Method createDummyMethod() {
        return [](const std::vector<Value>& args, Context& context) -> Value {
            return Int(42);  // Dummy return value
        };
    }

    Context* getContext() {
        return context.get();
    }

   protected:
    std::unique_ptr<Context> context;
    std::shared_ptr<ProtocolInstance> protocol;
    std::shared_ptr<ObjectInstance> correct_object;
    std::shared_ptr<ObjectInstance> missing_method_object;
    std::shared_ptr<ObjectInstance> wrong_param_count_object;
    std::shared_ptr<ObjectInstance> wrong_param_type_object;
    std::shared_ptr<ObjectInstance> wrong_return_type_object;
    std::shared_ptr<ObjectInstance> no_signature_object;
};

//=============================================================================
// Basic Protocol Creation Tests
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, ProtocolCreation) {
    EXPECT_EQ(protocol->getName(), "Calculator");
    EXPECT_EQ(protocol->getMethodSignatures().size(), 5);

    const auto& signatures = protocol->getMethodSignatures();
    EXPECT_EQ(signatures[0].name, "add");
    EXPECT_EQ(signatures[0].parameters.size(), 2);
    EXPECT_EQ(signatures[0].return_type, "Int");

    EXPECT_EQ(signatures[1].name, "multiply");
    EXPECT_EQ(signatures[1].parameters.size(), 2);
    EXPECT_EQ(signatures[1].return_type, "Float");

    EXPECT_EQ(signatures[2].name, "getName");
    EXPECT_EQ(signatures[2].parameters.size(), 0);
    EXPECT_EQ(signatures[2].return_type, "Text");
}

//=============================================================================
// ObjectInstance Method Signature Tests
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, ObjectInstanceMethodSignatures) {
    // Test that method signatures are stored correctly
    EXPECT_TRUE(correct_object->hasMethodSignature("add"));
    EXPECT_TRUE(correct_object->hasMethodSignature("multiply"));
    EXPECT_TRUE(correct_object->hasMethodSignature("getName"));
    EXPECT_FALSE(correct_object->hasMethodSignature("nonexistent"));

    // Test signature retrieval
    const MethodSignature* add_signature = correct_object->getMethodSignature("add");
    ASSERT_NE(add_signature, nullptr);
    EXPECT_EQ(add_signature->name, "add");
    EXPECT_EQ(add_signature->parameters.size(), 2);
    EXPECT_EQ(add_signature->parameters[0].name, "a");
    EXPECT_EQ(add_signature->parameters[0].type, "Int");
    EXPECT_EQ(add_signature->parameters[1].name, "b");
    EXPECT_EQ(add_signature->parameters[1].type, "Int");
    EXPECT_EQ(add_signature->return_type, "Int");
    EXPECT_TRUE(add_signature->is_external);

    // Test method without signature
    const MethodSignature* missing_signature = correct_object->getMethodSignature("nonexistent");
    EXPECT_EQ(missing_signature, nullptr);
}

TEST_F(ProtocolSignatureValidationTest, BackwardCompatibilityObjectMethods) {
    // Objects created without signature information should still work
    EXPECT_TRUE(no_signature_object->hasMethod("add"));
    EXPECT_TRUE(no_signature_object->hasMethod("multiply"));
    EXPECT_FALSE(no_signature_object->hasMethodSignature("add"));
    EXPECT_FALSE(no_signature_object->hasMethodSignature("multiply"));
}

//=============================================================================
// Protocol Validation - Successful Cases
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, CorrectImplementationValidation) {
    // Correctly implementing object should pass validation
    EXPECT_TRUE(protocol->isImplementedBy(correct_object));

    // Should have no validation errors
    std::vector<std::string> errors = protocol->getValidationErrors(correct_object);
    EXPECT_TRUE(errors.empty());
}

TEST_F(ProtocolSignatureValidationTest, BackwardCompatibilityValidation) {
    // Object without signature information should pass basic validation
    EXPECT_TRUE(protocol->isImplementedBy(no_signature_object));

    // Should have validation warnings about missing signature info
    std::vector<std::string> errors = protocol->getValidationErrors(no_signature_object);
    EXPECT_EQ(errors.size(), 5);  // One warning per method

    for (const auto& error : errors) {
        EXPECT_TRUE(error.find("lacks signature information") != std::string::npos);
    }
}

//=============================================================================
// Protocol Validation - Missing Methods
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, MissingMethodValidation) {
    // Object missing methods should fail validation
    EXPECT_FALSE(protocol->isImplementedBy(missing_method_object));

    // Should report missing methods
    std::vector<std::string> errors = protocol->getValidationErrors(missing_method_object);
    EXPECT_GE(errors.size(), 3);  // At least 3 missing methods

    bool found_missing_getName = false;
    bool found_missing_isReady = false;
    bool found_missing_reset = false;

    for (const auto& error : errors) {
        if (error.find("Missing method: getName") != std::string::npos) {
            found_missing_getName = true;
        }
        if (error.find("Missing method: isReady") != std::string::npos) {
            found_missing_isReady = true;
        }
        if (error.find("Missing method: reset") != std::string::npos) {
            found_missing_reset = true;
        }
    }

    EXPECT_TRUE(found_missing_getName);
    EXPECT_TRUE(found_missing_isReady);
    EXPECT_TRUE(found_missing_reset);
}

//=============================================================================
// Protocol Validation - Parameter Count Errors
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, WrongParameterCountValidation) {
    // Object with wrong parameter count should fail validation
    EXPECT_FALSE(protocol->isImplementedBy(wrong_param_count_object));

    // Should report parameter count mismatch
    std::vector<std::string> errors = protocol->getValidationErrors(wrong_param_count_object);
    EXPECT_GE(errors.size(), 1);

    bool found_param_count_error = false;
    for (const auto& error : errors) {
        if (error.find("parameter count mismatch") != std::string::npos &&
            error.find("add") != std::string::npos) {
            found_param_count_error = true;
            // Should specify expected vs actual count
            EXPECT_TRUE(error.find("expected 2") != std::string::npos);
            EXPECT_TRUE(error.find("got 1") != std::string::npos);
        }
    }

    EXPECT_TRUE(found_param_count_error);
}

//=============================================================================
// Protocol Validation - Parameter Type Errors
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, WrongParameterTypeValidation) {
    // Object with wrong parameter types should fail validation
    EXPECT_FALSE(protocol->isImplementedBy(wrong_param_type_object));

    // Should report parameter type mismatches
    std::vector<std::string> errors = protocol->getValidationErrors(wrong_param_type_object);
    EXPECT_GE(errors.size(), 2);  // Two parameter type errors for 'add' method

    bool found_param0_error = false;
    bool found_param1_error = false;

    for (const auto& error : errors) {
        if (error.find("add") != std::string::npos &&
            error.find("parameter 0 type mismatch") != std::string::npos) {
            found_param0_error = true;
            EXPECT_TRUE(error.find("expected 'Int'") != std::string::npos);
            EXPECT_TRUE(error.find("got 'Text'") != std::string::npos);
        }
        if (error.find("add") != std::string::npos &&
            error.find("parameter 1 type mismatch") != std::string::npos) {
            found_param1_error = true;
            EXPECT_TRUE(error.find("expected 'Int'") != std::string::npos);
            EXPECT_TRUE(error.find("got 'Text'") != std::string::npos);
        }
    }

    EXPECT_TRUE(found_param0_error);
    EXPECT_TRUE(found_param1_error);
}

//=============================================================================
// Protocol Validation - Return Type Errors
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, WrongReturnTypeValidation) {
    // Object with wrong return type should fail validation
    EXPECT_FALSE(protocol->isImplementedBy(wrong_return_type_object));

    // Should report return type mismatch
    std::vector<std::string> errors = protocol->getValidationErrors(wrong_return_type_object);
    EXPECT_GE(errors.size(), 1);

    bool found_return_type_error = false;
    for (const auto& error : errors) {
        if (error.find("add") != std::string::npos &&
            error.find("return type mismatch") != std::string::npos) {
            found_return_type_error = true;
            EXPECT_TRUE(error.find("expected 'Int'") != std::string::npos);
            EXPECT_TRUE(error.find("got 'Text'") != std::string::npos);
        }
    }

    EXPECT_TRUE(found_return_type_error);
}

//=============================================================================
// Method Signature Validation Tests
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, ValidateMethodSignatureCorrect) {
    // Test correct method signature validation
    std::vector<Parameter> correct_params = {Parameter("x", "Int"), Parameter("y", "Int")};
    EXPECT_TRUE(protocol->validateMethodSignature("add", correct_params, "Int"));

    std::vector<Parameter> correct_multiply_params = {Parameter("a", "Float"),
                                                      Parameter("b", "Float")};
    EXPECT_TRUE(protocol->validateMethodSignature("multiply", correct_multiply_params, "Float"));

    std::vector<Parameter> no_params;
    EXPECT_TRUE(protocol->validateMethodSignature("getName", no_params, "Text"));
}

TEST_F(ProtocolSignatureValidationTest, ValidateMethodSignatureWrongParamCount) {
    // Test wrong parameter count
    std::vector<Parameter> wrong_params = {Parameter("x", "Int")};  // Should be 2 params
    EXPECT_FALSE(protocol->validateMethodSignature("add", wrong_params, "Int"));

    std::vector<Parameter> too_many_params = {Parameter("a", "Int"), Parameter("b", "Int"),
                                              Parameter("c", "Int")};
    EXPECT_FALSE(protocol->validateMethodSignature("add", too_many_params, "Int"));
}

TEST_F(ProtocolSignatureValidationTest, ValidateMethodSignatureWrongParamType) {
    // Test wrong parameter types
    std::vector<Parameter> wrong_type_params = {Parameter("x", "Text"), Parameter("y", "Text")};
    EXPECT_FALSE(protocol->validateMethodSignature("add", wrong_type_params, "Int"));

    std::vector<Parameter> mixed_wrong_params = {Parameter("x", "Int"), Parameter("y", "Text")};
    EXPECT_FALSE(protocol->validateMethodSignature("add", mixed_wrong_params, "Int"));
}

TEST_F(ProtocolSignatureValidationTest, ValidateMethodSignatureWrongReturnType) {
    // Test wrong return type
    std::vector<Parameter> correct_params = {Parameter("x", "Int"), Parameter("y", "Int")};
    EXPECT_FALSE(
        protocol->validateMethodSignature("add", correct_params, "Text"));  // Should be Int
    EXPECT_FALSE(protocol->validateMethodSignature("add", correct_params, "Float"));
    EXPECT_FALSE(protocol->validateMethodSignature("add", correct_params, "Bool"));
}

TEST_F(ProtocolSignatureValidationTest, ValidateMethodSignatureNonexistentMethod) {
    // Test validation of method that doesn't exist in protocol
    std::vector<Parameter> some_params = {Parameter("x", "Int")};
    EXPECT_FALSE(protocol->validateMethodSignature("nonexistentMethod", some_params, "Int"));
}

//=============================================================================
// Edge Cases and Error Handling
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, EmptyProtocolValidation) {
    // Create empty protocol
    std::vector<ProtocolMethodSignature> empty_signatures;
    auto empty_protocol = std::make_shared<ProtocolInstance>("EmptyProtocol", empty_signatures);

    // Any object should satisfy empty protocol
    EXPECT_TRUE(empty_protocol->isImplementedBy(correct_object));
    EXPECT_TRUE(empty_protocol->isImplementedBy(missing_method_object));

    // Should have no errors
    std::vector<std::string> errors = empty_protocol->getValidationErrors(correct_object);
    EXPECT_TRUE(errors.empty());
}

TEST_F(ProtocolSignatureValidationTest, NullObjectValidation) {
    // Test with null object should handle gracefully
    std::shared_ptr<ObjectInstance> null_object;

    // Null object should not implement any protocol
    EXPECT_FALSE(protocol->isImplementedBy(null_object));

    // Should get meaningful error message for null object
    std::vector<std::string> errors = protocol->getValidationErrors(null_object);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_TRUE(errors[0].find("null") != std::string::npos);
}

TEST_F(ProtocolSignatureValidationTest, ParameterNameIgnoring) {
    // Parameter names should not matter, only types
    std::vector<Parameter> different_names = {Parameter("first", "Int"),
                                              Parameter("second", "Int")};
    EXPECT_TRUE(protocol->validateMethodSignature("add", different_names, "Int"));

    std::vector<Parameter> weird_names = {Parameter("🎯", "Int"), Parameter("xyz123", "Int")};
    EXPECT_TRUE(protocol->validateMethodSignature("add", weird_names, "Int"));
}

//=============================================================================
// Complex Protocol Validation Tests
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, ComplexProtocolWithMultipleErrors) {
    // Create object with multiple different types of errors
    auto multi_error_object = std::make_shared<ObjectInstance>("MultiErrorCalculator");

    // Missing 'reset' method entirely
    // Wrong param count for 'add' (1 instead of 2)
    std::vector<Parameter> wrong_add_params = {Parameter("x", "Int")};
    multi_error_object->addMethod("add", createDummyMethod(), wrong_add_params, "Int", true);

    // Wrong param type for 'multiply' (Int instead of Float)
    std::vector<Parameter> wrong_multiply_params = {Parameter("x", "Int"), Parameter("y", "Int")};
    multi_error_object->addMethod("multiply", createDummyMethod(), wrong_multiply_params, "Float",
                                  true);

    // Wrong return type for 'getName' (Int instead of Text)
    std::vector<Parameter> getName_params;
    multi_error_object->addMethod("getName", createDummyMethod(), getName_params, "Int", true);

    // Correct method
    std::vector<Parameter> isReady_params;
    multi_error_object->addMethod("isReady", createDummyMethod(), isReady_params, "Bool", true);

    // Should fail validation
    EXPECT_FALSE(protocol->isImplementedBy(multi_error_object));

    // Should report multiple errors
    std::vector<std::string> errors = protocol->getValidationErrors(multi_error_object);
    EXPECT_GE(errors.size(), 4);  // At least 4 different errors

    // Check for different types of errors
    bool has_missing_method = false;
    bool has_param_count_error = false;
    bool has_param_type_error = false;
    bool has_return_type_error = false;

    for (const auto& error : errors) {
        if (error.find("Missing method: reset") != std::string::npos) {
            has_missing_method = true;
        }
        if (error.find("parameter count mismatch") != std::string::npos) {
            has_param_count_error = true;
        }
        if (error.find("parameter") != std::string::npos &&
            error.find("type mismatch") != std::string::npos) {
            has_param_type_error = true;
        }
        if (error.find("return type mismatch") != std::string::npos) {
            has_return_type_error = true;
        }
    }

    EXPECT_TRUE(has_missing_method);
    EXPECT_TRUE(has_param_count_error);
    EXPECT_TRUE(has_param_type_error);
    EXPECT_TRUE(has_return_type_error);
}

//=============================================================================
// Performance and Scalability Tests
//=============================================================================

TEST_F(ProtocolSignatureValidationTest, LargeProtocolPerformance) {
    // Create a protocol with many methods
    std::vector<ProtocolMethodSignature> large_signatures;

    for (int i = 0; i < 100; ++i) {
        std::string method_name = "method" + std::to_string(i);
        std::vector<Parameter> params = {Parameter("param", "Int")};
        large_signatures.emplace_back(method_name, params, "Int");
    }

    auto large_protocol = std::make_shared<ProtocolInstance>("LargeProtocol", large_signatures);

    // Create object implementing all methods
    auto large_object = std::make_shared<ObjectInstance>("LargeObject");
    for (int i = 0; i < 100; ++i) {
        std::string method_name = "method" + std::to_string(i);
        std::vector<Parameter> params = {Parameter("param", "Int")};
        large_object->addMethod(method_name, createDummyMethod(), params, "Int", true);
    }

    // Validation should still work efficiently
    EXPECT_TRUE(large_protocol->isImplementedBy(large_object));

    std::vector<std::string> errors = large_protocol->getValidationErrors(large_object);
    EXPECT_TRUE(errors.empty());
}