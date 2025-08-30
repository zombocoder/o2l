# Math Library

The Math library provides comprehensive mathematical functions and constants for O²L applications.

## Import

```obq
import math
```

## Mathematical Constants

### `pi() -> Double`
Returns the mathematical constant π (pi).

```obq
pi_value: Double = math.pi()  # 3.141592653589793
```

### `e() -> Double`
Returns the mathematical constant e (Euler's number).

```obq
e_value: Double = math.e()  # 2.718281828459045
```

### `tau() -> Double`
Returns the mathematical constant τ (tau) = 2π.

```obq
tau_value: Double = math.tau()  # 6.283185307179586
```

### `inf() -> Double`
Returns positive infinity.

```obq
infinity: Double = math.inf()
is_infinite: Bool = math.isInf(infinity)  # true
```

### `nan() -> Double`
Returns NaN (Not a Number).

```obq
not_a_number: Double = math.nan()
is_nan: Bool = math.isNan(not_a_number)  # true
```

## Basic Utility Functions

### `abs(x: Number) -> Number`
Returns the absolute value of a number. Preserves the input type.

```obq
abs_int: Int = math.abs(-5)        # 5
abs_long: Long = math.abs(-1000L)   # 1000
abs_float: Float = math.abs(-3.5f)  # 3.5
abs_double: Double = math.abs(-2.7) # 2.7
```

### `ceil(x: Number) -> Double`
Returns the smallest integer greater than or equal to x.

```obq
result: Double = math.ceil(3.2)   # 4.0
result2: Double = math.ceil(-3.2) # -3.0
```

### `floor(x: Number) -> Double`
Returns the largest integer less than or equal to x.

```obq
result: Double = math.floor(3.8)   # 3.0
result2: Double = math.floor(-3.2) # -4.0
```

### `round(x: Number) -> Double`
Returns the nearest integer to x.

```obq
result: Double = math.round(3.6) # 4.0
result2: Double = math.round(3.4) # 3.0
result3: Double = math.round(-3.6) # -4.0
```

### `trunc(x: Number) -> Double`
Returns the integer part of x (truncates towards zero).

```obq
result: Double = math.trunc(3.8)  # 3.0
result2: Double = math.trunc(-3.8) # -3.0
```

### `sign(x: Number) -> Int`
Returns the sign of x: 1 for positive, -1 for negative, 0 for zero.

```obq
sign_pos: Int = math.sign(5.0)  # 1
sign_neg: Int = math.sign(-5.0) # -1
sign_zero: Int = math.sign(0.0) # 0
```

### `max(x: Number, y: Number, ...more: Number) -> Double`
Returns the maximum value from the arguments.

```obq
max_val: Double = math.max(1.0, 3.0, 2.0) # 3.0
max_two: Double = math.max(5, 2)          # 5.0
```

### `min(x: Number, y: Number, ...more: Number) -> Double`
Returns the minimum value from the arguments.

```obq
min_val: Double = math.min(1.0, 3.0, 2.0) # 1.0
min_two: Double = math.min(5, 2)          # 2.0
```

## Power and Exponential Functions

### `pow(base: Number, exponent: Number) -> Double`
Returns base raised to the power of exponent.

```obq
result: Double = math.pow(2.0, 3.0) # 8.0
result2: Double = math.pow(4.0, 0.5) # 2.0
```

### `sqrt(x: Number) -> Double`
Returns the square root of x. Throws error for negative values.

```obq
result: Double = math.sqrt(16.0) # 4.0
result2: Double = math.sqrt(2.0) # 1.4142135623730951
```

### `cbrt(x: Number) -> Double`
Returns the cube root of x.

```obq
result: Double = math.cbrt(8.0)   # 2.0
result2: Double = math.cbrt(-8.0) # -2.0
```

### `exp(x: Number) -> Double`
Returns e raised to the power of x.

```obq
result: Double = math.exp(0.0) # 1.0
result2: Double = math.exp(1.0) # 2.718281828459045
```

### `exp2(x: Number) -> Double`
Returns 2 raised to the power of x.

```obq
result: Double = math.exp2(3.0) # 8.0
```

### `expm1(x: Number) -> Double`
Returns e^x - 1, computed accurately for small values of x.

```obq
result: Double = math.expm1(0.0) # 0.0
```

## Logarithmic Functions

### `log(x: Number) -> Double`
Returns the natural logarithm of x. Throws error for non-positive values.

```obq
result: Double = math.log(math.e()) # 1.0
result2: Double = math.log(1.0)     # 0.0
```

### `log2(x: Number) -> Double`
Returns the base-2 logarithm of x. Throws error for non-positive values.

```obq
result: Double = math.log2(8.0) # 3.0
result2: Double = math.log2(1.0) # 0.0
```

### `log10(x: Number) -> Double`
Returns the base-10 logarithm of x. Throws error for non-positive values.

```obq
result: Double = math.log10(100.0) # 2.0
result2: Double = math.log10(1.0)  # 0.0
```

### `log1p(x: Number) -> Double`
Returns ln(1 + x), computed accurately for small values of x. Throws error for x <= -1.

```obq
result: Double = math.log1p(0.0) # 0.0
```

## Trigonometric Functions

### `sin(x: Number) -> Double`
Returns the sine of x (in radians).

```obq
result: Double = math.sin(0.0)          # 0.0
result2: Double = math.sin(math.pi() / 2) # 1.0
```

### `cos(x: Number) -> Double`
Returns the cosine of x (in radians).

```obq
result: Double = math.cos(0.0)         # 1.0
result2: Double = math.cos(math.pi())  # -1.0
```

### `tan(x: Number) -> Double`
Returns the tangent of x (in radians).

```obq
result: Double = math.tan(0.0)          # 0.0
result2: Double = math.tan(math.pi() / 4) # 1.0
```

### `asin(x: Number) -> Double`
Returns the arcsine of x (in radians). Domain: [-1, 1].

```obq
result: Double = math.asin(0.0) # 0.0
result2: Double = math.asin(1.0) # π/2
```

### `acos(x: Number) -> Double`
Returns the arccosine of x (in radians). Domain: [-1, 1].

```obq
result: Double = math.acos(1.0) # 0.0
result2: Double = math.acos(0.0) # π/2
```

### `atan(x: Number) -> Double`
Returns the arctangent of x (in radians).

```obq
result: Double = math.atan(0.0) # 0.0
result2: Double = math.atan(1.0) # π/4
```

### `atan2(y: Number, x: Number) -> Double`
Returns the arctangent of y/x (in radians), using the signs of both arguments to determine the quadrant.

```obq
result: Double = math.atan2(1.0, 1.0) # π/4
result2: Double = math.atan2(0.0, 1.0) # 0.0
```

## Hyperbolic Functions

### `sinh(x: Number) -> Double`
Returns the hyperbolic sine of x.

```obq
result: Double = math.sinh(0.0) # 0.0
```

### `cosh(x: Number) -> Double`
Returns the hyperbolic cosine of x.

```obq
result: Double = math.cosh(0.0) # 1.0
```

### `tanh(x: Number) -> Double`
Returns the hyperbolic tangent of x.

```obq
result: Double = math.tanh(0.0) # 0.0
```

### `asinh(x: Number) -> Double`
Returns the inverse hyperbolic sine of x.

```obq
result: Double = math.asinh(0.0) # 0.0
```

### `acosh(x: Number) -> Double`
Returns the inverse hyperbolic cosine of x. Domain: [1, ∞).

```obq
result: Double = math.acosh(1.0) # 0.0
```

### `atanh(x: Number) -> Double`
Returns the inverse hyperbolic tangent of x. Domain: (-1, 1).

```obq
result: Double = math.atanh(0.0) # 0.0
```

## Angular Conversion Functions

### `radians(degrees: Number) -> Double`
Converts degrees to radians.

```obq
rad180: Double = math.radians(180.0) # π
rad90: Double = math.radians(90.0)   # π/2
```

### `degrees(radians: Number) -> Double`
Converts radians to degrees.

```obq
deg_pi: Double = math.degrees(math.pi())     # 180.0
deg_pi2: Double = math.degrees(math.pi() / 2) # 90.0
```

## Special Functions

### `factorial(n: Int) -> Long`
Returns the factorial of n (n!). Domain: [0, 20].

```obq
fact5: Long = math.factorial(5)  # 120
fact0: Long = math.factorial(0)  # 1
fact10: Long = math.factorial(10) # 3628800
```

### `gcd(a: Int, b: Int) -> Int`
Returns the greatest common divisor of a and b.

```obq
gcd_result: Int = math.gcd(12, 18) # 6
gcd_coprime: Int = math.gcd(17, 19) # 1
gcd_negative: Int = math.gcd(-12, 18) # 6
```

### `lcm(a: Int, b: Int) -> Long`
Returns the least common multiple of a and b.

```obq
lcm_result: Long = math.lcm(4, 6)   # 12
lcm_large: Long = math.lcm(12, 18)  # 36
lcm_zero: Long = math.lcm(0, 5)     # 0
```

### `gamma(x: Number) -> Double`
Returns the gamma function of x.

```obq
gamma1: Double = math.gamma(1.0) # 1.0 (0! = 1)
```

### `lgamma(x: Number) -> Double`
Returns the natural logarithm of the absolute value of the gamma function.

```obq
lgamma1: Double = math.lgamma(1.0) # 0.0 (log(1) = 0)
```

## Error Functions

### `erf(x: Number) -> Double`
Returns the error function of x.

```obq
erf_zero: Double = math.erf(0.0) # 0.0
```

### `erfc(x: Number) -> Double`
Returns the complementary error function of x.

```obq
erfc_zero: Double = math.erfc(0.0) # 1.0
```

## Classification Functions

### `isFinite(x: Number) -> Bool`
Returns true if x is finite (not infinite or NaN).

```obq
is_finite: Bool = math.isFinite(5.0)        # true
not_finite: Bool = math.isFinite(math.inf()) # false
```

### `isInf(x: Number) -> Bool`
Returns true if x is infinite.

```obq
is_inf: Bool = math.isInf(math.inf()) # true
not_inf: Bool = math.isInf(5.0)       # false
```

### `isNan(x: Number) -> Bool`
Returns true if x is NaN.

```obq
is_nan: Bool = math.isNan(math.nan()) # true
not_nan: Bool = math.isNan(5.0)       # false
```

### `isNormal(x: Number) -> Bool`
Returns true if x is a normal number (finite, non-zero, and not subnormal).

```obq
is_normal: Bool = math.isNormal(5.0)       # true
not_normal: Bool = math.isNormal(0.0)      # false
inf_not_normal: Bool = math.isNormal(math.inf()) # false
```

## Complete Example

```obq
import math
import system.io

Object MathExample {
    method demonstrateMath(): Text {
        io.print("=== Math Library Example ===")
        
        # Mathematical constants
        pi: Double = math.pi()
        e: Double = math.e()
        io.print("π = %f, e = %f", pi, e)
        
        # Basic operations
        x: Double = -3.7
        abs_x: Double = math.abs(x)
        ceil_x: Double = math.ceil(x)
        floor_x: Double = math.floor(x)
        io.print("abs(%f) = %f, ceil = %f, floor = %f", x, abs_x, ceil_x, floor_x)
        
        # Power functions
        base: Double = 2.0
        exp: Double = 3.0
        power: Double = math.pow(base, exp)
        sqrt_16: Double = math.sqrt(16.0)
        io.print("%f^%f = %f, sqrt(16) = %f", base, exp, power, sqrt_16)
        
        # Trigonometric functions
        angle_deg: Double = 45.0
        angle_rad: Double = math.radians(angle_deg)
        sin_val: Double = math.sin(angle_rad)
        cos_val: Double = math.cos(angle_rad)
        io.print("sin(%f°) = %f, cos(%f°) = %f", angle_deg, sin_val, angle_deg, cos_val)
        
        # Logarithmic functions
        log_e: Double = math.log(e)
        log10_100: Double = math.log10(100.0)
        io.print("ln(e) = %f, log10(100) = %f", log_e, log10_100)
        
        # Special functions
        fact5: Long = math.factorial(5)
        gcd_val: Int = math.gcd(12, 18)
        lcm_val: Long = math.lcm(4, 6)
        io.print("5! = %d, gcd(12,18) = %d, lcm(4,6) = %d", fact5, gcd_val, lcm_val)
        
        # Classification functions
        inf_val: Double = math.inf()
        nan_val: Double = math.nan()
        io.print("isInf(inf) = %s, isNan(nan) = %s", 
                 math.isInf(inf_val), math.isNan(nan_val))
        
        return "Math demonstration completed"
    }
}
```

## Error Handling

Math functions may throw `EvaluationError` for:
- Domain errors (e.g., `sqrt(-1)`, `log(0)`, `asin(2)`)
- Argument count errors (wrong number of arguments)
- Type errors (non-numeric arguments)
- Range errors (e.g., `factorial(25)`)

Always validate input ranges when working with user-provided mathematical data.

## Performance Notes

- Functions accept any numeric type (Int, Long, Float, Double) and convert as needed
- Most functions return Double for precision
- Integer functions (`factorial`, `gcd`, `lcm`) preserve integer types where appropriate
- `abs()` preserves the input numeric type