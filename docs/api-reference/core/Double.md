# Double API Reference

## Overview

Double is O²L's 64-bit double-precision floating-point type, compliant with IEEE 754 standard. It provides high-precision decimal number operations with approximately 15-17 decimal digits of precision and a range of approximately 4.9 × 10^-324 to 1.8 × 10^308. Double values are immutable and support all arithmetic operations, comparisons, and special value handling.

## Declaration & Literals

```o2l
# Double literals (default for decimal numbers)
simple: Double = 3.14159265358979323846
scientific: Double = 1.23456789e-10      # Scientific notation
negative: Double = -2.718281828459045
explicit: Double = 42.0d                 # Explicit 'd' suffix

# High-precision constants
pi: Double = 3.14159265358979323846264338327950288
e: Double = 2.71828182845904523536028747135266249
golden_ratio: Double = 1.61803398874989484820458683436563811

# Special values
zero: Double = 0.0
positive_inf: Double = 1.0 / 0.0     # Positive infinity
negative_inf: Double = -1.0 / 0.0    # Negative infinity
nan: Double = 0.0 / 0.0              # NaN (Not a Number)
```

## Quick Example

```o2l
import system.io

# High-precision calculations
radius: Double = 5.123456789
pi: Double = 3.14159265358979323846
area: Double = pi * radius * radius
circumference: Double = 2.0 * pi * radius

io.print("Radius: %f", radius)
io.print("Area: %f", area)
io.print("Circumference: %f", circumference)
io.print("As text: %s", area.toString())
```

---

## Arithmetic Operations

Double supports all standard floating-point arithmetic with high precision:

```o2l
# High-precision arithmetic
a: Double = 1.23456789012345678901234567890
b: Double = 9.87654321098765432109876543210

sum: Double = a + b
difference: Double = a - b
product: Double = a * b
quotient: Double = a / b
remainder: Double = a % b

io.print("High-precision sum: %f", sum)
io.print("High-precision product: %f", product)
io.print("Division result: %f", quotient)

# Demonstrate precision advantage over Float
pi_float: Float = 3.14159f
pi_double: Double = 3.14159265358979323846
difference_precision: Double = pi_double - pi_float.toDouble()

io.print("Float PI: %f", pi_float)
io.print("Double PI: %f", pi_double)
io.print("Precision difference: %f", difference_precision)
```

### High-Precision Mathematical Operations

```o2l
Object PrecisionMath {
    # Constants with full double precision
    property PI: Double
    property E: Double
    property SQRT_2: Double

    constructor() {
        this.PI = 3.14159265358979323846264338327950288
        this.E = 2.71828182845904523536028747135266249
        this.SQRT_2 = 1.41421356237309504880168872420969808
    }

    method circleArea(radius: Double): Double {
        return this.PI * radius * radius
    }

    method sphereVolume(radius: Double): Double {
        return (4.0 / 3.0) * this.PI * radius * radius * radius
    }

    method compoundInterest(principal: Double, rate: Double, time: Double, n: Double): Double {
        # A = P(1 + r/n)^(n*t) - continuous compounding approximation
        base: Double = 1.0 + (rate / n)

        # Iterative power calculation for high precision
        result: Double = principal
        exponent: Double = n * time
        i: Double = 0.0
        while (i < exponent) {
            result = result * base
            i = i + 1.0
        }
        return result
    }
}

# Usage with high precision
math: PrecisionMath = new PrecisionMath()
precise_radius: Double = 6.123456789012345
area: Double = math.circleArea(precise_radius)
volume: Double = math.sphereVolume(precise_radius)

io.print("High-precision area: %f", area)
io.print("High-precision volume: %f", volume)
```

---

## Comparison Operations

```o2l
# High-precision comparisons
x: Double = 2.123456789012345
y: Double = 2.123456789012346  # Very small difference
z: Double = 2.123456789012345

# Equality (exact match)
exact_equal: Bool = x == z      # true
tiny_diff: Bool = x == y        # false (tiny difference detected)

io.print("Exact equality: %f == %f is %o", x, z, exact_equal)
io.print("Tiny difference: %f == %f is %o", x, y, tiny_diff)

# Safe comparison for calculated values
method doubleEquals(a: Double, b: Double, epsilon: Double): Bool {
    difference: Double = a - b
    if (difference < 0.0) {
        difference = -difference
    }
    return difference < epsilon
}

# Ultra-precise comparison
calc1: Double = 1.0 / 3.0 * 3.0
calc2: Double = 1.0
ultra_precise_epsilon: Double = 1e-15  # 15 decimal places

safe_equal: Bool = doubleEquals(calc1, calc2, ultra_precise_epsilon)
io.print("Ultra-precise comparison: %o", safe_equal)
```

---

## Special Value Methods

### `isNaN() → Bool`

Returns true if the double is Not a Number (NaN).

```o2l
# NaN detection with high precision
normal: Double = 5.123456789012345
nan_value: Double = 0.0 / 0.0
sqrt_negative: Double = -1.0  # Would be NaN if sqrt(-1) was calculated

io.print("Normal value %f is NaN: %o", normal, normal.isNaN())
io.print("NaN value is NaN: %o", nan_value.isNaN())

# NaN propagation in calculations
calculation: Double = normal + nan_value
io.print("Normal + NaN = %f, is NaN: %o", calculation, calculation.isNaN())

# NaN comparison properties
io.print("NaN == NaN: %o", nan_value == nan_value)  # false
io.print("NaN != NaN: %o", nan_value != nan_value)  # true
```

### `isInfinite() → Bool`

Returns true if the double is positive or negative infinity.

```o2l
# Infinity detection
normal: Double = 1.23456789012345e100
pos_inf: Double = 1.0 / 0.0
neg_inf: Double = -1.0 / 0.0
very_large: Double = 1.0e308      # Large but finite

io.print("Normal large value %f is infinite: %o", normal, normal.isInfinite())
io.print("Positive infinity is infinite: %o", pos_inf.isInfinite())
io.print("Negative infinity is infinite: %o", neg_inf.isInfinite())
io.print("Very large %f is infinite: %o", very_large, very_large.isInfinite())

# Infinity arithmetic
inf_calc: Double = pos_inf + 1000000.0
io.print("Infinity + 1000000 is infinite: %o", inf_calc.isInfinite())
```

### `isFinite() → Bool`

Returns true if the double is neither NaN nor infinite.

```o2l
# Finite value validation
test_values: List<Double> = [
    5.123456789012345,
    0.0,
    -2.987654321098765,
    1.0 / 0.0,           # Infinity
    0.0 / 0.0,           # NaN
    1.7976931348623157e308,  # Near maximum Double
    4.9406564584124654e-324  # Near minimum positive Double
]

value_iterator: ListIterator = test_values.iterator()
while (value_iterator.hasNext()) {
    val: Double = value_iterator.next()
    is_finite: Bool = val.isFinite()
    is_nan: Bool = val.isNaN()
    is_infinite: Bool = val.isInfinite()

    io.print("Value %f: finite=%o, NaN=%o, infinite=%o",
             val, is_finite, is_nan, is_infinite)
}
```

---

## Type Conversion Methods

### `toString() → Text`

Converts the double to its string representation with high precision.

```o2l
# High-precision string conversion
precise_values: List<Double> = [
    3.14159265358979323846,
    2.71828182845904523536,
    1.41421356237309504880,
    1.23456789012345678901e-10,
    9.87654321098765432109e15
]

precision_iterator: ListIterator = precise_values.iterator()
while (precision_iterator.hasNext()) {
    num: Double = precision_iterator.next()
    text: Text = num.toString()
    io.print("Double %f as text: '%s'", num, text)
}

# Special values as strings
nan_str: Text = (0.0 / 0.0).toString()
pos_inf_str: Text = (1.0 / 0.0).toString()
neg_inf_str: Text = (-1.0 / 0.0).toString()

io.print("NaN as string: '%s'", nan_str)
io.print("Positive infinity: '%s'", pos_inf_str)
io.print("Negative infinity: '%s'", neg_inf_str)
```

### `toInt() → Int`

Converts the double to an integer by truncating the decimal part.

```o2l
# Double to integer conversion
test_doubles: List<Double> = [
    5.123456789,
    -3.987654321,
    0.999999999,
    -0.000000001,
    42.0,
    1.7976931348623157e10  # Large double
]

double_iterator: ListIterator = test_doubles.iterator()
while (double_iterator.hasNext()) {
    d: Double = double_iterator.next()
    i: Int = d.toInt()
    io.print("Double %f as Int: %d", d, i)
}

# Precision-aware rounding
method roundToInt(value: Double): Int {
    if (value >= 0.0) {
        return (value + 0.5).toInt()
    } else {
        return (value - 0.5).toInt()
    }
}

test_value: Double = 5.123456789
truncated: Int = test_value.toInt()
rounded: Int = roundToInt(test_value)

io.print("Value %f: truncated=%d, rounded=%d", test_value, truncated, rounded)
```

### `toLong() → Long`

Converts the double to a Long integer with extended range.

```o2l
# Double to Long conversion for high precision
large_doubles: List<Double> = [
    1.23456789012345e15,
    -9.87654321098765e18,
    5.5,
    1.7976931348623157e100  # Very large
]

large_iterator: ListIterator = large_doubles.iterator()
while (large_iterator.hasNext()) {
    d: Double = large_iterator.next()
    l: Long = d.toLong()
    io.print("Double %f as Long: %s", d, l.toString())
}

# High-precision time calculations
precise_timestamp: Double = 1609459200.123456789  # Unix timestamp with nanoseconds
timestamp_long: Long = precise_timestamp.toLong()
nanoseconds: Long = ((precise_timestamp - timestamp_long.toDouble()) * 1e9).toLong()

io.print("Timestamp: %f", precise_timestamp)
io.print("Seconds: %s", timestamp_long.toString())
io.print("Nanoseconds: %s", nanoseconds.toString())
```

### `toFloat() → Float`

Converts the double to a single-precision float (may lose precision).

```o2l
# Double to Float conversion (precision loss)
precise_doubles: List<Double> = [
    3.14159265358979323846,     # High precision pi
    1.23456789012345678901,     # Many decimal places
    1.0e-10,                    # Small number
    1.0e20                      # Large number
]

float_iterator: ListIterator = precise_doubles.iterator()
while (float_iterator.hasNext()) {
    d: Double = double_iterator.next()
    f: Float = d.toFloat()
    back_to_double: Double = f.toDouble()
    precision_loss: Double = d - back_to_double

    io.print("Double %f -> Float %f -> Double %f", d, f, back_to_double)
    io.print("Precision loss: %f", precision_loss)
}

# When to use Float vs Double
method chooseFloatOrDouble(precision_needed: Int): Text {
    if (precision_needed <= 7) {
        return "Float is sufficient"
    } else if (precision_needed <= 15) {
        return "Double recommended"
    } else {
        return "Consider using specialized high-precision library"
    }
}
```

### `toBool() → Bool`

Converts the double to a boolean (0.0 = false, non-zero = true).

```o2l
# Boolean conversion with high precision
test_values: List<Double> = [
    0.0,
    1.0e-100,                    # Very small positive
    -1.0e-100,                   # Very small negative
    5.123456789012345,
    0.0 / 0.0,                   # NaN
    1.0 / 0.0                    # Infinity
]

bool_iterator: ListIterator = test_values.iterator()
while (bool_iterator.hasNext()) {
    val: Double = bool_iterator.next()
    bool_result: Bool = val.toBool()
    io.print("Double %f as Bool: %o", val, bool_result)
}

# Epsilon-based boolean conversion
method isEffectivelyZero(value: Double, epsilon: Double): Bool {
    abs_value: Double = value
    if (abs_value < 0.0) {
        abs_value = -abs_value
    }
    return abs_value < epsilon
}

tiny_value: Double = 1.0e-15
ultra_tiny: Double = 1.0e-50
epsilon: Double = 1.0e-14

io.print("Tiny value %f is effectively zero: %o",
         tiny_value, isEffectivelyZero(tiny_value, epsilon))
io.print("Ultra-tiny value %f is effectively zero: %o",
         ultra_tiny, isEffectivelyZero(ultra_tiny, epsilon))
```

---

## Practical Examples

### Scientific Computing

```o2l
Object ScientificComputing {
    # Physical constants with maximum precision
    property SPEED_OF_LIGHT: Double      # 299,792,458 m/s
    property PLANCK_CONSTANT: Double     # 6.62607015e-34 J⋅Hz⁻¹
    property AVOGADRO_NUMBER: Double     # 6.02214076e23 mol⁻¹
    property ELECTRON_CHARGE: Double     # 1.602176634e-19 C

    constructor() {
        this.SPEED_OF_LIGHT = 299792458.0
        this.PLANCK_CONSTANT = 6.62607015e-34
        this.AVOGADRO_NUMBER = 6.02214076e23
        this.ELECTRON_CHARGE = 1.602176634e-19
    }

    method photonEnergy(frequency: Double): Double {
        # E = h * f
        return this.PLANCK_CONSTANT * frequency
    }

    method relativisticMomentum(mass: Double, velocity: Double): Double {
        # p = mv / √(1 - v²/c²)
        v_ratio: Double = velocity / this.SPEED_OF_LIGHT
        gamma: Double = 1.0 / this.sqrt(1.0 - v_ratio * v_ratio)
        return mass * velocity * gamma
    }

    method blackBodyRadiation(temperature: Double, wavelength: Double): Double {
        # Simplified Planck's law: B(λ,T) ∝ 1/(λ^5 * (e^(hc/λkT) - 1))
        boltzmann: Double = 1.380649e-23  # Boltzmann constant
        hc: Double = this.PLANCK_CONSTANT * this.SPEED_OF_LIGHT

        exponent: Double = hc / (wavelength * boltzmann * temperature)
        # Would need exp function: denominator = exp(exponent) - 1.0
        # For now, approximate for small exponents
        denominator: Double = exponent  # Linear approximation

        return 1.0 / (wavelength * wavelength * wavelength * wavelength * wavelength * denominator)
    }

    method sqrt(x: Double): Double {
        # Newton's method for square root
        if (x < 0.0) {
            return 0.0 / 0.0  # NaN
        }
        if (x == 0.0) {
            return 0.0
        }

        guess: Double = x / 2.0
        epsilon: Double = 1.0e-15

        i: Int = 0
        while (i < 50) {  # Maximum iterations
            new_guess: Double = 0.5 * (guess + x / guess)
            difference: Double = new_guess - guess
            if (difference < 0.0) difference = -difference

            if (difference < epsilon) {
                return new_guess
            }
            guess = new_guess
            i = i + 1
        }

        return guess
    }
}

# Usage
science: ScientificComputing = new ScientificComputing()
frequency: Double = 5.0e14  # 500 THz (visible light)
mass: Double = 9.1093837015e-31  # Electron mass in kg
velocity: Double = 1.0e8  # 1/3 speed of light

photon_energy: Double = science.photonEnergy(frequency)
momentum: Double = science.relativisticMomentum(mass, velocity)

io.print("Photon energy at %f Hz: %e Joules", frequency, photon_energy)
io.print("Relativistic momentum: %e kg⋅m/s", momentum)
```

### Financial Modeling with High Precision

```o2l
Object HighPrecisionFinance {
    method presentValue(future_value: Double, rate: Double, periods: Double): Double {
        # PV = FV / (1 + r)^n
        discount_factor: Double = 1.0 + rate

        # Calculate (1 + r)^n iteratively for precision
        power: Double = 1.0
        i: Double = 0.0
        while (i < periods) {
            power = power * discount_factor
            i = i + 1.0
        }

        return future_value / power
    }

    method netPresentValue(cash_flows: List<Double>, rate: Double): Double {
        npv: Double = 0.0
        period: Double = 0.0

        iterator: ListIterator = cash_flows.iterator()
        while (iterator.hasNext()) {
            cash_flow: Double = iterator.next()
            present_value: Double = this.presentValue(cash_flow, rate, period)
            npv = npv + present_value
            period = period + 1.0
        }

        return npv
    }

    method blackScholes(S: Double, K: Double, T: Double, r: Double, sigma: Double): Double {
        # Simplified Black-Scholes formula (without statistical functions)
        # C = S * N(d1) - K * e^(-rT) * N(d2)
        # This is a simplified approximation

        d1: Double = (this.ln(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * this.sqrt(T))
        d2: Double = d1 - sigma * this.sqrt(T)

        # Approximate normal distribution (would need proper implementation)
        N_d1: Double = 0.5 + 0.3989422804 * d1  # Very rough approximation
        N_d2: Double = 0.5 + 0.3989422804 * d2

        discount_factor: Double = this.exp(-r * T)

        return S * N_d1 - K * discount_factor * N_d2
    }

    # Helper methods (simplified implementations)
    method ln(x: Double): Double {
        # Natural logarithm using series expansion (simplified)
        if (x <= 0.0) return 0.0 / 0.0  # NaN

        # Use ln(x) = 2 * [(x-1)/(x+1) + (x-1)³/(3(x+1)³) + ...]
        # For x close to 1
        if (x > 0.5 && x < 2.0) {
            u: Double = (x - 1.0) / (x + 1.0)
            u2: Double = u * u
            return 2.0 * u * (1.0 + u2 / 3.0 + u2 * u2 / 5.0)
        } else {
            return 0.0  # Simplified - would need full implementation
        }
    }

    method exp(x: Double): Double {
        # e^x using series expansion (simplified)
        if (x > 10.0) return 1.0 / 0.0  # Approximate infinity
        if (x < -10.0) return 0.0

        result: Double = 1.0
        term: Double = 1.0
        i: Double = 1.0

        while (i <= 20.0) {  # First 20 terms
            term = term * x / i
            result = result + term
            i = i + 1.0
        }

        return result
    }

    method sqrt(x: Double): Double {
        # Same as in ScientificComputing
        if (x < 0.0) return 0.0 / 0.0
        if (x == 0.0) return 0.0

        guess: Double = x / 2.0
        epsilon: Double = 1.0e-15

        i: Int = 0
        while (i < 50) {
            new_guess: Double = 0.5 * (guess + x / guess)
            difference: Double = new_guess - guess
            if (difference < 0.0) difference = -difference

            if (difference < epsilon) return new_guess
            guess = new_guess
            i = i + 1
        }

        return guess
    }
}

# Usage
finance: HighPrecisionFinance = new HighPrecisionFinance()

# Cash flow analysis
cash_flows: List<Double> = [-100000.0, 25000.0, 30000.0, 35000.0, 40000.0, 45000.0]
discount_rate: Double = 0.08  # 8% discount rate
npv: Double = finance.netPresentValue(cash_flows, discount_rate)

# Present value calculation
future_value: Double = 150000.0
periods: Double = 10.0
pv: Double = finance.presentValue(future_value, discount_rate, periods)

io.print("Net Present Value: $%f", npv)
io.print("Present Value of $%f in %f years: $%f", future_value, periods, pv)

# Option pricing (simplified)
stock_price: Double = 100.0
strike_price: Double = 105.0
time_to_expiry: Double = 0.25  # 3 months
risk_free_rate: Double = 0.03  # 3%
volatility: Double = 0.20  # 20%

option_value: Double = finance.blackScholes(stock_price, strike_price, time_to_expiry, risk_free_rate, volatility)
io.print("Call option value: $%f", option_value)
```

### Engineering Calculations

```o2l
Object EngineeringCalculations {
    method beamDeflection(load: Double, length: Double, elasticity: Double, moment: Double): Double {
        # Maximum deflection of simply supported beam: δ = 5wL⁴/(384EI)
        coefficient: Double = 5.0 / 384.0
        deflection: Double = coefficient * load * length * length * length * length / (elasticity * moment)
        return deflection
    }

    method reynoldsNumber(density: Double, velocity: Double, length: Double, viscosity: Double): Double {
        # Re = ρvL/μ
        return density * velocity * length / viscosity
    }

    method heatTransferCoefficient(nusselt: Double, conductivity: Double, length: Double): Double {
        # h = NuK/L
        return nusselt * conductivity / length
    }

    method stressIntensityFactor(stress: Double, crack_length: Double, geometry_factor: Double): Double {
        # K = Yσ√(πa)
        pi: Double = 3.14159265358979323846
        return geometry_factor * stress * this.sqrt(pi * crack_length)
    }

    method fatigueLife(stress_amplitude: Double, material_constant: Double, stress_exponent: Double): Double {
        # N = C/σᵐ (simplified S-N curve)
        # Would need pow function for proper implementation

        # Approximate using iterative multiplication for integer exponents
        exponent_int: Int = stress_exponent.toInt()
        stress_power: Double = 1.0
        i: Int = 0
        while (i < exponent_int) {
            stress_power = stress_power * stress_amplitude
            i = i + 1
        }

        return material_constant / stress_power
    }

    method sqrt(x: Double): Double {
        # Newton's method implementation
        if (x < 0.0) return 0.0 / 0.0
        if (x == 0.0) return 0.0

        guess: Double = x / 2.0
        epsilon: Double = 1.0e-15

        i: Int = 0
        while (i < 50) {
            new_guess: Double = 0.5 * (guess + x / guess)
            difference: Double = new_guess - guess
            if (difference < 0.0) difference = -difference

            if (difference < epsilon) return new_guess
            guess = new_guess
            i = i + 1
        }

        return guess
    }
}

# Usage
engineering: EngineeringCalculations = new EngineeringCalculations()

# Structural analysis
load: Double = 10000.0      # N/m
beam_length: Double = 5.0   # meters
elastic_modulus: Double = 200.0e9  # Pa (steel)
moment_of_inertia: Double = 8.33e-6  # m⁴

deflection: Double = engineering.beamDeflection(load, beam_length, elastic_modulus, moment_of_inertia)

# Fluid mechanics
density: Double = 1000.0    # kg/m³ (water)
velocity: Double = 2.5      # m/s
characteristic_length: Double = 0.1  # m
viscosity: Double = 1.0e-3  # Pa⋅s

reynolds: Double = engineering.reynoldsNumber(density, velocity, characteristic_length, viscosity)

# Fracture mechanics
applied_stress: Double = 150.0e6  # Pa
crack_size: Double = 0.001        # m (1 mm crack)
geometry_factor: Double = 1.12    # Typical value

stress_intensity: Double = engineering.stressIntensityFactor(applied_stress, crack_size, geometry_factor)

io.print("Beam deflection: %f mm", deflection * 1000.0)
io.print("Reynolds number: %f", reynolds)
io.print("Stress intensity factor: %f MPa⋅√m", stress_intensity / 1.0e6)
```

---

## Error Handling

```o2l
# Safe Double operations with high precision
method safeHighPrecisionDivision(dividend: Double, divisor: Double): Double {
    epsilon: Double = 1.0e-100  # Very small threshold for Double

    if (divisor == 0.0) {
        io.error("Division by zero in Double operation")
        return 0.0
    }

    # Check for very small divisor that might cause overflow
    abs_divisor: Double = divisor
    if (abs_divisor < 0.0) abs_divisor = -abs_divisor
    if (abs_divisor < epsilon) {
        io.error("Divisor too small, may cause overflow: %e", divisor)
        return 0.0
    }

    result: Double = dividend / divisor
    if (result.isNaN()) {
        io.error("Division resulted in NaN")
        return 0.0
    }
    if (result.isInfinite()) {
        io.error("Division resulted in infinity")
        return 0.0
    }

    return result
}

# Safe high-precision parsing
method parseDoubleSafely(text: Text): Double {
    try {
        result: Double = text.toDouble()
        if (result.isNaN()) {
            io.error("Parsed Double is NaN: %s", text)
            return 0.0
        }
        if (result.isInfinite()) {
            io.error("Parsed Double is infinite: %s", text)
            return 0.0
        }
        return result
    } catch (error: Exception) {
        io.error("Invalid Double format: %s", text)
        return 0.0
    }
}

# Numerical stability checking
method checkNumericalStability(result: Double, operation: Text): Bool {
    if (result.isNaN()) {
        io.error("Operation '%s' produced NaN - possible numerical instability", operation)
        return false
    }
    if (result.isInfinite()) {
        io.error("Operation '%s' produced infinity - possible overflow", operation)
        return false
    }

    # Check for subnormal numbers (very small values that may indicate underflow)
    min_normal: Double = 2.2250738585072014e-308  # Minimum normal Double
    abs_result: Double = result
    if (abs_result < 0.0) abs_result = -abs_result

    if (abs_result != 0.0 && abs_result < min_normal) {
        io.warn("Operation '%s' produced subnormal result: %e", operation, result)
        return true  # Still valid, just a warning
    }

    return true
}
```

---

## Performance Considerations

1. **Memory Usage**: Double uses 64 bits (8 bytes) per value
2. **Arithmetic Speed**: Slightly slower than Float but still very fast on modern hardware
3. **Precision Advantage**: ~15-17 decimal digits vs Float's ~7 digits
4. **Cache Efficiency**: Twice the memory footprint of Float affects cache performance
5. **Conversion Costs**:
   - `toString()`: More expensive than Float due to higher precision formatting
   - `toFloat()`: Low cost but loses precision
   - Integer conversions: Low cost (truncation)
   - `toBool()`: Very low cost (zero check)

---

## Best Practices

1. **Default Choice**: Use Double as the default floating-point type for most applications
2. **Precision Awareness**: Remember Double still has limited precision (~15-17 digits)
3. **Epsilon Comparisons**: Always use epsilon-based comparison for calculated values
4. **Special Value Handling**: Check for NaN and infinity in critical calculations
5. **Memory vs Precision**: Consider Float for memory-intensive applications with moderate precision needs
6. **Scientific Computing**: Double is essential for scientific and engineering applications
7. **Financial Calculations**: Use Double for monetary calculations requiring high precision

---

## See Also

- [Float API Reference](Float.md) - Single-precision floating-point
- [Int API Reference](Int.md) - 64-bit integer type
- [Long API Reference](Long.md) - 128-bit integer type
- [Bool API Reference](Bool.md) - Boolean type
- [Text API Reference](Text.md) - String representation and parsing
- [Math Library](../system/math.md) - Mathematical functions for Double
