# Long API Reference

## Overview

Long is O²L's 128-bit signed integer type (where supported), providing extended-precision arithmetic for applications requiring very large integer values. On systems without 128-bit support, Long falls back to 64-bit. Long values are immutable and thread-safe, supporting all arithmetic operations, comparisons, and type conversions.

## Declaration & Literals

```o2l
# Long literals (large numbers)
small_long: Long = 42
large_long: Long = 123456789012345678901234567890
very_large: Long = 170141183460469231731687303715884105727  # Max 128-bit value

# Conversion from Int
int_val: Int = 1000000
long_val: Long = int_val.toLong()

# Extended precision calculations
factorial_result: Long = 40320  # 8! (factorial of 8)
```

## Quick Example

```o2l
import system.io

# Large number operations
big_number: Long = 9876543210123456789
text: Text = big_number.toString()
as_double: Double = big_number.toDouble()
io.print("Big number: %s, As text: %s", text, text)
```

---

## Arithmetic Operations

Long supports all standard arithmetic operations with extended precision:

```o2l
# Basic arithmetic with large numbers
a: Long = 12345678901234567890
b: Long = 98765432109876543210

sum: Long = a + b
difference: Long = a - b
product: Long = a * b      # May overflow even with 128-bit
quotient: Long = b / a
remainder: Long = b % a

io.print("Sum: %s", sum.toString())
io.print("Product: %s", product.toString())
io.print("Division: %s / %s = %s", b.toString(), a.toString(), quotient.toString())

# Unary operations
positive: Long = +a
negative: Long = -a
```

### Large Number Calculations

```o2l
# Factorial calculation using Long
Object LongMath {
    method factorial(n: Int): Long {
        if (n < 0) {
            return 0
        }
        if (n <= 1) {
            return 1
        }

        result: Long = 1
        i: Int = 2
        while (i <= n) {
            result = result * i.toLong()
            i = i + 1
        }
        return result
    }

    method power(base: Long, exponent: Int): Long {
        if (exponent == 0) {
            return 1
        }
        if (exponent < 0) {
            return 0  # Cannot represent fractional results
        }

        result: Long = 1
        i: Int = 0
        while (i < exponent) {
            result = result * base
            i = i + 1
        }
        return result
    }
}

# Usage
math: LongMath = new LongMath()
fact_15: Long = math.factorial(15)  # 1307674368000
base: Long = 2
power_result: Long = math.power(base, 60)  # 2^60

io.print("15! = %s", fact_15.toString())
io.print("2^60 = %s", power_result.toString())
```

---

## Comparison Operations

```o2l
# Comparison with large numbers
x: Long = 123456789012345678901234567890
y: Long = 987654321098765432109876543210
z: Long = 123456789012345678901234567890

# Equality
equal: Bool = x == z       # true
not_equal: Bool = x != y   # true

# Ordering
less: Bool = x < y         # true
less_equal: Bool = x <= z  # true
greater: Bool = y > x      # true
greater_equal: Bool = z >= x  # true

io.print("Large number comparison: %s == %s is %o",
         x.toString(), z.toString(), equal)
```

---

## Type Conversion Methods

### `toString() → Text`

Converts the Long to its string representation.

```o2l
# String representation of large numbers
large_numbers: List<Long> = [
    0,
    123456789012345678901234567890,
    -987654321098765432109876543210,
    170141183460469231731687303715884105727  # Max 128-bit
]

iterator: ListIterator = large_numbers.iterator()
while (iterator.hasNext()) {
    num: Long = iterator.next()
    text: Text = num.toString()
    io.print("Long %s as text: '%s'", text, text)  # Note: same value
}

# Use in formatted output
scientific_notation: Long = 602214076000000000000000  # Avogadro's number (approx)
message: Text = "Avogadro's number: " + scientific_notation.toString()
io.print(message)
```

### `toInt() → Int`

Converts the Long to an Int, truncating if the value is too large.

```o2l
# Safe conversion to Int
small_long: Long = 42
int_value: Int = small_long.toInt()
io.print("Small Long %s as Int: %d", small_long.toString(), int_value)

# Large number conversion (may lose precision)
large_long: Long = 12345678901234567890
truncated_int: Int = large_long.toInt()
io.print("Large Long %s truncated to Int: %d",
         large_long.toString(), truncated_int)

# Safe conversion with range checking
method safeLongToInt(value: Long): Int {
    max_int: Long = 9223372036854775807
    min_int: Long = -9223372036854775808

    if (value > max_int) {
        io.error("Long value too large for Int: %s", value.toString())
        return 9223372036854775807  # Return max Int
    } else if (value < min_int) {
        io.error("Long value too small for Int: %s", value.toString())
        return -9223372036854775808  # Return min Int
    } else {
        return value.toInt()
    }
}
```

### `toDouble() → Double`

Converts the Long to a double-precision floating-point number.

```o2l
# Convert Long to Double for floating-point calculations
long_radius: Long = 1000000
pi: Double = 3.14159265358979323846
area: Double = pi * long_radius.toDouble() * long_radius.toDouble()

io.print("Circle area with radius %s: %f", long_radius.toString(), area)

# Precision considerations with very large Longs
very_large: Long = 123456789012345678901234567890
as_double: Double = very_large.toDouble()
io.print("Very large Long %s as Double: %f", very_large.toString(), as_double)
# Note: May lose precision due to Double's 53-bit mantissa
```

### `toFloat() → Float`

Converts the Long to a single-precision floating-point number.

```o2l
# Convert to Float (significant precision loss for large values)
coordinates: List<Long> = [1000000, 2000000, 3000000000000]
coord_iterator: ListIterator = coordinates.iterator()

while (coord_iterator.hasNext()) {
    coord: Long = coord_iterator.next()
    as_float: Float = coord.toFloat()
    io.print("Long coordinate %s as Float: %f", coord.toString(), as_float)
}

# Precision warning
huge_long: Long = 12345678901234567890
float_val: Float = huge_long.toFloat()
io.print("Precision loss: %s -> %f", huge_long.toString(), float_val)
```

### `toBool() → Bool`

Converts the Long to a boolean value (0 = false, non-zero = true).

```o2l
# Boolean conversion for large numbers
test_values: List<Long> = [0, 1, -1, 123456789012345678901234567890]
value_iterator: ListIterator = test_values.iterator()

while (value_iterator.hasNext()) {
    num: Long = value_iterator.next()
    bool_val: Bool = num.toBool()
    io.print("Long %s as boolean: %o", num.toString(), bool_val)
}

# Use in conditional logic
large_counter: Long = getEventCount()  # Imaginary function
if (large_counter.toBool()) {
    io.print("Events recorded: %s", large_counter.toString())
} else {
    io.print("No events recorded")
}
```

---

## Practical Examples

### Big Integer Arithmetic

```o2l
Object BigCalculator {
    method fibonacci(n: Int): Long {
        if (n <= 1) {
            return n.toLong()
        }

        a: Long = 0
        b: Long = 1
        i: Int = 2

        while (i <= n) {
            temp: Long = a + b
            a = b
            b = temp
            i = i + 1
        }
        return b
    }

    method gcd(a: Long, b: Long): Long {
        # Greatest Common Divisor for large numbers
        while (b != 0) {
            temp: Long = b
            b = a % b
            a = temp
        }
        return a
    }

    method combination(n: Long, r: Long): Long {
        # Calculate C(n,r) = n! / (r! * (n-r)!)
        # Using iterative approach to avoid factorial overflow
        if (r > n) {
            return 0
        }
        if (r == 0 || r == n) {
            return 1
        }

        # Choose smaller of r and n-r for efficiency
        if (r > n - r) {
            r = n - r
        }

        result: Long = 1
        i: Long = 0
        while (i < r) {
            result = result * (n - i)
            result = result / (i + 1)
            i = i + 1
        }
        return result
    }
}

# Usage
calc: BigCalculator = new BigCalculator()
fib_50: Long = calc.fibonacci(50)
large_gcd: Long = calc.gcd(123456789012345678901234567890, 987654321098765432109876543210)
combination: Long = calc.combination(100, 5)

io.print("50th Fibonacci number: %s", fib_50.toString())
io.print("Large GCD result: %s", large_gcd.toString())
io.print("C(100,5) = %s", combination.toString())
```

### Time and Duration Calculations

```o2l
Object TimeCalculator {
    # Nanoseconds since epoch (requires Long precision)
    method getCurrentNanos(): Long {
        # This would be implemented by system library
        return 1609459200000000000  # Example: 2021-01-01 00:00:00 UTC in nanoseconds
    }

    method nanosecondsToSeconds(nanos: Long): Long {
        nanos_per_second: Long = 1000000000
        return nanos / nanos_per_second
    }

    method nanosecondsToMillis(nanos: Long): Long {
        nanos_per_milli: Long = 1000000
        return nanos / nanos_per_milli
    }

    method addDuration(start_nanos: Long, duration_seconds: Int): Long {
        duration_nanos: Long = duration_seconds.toLong() * 1000000000
        return start_nanos + duration_nanos
    }
}

# Usage
timer: TimeCalculator = new TimeCalculator()
current_time: Long = timer.getCurrentNanos()
future_time: Long = timer.addDuration(current_time, 3600)  # Add 1 hour

io.print("Current time (nanoseconds): %s", current_time.toString())
io.print("Future time (nanoseconds): %s", future_time.toString())
io.print("Current time (seconds): %s", timer.nanosecondsToSeconds(current_time).toString())
```

### Large Data Set Processing

```o2l
Object DataProcessor {
    method processLargeIds(ids: List<Long>): Long {
        # Process a list of large ID numbers
        total: Long = 0
        count: Long = 0

        iterator: ListIterator = ids.iterator()
        while (iterator.hasNext()) {
            id: Long = iterator.next()
            total = total + id
            count = count + 1

            # Process in chunks to avoid memory issues
            if (count % 1000 == 0) {
                io.print("Processed %s IDs so far", count.toString())
            }
        }

        return total
    }

    method findLargestId(ids: List<Long>): Long {
        if (ids.size() == 0) {
            return 0
        }

        iterator: ListIterator = ids.iterator()
        largest: Long = iterator.next()

        while (iterator.hasNext()) {
            current: Long = iterator.next()
            if (current > largest) {
                largest = current
            }
        }

        return largest
    }

    method checksumIds(ids: List<Long>): Long {
        # Simple checksum algorithm for large IDs
        checksum: Long = 0
        multiplier: Long = 31

        iterator: ListIterator = ids.iterator()
        while (iterator.hasNext()) {
            id: Long = iterator.next()
            checksum = (checksum * multiplier + id) % 1000000007  # Large prime
        }

        return checksum
    }
}

# Usage with large ID numbers
processor: DataProcessor = new DataProcessor()
large_ids: List<Long> = [
    123456789012345678901234567890,
    987654321098765432109876543210,
    555666777888999000111222333444
]

total: Long = processor.processLargeIds(large_ids)
largest: Long = processor.findLargestId(large_ids)
checksum: Long = processor.checksumIds(large_ids)

io.print("Total of large IDs: %s", total.toString())
io.print("Largest ID: %s", largest.toString())
io.print("Checksum: %s", checksum.toString())
```

---

## Error Handling

```o2l
# Safe Long operations
method safeLongDivision(dividend: Long, divisor: Long): Long {
    if (divisor == 0) {
        io.error("Division by zero in Long operation")
        return 0
    }
    return dividend / divisor
}

# Safe parsing from text
method parseLongSafely(text: Text): Long {
    try {
        return text.toLong()
    } catch (error: Exception) {
        io.error("Invalid Long format: %s", text)
        return 0  # Default value
    }
}

# Overflow detection (approximate, since 128-bit limits are huge)
method safeMultiplication(a: Long, b: Long): Long {
    # Simple overflow check - more sophisticated checking would be needed
    if (a != 0 && b != 0) {
        # Approximate check using string length
        a_str: Text = a.toString()
        b_str: Text = b.toString()
        if (a_str.length() + b_str.length() > 35) {  # Rough estimate
            io.error("Multiplication may overflow Long capacity")
            return 0
        }
    }
    return a * b
}
```

---

## Performance Considerations

1. **Memory Usage**: Long uses 128 bits (16 bytes) where supported, 64 bits otherwise
2. **Arithmetic Speed**: Slower than Int operations due to extended precision
3. **Conversion Cost**:
   - `toString()`: Higher cost due to large number formatting
   - `toInt()`: Low cost (truncation)
   - `toDouble()`, `toFloat()`: Moderate cost, potential precision loss
   - `toBool()`: Very low cost (zero check)
4. **String Operations**: Long number strings can be expensive to process

---

## Platform Considerations

1. **128-bit Support**: Available on systems with `__int128` support
2. **Fallback Mode**: Uses 64-bit on systems without 128-bit integers
3. **Cross-Platform**: Behavior may vary between platforms
4. **Library Functions**: Some mathematical operations may have platform-specific implementations

---

## Best Practices

1. **Use When Needed**: Only use Long for calculations requiring extended precision
2. **Monitor Performance**: Long operations are slower than Int
3. **String Formatting**: Long.toString() can be expensive for very large numbers
4. **Overflow Awareness**: Even 128-bit integers can overflow with large calculations
5. **Conversion Safety**: Check ranges when converting Long to smaller types

---

## See Also

- [Int API Reference](Int.md) - 64-bit integer type
- [Float API Reference](Float.md) - Single-precision floating-point
- [Double API Reference](Double.md) - Double-precision floating-point
- [Bool API Reference](Bool.md) - Boolean type
- [Text API Reference](Text.md) - String representation and parsing
