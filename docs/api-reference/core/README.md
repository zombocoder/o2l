# Core Types Reference

O²L provides a rich set of built-in types for common programming tasks. Each type includes comprehensive methods for manipulation and interaction.

## Data Types Overview

### Primitive Types
- **[Text](Text.md)** - Unicode strings with 48+ methods for manipulation
- **[Int](Numeric.md#int)** - 32-bit signed integers
- **[Long](Numeric.md#long)** - 64-bit signed integers  
- **[Float](Numeric.md#float)** - 32-bit floating point numbers
- **[Double](Numeric.md#double)** - 64-bit floating point numbers
- **[Bool](Bool.md)** - Boolean true/false values

### Collection Types
- **[List](List.md)** - Dynamic arrays with functional programming support
- **[Map](Map.md)** - Key-value dictionaries
- **[Set](Set.md)** - Collections of unique values

### Operators
- **[Operators](Operators.md)** - All operator types, precedence, and usage

## Quick Type Reference

| Type | Size | Range/Capacity | Key Methods |
|------|------|----------------|-------------|
| **Text** | Variable | Unicode strings | `length()`, `substring()`, `split()`, `replace()` |
| **Int** | 32-bit | -2³¹ to 2³¹-1 | `toString()`, `abs()`, arithmetic ops |
| **Long** | 64-bit | -2⁶³ to 2⁶³-1 | `toString()`, `abs()`, arithmetic ops |
| **Float** | 32-bit | IEEE 754 single | `toString()`, `isNaN()`, `isInfinite()` |
| **Double** | 64-bit | IEEE 754 double | `toString()`, `isNaN()`, `isInfinite()` |
| **Bool** | 1-bit | true/false | `toString()`, logical ops |
| **List** | Dynamic | Unlimited | `add()`, `get()`, `size()`, `filter()`, `map()` |
| **Map** | Dynamic | Unlimited | `put()`, `get()`, `containsKey()`, `keys()` |
| **Set** | Dynamic | Unlimited | `add()`, `contains()`, `size()`, `union()` |

## Type Conversions

### To Text (String)
```obq
# All types have toString()
int_str: Text = (42).toString()        # "42"
float_str: Text = (3.14).toString()    # "3.14"
bool_str: Text = true.toString()       # "true"
list_str: Text = my_list.toString()    # "[1, 2, 3]"
```

### From Text
```obq
# Text parsing methods
number: Int = "42".toInt()             # 42
decimal: Float = "3.14".toFloat()      # 3.14
flag: Bool = "true".toBool()           # true
words: List = "hello world".split()   # ["hello", "world"]
```

### Between Numeric Types
```obq
# Automatic promotion in expressions
result: Double = 42 + 3.14            # Int + Double = Double

# Explicit conversion
int_val: Int = 42
long_val: Long = int_val.toLong()      # Explicit conversion
float_val: Float = int_val.toFloat()   # Explicit conversion
```

## Common Patterns

### String Manipulation
```obq
text: Text = "Hello, World!"
upper: Text = text.toUpperCase()       # "HELLO, WORLD!"
words: List = text.split(", ")         # ["Hello", "World!"]
replaced: Text = text.replace("Hello", "Hi")  # "Hi, World!"
```

### List Operations
```obq
numbers: List = [1, 2, 3, 4, 5]
evens: List = numbers.filter(x -> x % 2 == 0)    # [2, 4]
doubled: List = numbers.map(x -> x * 2)          # [2, 4, 6, 8, 10]
sum: Int = numbers.reduce(0, (acc, x) -> acc + x) # 15
```

### Map Operations
```obq
person: Map = Map()
person.put("name", "Alice")
person.put("age", 30)

name: Text = person.get("name")        # "Alice"
has_email: Bool = person.containsKey("email")  # false
all_keys: List = person.keys()         # ["name", "age"]
```

### Set Operations
```obq
set1: Set = Set([1, 2, 3])
set2: Set = Set([3, 4, 5])

union: Set = set1.union(set2)          # {1, 2, 3, 4, 5}
intersection: Set = set1.intersection(set2)  # {3}
difference: Set = set1.difference(set2)      # {1, 2}
```

## Memory Management

- **Automatic Memory Management**: All types use garbage collection
- **Reference Semantics**: Objects (List, Map, Set) are passed by reference
- **Value Semantics**: Primitives (Int, Float, Bool, Text) are passed by value
- **Copy Behavior**: Use explicit `copy()` methods when needed

## Thread Safety

- **Immutable Types**: Text and numeric types are thread-safe
- **Mutable Collections**: List, Map, Set require synchronization for concurrent access
- **Atomic Operations**: Simple reads/writes to primitives are atomic

## Best Practices

### Type Selection
- Use **Text** for all string operations (comprehensive method set)
- Use **List** for ordered collections with duplicates
- Use **Set** for collections requiring uniqueness
- Use **Map** for key-value associations
- Choose appropriate numeric precision (Int vs Long, Float vs Double)

### Performance Tips
- **Text**: Use `StringBuilder` pattern for multiple concatenations
- **List**: Pre-size collections when capacity is known
- **Map**: Use appropriate hash-friendly key types
- **Set**: Leverage set operations for efficient filtering

### Error Handling
```obq
# Safe type conversions
try {
    number: Int = text.toInt()
} catch error {
    io.print("Invalid number format: %s", error.getMessage())
}

# Bounds checking
if index >= 0 && index < list.size() {
    value: Value = list.get(index)
}

# Null safety
if map.containsKey(key) {
    value: Value = map.get(key)
}
```

## See Also

- **[Operators](Operators.md)** - Operators that work with these types
- **[System Libraries](../system/README.md)** - Built-in system functionality
- **[JSON Library](../libraries/json.md)** - Serialization support for these types
- **[HTTP Libraries](../libraries/http-client.md)** - Network programming with core types