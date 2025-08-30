# Standard Library Overview

O²L Programming Language comes with a comprehensive standard library providing essential functionality.

## System Modules

### system.io
Input/output operations for console interaction:

```obq
import system.io

# Print formatted output
io.print("Hello, %s!", "World")
io.print("Number: %d", 42)
io.print("Float: %f", 3.14)

# Read user input
input: Text = io.readLine("Enter your name: ")
```

**Available functions:**
- `print(format: Text, ...args)` - Print formatted text
- `readLine(prompt: Text): Text` - Read user input

### system.utils
Utility functions and iterators:

```obq
import system.utils

# Create repeat iterator
repeater: RepeatIterator = utils.repeat(5)  # Counts 0 to 4
```

**Available functions:**
- `repeat(count: Int): RepeatIterator` - Create counting iterator

### system.fs
File system operations:

```obq
import system.fs

# File operations (check API reference for details)
```

### system.os
Operating system utilities:

```obq
import system.os

# OS-level operations (check API reference for details)
```

## Core Collection Types

### List<T>
Dynamic arrays with type safety:

```obq
numbers: List<Int> = [1, 2, 3]
numbers.add(4)                    # Add element
first: Int = numbers.get(0)       # Get by index
numbers.remove(1)                 # Remove by index
size: Int = numbers.size()        # Get size
numbers.reverse()                 # Reverse order
last: Int = numbers.pop()         # Remove and return last
iter: ListIterator = numbers.iterator()  # Get iterator
```

### Map<K,V>
Key-value collections:

```obq
ages: Map<Text, Int> = new Map<Text, Int>()
ages.put("Alice", 25)             # Add key-value pair
age: Int = ages.get("Alice")      # Get by key
ages.remove("Alice")              # Remove by key
size: Int = ages.size()           # Get size
```

### Set<T>
Unique element collections:

```obq
categories: Set<Text> = new Set<Text>()
categories.add("Technology")      # Add element
has: Bool = categories.contains("Technology")  # Check membership
categories.remove("Technology")   # Remove element
size: Int = categories.size()     # Get size
```

## Iterator System

### ListIterator
For traversing List collections:

```obq
numbers: List<Int> = [1, 2, 3]
iter: ListIterator = numbers.iterator()

while (iter.hasNext()) {
    value: Int = iter.next()
    io.print("Value: %d", value)
}
```

### RepeatIterator
For counting operations:

```obq
import system.utils

repeater: RepeatIterator = utils.repeat(5)
while (repeater.hasNext()) {
    count: Int = repeater.next()  # 0, 1, 2, 3, 4
    io.print("Count: %d", count)
}

# Reset and reuse
repeater.reset()
currentCount: Int = repeater.getCurrentCount()  # 0
```

## Usage Examples

### Basic I/O Program
```obq
import system.io

Object Main {
    method main(): Int {
        io.print("What's your name?")
        name: Text = io.readLine("")
        io.print("Hello, %s!", name)
        return 0
    }
}
```

### Collection Processing
```obq
import system.io
import system.utils

Object DataProcessor {
    @external method processNumbers(): Int {
        # Create data
        numbers: List<Int> = []
        
        # Generate numbers using repeat iterator
        generator: RepeatIterator = utils.repeat(5)
        while (generator.hasNext()) {
            numbers.add(generator.next() * 10)
        }
        
        # Process data
        iter: ListIterator = numbers.iterator()
        total: Int = 0
        while (iter.hasNext()) {
            value: Int = iter.next()
            total = total + value
            io.print("Processing: %d", value)
        }
        
        io.print("Total: %d", total)
        return 0
    }
}
```

## Library Documentation

For detailed API documentation, see:
- [system.io API](api-reference/system/io.md)
- [system.fs API](api-reference/system/fs.md)  
- [system.os API](api-reference/system/os.md)
- [List API](api-reference/core/List.md)
- [Map API](api-reference/core/Map.md)
- [Set API](api-reference/core/Set.md)