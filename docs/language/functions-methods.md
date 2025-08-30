# Functions and Methods

O²L Programming Language uses methods within objects for all executable code.

## Method Definitions

Methods are defined inside objects with explicit parameter and return types:

```obq
Object Calculator {
    method add(a: Int, b: Int): Int {
        return a + b
    }
    
    method greet(name: Text): Text {
        return "Hello, " + name
    }
}
```

## Method Parameters

Parameters must specify their types:

```obq
method processData(input: Text, count: Int, options: List<Text>): Int {
    # Method body
    return 0
}
```

## Return Types

All methods must declare their return type:

```obq
method getName(): Text {
    return this.name
}

method calculate(): Int {
    return 42
}

method processItems(): Int {
    # Return 0 for success
    return 0
}
```

## External Methods

Methods marked with `@external` can interact with the runtime:

```obq
Object Demo {
    @external method testMethod(): Int {
        # External methods can perform I/O and other runtime operations
        return 0
    }
}
```

## Method Calls

Call methods on objects using dot notation:

```obq
calc: Calculator = new Calculator()
result: Int = calc.add(5, 3)
message: Text = calc.greet("Alice")
```

## Constructors

Objects can have constructors to initialize state:

```obq
Object Person {
    constructor(name: Text) {
        this.name = name
    }
    
    method getName(): Text {
        return this.name
    }
}

# Create with constructor
person: Person = new Person("Bob")
```

## Method Chaining

Methods can be chained when they return objects:

```obq
# If methods return objects, they can be chained
result: Text = new Person("Alice").getName()
```