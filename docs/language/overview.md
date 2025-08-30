# Language Overview

O²L is an object-oriented programming language with a focus on simplicity and clarity.

## Key Features

- **Object-oriented**: Everything is built around objects and methods
- **Strong typing**: Variables must be declared with types
- **Automatic memory management**: No manual memory allocation needed
- **Simple syntax**: Clean, readable code structure

## Basic Syntax

### Variable Declarations

Variables are declared with explicit types:

```obq
name: Text = "Alice"
age: Int = 25
scores: List<Int> = [95, 87, 92]
```

### Objects and Methods

Define objects with methods:

```obq
Object Calculator {
    method add(a: Int, b: Int): Int {
        return a + b
    }
}
```

### Control Flow

Use parentheses for conditions:

```obq
if (age >= 18) {
    io.print("Adult")
} else {
    io.print("Minor")
}

while (iter.hasNext()) {
    value: Int = iter.next()
    io.print("Value: %d", value)
}
```

### Creating Objects

Use the `new` keyword to create instances:

```obq
calc: Calculator = new Calculator()
result: Int = calc.add(5, 3)
```

## File Structure

O²L files use the `.obq` extension and typically contain:

1. Import statements (if needed)
2. Object definitions
3. A Main object with a main() method

```obq
import system.io

Object Example {
    method greet(): Int {
        io.print("Hello, O²L!")
        return 0
    }
}

Object Main {
    method main(): Int {
        example: Example = new Example()
        return example.greet()
    }
}
```