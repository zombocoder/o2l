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

#include "MathLibrary.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>
#include <numeric>

#include "../Common/Exceptions.hpp"
#include "ListInstance.hpp"

namespace o2l {

std::shared_ptr<ObjectInstance> MathLibrary::createMathObject() {
    auto math_object = std::make_shared<ObjectInstance>("math");

    // Mathematical constants
    Method pi_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::getPi(args, ctx);
    };
    math_object->addMethod("pi", pi_method, true);  // external

    Method e_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::getE(args, ctx);
    };
    math_object->addMethod("e", e_method, true);  // external

    Method tau_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::getTau(args, ctx);
    };
    math_object->addMethod("tau", tau_method, true);  // external

    Method inf_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::getInf(args, ctx);
    };
    math_object->addMethod("inf", inf_method, true);  // external

    Method nan_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::getNan(args, ctx);
    };
    math_object->addMethod("nan", nan_method, true);  // external

    // Basic utility functions
    Method abs_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAbs(args, ctx);
    };
    math_object->addMethod("abs", abs_method, true);  // external

    Method ceil_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeCeil(args, ctx);
    };
    math_object->addMethod("ceil", ceil_method, true);  // external

    Method floor_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeFloor(args, ctx);
    };
    math_object->addMethod("floor", floor_method, true);  // external

    Method round_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeRound(args, ctx);
    };
    math_object->addMethod("round", round_method, true);  // external

    Method trunc_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeTrunc(args, ctx);
    };
    math_object->addMethod("trunc", trunc_method, true);  // external

    Method sign_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeSign(args, ctx);
    };
    math_object->addMethod("sign", sign_method, true);  // external

    Method max_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeMax(args, ctx);
    };
    math_object->addMethod("max", max_method, true);  // external

    Method min_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeMin(args, ctx);
    };
    math_object->addMethod("min", min_method, true);  // external

    // Power and exponential functions
    Method pow_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativePow(args, ctx);
    };
    math_object->addMethod("pow", pow_method, true);  // external

    Method sqrt_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeSqrt(args, ctx);
    };
    math_object->addMethod("sqrt", sqrt_method, true);  // external

    Method cbrt_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeCbrt(args, ctx);
    };
    math_object->addMethod("cbrt", cbrt_method, true);  // external

    Method exp_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeExp(args, ctx);
    };
    math_object->addMethod("exp", exp_method, true);  // external

    Method exp2_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeExp2(args, ctx);
    };
    math_object->addMethod("exp2", exp2_method, true);  // external

    Method expm1_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeExpm1(args, ctx);
    };
    math_object->addMethod("expm1", expm1_method, true);  // external

    // Logarithmic functions
    Method log_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeLog(args, ctx);
    };
    math_object->addMethod("log", log_method, true);  // external

    Method log2_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeLog2(args, ctx);
    };
    math_object->addMethod("log2", log2_method, true);  // external

    Method log10_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeLog10(args, ctx);
    };
    math_object->addMethod("log10", log10_method, true);  // external

    Method log1p_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeLog1p(args, ctx);
    };
    math_object->addMethod("log1p", log1p_method, true);  // external

    // Trigonometric functions
    Method sin_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeSin(args, ctx);
    };
    math_object->addMethod("sin", sin_method, true);  // external

    Method cos_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeCos(args, ctx);
    };
    math_object->addMethod("cos", cos_method, true);  // external

    Method tan_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeTan(args, ctx);
    };
    math_object->addMethod("tan", tan_method, true);  // external

    Method asin_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAsin(args, ctx);
    };
    math_object->addMethod("asin", asin_method, true);  // external

    Method acos_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAcos(args, ctx);
    };
    math_object->addMethod("acos", acos_method, true);  // external

    Method atan_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAtan(args, ctx);
    };
    math_object->addMethod("atan", atan_method, true);  // external

    Method atan2_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAtan2(args, ctx);
    };
    math_object->addMethod("atan2", atan2_method, true);  // external

    // Hyperbolic functions
    Method sinh_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeSinh(args, ctx);
    };
    math_object->addMethod("sinh", sinh_method, true);  // external

    Method cosh_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeCosh(args, ctx);
    };
    math_object->addMethod("cosh", cosh_method, true);  // external

    Method tanh_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeTanh(args, ctx);
    };
    math_object->addMethod("tanh", tanh_method, true);  // external

    Method asinh_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAsinh(args, ctx);
    };
    math_object->addMethod("asinh", asinh_method, true);  // external

    Method acosh_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAcosh(args, ctx);
    };
    math_object->addMethod("acosh", acosh_method, true);  // external

    Method atanh_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeAtanh(args, ctx);
    };
    math_object->addMethod("atanh", atanh_method, true);  // external

    // Angular conversion functions
    Method radians_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeRadians(args, ctx);
    };
    math_object->addMethod("radians", radians_method, true);  // external

    Method degrees_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeDegrees(args, ctx);
    };
    math_object->addMethod("degrees", degrees_method, true);  // external

    // Special functions
    Method factorial_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeFactorial(args, ctx);
    };
    math_object->addMethod("factorial", factorial_method, true);  // external

    Method gcd_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeGcd(args, ctx);
    };
    math_object->addMethod("gcd", gcd_method, true);  // external

    Method lcm_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeLcm(args, ctx);
    };
    math_object->addMethod("lcm", lcm_method, true);  // external

    Method gamma_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeGamma(args, ctx);
    };
    math_object->addMethod("gamma", gamma_method, true);  // external

    Method lgamma_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeLgamma(args, ctx);
    };
    math_object->addMethod("lgamma", lgamma_method, true);  // external

    // Error functions
    Method erf_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeErf(args, ctx);
    };
    math_object->addMethod("erf", erf_method, true);  // external

    Method erfc_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeErfc(args, ctx);
    };
    math_object->addMethod("erfc", erfc_method, true);  // external

    // Classification functions
    Method isFinite_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeIsFinite(args, ctx);
    };
    math_object->addMethod("isFinite", isFinite_method, true);  // external

    Method isInf_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeIsInf(args, ctx);
    };
    math_object->addMethod("isInf", isInf_method, true);  // external

    Method isNan_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeIsNan(args, ctx);
    };
    math_object->addMethod("isNan", isNan_method, true);  // external

    Method isNormal_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return MathLibrary::nativeIsNormal(args, ctx);
    };
    math_object->addMethod("isNormal", isNormal_method, true);  // external

    return math_object;
}

// Mathematical constants
Value MathLibrary::getPi(const std::vector<Value>& args, Context& context) {
    if (!args.empty()) {
        throw EvaluationError("math.pi() takes no arguments", context);
    }
    return Value(Double(std::numbers::pi));
}

Value MathLibrary::getE(const std::vector<Value>& args, Context& context) {
    if (!args.empty()) {
        throw EvaluationError("math.e() takes no arguments", context);
    }
    return Value(Double(std::numbers::e));
}

Value MathLibrary::getTau(const std::vector<Value>& args, Context& context) {
    if (!args.empty()) {
        throw EvaluationError("math.tau() takes no arguments", context);
    }
    return Value(Double(2.0 * std::numbers::pi));
}

Value MathLibrary::getInf(const std::vector<Value>& args, Context& context) {
    if (!args.empty()) {
        throw EvaluationError("math.inf() takes no arguments", context);
    }
    return Value(Double(std::numeric_limits<double>::infinity()));
}

Value MathLibrary::getNan(const std::vector<Value>& args, Context& context) {
    if (!args.empty()) {
        throw EvaluationError("math.nan() takes no arguments", context);
    }
    return Value(Double(std::numeric_limits<double>::quiet_NaN()));
}

// Helper functions
double MathLibrary::extractNumber(const Value& value, const std::string& function_name,
                                  Context& context) {
    if (std::holds_alternative<Int>(value)) {
        return static_cast<double>(std::get<Int>(value));
    } else if (std::holds_alternative<Long>(value)) {
        return static_cast<double>(std::get<Long>(value));
    } else if (std::holds_alternative<Float>(value)) {
        return static_cast<double>(std::get<Float>(value));
    } else if (std::holds_alternative<Double>(value)) {
        return std::get<Double>(value);
    } else {
        throw EvaluationError("math." + function_name + "() requires a numeric argument", context);
    }
}

Int MathLibrary::extractInteger(const Value& value, const std::string& function_name,
                                Context& context) {
    if (std::holds_alternative<Int>(value)) {
        return std::get<Int>(value);
    } else if (std::holds_alternative<Long>(value)) {
        Long long_val = std::get<Long>(value);
        if (long_val > std::numeric_limits<Int>::max() ||
            long_val < std::numeric_limits<Int>::min()) {
            throw EvaluationError("math." + function_name + "() integer argument out of range",
                                  context);
        }
        return static_cast<Int>(long_val);
    } else {
        throw EvaluationError("math." + function_name + "() requires an integer argument", context);
    }
}

Value MathLibrary::createNumberResult(double result) {
    // Return Double for floating point results
    return Value(Double(result));
}

// Basic utility functions
Value MathLibrary::nativeAbs(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.abs() requires exactly one argument", context);
    }

    if (std::holds_alternative<Int>(args[0])) {
        Int val = std::get<Int>(args[0]);
        return Value(Int(std::abs(val)));
    } else if (std::holds_alternative<Long>(args[0])) {
        Long val = std::get<Long>(args[0]);
        return Value(Long(val >= 0 ? val : -val));
    } else if (std::holds_alternative<Float>(args[0])) {
        Float val = std::get<Float>(args[0]);
        return Value(Float(std::abs(val)));
    } else if (std::holds_alternative<Double>(args[0])) {
        Double val = std::get<Double>(args[0]);
        return Value(Double(std::abs(val)));
    } else {
        throw EvaluationError("math.abs() requires a numeric argument", context);
    }
}

Value MathLibrary::nativeCeil(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.ceil() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "ceil", context);
    return createNumberResult(std::ceil(val));
}

Value MathLibrary::nativeFloor(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.floor() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "floor", context);
    return createNumberResult(std::floor(val));
}

Value MathLibrary::nativeRound(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.round() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "round", context);
    return createNumberResult(std::round(val));
}

Value MathLibrary::nativeTrunc(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.trunc() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "trunc", context);
    return createNumberResult(std::trunc(val));
}

Value MathLibrary::nativeSign(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.sign() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "sign", context);
    if (val > 0.0) return Value(Int(1));
    if (val < 0.0) return Value(Int(-1));
    return Value(Int(0));
}

Value MathLibrary::nativeMax(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw EvaluationError("math.max() requires at least two arguments", context);
    }

    double max_val = extractNumber(args[0], "max", context);
    for (size_t i = 1; i < args.size(); ++i) {
        double val = extractNumber(args[i], "max", context);
        max_val = std::max(max_val, val);
    }

    return createNumberResult(max_val);
}

Value MathLibrary::nativeMin(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw EvaluationError("math.min() requires at least two arguments", context);
    }

    double min_val = extractNumber(args[0], "min", context);
    for (size_t i = 1; i < args.size(); ++i) {
        double val = extractNumber(args[i], "min", context);
        min_val = std::min(min_val, val);
    }

    return createNumberResult(min_val);
}

// Power and exponential functions
Value MathLibrary::nativePow(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("math.pow() requires exactly two arguments", context);
    }

    double base = extractNumber(args[0], "pow", context);
    double exponent = extractNumber(args[1], "pow", context);

    return createNumberResult(std::pow(base, exponent));
}

Value MathLibrary::nativeSqrt(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.sqrt() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "sqrt", context);
    if (val < 0.0) {
        throw EvaluationError("math.sqrt() domain error: negative argument", context);
    }

    return createNumberResult(std::sqrt(val));
}

Value MathLibrary::nativeCbrt(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.cbrt() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "cbrt", context);
    return createNumberResult(std::cbrt(val));
}

Value MathLibrary::nativeExp(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.exp() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "exp", context);
    return createNumberResult(std::exp(val));
}

Value MathLibrary::nativeExp2(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.exp2() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "exp2", context);
    return createNumberResult(std::exp2(val));
}

Value MathLibrary::nativeExpm1(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.expm1() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "expm1", context);
    return createNumberResult(std::expm1(val));
}

// Logarithmic functions
Value MathLibrary::nativeLog(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.log() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "log", context);
    if (val <= 0.0) {
        throw EvaluationError("math.log() domain error: non-positive argument", context);
    }

    return createNumberResult(std::log(val));
}

Value MathLibrary::nativeLog2(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.log2() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "log2", context);
    if (val <= 0.0) {
        throw EvaluationError("math.log2() domain error: non-positive argument", context);
    }

    return createNumberResult(std::log2(val));
}

Value MathLibrary::nativeLog10(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.log10() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "log10", context);
    if (val <= 0.0) {
        throw EvaluationError("math.log10() domain error: non-positive argument", context);
    }

    return createNumberResult(std::log10(val));
}

Value MathLibrary::nativeLog1p(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.log1p() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "log1p", context);
    if (val <= -1.0) {
        throw EvaluationError("math.log1p() domain error: argument <= -1", context);
    }

    return createNumberResult(std::log1p(val));
}

// Trigonometric functions
Value MathLibrary::nativeSin(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.sin() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "sin", context);
    return createNumberResult(std::sin(val));
}

Value MathLibrary::nativeCos(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.cos() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "cos", context);
    return createNumberResult(std::cos(val));
}

Value MathLibrary::nativeTan(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.tan() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "tan", context);
    return createNumberResult(std::tan(val));
}

Value MathLibrary::nativeAsin(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.asin() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "asin", context);
    if (val < -1.0 || val > 1.0) {
        throw EvaluationError("math.asin() domain error: argument must be in [-1, 1]", context);
    }

    return createNumberResult(std::asin(val));
}

Value MathLibrary::nativeAcos(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.acos() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "acos", context);
    if (val < -1.0 || val > 1.0) {
        throw EvaluationError("math.acos() domain error: argument must be in [-1, 1]", context);
    }

    return createNumberResult(std::acos(val));
}

Value MathLibrary::nativeAtan(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.atan() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "atan", context);
    return createNumberResult(std::atan(val));
}

Value MathLibrary::nativeAtan2(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("math.atan2() requires exactly two arguments", context);
    }

    double y = extractNumber(args[0], "atan2", context);
    double x = extractNumber(args[1], "atan2", context);

    return createNumberResult(std::atan2(y, x));
}

// Hyperbolic functions
Value MathLibrary::nativeSinh(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.sinh() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "sinh", context);
    return createNumberResult(std::sinh(val));
}

Value MathLibrary::nativeCosh(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.cosh() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "cosh", context);
    return createNumberResult(std::cosh(val));
}

Value MathLibrary::nativeTanh(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.tanh() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "tanh", context);
    return createNumberResult(std::tanh(val));
}

Value MathLibrary::nativeAsinh(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.asinh() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "asinh", context);
    return createNumberResult(std::asinh(val));
}

Value MathLibrary::nativeAcosh(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.acosh() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "acosh", context);
    if (val < 1.0) {
        throw EvaluationError("math.acosh() domain error: argument must be >= 1", context);
    }

    return createNumberResult(std::acosh(val));
}

Value MathLibrary::nativeAtanh(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.atanh() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "atanh", context);
    if (val <= -1.0 || val >= 1.0) {
        throw EvaluationError("math.atanh() domain error: argument must be in (-1, 1)", context);
    }

    return createNumberResult(std::atanh(val));
}

// Angular conversion functions
Value MathLibrary::nativeRadians(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.radians() requires exactly one argument", context);
    }

    double degrees = extractNumber(args[0], "radians", context);
    return createNumberResult(degrees * std::numbers::pi / 180.0);
}

Value MathLibrary::nativeDegrees(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.degrees() requires exactly one argument", context);
    }

    double radians = extractNumber(args[0], "degrees", context);
    return createNumberResult(radians * 180.0 / std::numbers::pi);
}

// Special functions
Value MathLibrary::nativeFactorial(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.factorial() requires exactly one argument", context);
    }

    Int n = extractInteger(args[0], "factorial", context);
    if (n < 0) {
        throw EvaluationError("math.factorial() domain error: negative argument", context);
    }
    if (n > 20) {
        throw EvaluationError("math.factorial() overflow: argument too large", context);
    }

    return Value(Long(static_cast<Long>(computeFactorial(n))));
}

Int MathLibrary::computeFactorial(Int n) {
    if (n <= 1) return 1;
    Int result = 1;
    for (Int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

Value MathLibrary::nativeGcd(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("math.gcd() requires exactly two arguments", context);
    }

    Int a = extractInteger(args[0], "gcd", context);
    Int b = extractInteger(args[1], "gcd", context);

    return Value(Int(computeGcd(a >= 0 ? a : -a, b >= 0 ? b : -b)));
}

Int MathLibrary::computeGcd(Int a, Int b) {
    while (b != 0) {
        Int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

Value MathLibrary::nativeLcm(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("math.lcm() requires exactly two arguments", context);
    }

    Int a = extractInteger(args[0], "lcm", context);
    Int b = extractInteger(args[1], "lcm", context);

    if (a == 0 || b == 0) {
        return Value(Long(0));
    }

    Int abs_a = (a >= 0 ? a : -a);
    Int abs_b = (b >= 0 ? b : -b);
    Int gcd_val = computeGcd(abs_a, abs_b);
    return Value(Long(static_cast<Long>(abs_a) * abs_b / gcd_val));
}

Value MathLibrary::nativeGamma(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.gamma() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "gamma", context);
    return createNumberResult(std::tgamma(val));
}

Value MathLibrary::nativeLgamma(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.lgamma() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "lgamma", context);
    return createNumberResult(std::lgamma(val));
}

// Error functions
Value MathLibrary::nativeErf(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.erf() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "erf", context);
    return createNumberResult(std::erf(val));
}

Value MathLibrary::nativeErfc(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.erfc() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "erfc", context);
    return createNumberResult(std::erfc(val));
}

// Classification functions
Value MathLibrary::nativeIsFinite(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.isFinite() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "isFinite", context);
    return Value(Bool(std::isfinite(val)));
}

Value MathLibrary::nativeIsInf(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.isInf() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "isInf", context);
    return Value(Bool(std::isinf(val)));
}

Value MathLibrary::nativeIsNan(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.isNan() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "isNan", context);
    return Value(Bool(std::isnan(val)));
}

Value MathLibrary::nativeIsNormal(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("math.isNormal() requires exactly one argument", context);
    }

    double val = extractNumber(args[0], "isNormal", context);
    return Value(Bool(std::isnormal(val)));
}

}  // namespace o2l