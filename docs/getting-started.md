# Getting Started with O²L

Welcome to O²L Programming Language! This guide will help you write your first O²L program.

## Your First Program

Create a file called `hello.obq` with the following content:

```obq
import system.io

Object Main {
    method main(): Int {
        io.print("Hello, O²L!")
        return 0
    }
}
```

## Program Structure

Every O²L program consists of:

1. **Import statements** (optional) - Load external modules
2. **Object definitions** - Define your program's objects
3. **Main object** - Entry point with `main()` method

```obq
# 1. Imports
import system.io

# 2. Object definitions
Object Greeter {
    method sayHello(name: Text): Void {
        io.print("Hello, %s!", name)
    }
}

# 3. Main object
Object Main {
    method main(): Int {
        greeter: Greeter = new Greeter()
        greeter.sayHello("World")
        return 0
    }
}
```

## Basic Concepts

### Variables and Types

Variables must be declared with explicit types:

```obq
name: Text = "Alice"
age: Int = 25
score: Float = 95.5
```

### Objects and Methods

Everything in O²L is organized around objects:

```obq
Object Calculator {
    method add(a: Int, b: Int): Int {
        return a + b
    }
}
```

### Creating Objects

Use the `new` keyword to create object instances:

```obq
calc: Calculator = new Calculator()
result: Int = calc.add(5, 3)
```

## Working with Collections

O²L provides built-in collection types:

```obq
import system.io

Object CollectionExample {
    method demo(): Int {
        # Lists
        numbers: List<Int> = [1, 2, 3]
        numbers.add(4)
        
        # Iterate through collections
        iter: ListIterator = numbers.iterator()
        while (iter.hasNext()) {
            value: Int = iter.next()
            io.print("Number: %d", value)
        }
        
        return 0
    }
}
```

## Control Flow

Use familiar control structures with parentheses:

```obq
import system.io

Object ControlExample {
    method demo(): Int {
        count: Int = 0
        
        # While loop
        while (count < 3) {
            io.print("Count: %d", count)
            count = count + 1
        }
        
        # If statement
        if (count >= 3) {
            io.print("Loop completed!")
        }
        
        return 0
    }
}
```

## Next Steps

- Read the [Language Overview](language/overview.md) for complete syntax
- Explore [Control Flow](language/control-flow.md) for loops and conditionals
- Learn about [Objects and Classes](language/objects-classes.md)
- Check out the [Standard Library Overview](standard-library.md) for available modules
- Browse [Examples and Tutorials](examples-tutorials.md) for practical code

## Common Patterns

### Error Handling
```obq
method divide(a: Int, b: Int): Int {
    if (b == 0) {
        return -1  # Error indicator
    }
    return a / b
}
```

### Object Properties
```obq
Object Person {
    property name: Text
    
    constructor(name: Text) {
        this.name = name
    }
    
    method getName(): Text {
        return this.name
    }
}
```