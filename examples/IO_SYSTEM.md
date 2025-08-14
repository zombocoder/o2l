# O²L Native I/O System

The O²L language now includes a native I/O system implemented in C++ for efficient input/output operations.

## Import

```o2l
import system.io.*
```

## Available Functions

### `io.print(format: Text, ...args: Any): Text`

Prints formatted text to stdout with optional formatting.

**Examples:**
```o2l
io.print("Hello, World!")                    // Simple text
io.print("Number: %s", 42)                   // Format number
io.print("Text: %s", "Hello")                // Format string  
io.print("Multiple: %s and %s", 10, 20)      // Multiple values
```

**Format Specifiers:**
- `%s` - Replace with string representation of any value
- `%d` - Replace with integer value (future enhancement)

### `io.input(prompt: Text): Text`

Reads a line of text from stdin with optional prompt.

**Examples:**
```o2l
name: Text = io.input("Enter your name: ")   // With prompt
data: Text = io.input("")                    // No prompt
```

## Complete Example

```o2l
import system.io.*

Object Main {
    method main(): Text {
        // Get user input
        name: Text = io.input("What's your name? ")
        age: Text = io.input("What's your age? ")
        
        // Print formatted output
        io.print("Hello, %s!", name)
        io.print("You are %s years old.", age)
        
        return "Done"
    }
}
```

## Implementation Details

- **Native C++**: Implemented in `SystemLibrary.cpp` for performance
- **stdout/stdin**: Direct access to console I/O streams
- **String formatting**: Basic `%s` replacement with value conversion
- **Type safety**: Automatic conversion of O²L values to display strings
- **Error handling**: Proper exception handling for I/O errors

## Value Type Conversions

The I/O system automatically converts O²L values to strings:

- `Int` → `"42"`
- `Float` → `"3.14"`
- `Text` → `"hello"`
- `Bool` → `"true"` or `"false"`
- `Char` → `"A"`
- `Object` → `"Object(ClassName)"`

## Performance

Native I/O functions provide:
- Direct system call access
- No parsing/interpretation overhead
- Efficient string formatting
- Immediate console output