# Float API Reference

## Overview

Float is O²L's 32-bit single-precision floating-point type, compliant with IEEE 754 standard. It provides efficient decimal number operations with approximately 7 decimal digits of precision and a range of approximately 1.4 × 10^-45 to 3.4 × 10^38. Float values are immutable and support arithmetic operations, comparisons, and special value handling.

## Declaration & Literals

```o2l
# Float literals (require 'f' suffix)
simple: Float = 3.14f
scientific: Float = 1.23e-4f       # 0.000123
negative: Float = -2.5f
integer_as_float: Float = 42.0f

# Special values
zero: Float = 0.0f
positive_inf: Float = 1.0f / 0.0f  # Positive infinity (if supported)
negative_inf: Float = -1.0f / 0.0f # Negative infinity
nan: Float = 0.0f / 0.0f           # NaN (Not a Number)

# Precision examples
precise: Float = 1.23456789f       # Stored as ~1.2345679 (7 digits)
```

## Quick Example

```o2l
import system.io

# Basic float operations
radius: Float = 5.5f
pi: Float = 3.14159f
area: Float = pi * radius * radius
text: Text = area.toString()
io.print("Circle area: %f (as text: %s)", area, text)

# Special value checking
if (area.isFinite()) {
    io.print("Area is a valid finite number")
}
```

---

## Arithmetic Operations

Float supports all standard floating-point arithmetic:

```o2l
# Basic arithmetic
a: Float = 10.5f
b: Float = 3.2f

sum: Float = a + b          # 13.7
difference: Float = a - b   # 7.3
product: Float = a * b      # 33.6
quotient: Float = a / b     # 3.28125
remainder: Float = a % b    # Modulo operation

io.print("Sum: %f, Product: %f", sum, product)
io.print("Division: %f / %f = %f", a, b, quotient)

# Unary operations
positive: Float = +a        # 10.5
negative: Float = -a        # -10.5
```

### Precision and Rounding

```o2l
# Demonstrate floating-point precision limits
precise_calc: Float = 0.1f + 0.2f  # May not equal exactly 0.3f
expected: Float = 0.3f

io.print("0.1 + 0.2 = %f", precise_calc)
io.print("Expected 0.3 = %f", expected)
io.print("Difference: %f", precise_calc - expected)

# Safe floating-point comparison
method floatEquals(a: Float, b: Float, epsilon: Float): Bool {
    difference: Float = a - b
    if (difference < 0.0f) {
        difference = -difference
    }
    return difference < epsilon
}

# Usage
epsilon: Float = 0.0001f
are_equal: Bool = floatEquals(precise_calc, expected, epsilon)
io.print("Are they approximately equal? %o", are_equal)
```

---

## Comparison Operations

```o2l
# Standard comparisons
x: Float = 2.5f
y: Float = 3.7f
z: Float = 2.5f

# Equality (be careful with floating-point equality)
equal: Bool = x == z        # true (exact match)
not_equal: Bool = x != y    # true

# Ordering
less: Bool = x < y          # true
less_equal: Bool = x <= z   # true
greater: Bool = y > x       # true
greater_equal: Bool = z >= x  # true

io.print("Float comparison: %f == %f is %o", x, z, equal)

# Safe comparison for calculated values
calc1: Float = 1.0f / 3.0f * 3.0f  # Should be 1.0, but may have precision error
calc2: Float = 1.0f
tolerance: Float = 0.00001f

safe_equal: Bool = floatEquals(calc1, calc2, tolerance)
io.print("Calculated values equal: %o", safe_equal)
```

---

## Special Value Methods

### `isNaN() → Bool`

Returns true if the float is Not a Number (NaN).

```o2l
# Test for NaN values
normal: Float = 5.5f
nan_value: Float = 0.0f / 0.0f  # Creates NaN
another_nan: Float = sqrt(-1.0f)  # Would create NaN if sqrt available

io.print("Normal value %f is NaN: %o", normal, normal.isNaN())
io.print("NaN value is NaN: %o", nan_value.isNaN())

# NaN properties
io.print("NaN == NaN: %o", nan_value == nan_value)  # false! NaN != NaN
io.print("Using isNaN: %o", nan_value.isNaN())      # true

# Safe NaN checking in calculations
method safeDivision(a: Float, b: Float): Float {
    result: Float = a / b
    if (result.isNaN()) {
        io.error("Division resulted in NaN")
        return 0.0f
    }
    return result
}
```

### `isInfinite() → Bool`

Returns true if the float is positive or negative infinity.

```o2l
# Test for infinity
normal: Float = 100.0f
pos_inf: Float = 1.0f / 0.0f       # Positive infinity
neg_inf: Float = -1.0f / 0.0f      # Negative infinity
large: Float = 1.0e30f             # Large but finite

io.print("Normal %f is infinite: %o", normal, normal.isInfinite())
io.print("Positive infinity is infinite: %o", pos_inf.isInfinite())
io.print("Negative infinity is infinite: %o", neg_inf.isInfinite())
io.print("Large number %f is infinite: %o", large, large.isInfinite())

# Infinity in calculations
method safeMultiplication(a: Float, b: Float): Float {
    result: Float = a * b
    if (result.isInfinite()) {
        io.error("Multiplication resulted in infinity")
        return 0.0f
    }
    return result
}
```

### `isFinite() → Bool`

Returns true if the float is neither NaN nor infinite.

```o2l
# Test for finite values
values: List<Float> = [5.5f, 0.0f, -2.3f, 1.0f/0.0f, 0.0f/0.0f]
value_iterator: ListIterator = values.iterator()

while (value_iterator.hasNext()) {
    val: Float = value_iterator.next()
    is_finite: Bool = val.isFinite()
    is_nan: Bool = val.isNaN()
    is_infinite: Bool = val.isInfinite()

    io.print("Value %f: finite=%o, NaN=%o, infinite=%o",
             val, is_finite, is_nan, is_infinite)
}

# Use in validation
method validateMeasurement(measurement: Float): Bool {
    return measurement.isFinite() && measurement >= 0.0f
}
```

---

## Type Conversion Methods

### `toString() → Text`

Converts the float to its string representation.

```o2l
# String conversion examples
values: List<Float> = [0.0f, 3.14159f, -2.5f, 1.23e-4f, 1.0e6f]
value_iterator: ListIterator = values.iterator()

while (value_iterator.hasNext()) {
    num: Float = value_iterator.next()
    text: Text = num.toString()
    io.print("Float %f as text: '%s'", num, text)
}

# Formatting in messages
temperature: Float = 23.7f
message: Text = "Current temperature: " + temperature.toString() + "°C"
io.print(message)

# Special values as strings
nan_val: Float = 0.0f / 0.0f
inf_val: Float = 1.0f / 0.0f
io.print("NaN as string: '%s'", nan_val.toString())
io.print("Infinity as string: '%s'", inf_val.toString())
```

### `toInt() → Int`

Converts the float to an integer by truncating the decimal part.

```o2l
# Float to integer conversion
test_floats: List<Float> = [5.7f, -3.2f, 0.9f, -0.1f, 42.0f]
float_iterator: ListIterator = test_floats.iterator()

while (float_iterator.hasNext()) {
    f: Float = float_iterator.next()
    i: Int = f.toInt()
    io.print("Float %f as Int: %d", f, i)
}

# Rounding vs truncation
value: Float = 5.7f
truncated: Int = value.toInt()           # 5 (truncated)
rounded: Int = (value + 0.5f).toInt()    # 6 (manual rounding)

io.print("Value %f: truncated=%d, rounded=%d", value, truncated, rounded)

# Range checking for large floats
large_float: Float = 1.0e20f
int_result: Int = large_float.toInt()
io.print("Large float %f as Int: %d (may overflow)", large_float, int_result)
```

### `toLong() → Long`

Converts the float to a Long integer.

```o2l
# Float to Long conversion for extended range
precise_floats: List<Float> = [1.23e10f, -9.87e15f, 0.5f]
precise_iterator: ListIterator = precise_floats.iterator()

while (precise_iterator.hasNext()) {
    f: Float = precise_iterator.next()
    l: Long = f.toLong()
    io.print("Float %f as Long: %s", f, l.toString())
}

# Use in time calculations
seconds_float: Float = 1609459200.5f  # Unix timestamp with fractional seconds
seconds_long: Long = seconds_float.toLong()
io.print("Timestamp %f as Long: %s", seconds_float, seconds_long.toString())
```

### `toBool() → Bool`

Converts the float to a boolean (0.0 = false, non-zero = true).

```o2l
# Boolean conversion
test_values: List<Float> = [0.0f, 0.1f, -0.1f, 5.5f, -2.3f]
bool_iterator: ListIterator = test_values.iterator()

while (bool_iterator.hasNext()) {
    val: Float = val_iterator.next()
    bool_result: Bool = val.toBool()
    io.print("Float %f as Bool: %o", val, bool_result)
}

# Special values
nan_bool: Bool = (0.0f / 0.0f).toBool()      # NaN -> true (non-zero)
inf_bool: Bool = (1.0f / 0.0f).toBool()      # Infinity -> true
neg_zero: Bool = (-0.0f).toBool()            # -0.0 -> false

io.print("Special values: NaN->%o, Inf->%o, -0.0->%o", nan_bool, inf_bool, neg_zero)

# Use in conditional logic
sensor_reading: Float = getSensorValue()  # Imaginary function
if (sensor_reading.toBool()) {
    io.print("Sensor active: %f", sensor_reading)
} else {
    io.print("Sensor inactive or zero")
}
```

---

## Practical Examples

### Scientific Calculations

```o2l
Object PhysicsCalculator {
    property gravity: Float    # 9.81 m/s²
    property speed_of_light: Float  # 2.998e8 m/s

    constructor() {
        this.gravity = 9.81f
        this.speed_of_light = 2.998e8f
    }

    method freefall_distance(time: Float): Float {
        # d = 0.5 * g * t²
        return 0.5f * this.gravity * time * time
    }

    method kinetic_energy(mass: Float, velocity: Float): Float {
        # KE = 0.5 * m * v²
        return 0.5f * mass * velocity * velocity
    }

    method relativistic_mass(rest_mass: Float, velocity: Float): Float {
        # m = m₀ / √(1 - v²/c²)
        v_ratio: Float = velocity / this.speed_of_light
        gamma: Float = 1.0f / sqrt(1.0f - v_ratio * v_ratio)  # sqrt would need to be available
        return rest_mass * gamma
    }
}

# Usage
physics: PhysicsCalculator = new PhysicsCalculator()
fall_time: Float = 2.5f
mass: Float = 10.0f
velocity: Float = 15.0f

distance: Float = physics.freefall_distance(fall_time)
energy: Float = physics.kinetic_energy(mass, velocity)

io.print("Object falls %f meters in %f seconds", distance, fall_time)
io.print("Kinetic energy: %f Joules", energy)
```

### Graphics and Animation

```o2l
Object GraphicsUtils {
    method degreesToRadians(degrees: Float): Float {
        pi: Float = 3.14159265f
        return degrees * pi / 180.0f
    }

    method radiansToDegrees(radians: Float): Float {
        pi: Float = 3.14159265f
        return radians * 180.0f / pi
    }

    method interpolate(start: Float, end: Float, t: Float): Float {
        # Linear interpolation: start + t * (end - start)
        # t should be between 0.0 and 1.0
        if (t < 0.0f) t = 0.0f
        if (t > 1.0f) t = 1.0f
        return start + t * (end - start)
    }

    method easeInOut(t: Float): Float {
        # Smooth cubic easing function
        if (t < 0.5f) {
            return 2.0f * t * t
        } else {
            return -1.0f + (4.0f - 2.0f * t) * t
        }
    }
}

# Usage for animation
graphics: GraphicsUtils = new GraphicsUtils()
start_position: Float = 10.0f
end_position: Float = 100.0f
animation_progress: Float = 0.3f  # 30% through animation

current_position: Float = graphics.interpolate(start_position, end_position, animation_progress)
eased_progress: Float = graphics.easeInOut(animation_progress)
eased_position: Float = graphics.interpolate(start_position, end_position, eased_progress)

io.print("Linear position: %f", current_position)
io.print("Eased position: %f", eased_position)

# Angle conversion
angle_degrees: Float = 45.0f
angle_radians: Float = graphics.degreesToRadians(angle_degrees)
io.print("%f degrees = %f radians", angle_degrees, angle_radians)
```

### Financial Calculations

```o2l
Object FinanceCalculator {
    method compound_interest(principal: Float, rate: Float, time: Float): Float {
        # A = P(1 + r)^t (simplified annual compounding)
        base: Float = 1.0f + rate
        # Would need pow function: return principal * pow(base, time)

        # Manual calculation for small integer times
        result: Float = principal
        i: Int = 0
        time_int: Int = time.toInt()
        while (i < time_int) {
            result = result * base
            i = i + 1
        }
        return result
    }

    method monthly_payment(loan: Float, rate: Float, months: Int): Float {
        # Monthly payment formula for loans
        if (rate == 0.0f) {
            return loan / months.toFloat()
        }

        monthly_rate: Float = rate / 12.0f
        # Payment = P * [r(1+r)^n] / [(1+r)^n - 1]
        # Simplified calculation without pow function
        factor: Float = 1.0f + monthly_rate

        # Calculate (1+r)^n iteratively
        power_factor: Float = 1.0f
        i: Int = 0
        while (i < months) {
            power_factor = power_factor * factor
            i = i + 1
        }

        numerator: Float = loan * monthly_rate * power_factor
        denominator: Float = power_factor - 1.0f

        return numerator / denominator
    }

    method percentage_change(old_value: Float, new_value: Float): Float {
        if (old_value == 0.0f) {
            return 0.0f  # Or handle error
        }
        return ((new_value - old_value) / old_value) * 100.0f
    }
}

# Usage
finance: FinanceCalculator = new FinanceCalculator()
principal: Float = 10000.0f
annual_rate: Float = 0.05f  # 5% annual interest
years: Float = 5.0f

final_amount: Float = finance.compound_interest(principal, annual_rate, years)
loan_payment: Float = finance.monthly_payment(20000.0f, 0.04f, 360)  # 30-year loan

old_price: Float = 100.0f
new_price: Float = 115.0f
price_change: Float = finance.percentage_change(old_price, new_price)

io.print("Investment of %f grows to %f in %f years", principal, final_amount, years)
io.print("Monthly loan payment: %f", loan_payment)
io.print("Price changed by %f%%", price_change)
```

---

## Error Handling

```o2l
# Safe Float operations
method safeDivision(dividend: Float, divisor: Float): Float {
    if (divisor == 0.0f) {
        io.error("Division by zero in Float operation")
        return 0.0f
    }

    result: Float = dividend / divisor
    if (result.isNaN()) {
        io.error("Division resulted in NaN")
        return 0.0f
    }
    if (result.isInfinite()) {
        io.error("Division resulted in infinity")
        return 0.0f
    }

    return result
}

# Safe conversion from text
method parseFloatSafely(text: Text): Float {
    try {
        result: Float = text.toFloat()
        if (result.isNaN()) {
            io.error("Parsed Float is NaN: %s", text)
            return 0.0f
        }
        return result
    } catch (error: Exception) {
        io.error("Invalid Float format: %s", text)
        return 0.0f
    }
}

# Validate calculation results
method validateResult(result: Float, operation: Text): Bool {
    if (result.isNaN()) {
        io.error("Operation '%s' produced NaN", operation)
        return false
    }
    if (result.isInfinite()) {
        io.error("Operation '%s' produced infinity", operation)
        return false
    }
    return true
}
```

---

## Performance Considerations

1. **Memory Usage**: Float uses 32 bits (4 bytes) per value
2. **Arithmetic Speed**: Single-precision operations are very fast
3. **Precision Trade-off**: 32-bit offers good speed but limited precision (~7 decimal digits)
4. **Special Values**: NaN and infinity checks have minimal overhead
5. **Conversion Costs**:
   - `toString()`: Moderate cost (string formatting)
   - Integer conversions: Low cost (truncation)
   - `toBool()`: Very low cost (zero check)

---

## Best Practices

1. **Precision Awareness**: Remember Float has only ~7 decimal digits of precision
2. **Equality Comparison**: Use epsilon-based comparison for calculated values
3. **Special Value Handling**: Always check for NaN and infinity in critical calculations
4. **Range Considerations**: Float range is large but finite
5. **Use Double When Needed**: For higher precision requirements, consider Double
6. **Input Validation**: Validate Float inputs in user-facing applications

---

## See Also

- [Double API Reference](Double.md) - Double-precision floating-point
- [Int API Reference](Int.md) - 64-bit integer type
- [Long API Reference](Long.md) - 128-bit integer type
- [Bool API Reference](Bool.md) - Boolean type
- [Text API Reference](Text.md) - String representation and parsing
- [Math Library](../system/math.md) - Mathematical functions for Float
