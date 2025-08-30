# system.io API Reference

## Overview

The system.io module provides essential input/output operations for O²L programs. It handles console input/output, formatted printing, error reporting, and basic user interaction through a clean, type-safe API.

## Import

```o2l
import system.io
# Or import all functions
import system.io.*
```

## Quick Example

```o2l
import system.io

io.print("Hello, %s!", "World")
name: Text = io.input("Enter your name: ")
io.print("Nice to meet you, %s", name)
```

---

## Output Functions

### `print(format: Text, ...args) → Void`

Prints formatted text to standard output with automatic newline.

**Format Specifiers:**

- `%s` - Text/String values
- `%d` - Integer values
- `%f` - Floating point values
- `%o` - Object representation (any type)
- `%%` - Literal percent sign

```o2l
# Basic printing
io.print("Hello, World!")

# Formatted printing with different types
name: Text = "Alice"
age: Int = 30
score: Float = 95.5f
active: Bool = true

io.print("Name: %s", name)                    // "Name: Alice"
io.print("Age: %d years old", age)            // "Age: 30 years old"
io.print("Score: %f points", score)           // "Score: 95.5 points"
io.print("Active: %o", active)                // "Active: true"

# Multiple arguments
io.print("User %s is %d years old with score %f", name, age, score)
// "User Alice is 30 years old with score 95.5"

# Collections and objects
numbers: List<Int> = [1, 2, 3, 4, 5]
io.print("Numbers: %o", numbers)              // "Numbers: [1, 2, 3, 4, 5]"

# Literal percent signs
io.print("Progress: 75%% complete")           // "Progress: 75% complete"
```

### `println(text: Text) → Void`

Prints text to standard output followed by a newline (no formatting).

```o2l
io.println("Simple text output")
io.println("Another line")
// Output:
// Simple text output
// Another line
```

### `write(text: Text) → Void`

Writes text to standard output without adding a newline.

```o2l
io.write("Loading")
io.write(".")
io.write(".")
io.write(".")
io.println(" Done!")
// Output: Loading... Done!
```

---

## Error Output

### `error(message: Text) → Void`

Prints an error message to standard error stream.

```o2l
io.error("File not found: config.txt")
io.error("Invalid input received")

# Can be used in error handling
try {
    risky_operation()
} catch (error: Exception) {
    io.error("Operation failed: %s", error.message)
}
```

### `warn(message: Text) → Void`

Prints a warning message to standard error stream.

```o2l
io.warn("Deprecated function used")
io.warn("Configuration file missing, using defaults")
```

---

## Input Functions

### `input(prompt: Text) → Text`

Displays a prompt and reads a line of input from the user.

```o2l
name: Text = io.input("Enter your name: ")
email: Text = io.input("Enter your email: ")

io.print("Hello %s, your email is %s", name, email)
```

### `readLine() → Text`

Reads a line of input without displaying a prompt.

```o2l
io.print("Please enter some text:")
user_input: Text = io.readLine()
io.print("You entered: %s", user_input)
```

### `readInt() → Int`

Reads and parses an integer from user input.

```o2l
age: Int = io.readInt("Enter your age: ")
io.print("You are %d years old", age)

# Error handling for invalid input
try {
    number: Int = io.readInt("Enter a number: ")
    io.print("Your number squared is %d", number * number)
} catch (error: EvaluationError) {
    io.error("Invalid number format")
}
```

### `readFloat() → Float`

Reads and parses a floating-point number from user input.

```o2l
temperature: Float = io.readFloat("Enter temperature in Celsius: ")
fahrenheit: Float = (temperature * 9.0f / 5.0f) + 32.0f
io.print("%.1f°C = %.1f°F", temperature, fahrenheit)
```

### `readBool() → Bool`

Reads and parses a boolean value from user input (accepts "true"/"false", "yes"/"no", "1"/"0").

```o2l
confirm: Bool = io.readBool("Do you want to continue? (yes/no): ")
if (confirm) {
    io.print("Continuing...")
} else {
    io.print("Operation cancelled")
}
```

---

## Advanced I/O Functions

### `printf(format: Text, ...args) → Void`

Advanced formatted printing with more control over output format.

```o2l
# Number formatting
value: Int = 42
io.printf("Value: %05d\n", value)        // "Value: 00042"

price: Float = 123.456f
io.printf("Price: $%.2f\n", price)       // "Price: $123.46"

# Alignment and padding
name: Text = "Alice"
io.printf("Name: %-10s|\n", name)        // "Name: Alice     |"
io.printf("Name: %10s|\n", name)         // "Name:      Alice|"
```

### `flush() → Void`

Forces any buffered output to be written immediately.

```o2l
io.write("Processing")
io.flush()  # Ensure "Processing" appears before long operation

# Simulate long operation
busy_work()

io.println(" Complete!")
```

---

## File-like Operations

### `getInput() → Optional<Text>`

Non-blocking input read. Returns empty if no input available.

```o2l
io.print("Press Enter to continue (or wait 5 seconds)...")

# Simulate timeout behavior
start_time: Long = system.time()
while (system.time() - start_time < 5000) {
    input: Optional<Text> = io.getInput()
    if (input.isPresent()) {
        io.print("Input received: %s", input.get())
        break
    }
    # Small delay to prevent busy waiting
    system.sleep(100)
}
```

### `hasInput() → Bool`

Checks if input is available for reading.

```o2l
if (io.hasInput()) {
    input: Text = io.readLine()
    io.print("Available input: %s", input)
} else {
    io.print("No input available")
}
```

---

## Interactive Applications

### Simple Calculator

```o2l
import system.io

Object Calculator {
    @external method run(): Int {
        io.print("=== Simple Calculator ===")
        io.print("Enter two numbers and an operation (+, -, *, /)")

        num1: Float = io.readFloat("First number: ")
        operator: Text = io.input("Operation (+, -, *, /): ")
        num2: Float = io.readFloat("Second number: ")

        result: Float = 0.0f

        if (operator == "+") {
            result = num1 + num2
        } else if (operator == "-") {
            result = num1 - num2
        } else if (operator == "*") {
            result = num1 * num2
        } else if (operator == "/") {
            if (num2 != 0.0f) {
                result = num1 / num2
            } else {
                io.error("Error: Division by zero!")
                return 1
            }
        } else {
            io.error("Error: Unknown operation '%s'", operator)
            return 1
        }

        io.print("Result: %.2f %s %.2f = %.2f", num1, operator, num2, result)
        return 0
    }
}
```

### Menu System

```o2l
import system.io

Object MenuDemo {
    @external method showMenu(): Int {
        while (true) {
            io.println("\n=== Main Menu ===")
            io.println("1. View Profile")
            io.println("2. Edit Settings")
            io.println("3. Help")
            io.println("0. Exit")

            choice: Int = io.readInt("Enter your choice: ")

            if (choice == 1) {
                this.showProfile()
            } else if (choice == 2) {
                this.editSettings()
            } else if (choice == 3) {
                this.showHelp()
            } else if (choice == 0) {
                io.print("Goodbye!")
                break
            } else {
                io.error("Invalid choice. Please try again.")
            }
        }
        return 0
    }

    method showProfile(): Void {
        io.println("Profile: John Doe (john@example.com)")
    }

    method editSettings(): Void {
        theme: Text = io.input("Enter theme (dark/light): ")
        io.print("Theme set to: %s", theme)
    }

    method showHelp(): Void {
        io.println("Help: Use the menu numbers to navigate")
    }
}
```

---

## Progress and Status Display

### Progress Bar

```o2l
import system.io

Object ProgressDemo {
    @external method showProgress(): Int {
        total: Int = 100
        i: Int = 0

        while (i <= total) {
            # Calculate progress percentage
            progress: Int = (i * 100) / total

            # Create progress bar
            io.write("\rProgress: [")

            # Draw filled portion
            filled: Int = progress / 5  # 20 chars = 100%
            j: Int = 0
            while (j < filled) {
                io.write("=")
                j = j + 1
            }

            # Draw empty portion
            while (j < 20) {
                io.write(" ")
                j = j + 1
            }

            io.write("] %d%%", progress)
            io.flush()

            # Simulate work
            system.sleep(50)
            i = i + 2
        }

        io.println("\nComplete!")
        return 0
    }
}
```

---

## Format Specification Reference

### Standard Format Specifiers

| Specifier | Type         | Description           | Example      |
| --------- | ------------ | --------------------- | ------------ |
| `%s`      | Text         | String representation | `"Hello"`    |
| `%d`      | Int/Long     | Decimal integer       | `42`, `-123` |
| `%f`      | Float/Double | Floating point        | `3.14159`    |
| `%o`      | Any          | Object representation | `[1, 2, 3]`  |
| `%%`      | -            | Literal % character   | `%`          |

### Advanced Format Options (printf)

| Format  | Description                    | Example        |
| ------- | ------------------------------ | -------------- |
| `%05d`  | Zero-padded integer            | `00042`        |
| `%.2f`  | Float with 2 decimal places    | `3.14`         |
| `%-10s` | Left-aligned string, 10 chars  | `"Hello     "` |
| `%10s`  | Right-aligned string, 10 chars | `"     Hello"` |

---

## Error Handling

### Input Validation

```o2l
import system.io

# Safe integer input with validation
method readValidInt(prompt: Text, min: Int, max: Int): Int {
    while (true) {
        try {
            value: Int = io.readInt(prompt)
            if (value >= min && value <= max) {
                return value
            } else {
                io.error("Please enter a number between %d and %d", min, max)
            }
        } catch (error: EvaluationError) {
            io.error("Invalid number format. Please try again.")
        }
    }
}

# Usage
age: Int = readValidInt("Enter age (0-120): ", 0, 120)
```

### Exception Handling in I/O

```o2l
try {
    # Risky I/O operations
    data: Text = io.readLine()
    number: Int = data.toInt()
    result: Float = 100.0f / number.toFloat()

    io.print("Result: %.2f", result)
} catch (error: EvaluationError) {
    io.error("Input error: %s", error.message)
} finally {
    io.flush()  # Always flush output
}
```

---

## Performance Considerations

1. **Buffered Output**: Use `flush()` when immediate output is needed
2. **Input Validation**: Always validate user input for robustness
3. **Error Streams**: Use `error()` and `warn()` for proper error reporting
4. **Format Efficiency**: Simple `print()` is more efficient than complex `printf()`
5. **Input Blocking**: Be aware that input functions block program execution

---

## See Also

- [system.fs API Reference](fs.md) - File system operations
- [Text API Reference](../core/Text.md) - String manipulation for I/O
- [Error Handling Guide](../../tutorials/intermediate/03-error-handling.md) - Exception handling patterns
- [Control Flow](../../language-reference/control-flow.md) - Using I/O in loops and conditionals
