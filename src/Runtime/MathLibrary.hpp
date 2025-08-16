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

#pragma once

#include <cmath>
#include <memory>
#include <numbers>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

class MathLibrary {
   public:
    // Create the math object with native methods
    static std::shared_ptr<ObjectInstance> createMathObject();

    // Mathematical constants as static methods
    static Value getPi(const std::vector<Value>& args, Context& context);
    static Value getE(const std::vector<Value>& args, Context& context);
    static Value getTau(const std::vector<Value>& args, Context& context);
    static Value getInf(const std::vector<Value>& args, Context& context);
    static Value getNan(const std::vector<Value>& args, Context& context);

    // Basic utility functions
    static Value nativeAbs(const std::vector<Value>& args, Context& context);
    static Value nativeCeil(const std::vector<Value>& args, Context& context);
    static Value nativeFloor(const std::vector<Value>& args, Context& context);
    static Value nativeRound(const std::vector<Value>& args, Context& context);
    static Value nativeTrunc(const std::vector<Value>& args, Context& context);
    static Value nativeSign(const std::vector<Value>& args, Context& context);
    static Value nativeMax(const std::vector<Value>& args, Context& context);
    static Value nativeMin(const std::vector<Value>& args, Context& context);

    // Power and exponential functions
    static Value nativePow(const std::vector<Value>& args, Context& context);
    static Value nativeSqrt(const std::vector<Value>& args, Context& context);
    static Value nativeCbrt(const std::vector<Value>& args, Context& context);
    static Value nativeExp(const std::vector<Value>& args, Context& context);
    static Value nativeExp2(const std::vector<Value>& args, Context& context);
    static Value nativeExpm1(const std::vector<Value>& args, Context& context);

    // Logarithmic functions
    static Value nativeLog(const std::vector<Value>& args, Context& context);
    static Value nativeLog2(const std::vector<Value>& args, Context& context);
    static Value nativeLog10(const std::vector<Value>& args, Context& context);
    static Value nativeLog1p(const std::vector<Value>& args, Context& context);

    // Trigonometric functions
    static Value nativeSin(const std::vector<Value>& args, Context& context);
    static Value nativeCos(const std::vector<Value>& args, Context& context);
    static Value nativeTan(const std::vector<Value>& args, Context& context);
    static Value nativeAsin(const std::vector<Value>& args, Context& context);
    static Value nativeAcos(const std::vector<Value>& args, Context& context);
    static Value nativeAtan(const std::vector<Value>& args, Context& context);
    static Value nativeAtan2(const std::vector<Value>& args, Context& context);

    // Hyperbolic functions
    static Value nativeSinh(const std::vector<Value>& args, Context& context);
    static Value nativeCosh(const std::vector<Value>& args, Context& context);
    static Value nativeTanh(const std::vector<Value>& args, Context& context);
    static Value nativeAsinh(const std::vector<Value>& args, Context& context);
    static Value nativeAcosh(const std::vector<Value>& args, Context& context);
    static Value nativeAtanh(const std::vector<Value>& args, Context& context);

    // Angular conversion functions
    static Value nativeRadians(const std::vector<Value>& args, Context& context);
    static Value nativeDegrees(const std::vector<Value>& args, Context& context);

    // Special functions
    static Value nativeFactorial(const std::vector<Value>& args, Context& context);
    static Value nativeGcd(const std::vector<Value>& args, Context& context);
    static Value nativeLcm(const std::vector<Value>& args, Context& context);
    static Value nativeGamma(const std::vector<Value>& args, Context& context);
    static Value nativeLgamma(const std::vector<Value>& args, Context& context);

    // Error and gamma functions
    static Value nativeErf(const std::vector<Value>& args, Context& context);
    static Value nativeErfc(const std::vector<Value>& args, Context& context);

    // Classification functions
    static Value nativeIsFinite(const std::vector<Value>& args, Context& context);
    static Value nativeIsInf(const std::vector<Value>& args, Context& context);
    static Value nativeIsNan(const std::vector<Value>& args, Context& context);
    static Value nativeIsNormal(const std::vector<Value>& args, Context& context);

   private:
    // Helper functions
    static double extractNumber(const Value& value, const std::string& function_name,
                                Context& context);
    static Int extractInteger(const Value& value, const std::string& function_name,
                              Context& context);
    static Value createNumberResult(double result);

    // Factorial helper for integers
    static Int computeFactorial(Int n);

    // GCD helper
    static Int computeGcd(Int a, Int b);
};

}  // namespace o2l