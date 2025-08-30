# Int API Reference

## Overview

Int is O²L's 64-bit signed integer type, providing efficient whole number operations with a range of -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807. Int values are immutable and thread-safe, supporting arithmetic operations, comparisons, and type conversions.

## Declaration & Literals

```o2l
# Integer literals
small: Int = 42
negative: Int = -100
large: Int = 9223372036854775807

# Binary, octal, and hex literals (if supported)
binary: Int = 0b101010       # 42 in binary
octal: Int = 0o52            # 42 in octal
hex: Int = 0x2A              # 42 in hexadecimal

# Zero and special values
zero: Int = 0
max_int: Int = 9223372036854775807
min_int: Int = -9223372036854775808
```

## Quick Example

```o2l
import system.io

# Basic integer operations
number: Int = 42
doubled: Int = number * 2
text: Text = number.toString()        # "42"
as_float: Float = number.toDouble()   # 42.0
io.print("Number: %d, Doubled: %d, As text: %s", number, doubled, text)
```

---

## Arithmetic Operations

Integers support all standard arithmetic operations:

```o2l
# Basic arithmetic
a: Int = 20
b: Int = 3

sum: Int = a + b          # 23
difference: Int = a - b   # 17
product: Int = a * b      # 60
quotient: Int = a / b     # 6 (integer division)
remainder: Int = a % b    # 2 (modulo)

io.print("Addition: %d + %d = %d", a, b, sum)
io.print("Division: %d / %d = %d remainder %d", a, b, quotient, remainder)

# Unary operations
positive: Int = +a        # 20
negative: Int = -a        # -20
```

### Overflow Behavior

```o2l
# Integer overflow protection
large1: Int = 9223372036854775800
large2: Int = 100

# This would overflow - behavior depends on implementation
# result: Int = large1 + large2  # May throw exception or wrap

# Safe arithmetic checking
if (large1 > 0 && large2 > 0) {
    max_safe: Int = 9223372036854775807
    if (large1 <= max_safe - large2) {
        safe_sum: Int = large1 + large2
        io.print("Safe addition: %d", safe_sum)
    } else {
        io.error("Addition would overflow")
    }
}
```

---

## Comparison Operations

```o2l
# Comparison operators
x: Int = 10
y: Int = 20
z: Int = 10

# Equality
equal: Bool = x == z       # true
not_equal: Bool = x != y   # true

# Ordering
less: Bool = x < y         # true
less_equal: Bool = x <= z  # true
greater: Bool = y > x      # true
greater_equal: Bool = z >= x  # true

io.print("Comparisons: %d == %d is %o", x, z, equal)
io.print("Ordering: %d < %d is %o", x, y, less)
```

---

## Type Conversion Methods

### `toString() → Text`

Converts the integer to its string representation.

```o2l
numbers: List<Int> = [0, 42, -17, 1000000]
iterator: ListIterator = numbers.iterator()

while (iterator.hasNext()) {
    num: Int = iterator.next()
    text: Text = num.toString()
    io.print("Integer %d as text: '%s'", num, text)
}

# Use in string building
count: Int = 5
message: Text = "You have " + count.toString() + " items"
io.print(message)  # "You have 5 items"
```

### `toDouble() → Double`

Converts the integer to a double-precision floating-point number.

```o2l
# Convert for mathematical operations
radius: Int = 5
pi: Double = 3.14159265358979323846
area: Double = pi * radius.toDouble() * radius.toDouble()
io.print("Circle area with radius %d: %f", radius, area)

# Precision preservation
large_int: Int = 1234567890
as_double: Double = large_int.toDouble()
io.print("Integer %d as double: %f", large_int, as_double)

# Use in mixed arithmetic
int_val: Int = 100
float_val: Float = 2.5
result: Float = int_val.toDouble() + float_val
io.print("Mixed arithmetic: %d + %f = %f", int_val, float_val, result)
```

### `toFloat() → Float`

Converts the integer to a single-precision floating-point number.

```o2l
# Float conversion for memory efficiency
coordinates: List<Int> = [100, 200, 300]
coord_iterator: ListIterator = coordinates.iterator()

while (coord_iterator.hasNext()) {
    coord: Int = coord_iterator.next()
    as_float: Float = coord.toFloat()
    io.print("Coordinate %d as float: %f", coord, as_float)
}

# Precision loss warning for large integers
very_large: Int = 16777217  # Beyond float precision
as_float: Float = very_large.toFloat()
back_to_int: Int = as_float.toInt()
io.print("Precision test: %d -> %f -> %d", very_large, as_float, back_to_int)
```

### `toLong() → Long`

Converts the integer to a Long (128-bit integer where supported).

```o2l
# Convert to Long for extended range
int_value: Int = 1000000
long_value: Long = int_value.toLong()
io.print("Integer %d as Long: %s", int_value, long_value.toString())

# Use in calculations requiring extended precision
factorial_base: Int = 20
factorial_long: Long = factorial_base.toLong()
# ... factorial calculation would use Long arithmetic
```

### `toBool() → Bool`

Converts the integer to a boolean value (0 = false, non-zero = true).

```o2l
# Boolean conversion examples
values: List<Int> = [0, 1, -1, 42, 100]
value_iterator: ListIterator = values.iterator()

while (value_iterator.hasNext()) {
    num: Int = value_iterator.next()
    bool_val: Bool = num.toBool()
    io.print("Integer %d as boolean: %o", num, bool_val)
}

# Use in conditional logic
status_code: Int = getStatusCode()  # Imaginary function
if (status_code.toBool()) {
    io.print("Operation successful (status: %d)", status_code)
} else {
    io.print("Operation failed")
}
```

---

## Practical Examples

### Range Validation

```o2l
Object RangeValidator {
    method validateAge(age: Int): Bool {
        min_age: Int = 0
        max_age: Int = 150
        return age >= min_age && age <= max_age
    }

    method validatePercentage(percent: Int): Bool {
        return percent >= 0 && percent <= 100
    }

    method clampValue(value: Int, min_val: Int, max_val: Int): Int {
        if (value < min_val) {
            return min_val
        } else if (value > max_val) {
            return max_val
        } else {
            return value
        }
    }
}

# Usage
validator: RangeValidator = new RangeValidator()
test_ages: List<Int> = [25, -5, 200, 45]

age_iterator: ListIterator = test_ages.iterator()
while (age_iterator.hasNext()) {
    age: Int = age_iterator.next()
    valid: Bool = validator.validateAge(age)
    io.print("Age %d is valid: %o", age, valid)
}
```

### Counter and Accumulator

```o2l
Object Counter {
    property value: Int

    constructor() {
        this.value = 0
    }

    constructor(initial: Int) {
        this.value = initial
    }

    method increment(): Int {
        this.value = this.value + 1
        return this.value
    }

    method decrement(): Int {
        this.value = this.value - 1
        return this.value
    }

    method add(amount: Int): Int {
        this.value = this.value + amount
        return this.value
    }

    method reset(): Void {
        this.value = 0
    }

    method getValue(): Int {
        return this.value
    }
}

# Usage
counter: Counter = new Counter()
io.print("Initial: %d", counter.getValue())
io.print("After increment: %d", counter.increment())
io.print("After adding 10: %d", counter.add(10))
io.print("After decrement: %d", counter.decrement())
```

### Mathematical Calculations

```o2l
Object MathUtils {
    method factorial(n: Int): Long {
        if (n < 0) {
            return 0  # Error case
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

    method gcd(a: Int, b: Int): Int {
        # Greatest Common Divisor using Euclidean algorithm
        while (b != 0) {
            temp: Int = b
            b = a % b
            a = temp
        }
        return a
    }

    method isPrime(n: Int): Bool {
        if (n <= 1) {
            return false
        }
        if (n <= 3) {
            return true
        }
        if (n % 2 == 0 || n % 3 == 0) {
            return false
        }

        i: Int = 5
        while (i * i <= n) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false
            }
            i = i + 6
        }
        return true
    }
}

# Usage
math: MathUtils = new MathUtils()
number: Int = 17

fact: Long = math.factorial(5)
is_prime: Bool = math.isPrime(number)
gcd_result: Int = math.gcd(48, 18)

io.print("Factorial of 5: %s", fact.toString())
io.print("Is %d prime? %o", number, is_prime)
io.print("GCD of 48 and 18: %d", gcd_result)
```

### Data Processing and Statistics

```o2l
Object Statistics {
    method sum(numbers: List<Int>): Int {
        total: Int = 0
        iterator: ListIterator = numbers.iterator()
        while (iterator.hasNext()) {
            total = total + iterator.next()
        }
        return total
    }

    method average(numbers: List<Int>): Double {
        if (numbers.size() == 0) {
            return 0.0
        }
        total: Int = this.sum(numbers)
        return total.toDouble() / numbers.size().toDouble()
    }

    method min(numbers: List<Int>): Int {
        if (numbers.size() == 0) {
            return 0  # Or throw exception
        }

        iterator: ListIterator = numbers.iterator()
        min_val: Int = iterator.next()

        while (iterator.hasNext()) {
            current: Int = iterator.next()
            if (current < min_val) {
                min_val = current
            }
        }
        return min_val
    }

    method max(numbers: List<Int>): Int {
        if (numbers.size() == 0) {
            return 0  # Or throw exception
        }

        iterator: ListIterator = numbers.iterator()
        max_val: Int = iterator.next()

        while (iterator.hasNext()) {
            current: Int = iterator.next()
            if (current > max_val) {
                max_val = current
            }
        }
        return max_val
    }
}

# Usage
stats: Statistics = new Statistics()
test_numbers: List<Int> = [10, 5, 8, 12, 3, 15, 7]

total: Int = stats.sum(test_numbers)
avg: Double = stats.average(test_numbers)
minimum: Int = stats.min(test_numbers)
maximum: Int = stats.max(test_numbers)

io.print("Numbers: [10, 5, 8, 12, 3, 15, 7]")
io.print("Sum: %d", total)
io.print("Average: %f", avg)
io.print("Min: %d, Max: %d", minimum, maximum)
```

---

## Error Handling

```o2l
# Safe integer operations
method safeDivision(dividend: Int, divisor: Int): Int {
    if (divisor == 0) {
        io.error("Division by zero")
        return 0
    }
    return dividend / divisor
}

# Safe conversion from text
method parseIntSafely(text: Text): Int {
    try {
        return text.toInt()
    } catch (error: Exception) {
        io.error("Invalid integer format: %s", text)
        return 0  # Default value
    }
}

# Range checking
method safeSubtraction(a: Int, b: Int): Int {
    min_int: Int = -9223372036854775808
    if (a > 0 && b < 0 && a - b < min_int + b) {
        io.error("Subtraction would underflow")
        return min_int
    }
    return a - b
}
```

---

## Performance Considerations

1. **Memory Efficiency**: Int uses 64 bits (8 bytes) per value
2. **Arithmetic Speed**: Integer operations are highly optimized
3. **Comparison Speed**: Integer comparisons are constant time
4. **Conversion Cost**:
   - `toString()`: Moderate cost (string allocation)
   - `toDouble()`, `toFloat()`: Low cost (direct conversion)
   - `toLong()`: Very low cost (extension)
   - `toBool()`: Very low cost (zero check)

---

## Best Practices

1. **Range Awareness**: Be mindful of integer overflow/underflow
2. **Division Safety**: Always check for division by zero
3. **Type Conversions**: Use appropriate target types for conversions
4. **Loop Counters**: Int is ideal for loop indices and counters
5. **Bit Operations**: Use for flags and packed data (if supported)

---

## See Also

- [Long API Reference](Long.md) - 128-bit integer type
- [Float API Reference](Float.md) - Single-precision floating-point
- [Double API Reference](Double.md) - Double-precision floating-point
- [Bool API Reference](Bool.md) - Boolean type
- [Text API Reference](Text.md) - String representation and parsing
