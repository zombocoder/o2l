# Type System

O²L Programming Language uses a strong, static type system with explicit type declarations.

## Basic Types

### Primitive Types

```obq
# Numeric types
age: Int = 25
price: Float = 19.99
distance: Double = 1234.5678
fileSize: Long = 1073741824

# Text and Boolean
name: Text = "Alice"
isActive: Bool = true
```

### Void Type

Methods that don't return a value use `Void`:

```obq
Object Printer {
    method displayMessage(msg: Text): Void {
        io.print(msg)
        # No return statement needed for Void
    }
}
```

## Generic Types

O²L supports generic types for collections:

### List Generics

```obq
# Strongly typed lists
numbers: List<Int> = [1, 2, 3]
names: List<Text> = ["Alice", "Bob", "Charlie"]
flags: List<Bool> = [true, false, true]
```

### Map Generics

```obq
# Key-value pairs with types
ages: Map<Text, Int> = new Map<Text, Int>()
settings: Map<Text, Text> = new Map<Text, Text>()
```

### Set Generics

```obq
# Unique collections with types
uniqueNumbers: Set<Int> = new Set<Int>()
categories: Set<Text> = new Set<Text>()
```

## Object Types

Objects can be used as types:

```obq
Object Person {
    property name: Text
    
    constructor(name: Text) {
        this.name = name
    }
}

Object Company {
    property employees: List<Person>
    
    constructor() {
        this.employees = []
    }
    
    method addEmployee(person: Person): Void {
        this.employees.add(person)
    }
}
```

## Type Inference

While O²L requires explicit type declarations, it can infer types in some contexts:

```obq
# Explicit typing required for variables
numbers: List<Int> = [1, 2, 3]

# But method calls can infer from context
iter: ListIterator = numbers.iterator()  # ListIterator type inferred
```

## Iterator Types

Special types for iteration:

```obq
import system.utils

Object IteratorTypes {
    method demo(): Int {
        # List iterator type
        numbers: List<Int> = [1, 2, 3]
        listIter: ListIterator = numbers.iterator()
        
        # Repeat iterator type
        repeatIter: RepeatIterator = utils.repeat(5)
        
        return 0
    }
}
```

## Type Safety

O²L enforces type safety at compile time:

```obq
Object TypeSafety {
    method example(): Int {
        # Valid - matching types
        count: Int = 42
        
        # Invalid - would cause compilation error
        # count = "not a number"  # Type mismatch error
        
        return count
    }
}
```

## Nullable Types

Currently, O²L doesn't have explicit nullable types. Objects are either initialized or not:

```obq
Object Container {
    property data: Text
    
    constructor() {
        this.data = ""  # Must initialize
    }
}
```