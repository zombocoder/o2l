#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <numbers>

#include "../src/Common/Exceptions.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/MathLibrary.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

class MathLibraryTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to call math methods
    Value callMathMethod(const std::string& method_name, const std::vector<Value>& args = {}) {
        auto math_object = MathLibrary::createMathObject();
        EXPECT_TRUE(math_object->hasMethod(method_name))
            << "Method " << method_name << " not found";
        return math_object->callMethod(method_name, args, context);
    }

    // Helper to check double equality with tolerance
    void expectDoubleNear(const Value& result, double expected, double tolerance = 1e-10) {
        ASSERT_TRUE(std::holds_alternative<Double>(result));
        EXPECT_NEAR(std::get<Double>(result), expected, tolerance);
    }

    // Helper to check integer equality
    void expectInt(const Value& result, Int expected) {
        ASSERT_TRUE(std::holds_alternative<Int>(result));
        EXPECT_EQ(std::get<Int>(result), expected);
    }

    // Helper to check long equality
    void expectLong(const Value& result, Long expected) {
        ASSERT_TRUE(std::holds_alternative<Long>(result));
        EXPECT_EQ(std::get<Long>(result), expected);
    }

    // Helper to check boolean equality
    void expectBool(const Value& result, Bool expected) {
        ASSERT_TRUE(std::holds_alternative<Bool>(result));
        EXPECT_EQ(std::get<Bool>(result), expected);
    }
};

// Test mathematical constants
TEST_F(MathLibraryTest, MathematicalConstants) {
    // Test pi
    Value pi_result = callMathMethod("pi");
    expectDoubleNear(pi_result, std::numbers::pi);

    // Test e
    Value e_result = callMathMethod("e");
    expectDoubleNear(e_result, std::numbers::e);

    // Test tau
    Value tau_result = callMathMethod("tau");
    expectDoubleNear(tau_result, 2.0 * std::numbers::pi);

    // Test infinity
    Value inf_result = callMathMethod("inf");
    ASSERT_TRUE(std::holds_alternative<Double>(inf_result));
    EXPECT_TRUE(std::isinf(std::get<Double>(inf_result)));

    // Test NaN
    Value nan_result = callMathMethod("nan");
    ASSERT_TRUE(std::holds_alternative<Double>(nan_result));
    EXPECT_TRUE(std::isnan(std::get<Double>(nan_result)));

    // Constants should take no arguments
    EXPECT_THROW(callMathMethod("pi", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callMathMethod("e", {Value(Int(1))}), EvaluationError);
}

// Test basic utility functions
TEST_F(MathLibraryTest, BasicUtilityFunctions) {
    // Test abs with different numeric types
    expectInt(callMathMethod("abs", {Value(Int(-5))}), 5);
    expectInt(callMathMethod("abs", {Value(Int(5))}), 5);
    expectLong(callMathMethod("abs", {Value(Long(-1000000000L))}), 1000000000L);

    Value float_abs = callMathMethod("abs", {Value(Float(-3.5f))});
    ASSERT_TRUE(std::holds_alternative<Float>(float_abs));
    EXPECT_FLOAT_EQ(std::get<Float>(float_abs), 3.5f);

    Value double_abs = callMathMethod("abs", {Value(Double(-2.7))});
    ASSERT_TRUE(std::holds_alternative<Double>(double_abs));
    EXPECT_DOUBLE_EQ(std::get<Double>(double_abs), 2.7);

    // Test ceil
    expectDoubleNear(callMathMethod("ceil", {Value(Double(3.2))}), 4.0);
    expectDoubleNear(callMathMethod("ceil", {Value(Double(-3.2))}), -3.0);
    expectDoubleNear(callMathMethod("ceil", {Value(Double(5.0))}), 5.0);

    // Test floor
    expectDoubleNear(callMathMethod("floor", {Value(Double(3.8))}), 3.0);
    expectDoubleNear(callMathMethod("floor", {Value(Double(-3.2))}), -4.0);
    expectDoubleNear(callMathMethod("floor", {Value(Double(5.0))}), 5.0);

    // Test round
    expectDoubleNear(callMathMethod("round", {Value(Double(3.6))}), 4.0);
    expectDoubleNear(callMathMethod("round", {Value(Double(3.4))}), 3.0);
    expectDoubleNear(callMathMethod("round", {Value(Double(-3.6))}), -4.0);

    // Test trunc
    expectDoubleNear(callMathMethod("trunc", {Value(Double(3.8))}), 3.0);
    expectDoubleNear(callMathMethod("trunc", {Value(Double(-3.8))}), -3.0);

    // Test sign
    expectInt(callMathMethod("sign", {Value(Double(5.0))}), 1);
    expectInt(callMathMethod("sign", {Value(Double(-5.0))}), -1);
    expectInt(callMathMethod("sign", {Value(Double(0.0))}), 0);
}

// Test min and max functions
TEST_F(MathLibraryTest, MinMaxFunctions) {
    // Test max with multiple arguments
    expectDoubleNear(
        callMathMethod("max", {Value(Double(1.0)), Value(Double(3.0)), Value(Double(2.0))}), 3.0);
    expectDoubleNear(callMathMethod("max", {Value(Int(5)), Value(Int(2))}), 5.0);

    // Test min with multiple arguments
    expectDoubleNear(
        callMathMethod("min", {Value(Double(1.0)), Value(Double(3.0)), Value(Double(2.0))}), 1.0);
    expectDoubleNear(callMathMethod("min", {Value(Int(5)), Value(Int(2))}), 2.0);

    // Test error conditions
    EXPECT_THROW(callMathMethod("max", {Value(Int(1))}), EvaluationError);  // Too few arguments
    EXPECT_THROW(callMathMethod("min", {Value(Int(1))}), EvaluationError);  // Too few arguments
}

// Test power and exponential functions
TEST_F(MathLibraryTest, PowerExponentialFunctions) {
    // Test pow
    expectDoubleNear(callMathMethod("pow", {Value(Double(2.0)), Value(Double(3.0))}), 8.0);
    expectDoubleNear(callMathMethod("pow", {Value(Double(4.0)), Value(Double(0.5))}), 2.0);

    // Test sqrt
    expectDoubleNear(callMathMethod("sqrt", {Value(Double(16.0))}), 4.0);
    expectDoubleNear(callMathMethod("sqrt", {Value(Double(2.0))}), std::sqrt(2.0));
    EXPECT_THROW(callMathMethod("sqrt", {Value(Double(-1.0))}),
                 EvaluationError);  // Negative argument

    // Test cbrt
    expectDoubleNear(callMathMethod("cbrt", {Value(Double(8.0))}), 2.0);
    expectDoubleNear(callMathMethod("cbrt", {Value(Double(-8.0))}), -2.0);

    // Test exp
    expectDoubleNear(callMathMethod("exp", {Value(Double(0.0))}), 1.0);
    expectDoubleNear(callMathMethod("exp", {Value(Double(1.0))}), std::numbers::e, 1e-10);

    // Test exp2
    expectDoubleNear(callMathMethod("exp2", {Value(Double(3.0))}), 8.0);

    // Test expm1
    expectDoubleNear(callMathMethod("expm1", {Value(Double(0.0))}), 0.0);
}

// Test logarithmic functions
TEST_F(MathLibraryTest, LogarithmicFunctions) {
    // Test log (natural logarithm)
    expectDoubleNear(callMathMethod("log", {Value(Double(std::numbers::e))}), 1.0);
    expectDoubleNear(callMathMethod("log", {Value(Double(1.0))}), 0.0);
    EXPECT_THROW(callMathMethod("log", {Value(Double(0.0))}), EvaluationError);   // Non-positive
    EXPECT_THROW(callMathMethod("log", {Value(Double(-1.0))}), EvaluationError);  // Non-positive

    // Test log2
    expectDoubleNear(callMathMethod("log2", {Value(Double(8.0))}), 3.0);
    expectDoubleNear(callMathMethod("log2", {Value(Double(1.0))}), 0.0);
    EXPECT_THROW(callMathMethod("log2", {Value(Double(0.0))}), EvaluationError);

    // Test log10
    expectDoubleNear(callMathMethod("log10", {Value(Double(100.0))}), 2.0);
    expectDoubleNear(callMathMethod("log10", {Value(Double(1.0))}), 0.0);
    EXPECT_THROW(callMathMethod("log10", {Value(Double(0.0))}), EvaluationError);

    // Test log1p
    expectDoubleNear(callMathMethod("log1p", {Value(Double(0.0))}), 0.0);
    EXPECT_THROW(callMathMethod("log1p", {Value(Double(-1.0))}), EvaluationError);  // <= -1
}

// Test trigonometric functions
TEST_F(MathLibraryTest, TrigonometricFunctions) {
    // Test sin
    expectDoubleNear(callMathMethod("sin", {Value(Double(0.0))}), 0.0);
    expectDoubleNear(callMathMethod("sin", {Value(Double(std::numbers::pi / 2))}), 1.0);
    expectDoubleNear(callMathMethod("sin", {Value(Double(std::numbers::pi))}), 0.0, 1e-15);

    // Test cos
    expectDoubleNear(callMathMethod("cos", {Value(Double(0.0))}), 1.0);
    expectDoubleNear(callMathMethod("cos", {Value(Double(std::numbers::pi / 2))}), 0.0, 1e-15);
    expectDoubleNear(callMathMethod("cos", {Value(Double(std::numbers::pi))}), -1.0);

    // Test tan
    expectDoubleNear(callMathMethod("tan", {Value(Double(0.0))}), 0.0);
    expectDoubleNear(callMathMethod("tan", {Value(Double(std::numbers::pi / 4))}), 1.0);

    // Test asin
    expectDoubleNear(callMathMethod("asin", {Value(Double(0.0))}), 0.0);
    expectDoubleNear(callMathMethod("asin", {Value(Double(1.0))}), std::numbers::pi / 2);
    EXPECT_THROW(callMathMethod("asin", {Value(Double(2.0))}), EvaluationError);  // Out of range

    // Test acos
    expectDoubleNear(callMathMethod("acos", {Value(Double(1.0))}), 0.0);
    expectDoubleNear(callMathMethod("acos", {Value(Double(0.0))}), std::numbers::pi / 2);
    EXPECT_THROW(callMathMethod("acos", {Value(Double(2.0))}), EvaluationError);  // Out of range

    // Test atan
    expectDoubleNear(callMathMethod("atan", {Value(Double(0.0))}), 0.0);
    expectDoubleNear(callMathMethod("atan", {Value(Double(1.0))}), std::numbers::pi / 4);

    // Test atan2
    expectDoubleNear(callMathMethod("atan2", {Value(Double(1.0)), Value(Double(1.0))}),
                     std::numbers::pi / 4);
    expectDoubleNear(callMathMethod("atan2", {Value(Double(0.0)), Value(Double(1.0))}), 0.0);
}

// Test hyperbolic functions
TEST_F(MathLibraryTest, HyperbolicFunctions) {
    // Test sinh
    expectDoubleNear(callMathMethod("sinh", {Value(Double(0.0))}), 0.0);

    // Test cosh
    expectDoubleNear(callMathMethod("cosh", {Value(Double(0.0))}), 1.0);

    // Test tanh
    expectDoubleNear(callMathMethod("tanh", {Value(Double(0.0))}), 0.0);

    // Test asinh
    expectDoubleNear(callMathMethod("asinh", {Value(Double(0.0))}), 0.0);

    // Test acosh
    expectDoubleNear(callMathMethod("acosh", {Value(Double(1.0))}), 0.0);
    EXPECT_THROW(callMathMethod("acosh", {Value(Double(0.5))}), EvaluationError);  // < 1

    // Test atanh
    expectDoubleNear(callMathMethod("atanh", {Value(Double(0.0))}), 0.0);
    EXPECT_THROW(callMathMethod("atanh", {Value(Double(1.0))}), EvaluationError);   // >= 1
    EXPECT_THROW(callMathMethod("atanh", {Value(Double(-1.0))}), EvaluationError);  // <= -1
}

// Test angular conversion functions
TEST_F(MathLibraryTest, AngularConversionFunctions) {
    // Test radians (degrees to radians)
    expectDoubleNear(callMathMethod("radians", {Value(Double(180.0))}), std::numbers::pi);
    expectDoubleNear(callMathMethod("radians", {Value(Double(90.0))}), std::numbers::pi / 2);
    expectDoubleNear(callMathMethod("radians", {Value(Double(0.0))}), 0.0);

    // Test degrees (radians to degrees)
    expectDoubleNear(callMathMethod("degrees", {Value(Double(std::numbers::pi))}), 180.0);
    expectDoubleNear(callMathMethod("degrees", {Value(Double(std::numbers::pi / 2))}), 90.0);
    expectDoubleNear(callMathMethod("degrees", {Value(Double(0.0))}), 0.0);
}

// Test special functions
TEST_F(MathLibraryTest, SpecialFunctions) {
    // Test factorial
    EXPECT_EQ(std::get<Long>(callMathMethod("factorial", {Value(Int(5))})), 120L);

    // Test more factorial cases
    EXPECT_EQ(std::get<Long>(callMathMethod("factorial", {Value(Int(0))})), 1L);
    EXPECT_EQ(std::get<Long>(callMathMethod("factorial", {Value(Int(1))})), 1L);
    EXPECT_EQ(std::get<Long>(callMathMethod("factorial", {Value(Int(10))})), 3628800L);
    EXPECT_THROW(callMathMethod("factorial", {Value(Int(-1))}), EvaluationError);  // Negative
    EXPECT_THROW(callMathMethod("factorial", {Value(Int(25))}), EvaluationError);  // Too large

    // Test gcd
    expectInt(callMathMethod("gcd", {Value(Int(12)), Value(Int(18))}), 6);
    expectInt(callMathMethod("gcd", {Value(Int(17)), Value(Int(19))}), 1);
    expectInt(callMathMethod("gcd", {Value(Int(-12)), Value(Int(18))}), 6);  // Handles negatives
    expectInt(callMathMethod("gcd", {Value(Int(0)), Value(Int(5))}), 5);

    // Test lcm
    EXPECT_EQ(std::get<Long>(callMathMethod("lcm", {Value(Int(4)), Value(Int(6))})), 12L);
    EXPECT_EQ(std::get<Long>(callMathMethod("lcm", {Value(Int(12)), Value(Int(18))})), 36L);
    EXPECT_EQ(std::get<Long>(callMathMethod("lcm", {Value(Int(0)), Value(Int(5))})), 0L);

    // Test gamma and lgamma (basic functionality)
    Value gamma_result = callMathMethod("gamma", {Value(Double(1.0))});
    expectDoubleNear(gamma_result, 1.0);  // gamma(1) = 0! = 1

    Value lgamma_result = callMathMethod("lgamma", {Value(Double(1.0))});
    expectDoubleNear(lgamma_result, 0.0);  // log(gamma(1)) = log(1) = 0
}

// Test error and classification functions
TEST_F(MathLibraryTest, ErrorAndClassificationFunctions) {
    // Test erf and erfc
    expectDoubleNear(callMathMethod("erf", {Value(Double(0.0))}), 0.0);
    expectDoubleNear(callMathMethod("erfc", {Value(Double(0.0))}), 1.0);

    // Test classification functions
    expectBool(callMathMethod("isFinite", {Value(Double(5.0))}), true);
    expectBool(callMathMethod("isFinite", {callMathMethod("inf")}), false);
    expectBool(callMathMethod("isFinite", {callMathMethod("nan")}), false);

    expectBool(callMathMethod("isInf", {callMathMethod("inf")}), true);
    expectBool(callMathMethod("isInf", {Value(Double(5.0))}), false);

    expectBool(callMathMethod("isNan", {callMathMethod("nan")}), true);
    expectBool(callMathMethod("isNan", {Value(Double(5.0))}), false);

    expectBool(callMathMethod("isNormal", {Value(Double(5.0))}), true);
    expectBool(callMathMethod("isNormal", {Value(Double(0.0))}), false);
    expectBool(callMathMethod("isNormal", {callMathMethod("inf")}), false);
}

// Test argument validation
TEST_F(MathLibraryTest, ArgumentValidation) {
    // Test functions that require exactly one argument
    EXPECT_THROW(callMathMethod("sqrt", {}), EvaluationError);  // No arguments
    EXPECT_THROW(callMathMethod("sqrt", {Value(Int(1)), Value(Int(2))}),
                 EvaluationError);  // Too many

    // Test functions that require exactly two arguments
    EXPECT_THROW(callMathMethod("pow", {Value(Int(2))}), EvaluationError);  // Too few
    EXPECT_THROW(callMathMethod("pow", {Value(Int(2)), Value(Int(3)), Value(Int(4))}),
                 EvaluationError);  // Too many

    // Test functions that require numeric arguments
    EXPECT_THROW(callMathMethod("sqrt", {Value(Text("invalid"))}), EvaluationError);
    EXPECT_THROW(callMathMethod("abs", {Value(Bool(true))}), EvaluationError);
}

// Test edge cases and boundary conditions
TEST_F(MathLibraryTest, EdgeCases) {
    // Test with very small numbers
    expectDoubleNear(callMathMethod("sqrt", {Value(Double(1e-100))}), 1e-50);

    // Test with very large numbers (within reasonable bounds)
    expectDoubleNear(callMathMethod("pow", {Value(Double(10.0)), Value(Double(10.0))}), 1e10);

    // Test mixed integer and floating point types
    expectDoubleNear(callMathMethod("pow", {Value(Int(2)), Value(Double(3.5))}), std::pow(2.0, 3.5),
                     1e-10);
    expectDoubleNear(callMathMethod("sqrt", {Value(Int(25))}), 5.0);
}