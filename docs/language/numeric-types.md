# Numeric Types

O²L provides several numeric types for different precision and performance requirements. This guide covers all numeric types, their characteristics, operations, and best practices.

## Numeric Type Overview

| Type | Size | Range | Precision | Usage |
|------|------|--------|-----------|--------|
| `Int` | 32-bit | -2³¹ to 2³¹-1 | Integer | General integer arithmetic |
| `Long` | 64-bit | -2⁶³ to 2⁶³-1 | Integer | Large integer values |
| `Float` | 32-bit | IEEE 754 single | ~7 decimal digits | General floating-point |
| `Double` | 64-bit | IEEE 754 double | ~15 decimal digits | High-precision decimals |

## Integer Types

### Int (32-bit Integer)

```obq
Object IntDemo {
    @external method demonstrateInt(): Int {
        # Basic integer literals
        small: Int = 42
        negative: Int = -123
        zero: Int = 0
        
        # Maximum and minimum values
        maxInt: Int = 2147483647
        minInt: Int = -2147483648
        
        io.print("Small: %d", small)
        io.print("Negative: %d", negative)
        io.print("Max Int: %d", maxInt)
        io.print("Min Int: %d", minInt)
        
        return 0
    }
    
    @external method intArithmetic(): Int {
        a: Int = 10
        b: Int = 3
        
        sum: Int = a + b
        difference: Int = a - b
        product: Int = a * b
        quotient: Int = a / b        # Integer division: 3
        remainder: Int = a % b       # Modulo: 1
        
        io.print("a = %d, b = %d", a, b)
        io.print("Sum: %d", sum)
        io.print("Difference: %d", difference)
        io.print("Product: %d", product)
        io.print("Quotient: %d", quotient)
        io.print("Remainder: %d", remainder)
        
        return 0
    }
}
```

### Long (64-bit Integer)

```obq
Object LongDemo {
    @external method demonstrateLong(): Int {
        # Long literals with 'L' suffix
        bigNumber: Long = 1234567890123456789L
        negativeLong: Long = -987654321098765432L
        
        # Maximum and minimum values
        maxLong: Long = 9223372036854775807L
        minLong: Long = -9223372036854775808L
        
        io.print("Big number: %l", bigNumber)
        io.print("Negative long: %l", negativeLong)
        io.print("Max Long: %l", maxLong)
        io.print("Min Long: %l", minLong)
        
        return 0
    }
    
    @external method longOperations(): Int {
        # Large number calculations
        billion: Long = 1000000000L
        trillion: Long = billion * 1000L
        
        # Time calculations (milliseconds since epoch)
        currentTime: Long = system.currentTimeMillis()
        futureTime: Long = currentTime + (365L * 24L * 60L * 60L * 1000L)  # +1 year
        
        io.print("Billion: %l", billion)
        io.print("Trillion: %l", trillion)
        io.print("Current time: %l ms", currentTime)
        io.print("Future time: %l ms", futureTime)
        
        return 0
    }
}
```

## Floating-Point Types

### Float (32-bit Floating-Point)

```obq
Object FloatDemo {
    @external method demonstrateFloat(): Int {
        # Float literals with 'f' suffix
        pi: Float = 3.14159f
        negative: Float = -2.718f
        scientific: Float = 1.23e-4f    # Scientific notation
        
        # Special values
        positiveInf: Float = Float.POSITIVE_INFINITY
        negativeInf: Float = Float.NEGATIVE_INFINITY
        notANumber: Float = Float.NaN
        
        io.print("Pi (float): %f", pi)
        io.print("Negative: %f", negative)
        io.print("Scientific: %f", scientific)
        io.print("Positive infinity: %f", positiveInf)
        io.print("Negative infinity: %f", negativeInf)
        io.print("NaN: %f", notANumber)
        
        return 0
    }
    
    @external method floatPrecision(): Int {
        # Demonstrate float precision limits
        value1: Float = 0.1f
        value2: Float = 0.2f
        sum: Float = value1 + value2
        
        # This might not equal exactly 0.3 due to floating-point precision
        io.print("0.1f + 0.2f = %f", sum)
        io.print("Equals 0.3f? %s", (sum == 0.3f))
        
        # Large number precision loss
        large: Float = 16777216f       # 2^24
        largePlusOne: Float = large + 1f
        
        io.print("Large: %f", large)
        io.print("Large + 1: %f", largePlusOne)
        io.print("Are they equal? %s", (large == largePlusOne))  # May be true!
        
        return 0
    }
}
```

### Double (64-bit Floating-Point)

```obq
Object DoubleDemo {
    @external method demonstrateDouble(): Int {
        # Double literals (default for decimal numbers)
        pi: Double = 3.141592653589793
        e: Double = 2.718281828459045
        scientific: Double = 6.022e23   # Avogadro's number
        
        # High precision calculations
        precise: Double = 1.0 / 3.0
        verySmall: Double = 1e-15
        veryLarge: Double = 1e15
        
        io.print("Pi (double): %.15f", pi)
        io.print("e: %.15f", e)
        io.print("Scientific: %.3e", scientific)
        io.print("1/3: %.15f", precise)
        io.print("Very small: %.15e", verySmall)
        io.print("Very large: %.3e", veryLarge)
        
        return 0
    }
    
    @external method doublePrecision(): Int {
        # Better precision than float
        value1: Double = 0.1
        value2: Double = 0.2
        sum: Double = value1 + value2
        
        io.print("0.1 + 0.2 = %.17f", sum)
        io.print("Equals 0.3? %s", (sum == 0.3))
        
        # Still has precision limits, but much better than float
        large: Double = 9007199254740992.0    # 2^53
        largePlusOne: Double = large + 1.0
        
        io.print("Large: %.1f", large)
        io.print("Large + 1: %.1f", largePlusOne)
        io.print("Are they equal? %s", (large == largePlusOne))
        
        return 0
    }
}
```

## Type Conversions

### Implicit Conversions (Widening)

```obq
Object ConversionDemo {
    @external method implicitConversions(): Int {
        # Automatic widening conversions
        intValue: Int = 42
        longValue: Long = intValue      # Int to Long
        floatValue: Float = intValue    # Int to Float
        doubleValue: Double = intValue  # Int to Double
        
        floatFromLong: Float = longValue     # Long to Float (may lose precision)
        doubleFromLong: Double = longValue   # Long to Double
        doubleFromFloat: Double = floatValue # Float to Double
        
        io.print("Original int: %d", intValue)
        io.print("As long: %l", longValue)
        io.print("As float: %f", floatValue)
        io.print("As double: %f", doubleValue)
        
        return 0
    }
}
```

### Explicit Conversions (Narrowing)

```obq
Object ExplicitConversionDemo {
    @external method explicitConversions(): Int {
        # Explicit conversions may lose data
        doubleValue: Double = 123.456
        floatValue: Float = doubleValue.toFloat()    # May lose precision
        longValue: Long = doubleValue.toLong()       # Truncates decimal part
        intValue: Int = doubleValue.toInt()          # May overflow
        
        io.print("Original double: %f", doubleValue)
        io.print("As float: %f", floatValue)
        io.print("As long: %l", longValue)
        io.print("As int: %d", intValue)
        
        # Overflow example
        bigLong: Long = 3000000000L
        overflowInt: Int = bigLong.toInt()  # Wraps around due to overflow
        
        io.print("Big long: %l", bigLong)
        io.print("Overflowed int: %d", overflowInt)
        
        return 0
    }
}
```

### String Conversions

```obq
Object StringConversionDemo {
    @external method numberToString(): Int {
        intValue: Int = 42
        longValue: Long = 1234567890L
        floatValue: Float = 3.14f
        doubleValue: Double = 2.718281828
        
        # Convert numbers to strings
        intString: Text = intValue.toString()
        longString: Text = longValue.toString()
        floatString: Text = floatValue.toString()
        doubleString: Text = doubleValue.toString()
        
        io.print("Int as string: '%s'", intString)
        io.print("Long as string: '%s'", longString)
        io.print("Float as string: '%s'", floatString)
        io.print("Double as string: '%s'", doubleString)
        
        return 0
    }
    
    @external method stringToNumber(): Int {
        # Parse strings to numbers
        intString: Text = "123"
        longString: Text = "9876543210"
        floatString: Text = "3.14159"
        doubleString: Text = "2.718281828459045"
        
        try {
            parsedInt: Int = intString.toInt()
            parsedLong: Long = longString.toLong()
            parsedFloat: Float = floatString.toFloat()
            parsedDouble: Double = doubleString.toDouble()
            
            io.print("Parsed int: %d", parsedInt)
            io.print("Parsed long: %l", parsedLong)
            io.print("Parsed float: %f", parsedFloat)
            io.print("Parsed double: %f", parsedDouble)
            
        } catch (NumberFormatException error) {
            io.print("Parse error: %s", error.getMessage())
        }
        
        return 0
    }
}
```

## Mathematical Operations

### Basic Arithmetic

```obq
Object ArithmeticDemo {
    @external method basicOperations(): Int {
        # Integer arithmetic
        a: Int = 17
        b: Int = 5
        
        io.print("Integer arithmetic with a=%d, b=%d:", a, b)
        io.print("Addition: %d + %d = %d", a, b, a + b)
        io.print("Subtraction: %d - %d = %d", a, b, a - b)
        io.print("Multiplication: %d * %d = %d", a, b, a * b)
        io.print("Division: %d / %d = %d", a, b, a / b)        # Integer division
        io.print("Modulo: %d %% %d = %d", a, b, a % b)
        
        # Floating-point arithmetic
        x: Double = 17.0
        y: Double = 5.0
        
        io.print("\nFloating-point arithmetic with x=%.1f, y=%.1f:", x, y)
        io.print("Addition: %.1f + %.1f = %.1f", x, y, x + y)
        io.print("Subtraction: %.1f - %.1f = %.1f", x, y, x - y)
        io.print("Multiplication: %.1f * %.1f = %.1f", x, y, x * y)
        io.print("Division: %.1f / %.1f = %.1f", x, y, x / y)   # Floating-point division
        
        return 0
    }
}
```

### Advanced Math Operations

```obq
import math

Object MathDemo {
    @external method advancedMath(): Int {
        # Power and roots
        base: Double = 2.0
        exponent: Double = 3.0
        power: Double = math.pow(base, exponent)
        sqrt: Double = math.sqrt(16.0)
        cbrt: Double = math.cbrt(8.0)
        
        io.print("Power: %.1f^%.1f = %.1f", base, exponent, power)
        io.print("Square root of 16: %.1f", sqrt)
        io.print("Cube root of 8: %.1f", cbrt)
        
        # Trigonometric functions
        angle: Double = math.pi() / 4.0  # 45 degrees in radians
        sin: Double = math.sin(angle)
        cos: Double = math.cos(angle)
        tan: Double = math.tan(angle)
        
        io.print("For angle π/4 (45°):")
        io.print("sin = %.6f", sin)
        io.print("cos = %.6f", cos)
        io.print("tan = %.6f", tan)
        
        # Logarithms
        logValue: Double = math.log(math.e())      # Natural log of e = 1
        log10Value: Double = math.log10(100.0)    # Base-10 log of 100 = 2
        
        io.print("ln(e) = %.1f", logValue)
        io.print("log10(100) = %.1f", log10Value)
        
        return 0
    }
}
```

## Numeric Ranges and Limits

### Overflow and Underflow

```obq
Object LimitsDemo {
    @external method integerOverflow(): Int {
        # Integer overflow wraps around
        maxInt: Int = 2147483647
        overflowed: Int = maxInt + 1
        
        io.print("Max int: %d", maxInt)
        io.print("Max int + 1: %d", overflowed)  # Becomes negative!
        
        # Underflow
        minInt: Int = -2147483648
        underflowed: Int = minInt - 1
        
        io.print("Min int: %d", minInt)
        io.print("Min int - 1: %d", underflowed)  # Becomes positive!
        
        return 0
    }
    
    @external method floatingPointLimits(): Int {
        # Float limits
        maxFloat: Float = Float.MAX_VALUE
        minFloat: Float = Float.MIN_VALUE
        epsilon: Float = Float.EPSILON
        
        io.print("Max float: %e", maxFloat)
        io.print("Min positive float: %e", minFloat)
        io.print("Float epsilon: %e", epsilon)
        
        # Double limits
        maxDouble: Double = Double.MAX_VALUE
        minDouble: Double = Double.MIN_VALUE
        doubleEpsilon: Double = Double.EPSILON
        
        io.print("Max double: %e", maxDouble)
        io.print("Min positive double: %e", minDouble)
        io.print("Double epsilon: %e", doubleEpsilon)
        
        # Special floating-point values
        infinity: Double = Double.POSITIVE_INFINITY
        negInfinity: Double = Double.NEGATIVE_INFINITY
        nan: Double = Double.NaN
        
        io.print("Positive infinity: %f", infinity)
        io.print("Negative infinity: %f", negInfinity)
        io.print("NaN: %f", nan)
        
        return 0
    }
}
```

## Comparison and Equality

### Integer Comparison

```obq
Object ComparisonDemo {
    @external method integerComparison(): Int {
        a: Int = 10
        b: Int = 20
        c: Int = 10
        
        # Comparison operators
        io.print("%d == %d: %s", a, c, a == c)
        io.print("%d != %d: %s", a, b, a != b)
        io.print("%d < %d: %s", a, b, a < b)
        io.print("%d <= %d: %s", a, c, a <= c)
        io.print("%d > %d: %s", b, a, b > a)
        io.print("%d >= %d: %s", a, c, a >= c)
        
        return 0
    }
    
    @external method floatingPointComparison(): Int {
        # Floating-point comparison pitfalls
        a: Double = 0.1 + 0.2
        b: Double = 0.3
        
        io.print("0.1 + 0.2 = %.17f", a)
        io.print("0.3 = %.17f", b)
        io.print("Are they equal? %s", a == b)  # Likely false!
        
        # Proper floating-point comparison
        epsilon: Double = 1e-10
        difference: Double = math.abs(a - b)
        isEqual: Bool = difference < epsilon
        
        io.print("Difference: %.17f", difference)
        io.print("Within epsilon? %s", isEqual)
        
        return 0
    }
}
```

## Utility Methods

### Number Utilities

```obq
Object NumberUtils {
    method isEven(number: Int): Bool {
        return (number % 2) == 0
    }
    
    method isOdd(number: Int): Bool {
        return (number % 2) != 0
    }
    
    method abs(number: Int): Int {
        if (number < 0) {
            return -number
        }
        return number
    }
    
    method max(a: Int, b: Int): Int {
        if (a > b) {
            return a
        }
        return b
    }
    
    method min(a: Int, b: Int): Int {
        if (a < b) {
            return a
        }
        return b
    }
    
    method clamp(value: Int, minValue: Int, maxValue: Int): Int {
        if (value < minValue) {
            return minValue
        }
        if (value > maxValue) {
            return maxValue
        }
        return value
    }
    
    @external method demonstrateUtilities(): Int {
        numbers: List<Int> = [42, -17, 0, 100, -5]
        
        for (i: Int = 0; i < numbers.size(); i = i + 1) {
            num: Int = numbers.get(i)
            io.print("Number: %d", num)
            io.print("  Even: %s", this.isEven(num))
            io.print("  Odd: %s", this.isOdd(num))
            io.print("  Absolute: %d", this.abs(num))
            io.print("  Clamped (0-50): %d", this.clamp(num, 0, 50))
        }
        
        return 0
    }
}
```

## Best Practices

### Choosing the Right Type

```obq
Object TypeChoiceDemo {
    @external method demonstrateTypeChoice(): Int {
        # Use Int for general integer arithmetic
        count: Int = 10
        index: Int = 0
        
        # Use Long for large numbers, timestamps, file sizes
        fileSize: Long = 2147483648L  # > 2GB
        timestamp: Long = system.currentTimeMillis()
        
        # Use Float for memory-constrained applications
        coordinates: List<Float> = [1.0f, 2.5f, 3.7f]  # 3D graphics
        
        # Use Double for scientific calculations, financial calculations
        pi: Double = 3.141592653589793
        interestRate: Double = 0.045  # 4.5%
        
        io.print("Count: %d", count)
        io.print("File size: %l bytes", fileSize)
        io.print("Timestamp: %l ms", timestamp)
        io.print("Pi: %.15f", pi)
        io.print("Interest rate: %.3f", interestRate)
        
        return 0
    }
}
```

### Avoiding Common Pitfalls

```obq
Object PitfallDemo {
    @external method avoidOverflow(): Int {
        # Check for potential overflow before arithmetic
        a: Int = 2000000000
        b: Int = 300000000
        
        # Unsafe: might overflow
        # sum: Int = a + b
        
        # Safe: use Long for intermediate calculation
        longSum: Long = a.toLong() + b.toLong()
        
        if (longSum > Int.MAX_VALUE) {
            io.print("Result would overflow Int: %l", longSum)
        } else {
            sum: Int = longSum.toInt()
            io.print("Safe sum: %d", sum)
        }
        
        return 0
    }
    
    @external method avoidFloatingPointErrors(): Int {
        # Don't use floating-point for exact decimal arithmetic
        # Bad for monetary calculations
        price: Double = 0.1
        quantity: Int = 3
        total: Double = price * quantity
        
        io.print("Price * Quantity: %.17f", total)  # Not exactly 0.3!
        
        # Better: use integers for cents
        priceCents: Int = 10  # 10 cents
        totalCents: Int = priceCents * quantity
        totalDollars: Double = totalCents / 100.0
        
        io.print("Total in cents: %d", totalCents)
        io.print("Total in dollars: %.2f", totalDollars)
        
        return 0
    }
}
```

Understanding numeric types and their characteristics is essential for writing efficient and correct O²L programs. Choose the appropriate type based on your precision requirements, value ranges, and performance needs.