# 🌟 O²L Programming Language

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/zombocoder/o2l) [![License: Apache 2.0](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![C++ Version](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp/23) [![Tests](https://img.shields.io/badge/tests-445%20passing-brightgreen.svg)](https://github.com/zombocoder/o2l) [![FFI System](https://img.shields.io/badge/FFI-SQLite%20integrated-brightgreen.svg)](https://github.com/zombocoder/o2l) [![Text Methods](https://img.shields.io/badge/text%20methods-48-blue.svg)](https://github.com/zombocoder/o2l) [![HTTP Client](https://img.shields.io/badge/http%20methods-30+-brightgreen.svg)](https://github.com/zombocoder/o2l) [![JSON Library](https://img.shields.io/badge/json%20methods-30+-brightgreen.svg)](https://github.com/zombocoder/o2l) [![URL Library](https://img.shields.io/badge/url%20methods-26-blue.svg)](https://github.com/zombocoder/o2l) [![Regexp Library](https://img.shields.io/badge/regexp%20methods-12-blue.svg)](https://github.com/zombocoder/o2l) [![Math Library](https://img.shields.io/badge/math%20functions-40+-brightgreen.svg)](https://github.com/zombocoder/o2l) [![DateTime Library](https://img.shields.io/badge/datetime%20functions-65+-blue.svg)](https://github.com/zombocoder/o2l)

**O²L** is a modern object-oriented programming language that balances pure object-oriented design with practical programming constructs. Built with C++23, O²L eliminates primitives and null values while providing essential control flow like while loops, comprehensive arithmetic operations, and extensive string manipulation capabilities for real-world programming needs.

### 🆕 **Latest Features**

- **🔗 Enhanced Foreign Function Interface (FFI)**: Complete FFI system with advanced type support, SQLite integration, and comprehensive C library interoperability
- **🌐 HTTP Client Library**: Complete `http.client` library with 30+ methods, multipart file upload, platform-specific implementations (Windows/Linux/macOS), and cross-platform libcurl fallback
- **🔄 JSON Library**: Revolutionary `json` library with 30+ methods including auto-detection parsing, fixed path navigation bug, and seamless native Map/List integration
- **🌐 URL Library**: Complete `url` library with 26 methods for URL parsing, construction, manipulation, and validation
- **🔄 Regular Expressions**: Complete `regexp` library with 12 methods for pattern matching, text searching, and string manipulation
- **🔀 Control Flow Enhancement**: `else if` syntax support for complex conditional logic with proper AST chaining
- **📏 Text Length Method**: `Text.length()` method for string length calculation with full type integration
- **📐 Math Library**: Comprehensive mathematical functions library with 40+ methods including trigonometry, logarithms, statistics, and constants (`import math`)
- **✨ Comprehensive Text Methods**: 48 string manipulation methods including case conversion, search, validation, formatting, and advanced utilities
- **🔄 Variable Mutability**: Method-scope variables are mutable while object properties remain immutable
- **🧠 Logical Operators**: Full `&&`, `||`, `!` support with proper precedence and short-circuit evaluation
- **📁 Filesystem Operations**: Complete `system.fs` module for file/directory management
- **🔗 Expression Enhancements**: Parentheses support for complex expression grouping
- **🧪 Complete Test Suite**: 282+ comprehensive tests covering all language features with Google Test framework

---

## 🧠 Philosophy & Design Vision

### **Everything is an Object**

In O²L, there are no primitives. Numbers, booleans, strings—everything is an object with methods and behaviors. This creates a unified, consistent programming model where all values follow the same patterns. For example, Text objects provide 47 comprehensive methods for manipulation, validation, and formatting, treating strings as first-class objects rather than simple data.

### **Immutability by Design**

All objects in O²L are immutable. Once created, an object's state cannot be changed. This eliminates entire classes of bugs related to shared mutable state, race conditions, and unexpected side effects.

### **Selective Procedural Constructs**

O²L adopts a pragmatic approach by supporting essential control flow like `while` loops for iteration alongside object-oriented patterns. This enables practical programming while maintaining the language's object-centric philosophy through iterators and method dispatch.

### **Elimination of Null**

There are no null pointers or undefined values. Optional values are handled through explicit `Optional<T>` types, making null-pointer exceptions impossible and forcing developers to handle all cases explicitly.

### **Behavior-Driven Architecture**

Rather than thinking in terms of data structures and algorithms, O²L encourages thinking in terms of objects that collaborate to solve problems. Each object has clear responsibilities and well-defined interfaces.

---

## 🚀 Quick Start

### macOS Installation (Homebrew)

The easiest way to install O²L on macOS:

```bash
# Add the O²L tap
brew tap zombocoder/o2l

# Install O²L
brew install o2l

# Verify installation
o2l --version

# Create your first project
mkdir my-project && cd my-project
o2l-pkg init
o2l run src/main.obq
```

### Manual Installation

For other platforms or if you want to build from source:

Looking at your `CMakeLists.txt`, the only **external dependency** you need to install is **libffi** (with `pkg-config` support).

#### Required Libraries

- **libffi** → Used for the Foreign Function Interface (`src/Runtime/FFI/...`).

  - On **Ubuntu/Debian**:

    ```bash
    sudo apt-get install libffi-dev pkg-config
    ```

  - On **Fedora**:

    ```bash
    sudo dnf install libffi-devel pkg-config
    ```

  - On **Arch Linux**:

    ```bash
    sudo pacman -S libffi pkgconf
    ```

  - On **macOS (Homebrew)**:

    ```bash
    brew install libffi pkg-config
    ```

  - On **Windows (MSYS2/MinGW)**:

    ```bash
    pacman -S mingw-w64-x86_64-libffi pkg-config
    ```

#### Optional / System-Provided

- `dl` (dynamic linking) → usually provided by `CMAKE_DL_LIBS` on Linux/macOS.
- `ws2_32`, `wininet` → provided by Windows SDK, no need to install separately.

#### Build Dependencies

- A **C++23-capable compiler**:

  - GCC ≥ 12
  - Clang ≥ 15
  - MSVC ≥ 19.35 (Visual Studio 2022 17.5)

- **CMake ≥ 3.20**

#### Build from Source

```bash
# Clone the repository
git clone https://github.com/zombocoder/o2l.git
cd o2l

# Build the interpreter
mkdir build && cd build
cmake ..
make

# Run your first O²L program
./o2l run ../examples/hello_world.obq
```

### Creating a New O²L Project

O²L provides a powerful package manager tool `o2l-pkg` to scaffold and manage projects:

```bash
# Create project directory
mkdir my-project
cd my-project

# Initialize the O²L project
o2l-pkg init

# Run your project
o2l run src/main.obq
```

#### Project Structure

The `o2l-pkg init` command generates a complete project structure:

```
my-project/
├── .o2l/                     # Package manager directory
│   ├── lib/                  # Downloaded libraries
│   └── cache/                # Build cache
├── src/
│   ├── calc/
│   │   └── Calculator.obq    # Calculator module
│   ├── greeters/
│   │   └── BasicGreeter.obq  # Greeter module
│   ├── tests/
│   │   ├── calc/
│   │   │   └── CalculatorTest.obq
│   │   ├── greeters/
│   │   │   └── GreeterTest.obq
│   │   └── main_test.obq     # Test runner
│   └── main.obq              # Entry point
├── o2l.toml                  # Project configuration
└── .gitignore                # Git ignore rules
```

#### Package Manager Commands

```bash
# Initialize new project in current directory
o2l-pkg init

# Create new objects with namespace structure
o2l-pkg create com.mycompany.utils Utils
o2l-pkg create data.models User

# List installed libraries
o2l-pkg list

# Clean cache directory
o2l-pkg clean

# Show help
o2l-pkg help
```

#### Running Your Project

Use the standard `o2l` command to run your programs:

```bash
# Run main program
o2l run src/main.obq

# Run all tests
o2l run src/tests/main_test.obq

# Run specific tests
o2l run src/tests/calc/CalculatorTest.obq
o2l run src/tests/greeters/GreeterTest.obq

# Run with arguments
o2l run src/main.obq arg1 arg2
```

#### Project Configuration (o2l.toml)

The initialization process creates an interactive configuration:

```toml
[package]
name = "my-project"
version = "0.1.0"
description = "An O²L project"
authors = ["Your Name <your.email@example.com>"]

[dependencies]
# Libraries added with o2l-pkg add will appear here
# collections = "latest"
# com.example.math = "1.2.0"
```

### Your First Program

Create `hello.obq`:

```obq
import system.io

Object Greeter {
    property message: Text

    constructor(greeting: Text) {
        this.message = greeting
    }

    @external method greet(name: Text): Text {
        io.print("%s, %s!", this.message, name)
        return this.message
    }
}

Object Main {
    method main(): Int {
        greeter: Greeter = new Greeter("Hello")
        greeter.greet("World")
        return 0
    }
}
```

Run it:

```bash
o2l run hello.obq
# Output: Hello, World!
```

---

## 🎯 Core Language Features

### **Object Declaration & Variable Mutability**

```obq
Object Calculator {
    # Object properties are immutable
    property initial_value: Int

    constructor(initial: Int) {
        this.initial_value = initial  # ✅ Property initialization
    }

    @external method processData(): Int {
        # Method variables are mutable
        result: Int = this.initial_value
        counter: Int = 1
        message: Text = "Processing"

        # ✅ Variable reassignment works perfectly
        result = result * 2
        counter = counter + 1
        message = message + "..."

        # ✅ Variables can be modified in loops
        while (counter <= 5) {
            result = result + counter
            counter = counter + 1
        }

        # ❌ Properties cannot be reassigned
        # this.initial_value = 100  # Error!

        return result
    }
}
```

### **Conditional Logic with else if Support**

```obq
Object GradeCalculator {
    @external method calculateGrade(score: Int): Text {
        # else if syntax for complex conditional logic
        if (score >= 90) {
            return "A - Excellent!"
        } else if (score >= 80) {
            return "B - Good work!"
        } else if (score >= 70) {
            return "C - Satisfactory"
        } else if (score >= 60) {
            return "D - Needs improvement"
        } else {
            return "F - Please try again"
        }
    }

    @external method classifyText(text: Text): Text {
        # Using Text.length() method with else if chains
        len: Int = text.length()  # New Text.length() method

        if (len == 0) {
            return "empty"
        } else if (len <= 3) {
            return "tiny"
        } else if (len <= 10) {
            return "short"
        } else if (len <= 50) {
            return "medium"
        } else if (len <= 100) {
            return "long"
        } else {
            return "very long"
        }
    }
}
```

### **Logical Operators & Expressions**

```obq
Object LogicProcessor {
    @external method evaluate(): Bool {
        a: Bool = true
        b: Bool = false
        c: Bool = true

        # Basic logical operators with proper precedence
        and_result: Bool = a && b        # false
        or_result: Bool = a || b         # true
        not_result: Bool = !a            # false

        # Complex expressions (! > && > ||)
        complex: Bool = !a && b || c     # (!a && b) || c = true

        # Parentheses override precedence
        grouped: Bool = !(a && b) || c   # true

        # Short-circuit evaluation
        safe_check: Bool = (c != null) && (c == true)

        return grouped
    }
}
```

### **Filesystem Operations**

```obq
import system.io
import system.fs

Object FileManager {
    @external method manageFiles(): Bool {
        filename: Text = "data.txt"
        content: Text = "Hello, O²L filesystem!"

        # File operations
        write_ok: Bool = fs.writeText(filename, content)
        file_content: Text = fs.readText(filename)
        exists: Bool = fs.exists(filename)

        # Directory operations
        dir_created: Bool = fs.createDirectory("temp")
        files: List = fs.listFiles(".")

        # Logical conditions with filesystem results
        success: Bool = write_ok && exists && fs.isFile(filename)

        # Cleanup
        cleanup: Bool = fs.deleteFile(filename)

        return success && cleanup
    }
}
```

### **Comprehensive Text Methods with Length Support**

```obq
import system.io

Object TextProcessor {
    @external method demonstrateTextMethods(): Text {
        original: Text = "  Hello World  "

        # Text length method - NEW!
        text_length: Int = original.length()                # 13 (including spaces)
        stripped_length: Int = original.strip().length()    # 11 (without leading/trailing spaces)

        # String case methods
        capitalized: Text = original.strip().capitalize()     # "Hello world"
        upper_case: Text = original.strip().upper()          # "HELLO WORLD"
        title_case: Text = original.strip().title()          # "Hello World"

        # String search and validation
        position: Int = original.find("World")               # 8
        starts_hello: Bool = original.strip().startswith("Hello")  # true
        is_printable: Bool = original.isPrintable()          # true
        is_alpha: Bool = "HelloWorld".isAlpha()              # true

        # String manipulation
        replaced: Text = original.replace("World", "O²L")  # "  Hello O²L  "
        words: List = "apple,banana,cherry".split(",")      # ["apple", "banana", "cherry"]
        joined: Text = ", ".join(words)                     # "apple, banana, cherry"

        # String formatting
        template: Text = "Hello {0}, you are {1} years old!"
        formatted: Text = template.format("Alice", 25)      # "Hello Alice, you are 25 years old!"

        # Advanced string operations
        centered: Text = "Hi".center(10)                    # "    Hi    "
        zero_filled: Text = "42".zfill(5)                  # "00042"
        parts: List = "name=value".partition("=")          # ["name", "=", "value"]

        # String validation methods
        validation_text: Text = "Hello123"
        is_alnum: Bool = validation_text.isAlnum()         # true
        is_digit: Bool = "12345".isDigit()                 # true
        is_identifier: Bool = "valid_name".isIdentifier()  # true

        io.print("Original: '%s' (length: %d)", original, text_length)
        io.print("Processed: '%s' (length: %d)", capitalized, capitalized.length())
        io.print("Position of 'World': %d", position)
        io.print("Formatted: '%s' (length: %d)", formatted, formatted.length())

        return "All 48 Text methods available including length()!"
    }
}
```

### **Text Method Categories**

O²L provides **48 comprehensive string methods** organized into categories:

**📏 Core Properties**: `length()` - Get string length (character count)
**🔤 Case Manipulation**: `capitalize()`, `lower()`, `upper()`, `title()`, `swapCase()`, `caseFold()`
**🔍 Search & Find**: `find()`, `rfind()`, `index()`, `rindex()`, `count()`, `startswith()`, `endswith()`
**✅ Validation**: `isAlnum()`, `isAlpha()`, `isDigit()`, `isLower()`, `isUpper()`, `isSpace()`, `isPrintable()`, `isAscii()`, `isIdentifier()`, `isDecimal()`, `isNumeric()`, `isTitle()`
**✂️ Manipulation**: `strip()`, `lstrip()`, `rstrip()`, `replace()`, `split()`, `rsplit()`, `splitlines()`, `join()`
**📐 Positioning**: `center()`, `ljust()`, `rjust()`, `zfill()`
**📝 Formatting**: `format()`, `formatMap()` with positional and named arguments
**🔧 Advanced**: `partition()`, `rpartition()`, `translate()`, `makeTrans()`

**Text.length() Method**:

```obq
text: Text = "Hello World"
len: Int = text.length()  # Returns 11

# Common usage patterns:
if (text.length() == 0) {
    return "empty"
} else if (text.length() < 10) {
    return "short"
} else {
    return "long"
}
```

### **Math Library (import math)**

O²L provides a comprehensive mathematical functions library with **40+ methods** covering all essential mathematical operations:

```obq
import math
import system.io

Object MathExamples {
    @external method demonstrateMath(): Text {
        # Mathematical constants
        pi: Double = math.pi()              # π = 3.141592653589793
        e: Double = math.e()                # e = 2.718281828459045
        tau: Double = math.tau()            # τ = 2π = 6.283185307179586

        # Basic utility functions
        abs_val: Double = math.abs(-42.0)   # 42.0
        ceil_val: Double = math.ceil(3.2)   # 4.0
        floor_val: Double = math.floor(3.8) # 3.0
        round_val: Double = math.round(3.6) # 4.0
        max_val: Double = math.max(1.0, 5.0, 3.0) # 5.0

        # Power and exponential functions
        power: Double = math.pow(2.0, 3.0)  # 8.0
        sqrt_val: Double = math.sqrt(16.0)  # 4.0
        exp_val: Double = math.exp(1.0)     # e^1 ≈ 2.718

        # Logarithmic functions
        ln: Double = math.log(e)            # Natural log: 1.0
        log10_val: Double = math.log10(100.0) # Base-10 log: 2.0
        log2_val: Double = math.log2(8.0)   # Base-2 log: 3.0

        # Trigonometric functions (radians)
        sin_val: Double = math.sin(pi / 2.0) # 1.0
        cos_val: Double = math.cos(0.0)      # 1.0
        tan_val: Double = math.tan(pi / 4.0) # 1.0

        # Inverse trigonometric functions
        asin_val: Double = math.asin(1.0)    # π/2
        acos_val: Double = math.acos(0.0)    # π/2
        atan_val: Double = math.atan(1.0)    # π/4

        # Angular conversions
        degrees_val: Double = math.degrees(pi) # 180.0 (radians to degrees)
        radians_val: Double = math.radians(180.0) # π (degrees to radians)

        # Hyperbolic functions
        sinh_val: Double = math.sinh(1.0)    # Hyperbolic sine
        cosh_val: Double = math.cosh(0.0)    # 1.0
        tanh_val: Double = math.tanh(0.0)    # 0.0

        # Special integer functions
        factorial: Long = math.factorial(5)  # 120 (5!)
        gcd_val: Int = math.gcd(12, 18)     # 6 (greatest common divisor)
        lcm_val: Long = math.lcm(4, 6)      # 12 (least common multiple)

        # Advanced special functions
        gamma_val: Double = math.gamma(4.0)  # 3! = 6.0 (gamma function)
        erf_val: Double = math.erf(1.0)     # Error function

        # Number classification
        is_finite: Bool = math.isFinite(42.0) # true
        is_inf: Bool = math.isInf(math.inf()) # true
        is_nan: Bool = math.isNan(math.nan()) # true

        io.print("π=%.6f, e=%.6f, sqrt(16)=%.1f", pi, e, sqrt_val)
        io.print("sin(π/2)=%.1f, cos(0)=%.1f, 5!=L%d", sin_val, cos_val, factorial)
        io.print("gcd(12,18)=%d, degrees(π)=%.1f", gcd_val, degrees_val)

        "Mathematical operations completed!"
    }
}
```

### **Math Function Categories**

O²L's math library provides **40+ mathematical functions** organized by category:

**📊 Constants**: `pi()`, `e()`, `tau()`, `inf()`, `nan()`
**⚡ Basic**: `abs()`, `ceil()`, `floor()`, `round()`, `trunc()`, `sign()`, `max()`, `min()`
**🔢 Power**: `pow()`, `sqrt()`, `cbrt()`, `exp()`, `exp2()`, `expm1()`
**📈 Logarithmic**: `log()`, `log2()`, `log10()`, `log1p()`
**📐 Trigonometric**: `sin()`, `cos()`, `tan()`, `asin()`, `acos()`, `atan()`, `atan2()`
**〰️ Hyperbolic**: `sinh()`, `cosh()`, `tanh()`, `asinh()`, `acosh()`, `atanh()`
**🔄 Angular**: `radians()`, `degrees()`
**🔬 Special**: `factorial()`, `gcd()`, `lcm()`, `gamma()`, `lgamma()`
**⚠️ Error Functions**: `erf()`, `erfc()`
**✅ Classification**: `isFinite()`, `isInf()`, `isNan()`, `isNormal()`

### **Numeric Type Hierarchy**

```obq
Object NumericDemo {
    property int_value: Int = 42                           # 64-bit integer
    property long_value: Long = 123456789012345L           # 128-bit integer
    property float_value: Float = 3.14159f                 # 32-bit precision
    property double_value: Double = 3.14159d               # 64-bit precision
    property default_decimal: Double = 2.718               # Defaults to Double

    @external method demonstrateHierarchy(): Text {
        # Type promotion in mixed arithmetic:
        int_plus_long: Long = this.int_value + this.long_value        # Int + Long = Long
        long_plus_float: Float = this.long_value + this.float_value   # Long + Float = Float
        float_plus_double: Double = this.float_value + this.double_value # Float + Double = Double

        return "Numeric type promotion working correctly"
    }
}
```

### **Enums with Values**

```obq
Enum Status {
    PENDING = 1,
    ACTIVE = 10,
    COMPLETED = 100
}

Object Task {
    property status: Status

    constructor() {
        this.status = Status.PENDING
    }

    @external method activate(): Void {
        this.status = Status.ACTIVE
    }
}
```

### **Records (Data Structures)**

```obq
Record Point {
    x: Int
    y: Int
}

Record Person {
    name: Text
    age: Int
    location: Point
}

Object Main {
    method main(): Int {
        location: Point = Point(x=10, y=20)
        person: Person = Person(name="Alice", age=30, location=location)

        # Access record fields
        io.print("Person: %s at (%d, %d)", person.name, person.location.x, person.location.y)
        return 0
    }
}
```

### **Generic Collections (List<T>) with Iterators**

```obq
import system.io

Object ListDemo {
    @external method demonstrateLists(): Int {
        # Create typed lists with literals
        numbers: List<Int> = [1, 2, 3, 4, 5]
        names: List<Text> = ["Alice", "Bob", "Charlie"]
        empty: List<Int> = []

        # List methods
        numbers.add(6)                    # Add element: [1, 2, 3, 4, 5, 6]
        first: Int = numbers.get(0)       # Get element at index: 1
        numbers.reverse()                 # Reverse: [6, 5, 4, 3, 2, 1]
        last: Int = numbers.pop()         # Remove and return last: 1
        numbers.remove(2)                 # Remove at index 2: [6, 5, 3, 2]
        size: Int = numbers.size()        # Get size: 4

        # Iterator with while loops
        iter: ListIterator = numbers.iterator()
        while (iter.hasNext()) {
            value: Int = iter.next()
            io.print("Processing: %d", value)
        }

        io.print("Numbers: %s", numbers)
        io.print("Final size: %d", size)

        return 0
    }
}
```

### **While Loops and Iterators**

```obq
import system.io
import system.utils

Object IterationDemo {
    @external method demonstrateWhileLoops(): Int {
        # While loops with List iterators
        numbers: List<Int> = [10, 20, 30, 40, 50]
        iter: ListIterator = numbers.iterator()

        while (iter.hasNext()) {
            value: Int = iter.next()
            io.print("Processing: %d", value)
        }

        # RepeatIterator for controlled repetition (replaces traditional for loops)
        repeater: RepeatIterator = utils.repeat(5)
        countdown: List<Int> = []

        while (repeater.hasNext()) {
            current: Int = repeater.next()
            countdown.add(5 - current)
            io.print("Countdown: %d", 5 - current)
        }

        # Using modulo operator for even/odd detection
        all_numbers: RepeatIterator = utils.repeat(10)
        even_numbers: List<Int> = []

        while (all_numbers.hasNext()) {
            num: Int = all_numbers.next()
            remainder: Int = num % 2
            if (remainder == 0) {
                even_numbers.add(num)
            }
        }

        io.print("Even numbers 0-9: %s", even_numbers)
        return 0
    }
}
```

### **Generic Sets (Set<T>) with Iterators**

```obq
import system.io

Record PersonalInfoDTO {
    personalName: Text
    PersonalAge: Int
}

Object SetDemo {
    @external method demonstrateSets(): Int {
        # Create typed sets with literals
        numbers: Set<Int> = (1, 2, 3, 4, 5, 2, 1)  # Duplicates automatically removed
        names: Set<Text> = ("Alice", "Bob", "Charlie", "Alice")
        empty: Set<Int> = ()

        # Set methods
        numbers.add(6)                    # Add element: (1, 2, 3, 4, 5, 6)
        has_three: Bool = numbers.contains(3)  # Check if element exists: true
        numbers.remove(2)                 # Remove element: (1, 3, 4, 5, 6)
        size: Int = numbers.size()        # Get size: 5

        # Set iterator with while loops
        iter: SetIterator = numbers.iterator()
        while (iter.hasNext()) {
            value: Int = iter.next()
            io.print("Processing: %d", value)
        }

        io.print("Numbers: %s", numbers)
        io.print("Final size: %d", size)

        return 0
    }

    @external method demonstrateRecordSets(): Int {
        # Create a Set with Records - demonstrates duplicate detection
        my_set: Set<PersonalInfoDTO> = (
            PersonalInfoDTO(personalName="John", PersonalAge=43),
            PersonalInfoDTO(personalName="Jane", PersonalAge=25),
            # This should be a duplicate and removed from the set
            PersonalInfoDTO(personalName="John", PersonalAge=43)
        )

        set_iter: SetIterator = my_set.iterator()
        io.print("Created set iterator")

        while (set_iter.hasNext()) {
            item: PersonalInfoDTO = set_iter.next()
            io.print("item: %o", item)
        }

        io.print("Set size: %d", my_set.size())  # Should be 2, not 3
        return 0
    }
}
```

### **Generic Maps (Map<K,V>) with Iterators**

```obq
import system.io

Object MapDemo {
    @external method demonstrateMaps(): Int {
        # Create typed maps with literals
        scores: Map<Text, Int> = {
            "Alice": 95,
            "Bob": 87,
            "Charlie": 92
        }

        config: Map<Text, Text> = {
            "host": "localhost",
            "port": "8080",
            "debug": "true"
        }

        empty: Map<Text, Int> = {}

        # Map methods
        scores.put("David", 88)               # Add element: {"Alice": 95, "Bob": 87, "Charlie": 92, "David": 88}
        alice_score: Int = scores.get("Alice") # Get element by key: 95
        has_bob: Bool = scores.contains("Bob") # Check if key exists: true
        scores.remove("Charlie")              # Remove by key: {"Alice": 95, "Bob": 87, "David": 88}
        size: Int = scores.size()             # Get size: 3

        # Map iterator with while loops
        map_iter: MapIterator = scores.iterator()
        while (map_iter.hasNext()) {
            item: MapObject = map_iter.MapItem()
            key: Text = item.getKey()
            value: Int = item.getVal()
            io.print("Student: %s, Score: %d", key, value)
        }

        # Alternative iteration methods
        map_iter.reset()
        io.print("=== Keys only ===")
        while (map_iter.hasNext()) {
            key: Text = map_iter.nextKey()
            io.print("Key: %s", key)
        }

        map_iter.reset()
        io.print("=== Values only ===")
        while (map_iter.hasNext()) {
            value: Int = map_iter.nextValue()
            io.print("Value: %d", value)
        }

        io.print("Final map: %s", scores)
        io.print("Final size: %d", size)

        return 0
    }

    @external method processGrades(): Int {
        # Practical example: Grade processing system
        grades: Map<Text, Int> = {
            "Math": 85,
            "Science": 92,
            "History": 78,
            "English": 88
        }

        total: Int = 0
        count: Int = 0
        high_scores: List<Text> = []

        grade_iter: MapIterator = grades.iterator()
        while (grade_iter.hasNext()) {
            entry: MapObject = grade_iter.MapItem()
            subject: Text = entry.getKey()
            score: Int = entry.getVal()

            total = total + score
            count = count + 1

            if (score >= 90) {
                high_scores.add(subject)
                io.print("Excellent work in %s: %d", subject, score)
            }
        }

        average: Int = total / count
        io.print("Average grade: %d", average)
        io.print("High-scoring subjects: %s", high_scores)

        return 0
    }
}
```

### **Protocols (Interfaces)**

```obq
Protocol Drawable {
    method draw(): Text
    method getArea(): Float
}

Object Circle: Drawable {
    property radius: Float

    constructor(r: Float) {
        this.radius = r
    }

    @external method draw(): Text {
        return "Drawing circle with radius: " + this.radius
    }

    @external method getArea(): Float {
        const PI: Float = 3.14159
        return PI * this.radius * this.radius
    }
}
```

#### **Protocol Signature Validation**

O²L provides comprehensive protocol signature validation that ensures implementing objects match the protocol's method signatures exactly, including parameter types, return types, and parameter counts.

```obq
Protocol Calculator {
    method add(a: Int, b: Int): Int
    method multiply(x: Float, y: Float): Float
    method getName(): Text
}

# ✅ Correct Implementation
Object BasicCalculator: Calculator {
    @external method add(a: Int, b: Int): Int {
        return a + b
    }

    @external method multiply(x: Float, y: Float): Float {
        return x * y
    }

    @external method getName(): Text {
        return "Basic Calculator"
    }
}

# ❌ Incorrect Implementation - Wrong Parameter Types
Object WrongTypeCalculator: Calculator {
    @external method add(a: Text, b: Text): Int {  # Should be Int, Int
        return 42
    }

    @external method multiply(x: Float, y: Float): Float {
        return x * y
    }

    @external method getName(): Text {
        return "Wrong Type Calculator"
    }
}
```

**Validation Features:**

- **Method Existence**: Verifies all protocol methods are implemented
- **Parameter Count**: Ensures correct number of parameters
- **Parameter Types**: Validates each parameter type matches exactly
- **Return Types**: Confirms return type matches protocol specification
- **Detailed Error Reporting**: Provides specific error messages for mismatches
- **Backward Compatibility**: Supports objects without signature information

**Error Examples:**

```obq
# Runtime validation errors for WrongTypeCalculator:
# "Method 'add': parameter 0 type mismatch (expected 'Int', got 'Text')"
# "Method 'add': parameter 1 type mismatch (expected 'Int', got 'Text')"
```

### **Method Visibility**

```obq
Object BankAccount {
    property balance: Int

    constructor(initial: Int) {
        this.balance = initial
    }

    # Private method - only accessible within the object
    method validateAmount(amount: Int): Bool {
        return amount > 0
    }

    # Public method - accessible from outside
    @external method deposit(amount: Int): Int {
        if (this.validateAmount(amount)) {
            this.balance = this.balance + amount
        }
        return this.balance
    }

    @external method getBalance(): Int {
        return this.balance
    }
}
```

### **Namespace System**

```obq
# Namespace organization for code structure and package management
import system.io

# Define namespace with nested objects
namespace com.mycompany.utils {
    Object Calculator {
        @external method add(a: Text, b: Text): Text {
            "Result: " + a + " + " + b
        }

        @external method multiply(x: Int, y: Int): Int {
            x * y
        }

        @external method getName(): Text {
            "Calculator"
        }

        @external method getNamespace(): Text {
            "com.mycompany.utils"
        }
    }

    Object Logger {
        @external method info(message: Text): Text {
            "[INFO] " + message
        }

        @external method error(message: Text): Text {
            "[ERROR] " + message
        }
    }
}

# Import specific objects from namespace
import com.mycompany.utils.Calculator
import com.mycompany.utils.Logger

Object Main {
    method main(): Text {
        # Create instances using imported types
        calc: Calculator = new Calculator()
        logger: Logger = new Logger()

        # Use typed variable declarations with method calls
        result: Text = calc.add("5", "3")
        product: Int = calc.multiply(4, 7)

        # Namespace-aware logging
        io.print("Calculator: %s", calc.getName())
        io.print("Namespace: %s", calc.getNamespace())
        io.print("Addition result: %s", result)
        io.print("Product: %d", product)

        # Use logger from same namespace
        info_msg: Text = logger.info("Calculation completed")
        io.print(info_msg)

        "Namespace system demonstration complete"
    }
}
```

**Key Namespace Features:**

- **Organization**: `namespace com.company.module { ... }` for code structure
- **Import Resolution**: `import com.company.module.ClassName`
- **Object-Oriented**: No static method calls - everything requires object instantiation
- **Typed Variables**: `variable: Type = expression` syntax supported
- **Package Management**: Compatible with `o2l-pkg` for library management

### **Error Handling System**

```obq
import system.io

Object Calculator {
    @external method safeDivide(a: Int, b: Int): Int {
        if (b == 0) {
            # Throw user-defined errors
            throw("Division by zero error")
        }
        return a / b
    }

    @external method safeOperation(a: Int, b: Int, operation: Text): Int {
        try {
            if (operation == "divide") {
                return this.safeDivide(a, b)
            }
            if (operation == "add") {
                return a + b
            }
            throw("Unknown operation: " + operation)
        } catch (error) {
            # Handle any thrown errors
            io.print("Error caught: %s", error)
            return -1
        } finally {
            # Always executed, even after return or throw
            io.print("Operation completed")
        }
    }
}

Object ErrorDemo {
    @external method demonstrateErrorHandling(): Int {
        calc: Calculator = new Calculator()

        # Test successful operation
        result1: Int = calc.safeOperation(10, 2, "divide")
        io.print("10 / 2 = %d", result1)

        # Test error handling
        result2: Int = calc.safeOperation(10, 0, "divide")
        io.print("10 / 0 = %d", result2)  # Returns -1 due to error

        # Test unknown operation
        result3: Int = calc.safeOperation(1, 2, "multiply")
        io.print("1 * 2 (unknown) = %d", result3)  # Returns -1 due to error

        return 0
    }
}
```

### **Result<T,E> Type for Functional Error Handling**

```obq
import system.io

Object SafeMath {
    @external method divide(a: Int, b: Int): Result<Int, Error> {
        if (b == 0) {
            error: Error = new Error("Division by zero")
            return Result.error(error)
        }
        return Result.success(a / b)
    }

    @external method processResult(): Int {
        result: Result<Int, Error> = this.divide(10, 2)

        if (result.isSuccess()) {
            value: Int = result.getResult()
            io.print("Success: %d", value)
            return value
        } else {
            error: Error = result.getError()
            io.print("Error: %s", error.getMessage())
            return -1
        }
    }
}
```

### **Command Line Arguments**

```obq
import system.io
import system.os

Object ArgumentProcessor {
    @external method processArguments(): Int {
        # Get all command line arguments (including program name)
        args: List = os.args()
        argc: Int = os.argc()

        io.print("Program received %d arguments:", argc)

        # Iterate through arguments with index tracking
        iterator: ListIterator = args.iterator()
        while (iterator.hasNext()) {
            arg: Text = iterator.next()
            io.print("args[%d] = '%s'", iterator.index(), arg)
        }

        # Access specific arguments
        if (argc > 0) {
            program_name: Text = args.get(0)
            io.print("Program name: %s", program_name)
        }

        if (argc > 1) {
            first_arg: Text = args.get(1)
            io.print("First user argument: %s", first_arg)
        }

        return argc
    }
}

Object Main {
    method main(): Int {
        processor: ArgumentProcessor = new ArgumentProcessor()
        return processor.processArguments()
    }
}
```

**Command Line Usage:**

```bash
# Run with arguments - follows C/C++ conventions
./o2l run program.obq hello world test

# Debug mode (--debug is filtered out from program arguments)
./o2l run program.obq --debug arg1 arg2

# Arguments received by program:
# args[0] = "program.obq" (program name, like argv[0] in C)
# args[1] = "hello"       (first user argument)
# args[2] = "world"       (second user argument)
```

## 📚 System Libraries Reference

O²L provides comprehensive system libraries for I/O, filesystem operations, datetime manipulation, mathematical functions, and testing.

### **system.io Library**

The system I/O library provides formatted output capabilities for all data types:

```obq
import system.io

Object IODemo {
    @external method demonstrateFormatting(): Text {
        # Basic type formatting
        io.print("Integer: %d", 42)
        io.print("Float: %f", 3.14159)
        io.print("Text: %s", "Hello World")
        io.print("Boolean: %o", true)

        # Multiple argument formatting
        name: Text = "Alice"
        age: Int = 30
        score: Float = 95.5
        io.print("User %s is %d years old with score %.1f", name, age, score)

        # Object formatting
        io.print("Object: %o", Status.ACTIVE)

        return "Formatting completed"
    }
}
```

**Available system.io Methods:**

- `print(format: Text, ...args)` - Formatted output with type-specific placeholders
  - `%d` - Integer formatting (Int, Long)
  - `%f` - Floating point formatting (Float, Double)
  - `%s` - Text string formatting
  - `%o` - Generic object formatting (Bool, Enum, etc.)

### **system.os Library**

Operating system interface for environment variables and command line arguments:

```obq
import system.io
import system.os

Object OSDemo {
    @external method showEnvironment(): Text {
        # Environment variable access
        user: Text = os.getEnv("USER")
        home: Text = os.getEnv("HOME")
        path: Text = os.getEnv("PATH")

        io.print("Current user: %s", user)
        io.print("Home directory: %s", home)
        io.print("PATH: %s", path)

        # Command line arguments
        args: List = os.args()
        argc: Int = os.argc()

        io.print("Program received %d arguments:", argc)

        iterator: ListIterator = args.iterator()
        while (iterator.hasNext()) {
            arg: Text = iterator.next()
            io.print("args[%d] = '%s'", iterator.index(), arg)
        }

        return "Environment information displayed"
    }
}
```

**Available system.os Methods:**

- `getEnv(variable: Text): Text` - Get environment variable value
- `args(): List<Text>` - Get command line arguments (includes program name at index 0)
- `argc(): Int` - Get command line argument count

### **system.fs Library**

Comprehensive filesystem operations for file and directory management:

```obq
import system.io
import system.fs

Object FileSystemDemo {
    @external method demonstrateFileOperations(): Text {
        filename: Text = "demo.txt"
        content: Text = "Hello, O²L filesystem!"

        # File writing and reading
        write_success: Bool = fs.writeText(filename, content)
        io.print("File write successful: %o", write_success)

        if (write_success) {
            file_content: Text = fs.readText(filename)
            io.print("File content: %s", file_content)
        }

        # File existence and type checking
        exists: Bool = fs.exists(filename)
        is_file: Bool = fs.isFile(filename)
        is_directory: Bool = fs.isDirectory(filename)

        io.print("File exists: %o", exists)
        io.print("Is file: %o", is_file)
        io.print("Is directory: %o", is_directory)

        # Directory operations
        dir_name: Text = "test_directory"
        dir_created: Bool = fs.createDirectory(dir_name)
        io.print("Directory created: %o", dir_created)

        if (dir_created) {
            files: List = fs.listFiles(dir_name)
            io.print("Files in directory: %s", files)

            # Cleanup
            fs.deleteFile(dir_name)
        }

        # File cleanup
        delete_success: Bool = fs.deleteFile(filename)
        io.print("File deleted: %o", delete_success)

        return "File operations completed"
    }
}
```

**Path Manipulation Example:**

```obq
import system.fs
import system.io

Object PathDemo {
    @external method demonstratePathOperations(): Text {
        # Basic path operations
        file_path: Text = "/home/user/documents/project/readme.txt"

        filename: Text = fs.basename(file_path)           # "readme.txt"
        directory: Text = fs.dirname(file_path)           # "/home/user/documents/project"
        extension: Text = fs.extname(file_path)           # ".txt"

        io.print("File: %s", filename)
        io.print("Directory: %s", directory)
        io.print("Extension: %s", extension)

        # Path construction
        config_path: Text = fs.join("config", "app.json")
        absolute_config: Text = fs.resolve(config_path)

        # Path normalization
        messy_path: Text = "./docs/../src/./main.cpp"
        clean_path: Text = fs.normalize(messy_path)       # "src/main.cpp"

        # Extension manipulation
        backup_file: Text = fs.changeExtension(file_path, ".backup")

        # Path analysis
        is_absolute: Bool = fs.isAbsolute(file_path)      # true
        components: List = fs.splitPath(file_path)        # ["home", "user", "documents", "project", "readme.txt"]

        io.print("Clean path: %s", clean_path)
        io.print("Backup file: %s", backup_file)
        io.print("Path components: %s", components)

        return "Path operations completed"
    }
}
```

**Available system.fs Methods:**

**File Operations:**

- `readText(path: Text): Text` - Read entire file content as text
- `writeText(path: Text, content: Text): Bool` - Write text content to file
- `exists(path: Text): Bool` - Check if file or directory exists
- `isFile(path: Text): Bool` - Check if path is a file
- `isDirectory(path: Text): Bool` - Check if path is a directory
- `createDirectory(path: Text): Bool` - Create directory (including parent directories)
- `deleteFile(path: Text): Bool` - Delete file or directory
- `listFiles(path: Text): List<Text>` - List directory contents

**Path Manipulation:**

- `basename(path: Text): Text` - Extract filename from path
- `dirname(path: Text): Text` - Extract directory portion from path
- `extname(path: Text): Text` - Extract file extension from path
- `join(path1: Text, path2: Text, ...): Text` - Join multiple path components
- `normalize(path: Text): Text` - Resolve . and .. components in path
- `resolve(path: Text): Text` - Convert relative path to absolute path
- `relative(from: Text, to: Text): Text` - Get relative path from one location to another
- `isAbsolute(path: Text): Bool` - Check if path is absolute
- `splitPath(path: Text): List<Text>` - Split path into individual components
- `getParent(path: Text): Text` - Get parent directory (alias for dirname)
- `changeExtension(path: Text, newExt: Text): Text` - Change or add file extension

### **datetime Library**

Comprehensive date and time manipulation with 65+ functions:

```obq
import datetime
import system.io

Object DateTimeDemo {
    @external method demonstrateDateTime(): Text {
        # Current date/time
        current: Text = datetime.now()
        today: Text = datetime.today()
        utc_now: Text = datetime.nowUTC()

        io.print("Current: %s", datetime.toString(current))
        io.print("Today: %s", datetime.toString(today))
        io.print("UTC: %s", datetime.toString(utc_now))

        # Create specific datetime
        custom: Text = datetime.create(2024, 8, 7, 15, 30, 45)
        io.print("Custom: %s", datetime.toString(custom))

        # Component extraction
        year: Int = datetime.getYear(custom)
        month: Int = datetime.getMonth(custom)
        day: Int = datetime.getDay(custom)

        io.print("Date: %d-%d-%d", year, month, day)

        # Date arithmetic
        plus_week: Text = datetime.addDays(custom, 7)
        plus_hours: Text = datetime.addHours(custom, 3)

        io.print("Plus 1 week: %s", datetime.toDateString(plus_week))
        io.print("Plus 3 hours: %s", datetime.toTimeString(plus_hours))

        # Formatting
        iso_format: Text = datetime.formatISO(custom)
        date_only: Text = datetime.toDateString(custom)
        time_only: Text = datetime.toTimeString(custom)

        io.print("ISO format: %s", iso_format)
        io.print("Date: %s, Time: %s", date_only, time_only)

        # Comparisons
        newer: Text = datetime.create(2024, 8, 8, 12, 0, 0)
        is_before: Bool = datetime.isBefore(custom, newer)
        io.print("Is before newer date: %o", is_before)

        # Utilities
        is_leap: Bool = datetime.isLeapYear(2024)
        days_in_feb: Int = datetime.daysInMonth(2024, 2)

        io.print("2024 is leap year: %o", is_leap)
        io.print("Days in Feb 2024: %d", days_in_feb)

        return "DateTime operations completed"
    }
}
```

**datetime Library Categories (65+ functions):**

**📅 Current Date/Time:**

- `now()`, `nowUTC()`, `today()`

**🏗️ Creation Functions:**

- `create(year, month, day, hour?, minute?, second?)` - Create specific datetime
- `createDate(year, month, day)` - Create date at midnight
- `createTime(hour, minute, second?)` - Create time for today
- `fromTimestamp(timestamp)` - Create from Unix timestamp
- `fromISOString(isoString)` - Parse ISO 8601 string

**📝 Formatting Functions:**

- `formatISO(datetime)` - ISO 8601 format (2024-08-07T15:30:45Z)
- `toString(datetime)` - Human readable (2024-08-07 15:30:45)
- `toDateString(datetime)` - Date only (2024-08-07)
- `toTimeString(datetime)` - Time only (15:30:45)
- `format(datetime, formatString)` - Custom formatting

**🔍 Component Extraction:**

- `getYear()`, `getMonth()`, `getDay()`
- `getHour()`, `getMinute()`, `getSecond()`, `getMillisecond()`
- `getDayOfWeek()` - Day of week (0=Sunday to 6=Saturday)

**➕ Date Arithmetic:**

- `addDays(datetime, days)`, `addHours(datetime, hours)`
- `addMinutes(datetime, minutes)`, `addSeconds(datetime, seconds)`
- `addMilliseconds(datetime, millis)` - All support negative values

**⚖️ Comparison Functions:**

- `isEqual(dt1, dt2)`, `isBefore(dt1, dt2)`, `isAfter(dt1, dt2)`

**🔧 Utility Functions:**

- `isLeapYear(year)` - Check if year is leap year
- `daysInMonth(year, month)` - Get days in specific month
- `isWeekend(datetime)`, `isWeekday(datetime)` - Weekend detection
- `getTimestamp(datetime)` - Get Unix timestamp

**📆 Calendar Functions:**

- `startOfDay(datetime)`, `endOfDay(datetime)` - Beginning/end of day
- `startOfMonth(datetime)`, `endOfMonth(datetime)` - Beginning/end of month
- `startOfYear(datetime)`, `endOfYear(datetime)` - Beginning/end of year

### **math Library**

Comprehensive mathematical functions with 40+ methods:

```obq
import math
import system.io

Object MathDemo {
    @external method demonstrateMath(): Text {
        # Mathematical constants
        pi_val: Double = math.pi()
        e_val: Double = math.e()

        io.print("π = %.6f", pi_val)
        io.print("e = %.6f", e_val)

        # Basic functions
        abs_val: Double = math.abs(-42.5)
        sqrt_val: Double = math.sqrt(16.0)
        power: Double = math.pow(2.0, 3.0)

        io.print("abs(-42.5) = %.1f", abs_val)
        io.print("sqrt(16) = %.0f", sqrt_val)
        io.print("2^3 = %.0f", power)

        # Trigonometric functions (radians)
        sin_val: Double = math.sin(pi_val / 2.0)
        cos_val: Double = math.cos(0.0)

        io.print("sin(π/2) = %.1f", sin_val)
        io.print("cos(0) = %.1f", cos_val)

        # Logarithmic functions
        log_e: Double = math.log(e_val)
        log10_val: Double = math.log10(100.0)

        io.print("ln(e) = %.1f", log_e)
        io.print("log₁₀(100) = %.0f", log10_val)

        # Special functions
        factorial: Long = math.factorial(5)
        gcd_result: Int = math.gcd(12, 18)

        io.print("5! = %d", factorial)
        io.print("gcd(12,18) = %d", gcd_result)

        return "Mathematical operations completed"
    }
}
```

**math Library Categories (40+ functions):**

**📊 Constants:**

- `pi()`, `e()`, `tau()`, `inf()`, `nan()`

**⚡ Basic Functions:**

- `abs()`, `ceil()`, `floor()`, `round()`, `trunc()`, `sign()`
- `max()`, `min()` - Support multiple arguments

**🔢 Power & Exponential:**

- `pow(base, exponent)`, `sqrt()`, `cbrt()`, `exp()`, `exp2()`, `expm1()`

**📈 Logarithmic:**

- `log()` (natural), `log2()`, `log10()`, `log1p()`

**📐 Trigonometric:**

- `sin()`, `cos()`, `tan()`, `asin()`, `acos()`, `atan()`, `atan2()`

**〰️ Hyperbolic:**

- `sinh()`, `cosh()`, `tanh()`, `asinh()`, `acosh()`, `atanh()`

**🔄 Angular Conversion:**

- `degrees(radians)`, `radians(degrees)`

**🔬 Special Functions:**

- `factorial()`, `gcd()`, `lcm()`, `gamma()`, `lgamma()`

**⚠️ Error Functions:**

- `erf()`, `erfc()`

**✅ Classification:**

- `isFinite()`, `isInf()`, `isNan()`, `isNormal()`

### **testing Library**

Built-in testing framework for unit tests and assertions:

```obq
import testing
import system.io

Object MathTester {
    @external method runTests(): Text {
        # Create test suite
        suite: TestSuite = testing.createSuite("MathTests")

        # Run individual tests
        testing.runTest(suite, "Addition", this.testAddition)
        testing.runTest(suite, "Division", this.testDivision)
        testing.runTest(suite, "Comparison", this.testComparison)

        # Print results
        testing.printResults(suite)

        # Get counts
        total: Int = testing.getTotalCount(suite)
        passed: Int = testing.getPassedCount(suite)
        failed: Int = testing.getFailedCount(suite)

        io.print("Tests: %d total, %d passed, %d failed", total, passed, failed)

        return "Testing completed"
    }

    @external method testAddition(): Text {
        # Basic assertions
        testing.assertEqual(2 + 3, 5, "Addition test")
        testing.assertNotEqual(2 + 3, 6, "Addition inequality")
        testing.assertTrue(5 > 3, "Greater than test")
        testing.assertFalse(2 > 5, "Less than test")

        return "Addition tests completed"
    }

    @external method testDivision(): Text {
        # Numeric assertions
        testing.assertGreater(10 / 2, 4, "Division result")
        testing.assertLess(3 / 2, 2, "Division fraction")
        testing.assertNear(22.0 / 7.0, 3.14159, 0.01, "Pi approximation")

        return "Division tests completed"
    }

    @external method testComparison(): Text {
        # Text assertions
        text: Text = "Hello World"
        testing.assertContains(text, "World", "Contains test")
        testing.assertStartsWith(text, "Hello", "Starts with test")
        testing.assertEmpty("", "Empty string test")

        return "Comparison tests completed"
    }
}
```

**testing Library Functions:**

**🏗️ Test Suite Management:**

- `createSuite(name: Text): TestSuite` - Create new test suite
- `runTest(suite, name, testFunction)` - Execute test function
- `printResults(suite)` - Display formatted test results
- `getTotalCount(suite): Int`, `getPassedCount(suite): Int`, `getFailedCount(suite): Int`

**✅ Assertion Functions:**

- `assertEqual(actual, expected, message)` - Test equality
- `assertNotEqual(actual, notExpected, message)` - Test inequality
- `assertTrue(condition, message)` - Test boolean true
- `assertFalse(condition, message)` - Test boolean false
- `assertGreater(a, b, message)` - Test numeric comparison
- `assertLess(a, b, message)` - Test numeric comparison
- `assertNear(actual, expected, tolerance, message)` - Test floating point equality
- `assertContains(text, substring, message)` - Test text contains
- `assertStartsWith(text, prefix, message)` - Test text starts with
- `assertEmpty(text, message)` - Test empty string
- `fail(message)` - Force test failure

### **regexp Library**

Comprehensive regular expression library for pattern matching, text searching, and string manipulation:

```obq
import regexp
import system.io

Object RegexpDemo {
    @external method demonstratePatternMatching(): Text {
        # Basic pattern matching
        text: Text = "Hello World 123"

        # Test if text matches patterns
        has_numbers: Bool = regexp.match(text, "\\d+")          # true
        has_email: Bool = regexp.match(text, "@\\w+\\.\\w+")    # false

        # Case-insensitive matching with flags
        case_match: Bool = regexp.match("Hello", "hello", "i")  # true

        io.print("Has numbers: %o", has_numbers)
        io.print("Case insensitive match: %o", case_match)

        return "Pattern matching completed"
    }

    @external method demonstrateFindingText(): Text {
        email_text: Text = "Contact john@example.com or jane@test.org"

        # Find first occurrence
        first_email: Text = regexp.find(email_text, "\\w+@\\w+\\.\\w+")

        # Find all occurrences
        all_emails: List = regexp.findAll(email_text, "\\w+@\\w+\\.\\w+")

        # Extract capture groups
        groups: List = regexp.groups("John Doe", "(\\w+) (\\w+)")
        # Returns: ["John Doe", "John", "Doe"]

        io.print("First email: %s", first_email)
        io.print("All emails: %s", all_emails)
        io.print("Name parts: %s", groups)

        return "Text finding completed"
    }

    @external method demonstrateTextTransformation(): Text {
        # Replace text patterns
        greeting: Text = "Hello World, Hello Everyone"
        single_replace: Text = regexp.replace(greeting, "Hello", "Hi")
        all_replace: Text = regexp.replaceAll(greeting, "Hello", "Hi")

        # Format phone numbers
        phone: Text = "5551234567"
        formatted: Text = regexp.replace(phone, "(\\d{3})(\\d{3})(\\d{4})", "($1) $2-$3")

        # Split text by patterns
        csv_data: Text = "apple,banana,cherry"
        fruits: List = regexp.split(csv_data, ",")

        io.print("Single replace: %s", single_replace)
        io.print("Replace all: %s", all_replace)
        io.print("Formatted phone: %s", formatted)
        io.print("Split fruits: %s", fruits)

        return "Text transformation completed"
    }
}
```

**Available regexp Methods:**

**Pattern Matching:**

- `match(text: Text, pattern: Text, flags?: Text): Bool` - Test if pattern matches text
- `find(text: Text, pattern: Text, flags?: Text): Text` - Find first match or empty string
- `findAll(text: Text, pattern: Text, flags?: Text): List<Text>` - Find all matches
- `count(text: Text, pattern: Text, flags?: Text): Int` - Count occurrences

**Text Replacement:**

- `replace(text: Text, pattern: Text, replacement: Text, flags?: Text): Text` - Replace first match
- `replaceAll(text: Text, pattern: Text, replacement: Text, flags?: Text): Text` - Replace all matches

**Text Splitting:**

- `split(text: Text, pattern: Text, flags?: Text): List<Text>` - Split text by pattern

**Advanced Features:**

- `groups(text: Text, pattern: Text, flags?: Text): List<Text>` - Extract capture groups
- `extract(text: Text, pattern: Text, flags?: Text): List<Text>` - Extract all capture groups from all matches

**Utilities:**

- `escape(text: Text): Text` - Escape special regex characters
- `isValid(pattern: Text, flags?: Text): Bool` - Validate regex pattern
- `test(pattern: Text, flags?: Text): Text` - Create pattern for reuse

**Supported Flags:**

- `i` - Case insensitive matching
- `m` - Multiline mode
- `s` - Dot matches newlines
- `g` - Global matching (for replace operations)

### **JSON Library (import json)**

Revolutionary JSON library with 30+ methods featuring automatic type detection and native Map/List integration:

#### **Auto-Detection Parsing (Recommended)**

The `parseAuto()` method automatically detects JSON type and returns native O²L collections:

```obq
import json

Object JsonExample {
    @external method demo(): Text {
        # Objects → Native Maps
        user_data: Map = json.parseAuto("{\"name\": \"Alice\", \"age\": 30}")
        name: Text = user_data.get("name")           # Direct Map access
        age: Int = user_data.get("age")

        # Arrays → Native Lists
        colors: List = json.parseAuto("[\"red\", \"green\", \"blue\"]")
        first_color: Text = colors.get(0)           # Direct List access
        size: Int = colors.size()

        # Primitives → Direct values
        number: Int = json.parseAuto("42")
        text: Text = json.parseAuto("\"hello\"")
        flag: Bool = json.parseAuto("true")

        # Complex nested structures work automatically
        data: Map = json.parseAuto("{\"users\": [\"Bob\", \"Alice\"], \"settings\": {\"theme\": \"dark\"}}")
        users: List = data.get("users")             # Automatically a List!
        theme: Text = data.get("settings").get("theme")  # Nested access

        "JSON auto-detection completed"
    }
}
```

#### **Specific Type Parsing**

Force parsing to specific native collection types:

```obq
import json

# Force parse to Map (objects only)
config_map: Map = json.parseToMap("{\"debug\": true, \"port\": 3000}")

# Force parse to List (arrays only)
items_list: List = json.parseToList("[1, 2, 3, 4, 5]")
```

#### **Path Navigation**

Access nested JSON values using dot notation paths:

```obq
import json

Object PathExample {
    @external method demo(): Text {
        nested_json: Text = "{\"user\": {\"profile\": {\"name\": \"Bob\", \"score\": 95}}}"

        # Navigate nested structures
        name: Text = json.get(nested_json, "user.profile.name")      # "Bob"
        score: Int = json.get(nested_json, "user.profile.score")     # 95

        # Array index access
        array_json: Text = "{\"items\": [\"first\", \"second\", \"third\"]}"
        first: Text = json.get(array_json, "items.0")               # "first"
        second: Text = json.get(array_json, "items.1")              # "second"

        "Path navigation completed"
    }
}
```

#### **JSON Generation & Manipulation**

```obq
import json

Object JsonGeneration {
    @external method demo(): Text {
        # Create JSON objects and arrays
        user_json: Text = json.createObject("name", "Alice", "age", 30, "active", true)
        items_json: Text = json.createArray("apple", "banana", "cherry")

        # Modify JSON data
        updated: Text = json.set(user_json, "age", 31)
        merged: Text = json.merge(user_json, "{\"city\": \"NYC\"}")

        # Array operations
        pushed: Text = json.push(items_json, "date")
        popped: Text = json.pop(pushed)
        sliced: Text = json.slice(items_json, 1, 3)

        # Extract information
        keys: List = json.keys(user_json)
        values: List = json.values(user_json)
        size: Int = json.size(user_json)

        "JSON generation completed"
    }
}
```

#### **Validation & Utility Functions**

```obq
import json

Object JsonUtils {
    @external method demo(): Text {
        test_json: Text = "{\"data\": [1, 2, 3], \"meta\": {\"count\": 3}}"

        # Validation
        is_valid: Bool = json.isValid(test_json)
        is_object: Bool = json.isObject(test_json)
        has_data: Bool = json.hasKey(test_json, "data")
        data_type: Text = json.getType(test_json, "data")           # "array"

        # Formatting
        pretty: Text = json.prettyPrint(test_json)
        minified: Text = json.minify(pretty)

        # Comparison & cloning
        other_json: Text = "{\"meta\": {\"count\": 3}, \"data\": [1, 2, 3]}"
        are_equal: Bool = json.equals(test_json, other_json)        # true
        cloned: Text = json.clone(test_json)

        "JSON utilities completed"
    }
}
```

#### **Complete JSON Method Reference**

**Parsing Methods:**

- `parse(jsonString)` - Parse JSON to string representation
- `parseAuto(jsonString)` - Auto-detect type, return native Map/List/primitives ⭐
- `parseToMap(jsonString)` - Force parse JSON object to native Map
- `parseToList(jsonString)` - Force parse JSON array to native List
- `isValid(jsonString)` - Validate JSON format

**Path Navigation (✅ Fixed Critical Bug):**

- `get(jsonString, path)` - Get value at dot-notation path (e.g., "user.profile.name", "items.0")
- `getType(jsonString, path)` - Get type name at path
- `hasKey(jsonString, key)` - Check if object has key

**🔧 Advanced Path Navigation Examples:**

```obq
complex_json: Text = "{\"user\": {\"profile\": {\"name\": \"Alice\", \"age\": 30}}, \"items\": [\"red\", \"blue\"]}"

# Navigate nested objects and arrays
name: Text = json.get(complex_json, "user.profile.name")    # "Alice"
age: Int = json.get(complex_json, "user.profile.age")       # 30 (✅ Fixed: now returns correct integer)
first_item: Text = json.get(complex_json, "items.0")        # "red"

# Type checking with paths
name_type: Text = json.getType(complex_json, "user.profile.name")  # "string"
age_type: Text = json.getType(complex_json, "user.profile.age")    # "number"
```

**🛠️ Critical Bug Fix:** Resolved std::variant recursive copy assignment issues in path navigation that caused integers to return 0 and booleans to return false. Now uses `std::visit` pattern for safe variant copying, ensuring all data types return correct values.

**Generation & Creation:**

- `stringify(value, [indent])` - Convert value to JSON string
- `create(value)` - Create JSON from O²L value
- `createObject(key1, val1, key2, val2, ...)` - Create JSON object
- `createArray(val1, val2, val3, ...)` - Create JSON array

**Manipulation:**

- `set(jsonString, path, value)` - Set value at path
- `remove(jsonString, path)` - Remove value at path
- `merge(json1, json2)` - Merge two JSON objects
- `keys(jsonString)` - Get object keys as List
- `values(jsonString)` - Get object values as List
- `size(jsonString)` - Get object/array size

**Array Operations:**

- `push(jsonArrayString, value)` - Add element to array
- `pop(jsonArrayString)` - Remove last element from array
- `slice(jsonArrayString, start, [end])` - Extract array slice
- `indexOf(jsonArrayString, value)` - Find element index

**Type Checking:**

- `isObject(jsonString)`, `isArray(jsonString)`, `isString(jsonString)`
- `isNumber(jsonString)`, `isBoolean(jsonString)`, `isNull(jsonString)`

**Utility Functions:**

- `prettyPrint(jsonString)` - Format with indentation
- `minify(jsonString)` - Remove whitespace
- `equals(json1, json2)` - Compare JSON values
- `clone(jsonString)` - Deep copy JSON

### **URL Library (import url)**

Complete URL manipulation library with 26 methods for URL parsing, construction, validation, and manipulation:

```obq
import url
import system.io

Object UrlDemo {
    @external method demonstrateUrlOperations(): Text {
        # URL validation and parsing
        test_url: Text = "https://api.example.com:8080/v1/users?limit=50#results"
        io.print("URL: %s", test_url)
        io.print("Valid: %o", url.isValid(test_url))

        # Extract URL components
        scheme: Text = url.getScheme(test_url)      # "https"
        host: Text = url.getHost(test_url)          # "api.example.com"
        port: Text = url.getPort(test_url)          # "8080"
        path: Text = url.getPath(test_url)          # "/v1/users"
        query: Text = url.getQuery(test_url)        # "limit=50"
        fragment: Text = url.getFragment(test_url)  # "results"

        io.print("Host: %s, Port: %s", host, port)
        return "URL parsing completed"
    }

    @external method demonstrateUrlConstruction(): Text {
        # Build URLs from components
        simple: Text = url.create("https", "example.com")  # "https://example.com/"
        with_port: Text = url.create("https", "api.example.com", "8080")
        complete: Text = url.create("https", "api.github.com", "", "/repos/user/project", "per_page=100", "readme")

        io.print("Simple: %s", simple)
        io.print("Complete: %s", complete)
        return "URL construction completed"
    }

    @external method demonstrateQueryParameters(): Text {
        base: Text = "https://search.example.com"

        # Add query parameters
        with_query: Text = url.setParam(base, "q", "o2l programming")
        with_limit: Text = url.setParam(with_query, "limit", "25")
        final: Text = url.setParam(with_limit, "sort", "relevance")

        # Extract parameters
        query_value: Text = url.getParam(final, "q")        # "o2l programming"
        limit_value: Text = url.getParam(final, "limit")    # "25"

        # Remove parameters
        without_sort: Text = url.removeParam(final, "sort")

        # Get all parameters as formatted object
        all_params: Text = url.getParams(final)
        # Returns: {"limit": "25", "q": "o2l programming", "sort": "relevance"}

        io.print("Query value: %s", query_value)
        io.print("All params: %s", all_params)
        return "Query parameter demo completed"
    }

    @external method demonstrateUrlManipulation(): Text {
        # URL normalization
        messy: Text = "HTTPS://Example.COM:443/api/../users/./profile/../settings"
        normalized: Text = url.normalize(messy)  # "https://example.com/users/settings"

        # URL resolution
        base: Text = "https://api.example.com/v1/users/"
        relative: Text = "../projects/123"
        resolved: Text = url.resolve(base, relative)  # "https://api.example.com/v1/projects/123"

        # URL joining
        joined: Text = url.join("https://cdn.example.com", "assets", "images", "logo.png")
        # "https://cdn.example.com/assets/images/logo.png"

        # URL encoding/decoding
        special: Text = "hello world! @#$%"
        encoded: Text = url.encode(special)      # "hello%20world%21%20%40%23%24%25"
        decoded: Text = url.decode(encoded)      # "hello world! @#$%"

        io.print("Normalized: %s", normalized)
        io.print("Resolved: %s", resolved)
        io.print("Encoded: %s", encoded)
        return "URL manipulation completed"
    }

    @external method demonstrateUrlUtilities(): Text {
        test_urls: List<Text> = [
            "https://www.example.com:8080/path",
            "https://example.com/path",
            "/relative/path"
        ]

        iterator: ListIterator = test_urls.iterator()
        while (iterator.hasNext()) {
            test_url: Text = iterator.next()

            is_absolute: Bool = url.isAbsolute(test_url)
            is_relative: Bool = url.isRelative(test_url)

            if (is_absolute) {
                domain: Text = url.getDomain(test_url)    # Extract base domain
                origin: Text = url.getOrigin(test_url)    # scheme + host + port
                io.print("URL: %s -> Domain: %s, Origin: %s", test_url, domain, origin)
            } else {
                io.print("Relative URL: %s", test_url)
            }
        }

        return "URL utilities completed"
    }
}
```

**Available URL Methods:**

**URL Parsing & Validation:**

- `isValid(url: Text): Bool` - Validate URL format
- `parse(url: Text): Text` - Parse and normalize URL
- `getScheme(url: Text): Text` - Extract protocol (http, https, etc.)
- `getHost(url: Text): Text` - Extract hostname
- `getPort(url: Text): Text` - Extract port number
- `getPath(url: Text): Text` - Extract path component
- `getQuery(url: Text): Text` - Extract query string
- `getFragment(url: Text): Text` - Extract fragment/hash

**URL Construction & Modification:**

- `create(scheme: Text, host: Text, [port, path, query, fragment]): Text` - Build URL from components
- `setScheme(url: Text, scheme: Text): Text` - Change URL protocol
- `setHost(url: Text, host: Text): Text` - Change hostname
- `setPort(url: Text, port: Text|Int): Text` - Change port number
- `setPath(url: Text, path: Text): Text` - Change path
- `setQuery(url: Text, query: Text): Text` - Change query string
- `setFragment(url: Text, fragment: Text): Text` - Change fragment

**Query Parameter Management:**

- `getParam(url: Text, name: Text): Text` - Get specific query parameter
- `setParam(url: Text, name: Text, value: Text): Text` - Set query parameter
- `removeParam(url: Text, name: Text): Text` - Remove query parameter
- `getParams(url: Text): Text` - Get all parameters as formatted object

**URL Manipulation:**

- `normalize(url: Text): Text` - Clean and normalize URL (lowercase, remove dots)
- `resolve(baseUrl: Text, relativeUrl: Text): Text` - Resolve relative URLs
- `join(base: Text, ...parts: Text): Text` - Join URL parts with proper separators
- `encode(text: Text): Text` - URL encode special characters
- `decode(encodedText: Text): Text` - URL decode text

**Utility Methods:**

- `getDomain(url: Text): Text` - Extract base domain (removes subdomains)
- `getOrigin(url: Text): Text` - Get origin (scheme + host + port)
- `isAbsolute(url: Text): Bool` - Check if URL is absolute
- `isRelative(url: Text): Bool` - Check if URL is relative

**URL Features:**

- **Smart Port Handling**: Automatically omits default ports (80 for HTTP, 443 for HTTPS)
- **RFC 3986 Compliant**: Full URL encoding/decoding with proper character handling
- **Path Normalization**: Resolves `.` and `..` components in paths
- **Query Parameter Encoding**: Automatically handles special characters in parameters
- **Domain Extraction**: Intelligent domain parsing with subdomain handling
- **Comprehensive Validation**: Detailed error messages for invalid URLs

### **HTTP Client Library (import http.client)**

Complete HTTP client library for making REST API calls, handling webhooks, and integrating with web services. Features 30+ methods for HTTP operations, request/response handling, authentication, and utilities:

```obq
import http.client
import system.io
import json

Object HttpDemo {
    @external method basicHttpOperations(): Text {
        # Simple GET request
        response: HttpResponse = client.get("https://api.github.com/users/octocat")

        status: Int = client.getStatus(response)
        success: Bool = client.isSuccess(response)
        body: Text = client.getBody(response)

        io.print("Status: %d, Success: %b", status, success)

        # POST JSON data
        user_data: Text = json.createObject("name", "Alice", "email", "alice@example.com")
        headers: Map<Text, Text> = {"Content-Type": "application/json"}

        post_response: HttpResponse = client.post("https://httpbin.org/post", user_data, headers)
        io.print("POST Status: %d", client.getStatus(post_response))

        return "Basic HTTP operations completed"
    }

    @external method advancedRequestConfiguration(): Text {
        # Create and configure a request
        request: HttpRequest = client.createRequest("POST", "https://api.example.com/data")

        # Set authentication
        client.setBearerToken(request, "your-api-token")

        # Configure headers
        client.setHeader(request, "User-Agent", "O²L-Client/1.0")
        client.setHeaders(request, {"Accept": "application/json", "Custom-Header": "value"})

        # Set query parameters
        client.setQueryParam(request, "version", "v1")
        client.setQueryParams(request, {"format": "json", "pretty": "true"})

        # Configure request body
        request_body: Text = json.createObject("message", "Hello from O²L")
        client.setJsonBody(request, request_body)

        # Set timeout
        client.setTimeout(request, 30)

        return "Request configuration completed"
    }

    @external method authenticationMethods(): Text {
        request: HttpRequest = client.createRequest("GET", "https://api.example.com/protected")

        # Basic Authentication
        client.setBasicAuth(request, "username", "password")

        # Bearer Token
        client.setBearerToken(request, "jwt-token-here")

        # API Key
        client.setApiKey(request, "X-API-Key", "secret-api-key")

        return "Authentication methods configured"
    }

    @external method responseHandling(): Text {
        response: HttpResponse = client.get("https://api.github.com/repos/octocat/Hello-World")

        # Response inspection
        status: Int = client.getStatus(response)
        status_msg: Text = client.getStatusMessage(response)
        headers: Map<Text, Text> = client.getHeaders(response)
        content_type: Text = client.getHeader(response, "Content-Type")

        # Body handling
        body: Text = client.getBody(response)
        json_data: Text = client.getJson(response)  # Parse as JSON

        # Error checking
        if (!client.isSuccess(response)) {
            error: Text = client.getError(response)
            io.print("Error: %s", error)
        }

        return "Response handling completed"
    }
}
```

#### **HTTP Method Support**

All standard HTTP methods with flexible parameter handling:

```obq
import http.client

Object HttpMethods {
    @external method demonstrateHttpMethods(): Text {
        base_url: Text = "https://httpbin.org"

        # GET request
        get_response: HttpResponse = client.get(base_url + "/get")

        # GET with headers
        headers: Map<Text, Text> = {"User-Agent": "O²L/1.0"}
        get_with_headers: HttpResponse = client.get(base_url + "/get", headers)

        # POST with body and headers
        json_body: Text = json.createObject("key", "value")
        post_response: HttpResponse = client.post(base_url + "/post", json_body, headers)

        # PUT for updates
        update_data: Text = json.createObject("id", 123, "status", "updated")
        put_response: HttpResponse = client.put(base_url + "/put", update_data, headers)

        # PATCH for partial updates
        patch_data: Text = json.createObject("status", "active")
        patch_response: HttpResponse = client.patch(base_url + "/patch", patch_data, headers)

        # DELETE
        delete_response: HttpResponse = client.delete(base_url + "/delete", headers)

        # HEAD for metadata only
        head_response: HttpResponse = client.head(base_url + "/get", headers)

        # OPTIONS for capability discovery
        options_response: HttpResponse = client.options(base_url + "/get", headers)

        # Generic request method
        custom_response: HttpResponse = client.request("GET", base_url + "/get", "", headers)

        return "All HTTP methods demonstrated"
    }
}
```

#### **URL Utilities & Helper Functions**

```obq
import http.client

Object UrlUtilities {
    @external method demonstrateUrlUtils(): Text {
        # URL encoding/decoding
        original: Text = "hello world & special chars!"
        encoded: Text = client.urlEncode(original)
        decoded: Text = client.urlDecode(encoded)

        # URL building with parameters
        base_url: Text = "https://api.example.com/search"
        params: Map<Text, Text> = {
            "q": "o2l programming",
            "limit": "50",
            "sort": "relevance"
        }
        full_url: Text = client.buildUrl(base_url, params)
        # Result: "https://api.example.com/search?q=o2l%20programming&limit=50&sort=relevance"

        # URL parsing
        test_url: Text = "https://api.example.com:8080/v1/users?page=2&limit=10#results"
        url_parts: Map<Text, Text> = client.parseUrl(test_url)

        protocol: Text = url_parts.get("protocol")  # "https"
        host: Text = url_parts.get("host")          # "api.example.com"
        path: Text = url_parts.get("path")          # "/v1/users"
        query: Text = url_parts.get("query")        # "page=2&limit=10"

        return "URL utilities demonstrated"
    }
}
```

#### **File Upload & Download**

```obq
import http.client

Object FileOperations {
    @external method uploadDownloadDemo(): Text {
        # Upload file
        upload_response: HttpResponse = client.uploadFile(
            "https://httpbin.org/post",
            "/path/to/file.txt"
        )

        if (client.isSuccess(upload_response)) {
            io.print("File uploaded successfully")
        }

        # Download file
        download_response: HttpResponse = client.downloadFile(
            "https://httpbin.org/json",
            "/tmp/downloaded_data.json"
        )

        if (client.isSuccess(download_response)) {
            io.print("File downloaded to /tmp/downloaded_data.json")
        }

        return "File operations completed"
    }
}
```

#### **RESTful API Client Pattern**

```obq
import http.client
import json

Object ApiClient {
    property base_url: Text
    property auth_token: Text

    constructor(base_url: Text, auth_token: Text) {
        this.base_url = base_url
        this.auth_token = auth_token
    }

    @external method createAuthHeaders(): Map<Text, Text> {
        return {
            "Authorization": "Bearer " + this.auth_token,
            "Content-Type": "application/json",
            "Accept": "application/json"
        }
    }

    @external method getUsers(): Map<Text, Value> {
        headers: Map<Text, Text> = this.createAuthHeaders()
        response: HttpResponse = client.get(this.base_url + "/users", headers)

        result: Map<Text, Value> = {}
        result.put("success", client.isSuccess(response))
        result.put("status", client.getStatus(response))
        result.put("data", client.getBody(response))

        return result
    }

    @external method createUser(name: Text, email: Text): Map<Text, Value> {
        user_data: Text = json.createObject("name", name, "email", email)
        headers: Map<Text, Text> = this.createAuthHeaders()

        response: HttpResponse = client.post(this.base_url + "/users", user_data, headers)

        result: Map<Text, Value> = {}
        result.put("success", client.isSuccess(response))
        result.put("status", client.getStatus(response))

        if (client.isSuccess(response)) {
            result.put("user", client.getJson(response))
        } else {
            result.put("error", client.getError(response))
        }

        return result
    }

    @external method updateUser(user_id: Int, data: Text): Map<Text, Value> {
        headers: Map<Text, Text> = this.createAuthHeaders()
        url: Text = this.base_url + "/users/" + user_id.toString()

        response: HttpResponse = client.put(url, data, headers)

        result: Map<Text, Value> = {}
        result.put("success", client.isSuccess(response))
        result.put("status", client.getStatus(response))

        return result
    }

    @external method deleteUser(user_id: Int): Bool {
        headers: Map<Text, Text> = this.createAuthHeaders()
        url: Text = this.base_url + "/users/" + user_id.toString()

        response: HttpResponse = client.delete(url, headers)
        return client.isSuccess(response)
    }
}
```

#### **Complete HTTP Method Reference**

**Core HTTP Methods:**

- `get(url, [headers])` - HTTP GET request
- `post(url, [body], [headers])` - HTTP POST request
- `put(url, [body], [headers])` - HTTP PUT request
- `delete(url, [headers])` - HTTP DELETE request
- `patch(url, [body], [headers])` - HTTP PATCH request
- `head(url, [headers])` - HTTP HEAD request
- `options(url, [headers])` - HTTP OPTIONS request

**Advanced Request Methods:**

- `request(method, url, [body], [headers])` - Generic HTTP request
- `requestWithConfig(requestConfig)` - Request with full configuration object

**Request Configuration:**

- `createRequest(method, url)` - Create configurable request object
- `setHeader(request, name, value)` - Set single header
- `setHeaders(request, headersMap)` - Set multiple headers
- `setQueryParam(request, name, value)` - Set query parameter
- `setQueryParams(request, paramsMap)` - Set multiple query parameters
- `setTimeout(request, seconds)` - Set request timeout
- `setBody(request, body)` - Set request body
- `setJsonBody(request, jsonData)` - Set JSON body with content-type

**Authentication:**

- `setBasicAuth(request, username, password)` - HTTP Basic Authentication
- `setBearerToken(request, token)` - Bearer token authentication
- `setApiKey(request, headerName, apiKey)` - API key authentication

**Response Handling:**

- `getStatus(response)` - Get HTTP status code
- `getStatusMessage(response)` - Get status message
- `getHeader(response, name)` - Get specific header
- `getHeaders(response)` - Get all headers as Map
- `getBody(response)` - Get response body
- `getJson(response)` - Parse response as JSON
- `isSuccess(response)` - Check if status indicates success (200-299)
- `getError(response)` - Get error message

**URL Utilities:**

- `urlEncode(string)` - URL encode string
- `urlDecode(string)` - URL decode string
- `buildUrl(baseUrl, paramsMap)` - Build URL with query parameters
- `parseUrl(url)` - Parse URL into components

**File Operations:**

- `uploadFile(url, filePath)` - Upload file via multipart form
- `uploadFile(url, filePath, formFields)` - Upload file with additional form data
- `downloadFile(url, destinationPath)` - Download file to local path

#### **Advanced Features**

**🔧 Multipart File Upload with Form Data:**

```obq
# Upload file with additional form fields
form_fields: Map<Text, Text> = {
    "description": "User profile photo",
    "category": "avatar",
    "user_id": "12345"
}
response: HttpResponse = client.uploadFile("https://api.example.com/upload", "/path/to/photo.jpg", form_fields)
```

**🔐 Comprehensive Authentication:**

```obq
request: HttpRequest = client.createRequest("GET", "https://api.example.com/data")

# Basic Authentication
client.setBasicAuth(request, "username", "password")

# Bearer Token
client.setBearerToken(request, "your-jwt-token-here")

# API Keys (supports multiple custom headers)
client.setApiKey(request, "X-API-Key", "secret-key-123")
client.setApiKey(request, "X-Custom-Auth", "custom-value")
```

**⚙️ Advanced Request Configuration:**

```obq
request: HttpRequest = client.createRequest("POST", "https://api.example.com/users")

# Set comprehensive headers
client.setHeader(request, "User-Agent", "MyApp/1.0")
headers: Map<Text, Text> = {
    "Accept": "application/json",
    "X-Request-ID": "req-12345"
}
client.setHeaders(request, headers)

# Configure query parameters
client.setQueryParam(request, "version", "v2")
params: Map<Text, Text> = {"debug": "true", "format": "json"}
client.setQueryParams(request, params)

# Set timeout and body
client.setTimeout(request, 60)
client.setJsonBody(request, json_data)

# Execute configured request
response: HttpResponse = client.requestWithConfig(request)
```

#### **Platform-Specific Implementations**

**🪟 Windows (WinINet):** Native Windows HTTP client using WinINet API for optimal performance and system integration.

**🐧 Linux (POSIX Sockets):** Low-level implementation using POSIX sockets for direct HTTP protocol handling.

**🍎 macOS (Native):** macOS-optimized implementation leveraging system frameworks.

**🌍 libcurl Fallback:** Cross-platform fallback using libcurl for maximum compatibility and feature support.

#### **Key Features**

- **Cross-Platform Support**: Works on Windows, macOS, and Linux with platform-specific optimizations
- **JSON Integration**: Seamless integration with the JSON library for API responses
- **Flexible Authentication**: Support for Basic Auth, Bearer tokens, and custom API keys
- **Request Configuration**: Fluent API for building complex HTTP requests
- **Error Handling**: Comprehensive error reporting with status codes and messages
- **URL Utilities**: Complete URL manipulation and validation tools
- **Advanced File Transfer**: Multipart/form-data uploads with custom form fields
- **Thread Safety**: Safe for concurrent usage with proper synchronization
- **Response Parsing**: Automatic JSON detection and parsing capabilities
- **MIME Type Detection**: Automatic content type detection for file uploads

### **system.utils Library**

Utility functions for iteration and common operations:

```obq
import system.utils
import system.io

Object UtilsDemo {
    @external method demonstrateRepeatIterator(): Text {
        # Create RepeatIterator for controlled repetition
        repeater: RepeatIterator = utils.repeat(5)

        io.print("=== Counting 0 to 4 ===")
        while (repeater.hasNext()) {
            count: Int = repeater.next()
            io.print("Count: %d", count)
        }

        # Reset and reuse
        repeater.reset()
        io.print("After reset, hasNext: %o", repeater.hasNext())

        # Use with calculations
        sum: Int = 0
        while (repeater.hasNext()) {
            value: Int = repeater.next()
            sum = sum + value
        }

        io.print("Sum of 0+1+2+3+4 = %d", sum)

        return "Repeat iterator demonstration completed"
    }
}
```

**system.utils Methods:**

- `repeat(count: Int): RepeatIterator` - Create iterator that counts from 0 to count-1
  - `RepeatIterator.hasNext(): Bool` - Check if more iterations available
  - `RepeatIterator.next(): Int` - Get next iteration count
  - `RepeatIterator.reset()` - Reset iterator to beginning
  - `RepeatIterator.getCurrentCount(): Int` - Get current iteration index
  - `RepeatIterator.getTotalCount(): Int` - Get total iteration count

### **Foreign Function Interface (FFI) Library**

O²L provides a comprehensive Foreign Function Interface (FFI) system that enables seamless integration with C libraries and native code. The FFI system is built on libffi and supports advanced type marshaling, including structs, arrays, callbacks, and pointer conversions.

#### **Basic FFI Usage**

```obq
import ffi
import system.io

Object FFIExample {
    @external method basicExample(): Int {
        # Load a shared library
        libResult: Result<Value, Error> = ffi.load("/usr/lib/libm.so")
        if (!libResult.isSuccess()) {
            io.print("Failed to load math library")
            return 1
        }

        lib: Value = libResult.getResult()

        # Get a function symbol
        sqrtResult: Result<Value, Error> = lib.symbol("sqrt", "f64->f64")
        if (!sqrtResult.isSuccess()) {
            io.print("Failed to get sqrt function")
            return 1
        }

        sqrtFunc: Value = sqrtResult.getResult()

        # Call the function
        callResult: Result<Value, Error> = sqrtFunc.call(25.0)
        if (!callResult.isSuccess()) {
            io.print("Function call failed")
            return 1
        }

        result: Value = callResult.getResult()
        io.print("sqrt(25.0) = %s", result.toString())

        return 0
    }
}
```

#### **Enhanced FFI Types**

The FFI system supports advanced types for complex C library integration:

```obq
import ffi
import system.io

Object AdvancedFFIExample {
    @external method demonstrateAdvancedTypes(): Int {
        # Create C strings
        cstrResult: Result<Value, Error> = ffi.cstring("Hello, C World!")
        cstr: Value = cstrResult.getResult()

        # Create structs
        structResult: Result<Value, Error> = ffi.struct(32)  # 32-byte struct
        struct: Value = structResult.getResult()

        # Add fields to struct
        struct.addField("id", "i32", 0)
        struct.addField("name", "ptr", 8)
        struct.addField("score", "f64", 16)

        # Set field values
        struct.setField("id", 42)
        struct.setField("name", cstr)
        struct.setField("score", 95.5)

        # Create arrays
        arrayResult: Result<Value, Error> = ffi.array("i32", 5)
        array: Value = arrayResult.getResult()

        # Set array elements
        i: Int = 0
        while (i < 5) {
            array.set(i, i * 10)
            i = i + 1
        }

        # Access array elements
        firstElement: Value = array.get(0)
        io.print("First array element: %s", firstElement.toString())

        return 0
    }
}
```

#### **Pointer Conversions**

O²L FFI provides safe pointer conversion functions:

```obq
import ffi
import system.io

Object PointerExample {
    @external method demonstratePointerConversions(): Int {
        # Get a pointer from C function call
        lib: Value = ffi.load("/usr/lib/libc.so").getResult()
        mallocFunc: Value = lib.symbol("malloc", "u64->ptr").getResult()

        # Allocate memory
        ptr: Value = mallocFunc.call(64).getResult()

        # Convert pointer to different types
        stringResult: Result<Value, Error> = ffi.ptrToString(ptr)
        intResult: Result<Value, Error> = ffi.ptrToInt(ptr)
        doubleResult: Result<Value, Error> = ffi.ptrToDouble(ptr)

        if (stringResult.isSuccess()) {
            text: Value = stringResult.getResult()
            io.print("Pointer as string: %s", text)
        }

        if (intResult.isSuccess()) {
            number: Value = intResult.getResult()
            io.print("Pointer as int: %s", number.toString())
        }

        return 0
    }
}
```

#### **SQLite Integration Example**

The FFI system enables seamless database integration:

```obq
import ffi
import system.io

Object SQLiteExample {
    @external method sqliteDemo(): Int {
        # Load SQLite library
        lib: Value = ffi.load("/opt/homebrew/lib/libsqlite3.dylib").getResult()

        # Open database
        openFunc: Value = lib.symbol("sqlite3_open", "ptr,ptr->i32").getResult()
        dbPath: Value = ffi.cstring(":memory:").getResult()
        dbHandleArray: Value = ffi.array("ptr", 1).getResult()

        openResult: Int = openFunc.call(dbPath, dbHandleArray).getResult().toInt()
        if (openResult != 0) {
            io.print("Failed to open database")
            return 1
        }

        dbHandle: Value = dbHandleArray.get(0)

        # Execute SQL
        execFunc: Value = lib.symbol("sqlite3_exec", "ptr,ptr,ptr,ptr,ptr->i32").getResult()
        createSQL: Value = ffi.cstring("CREATE TABLE users (id INTEGER, name TEXT)").getResult()

        execResult: Int = execFunc.call(dbHandle, createSQL, ffi.nullPtr(), ffi.nullPtr(), ffi.nullPtr()).getResult().toInt()
        if (execResult == 0) {
            io.print("Table created successfully")
        }

        # Close database
        closeFunc: Value = lib.symbol("sqlite3_close", "ptr->i32").getResult()
        closeFunc.call(dbHandle)

        return 0
    }
}
```

#### **FFI Core Functions**

**Library Management:**

- `ffi.load(path: Text): Result<Value, Error>` - Load shared library
- `library.symbol(name: Text, signature: Text): Result<Value, Error>` - Get function symbol
- `function.call(...args): Result<Value, Error>` - Call C function

**Type Creation:**

- `ffi.cstring(text: Text): Result<Value, Error>` - Create null-terminated C string
- `ffi.struct(size: Int): Result<Value, Error>` - Create struct with specified size
- `ffi.array(type: Text, count: Int): Result<Value, Error>` - Create typed array
- `ffi.nullPtr(): Value` - Create null pointer

**Enhanced Type Operations:**

- `struct.addField(name: Text, type: Text, offset: Int): Bool` - Add field to struct
- `struct.setField(name: Text, value: Value): Bool` - Set field value
- `struct.getField(name: Text): Value` - Get field value
- `array.set(index: Int, value: Value): Bool` - Set array element
- `array.get(index: Int): Value` - Get array element

**Pointer Conversions:**

- `ffi.ptrToString(ptr: Value): Result<Value, Error>` - Convert pointer to string
- `ffi.ptrToInt(ptr: Value): Result<Value, Error>` - Convert pointer to integer
- `ffi.ptrToDouble(ptr: Value): Result<Value, Error>` - Convert pointer to double
- `ffi.ptrToFloat(ptr: Value): Result<Value, Error>` - Convert pointer to float
- `ffi.ptrToBool(ptr: Value): Result<Value, Error>` - Convert pointer to boolean

#### **Type Signatures**

FFI functions use C-style type signatures:

**Basic Types:**

- `i8`, `i16`, `i32`, `i64` - Signed integers
- `u8`, `u16`, `u32`, `u64` - Unsigned integers
- `f32`, `f64` - Floating point
- `ptr` - Pointer
- `void` - No return value

**Function Signatures:**

- `"i32,i32->i32"` - Takes two int32, returns int32
- `"ptr,f64->ptr"` - Takes pointer and double, returns pointer
- `"->i32"` - No parameters, returns int32
- `"ptr,ptr,i32,ptr,ptr->i32"` - Multiple parameters

#### **Memory Management**

The FFI system provides automatic memory management for O²L-created objects:

- **Automatic Cleanup**: CBufferInstance, CArrayInstance, and CStructInstance objects are automatically cleaned up
- **Safe Pointers**: PtrInstance objects provide safe pointer handling with null checking
- **Result Types**: All FFI operations return Result<T, Error> for safe error handling
- **Type Safety**: Enhanced types provide bounds checking and type validation

#### **SQLite Library Integration**

O²L includes a complete SQLite wrapper library (`lib/sqlite/`) demonstrating real-world FFI usage:

```bash
# Example SQLite library files
lib/sqlite/
├── test_sqlite_basic.obq       # Basic SQLite operations
├── sqlite_wrapper.obq         # High-level wrapper functions
├── fixed_wrapper.obq          # Production-ready wrapper
└── test_ptr_conversions.obq    # Pointer conversion examples
```

The SQLite integration demonstrates:

- Dynamic library loading and symbol resolution
- Complex function calls with multiple parameters
- Proper resource management and cleanup
- String and pointer handling
- Error handling and result processing

### **Complete System Import Example**

```obq
import system.io
import system.os
import system.fs
import system.utils
import datetime
import math
import testing

Object SystemDemo {
    @external method demonstrateAllLibraries(): Text {
        io.print("=== O²L System Libraries Demo ===")

        # I/O operations
        io.print("Current user: %s", os.getEnv("USER"))

        # File operations
        success: Bool = fs.writeText("demo.txt", "Test content")
        if (success) {
            content: Text = fs.readText("demo.txt")
            io.print("File content: %s", content)
            fs.deleteFile("demo.txt")
        }

        # DateTime operations
        now: Text = datetime.now()
        io.print("Current time: %s", datetime.toString(now))

        # Math operations
        result: Double = math.sqrt(math.pow(3.0, 2.0) + math.pow(4.0, 2.0))
        io.print("Hypotenuse of 3-4-5 triangle: %.1f", result)

        # Iteration utilities
        repeater: RepeatIterator = utils.repeat(3)
        factorial: Int = 1
        counter: Int = 1

        while (repeater.hasNext()) {
            repeater.next()
            counter = counter + 1
            factorial = factorial * counter
        }

        io.print("4! = %d", factorial)

        # Testing framework
        suite: TestSuite = testing.createSuite("SystemTests")
        testing.runTest(suite, "MathTest", this.testMathFunction)
        testing.printResults(suite)

        return "All system libraries demonstrated successfully"
    }

    @external method testMathFunction(): Text {
        # Test math operations
        testing.assertEqual(math.max(1.0, 3.0, 2.0), 3.0, "Max function test")
        testing.assertNear(math.pi(), 3.14159, 0.01, "Pi constant test")
        testing.assertTrue(math.isFinite(42.0), "Finite number test")

        return "Math tests completed"
    }
}

Object Main {
    method main(): Text {
        demo: SystemDemo = new SystemDemo()
        return demo.demonstrateAllLibraries()
    }
}
```

---

## 🏗️ Advanced Concepts

### **Constructor Patterns**

```obq
Record Config {
    name: Text
    enabled: Bool
    timeout: Int
}

Object Service {
    property config: Config
    property isRunning: Bool

    constructor(serviceConfig: Config) {
        this.config = serviceConfig
        this.isRunning = false
    }

    @external method start(): Bool {
        if (this.config.enabled) {
            this.isRunning = true
            this.initializeService()
        }
        return this.isRunning
    }

    method initializeService(): Void {
        # Internal initialization logic
        io.print("Starting service: %s", this.config.name)
    }
}
```

### **Object Collaboration**

```obq
Object Logger {
    @external method log(message: Text): Void {
        io.print("[LOG] %s", message)
    }
}

Object Database {
    property logger: Logger

    constructor() {
        this.logger = new Logger()
    }

    @external method save(data: Text): Bool {
        this.logger.log("Saving data: " + data)
        # Simulate save operation
        return true
    }
}

Object UserService {
    property db: Database

    constructor() {
        this.db = new Database()
    }

    @external method createUser(name: Text): Bool {
        userData: Text = "User: " + name
        return this.db.save(userData)
    }
}
```

### **Complex Data Flow**

```obq
Record Address {
    street: Text
    city: Text
    zipCode: Text
}

Record Contact {
    email: Text
    phone: Text
    address: Address
}

Object ContactValidator {
    @external method validateEmail(email: Text): Bool {
        # Simple email validation (contains @)
        return email.contains("@")
    }

    @external method validateContact(contact: Contact): Bool {
        return this.validateEmail(contact.email)
    }
}

Object UserManager {
    property validator: ContactValidator

    constructor() {
        this.validator = new ContactValidator()
    }

    @external method processUser(contact: Contact): Text {
        if (this.validator.validateContact(contact)) {
            return "User created successfully"
        }
        return "Invalid contact information"
    }
}
```

---

## 🔧 Development Tools

### **Command Line Interface**

```bash
# Run a program
o2l run program.obq

# Run with command line arguments
o2l run program.obq arg1 arg2 arg3

# Run with debug output (debug flag is filtered from program args)
o2l run program.obq --debug arg1 arg2

# Start interactive REPL
o2l repl

# Show help
o2l --help

# Show version
o2l --version
```

**Command Line Arguments:**

- Arguments are passed to the program via `system.os.args()` and `system.os.argc()`
- Following C/C++ conventions: `args[0]` contains the program name
- The `--debug` flag is automatically filtered out and does not appear in program arguments
- All other arguments are passed through to the program unchanged

### **Debugging**

Enable debug mode to see detailed execution traces:

```bash
o2l run program.obq --debug
```

Example debug output:

```
[DEBUG] Running file: program.obq
[DEBUG] Source code length: 1247 characters
[DEBUG] Starting lexical analysis...
[DEBUG] Tokenized 156 tokens
[DEBUG] Starting parsing...
[DEBUG] Parsed 3 AST nodes
[DEBUG] AST Node: Object(Calculator)
[DEBUG] AST Node: Object(Main)
[DEBUG] Starting interpretation...
[DEBUG] Execution completed
[DEBUG] Main returned exit code: 0
```

### **Error Handling**

O²L provides detailed error messages with stack traces:

```obq
Error: Evaluation Error: Method 'nonExistentMethod' not found on object 'Calculator'
Stack trace:
  at Calculator.compute (line 15)
  at Main.main (line 8)
```

---

## 📚 Language Reference

### **Built-in Types**

| Type             | Description                              | Example                                      |
| ---------------- | ---------------------------------------- | -------------------------------------------- |
| `Int`            | 64-bit signed integer                    | `42`, `-100`                                 |
| `Long`           | 128-bit extended precision integer       | `123456789012345L`, `-999999999999999999l`   |
| `Float`          | 32-bit single-precision floating point   | `3.14f`, `-2.5F`                             |
| `Double`         | 64-bit double-precision floating point   | `3.14159d`, `-2.5D`, `1.41`                  |
| `Text`           | UTF-8 string                             | `"Hello World"`                              |
| `Bool`           | Boolean value                            | `true`, `false`                              |
| `Char`           | Single character                         | `'A'`, `'$'`                                 |
| `List<T>`        | Generic list collection                  | `[1, 2, 3]`, `["a", "b"]`                    |
| `Map<K,V>`       | Generic key-value collection             | `{"key": "value"}`, `{"a": 1}`               |
| `Set<T>`         | Generic set collection (unique elements) | `(1, 2, 3)`, `("a", "b")`                    |
| `ListIterator`   | Iterator for List<T> collections         | `list.iterator()`                            |
| `MapIterator`    | Iterator for Map<K,V> collections        | `map.iterator()`                             |
| `MapObject`      | Key-value pair from Map iteration        | `iterator.MapItem()`                         |
| `SetIterator`    | Iterator for Set<T> collections          | `set.iterator()`                             |
| `RepeatIterator` | Controlled repetition iterator           | `utils.repeat(5)`                            |
| `Result<T,E>`    | Functional error handling type           | `Result.success(value)`, `Result.error(err)` |
| `Error`          | Structured error object                  | `new Error("message")`                       |
| `Void`           | No return value                          | Method return type only                      |

**Type Notes**:

- Decimal literals without a suffix (e.g., `3.14`) default to `Double` type
- Use `f`/`F` suffix for `Float` literals and `d`/`D` suffix for `Double` literals
- Use `l`/`L` suffix for `Long` literals to specify 128-bit precision integers
- **Type hierarchy for arithmetic**: `Long` > `Double` > `Float` > `Int` (operations promote to highest precision)
- **List<T> operations**: `add(element)`, `get(index)`, `remove(index)`, `reverse()`, `pop()`, `size()`
- **List type safety**: `List<Int> = [1, 2, "text"]` will produce a compile-time type error
- **ListIterator operations**: `hasNext()`, `next()`, `reset()`, `index()` - `index()` returns current position after `next()` call
- **Map<K,V> operations**: `put(key, value)`, `get(key)`, `contains(key)`, `remove(key)`, `size()`, `iterator()`
- **Map type safety**: Keys and values must match declared types: `Map<Text,Int> = {"key": "value"}` will produce a type error
- **MapIterator operations**: `hasNext()`, `MapItem()`, `nextKey()`, `nextValue()`, `nextEntry()`, `reset()`, `getCurrentIndex()`
- **Set<T> operations**: `add(element)`, `contains(element)`, `remove(element)`, `clear()`, `size()`, `empty()`, `elements()`, `iterator()`
- **Set type safety**: Elements must match declared type and duplicates are automatically removed based on structural equality
- **SetIterator operations**: `hasNext()`, `next()`, `reset()`, `getCurrentIndex()`, `getTotalSize()`
- **RepeatIterator operations**: `hasNext()`, `next()`, `reset()`, `getCurrentCount()`, `getTotalCount()`
- **Result<T,E> operations**: `isSuccess()`, `getResult()`, `getError()`, `Result.success(value)`, `Result.error(error)`
- **Error operations**: `getMessage()`, `getCode()`, `getCause()` - structured error objects with optional error codes and causal chains
- **system.os operations**: `args()` returns `List<Text>` of command line arguments, `argc()` returns `Int` argument count

### **Type Conversion Methods**

O²L provides comprehensive type conversion methods for seamless conversion between built-in types with proper error handling and range checking.

#### **Text Conversions**

```obq
text_val: Text = "42"
int_result: Int = text_val.toInt()        # "42" → 42
long_result: Long = text_val.toLong()     # "42" → 42L
float_result: Float = "3.14".toFloat()    # "3.14" → 3.14f
double_result: Double = "3.14".toDouble() # "3.14" → 3.14
bool_result: Bool = "true".toBool()       # "true" → true
```

#### **Numeric Conversions**

```obq
# Int conversions
int_val: Int = 42
str_result: Text = int_val.toString()      # 42 → "42"
float_result: Float = int_val.toFloat()    # 42 → 42.0f
double_result: Double = int_val.toDouble() # 42 → 42.0
long_result: Long = int_val.toLong()       # 42 → 42L
bool_result: Bool = int_val.toBool()       # 42 → true (0 → false)

# Float conversions
float_val: Float = 3.14f
str_result: Text = float_val.toString()    # 3.14f → "3.14"
int_result: Int = float_val.toInt()        # 3.14f → 3 (truncated)
long_result: Long = float_val.toLong()     # 3.14f → 3L
bool_result: Bool = float_val.toBool()     # 3.14f → true (0.0f → false)

# Special Float methods
is_nan: Bool = float_val.isNaN()           # Check if Not-a-Number
is_infinite: Bool = float_val.isInfinite() # Check if infinite
is_finite: Bool = float_val.isFinite()     # Check if finite
```

#### **Boolean Conversions**

```obq
bool_val: Bool = true
str_result: Text = bool_val.toString()     # true → "true"
int_result: Int = bool_val.toInt()         # true → 1, false → 0
long_result: Long = bool_val.toLong()      # true → 1L, false → 0L
float_result: Float = bool_val.toFloat()   # true → 1.0f, false → 0.0f
double_result: Double = bool_val.toDouble() # true → 1.0, false → 0.0
```

#### **Conversion Chaining**

```obq
# Chain multiple conversions
result: Text = "42".toInt().toDouble().toBool().toString() # "42" → 42 → 42.0 → true → "true"
```

#### **Error Handling**

```obq
# Invalid conversions throw runtime exceptions
try {
    invalid_int: Int = "not_a_number".toInt() # Throws conversion error
} catch (error) {
    io.print("Conversion failed: invalid input")
}
```

**Conversion Rules:**

- **Text → Numeric**: Parses standard number formats, throws error for invalid input
- **Numeric → Text**: Standard string representation with appropriate precision
- **Boolean → Numeric**: `true` → 1, `false` → 0
- **Numeric → Boolean**: Zero values → `false`, non-zero → `true`
- **Float Precision**: `toInt()` and `toLong()` truncate decimal parts
- **Range Checking**: Conversions validate target type ranges and throw errors for overflow

### **Keywords**

| Keyword                   | Purpose                            |
| ------------------------- | ---------------------------------- |
| `Object`                  | Define an object                   |
| `Enum`                    | Define an enumeration              |
| `Record`                  | Define a data structure            |
| `Protocol`                | Define an interface                |
| `method`                  | Define a method                    |
| `constructor`             | Define object constructor          |
| `property`                | Define object property             |
| `@external`               | Mark method as publicly accessible |
| `const`                   | Define constant value              |
| `this`                    | Reference to current object        |
| `new`                     | Create object instance             |
| `return`                  | Return value from method           |
| `import`                  | Import system modules              |
| `if`, `else if`, `else`   | Conditional logic with chaining    |
| `while`                   | Control flow loop                  |
| `throw`                   | Throw user-defined errors          |
| `try`, `catch`, `finally` | Structured exception handling      |
| `Result`, `Error`         | Error handling types               |
| `true`, `false`           | Boolean literals                   |

### **Operators**

| Operator   | Description                                      |
| ---------- | ------------------------------------------------ |
| `+`        | Addition/String concatenation                    |
| `-`        | Subtraction                                      |
| `*`        | Multiplication                                   |
| `/`        | Division                                         |
| `%`        | Modulo (remainder)                               |
| `==`       | Equality                                         |
| `!=`       | Inequality                                       |
| `<`, `>`   | Comparison (also used for generics: `List<Int>`) |
| `<=`, `>=` | Comparison                                       |
| `=`        | Assignment                                       |
| `.`        | Member access                                    |
| `:`        | Type annotation                                  |
| `[`, `]`   | List literal syntax                              |
| `{`, `}`   | Map literal syntax                               |
| `(`, `)`   | Set literal syntax                               |

### **Comments**

```obq
# Single line comment

###
Multi-line comment
Can span multiple lines
###
```

---

## 🧪 Testing

O²L includes a comprehensive test suite with **282 tests** covering all language features:

- **Lexical analysis** (27 tests) - Token parsing, keywords, literals, operators
- **Parser functionality** (29 tests) - AST generation, object declarations, method parsing
- **Runtime behavior** (44 tests) - Value types, contexts, collections, iterators
- **Protocol system** (20 tests) - Interface compliance, method dispatch, and comprehensive signature validation
- **Integration tests** (35 tests) - End-to-end execution, error handling, file processing
- **Text methods** (12 tests) - Complete coverage of all 48 string methods including length()
- **Math library** (12 tests) - Comprehensive testing of mathematical functions and constants
- **Regular expressions** (65 tests) - Complete regexp library functionality and pattern matching
- **URL library** (43 tests) - Comprehensive URL parsing, manipulation, and validation testing
- **Control flow** (14 tests) - else if syntax and Text.length() method testing
- **Type conversions** (33 tests) - Comprehensive type conversion methods with error handling and boundary value testing

Run tests:

```bash
cd build/tests
./o2l_tests
```

---

## 🎨 Example Programs

### **Mathematical Operations with Modulo**

```obq
import system.io
import system.utils

Object MathDemo {
    @external method demonstrateArithmetic(): Int {
        # Basic arithmetic with all operators
        a: Int = 17
        b: Int = 5

        io.print("a = %d, b = %d", a, b)
        io.print("a + b = %d", a + b)
        io.print("a - b = %d", a - b)
        io.print("a * b = %d", a * b)
        io.print("a / b = %d", a / b)
        io.print("a %% b = %d", a % b)  # Modulo operator

        # Mixed-type arithmetic with modulo
        float_num: Float = 12.5f
        int_num: Int = 3
        result: Float = float_num % int_num  # Float % Int = Float
        io.print("%.1f %% %d = %.1f", float_num, int_num, result)

        return 0
    }

    @external method findPrimeNumbers(): Int {
        # Use RepeatIterator and modulo to find prime numbers
        numbers: RepeatIterator = utils.repeat(20)
        primes: List<Int> = []

        while (numbers.hasNext()) {
            candidate: Int = numbers.next()

            if (candidate < 2) {
                # Skip 0 and 1
            } else {
                isPrime: Bool = this.checkPrime(candidate)
                if (isPrime) {
                    primes.add(candidate)
                }
            }
        }

        io.print("Prime numbers 0-19: %s", primes)
        return 0
    }

    method checkPrime(n: Int): Bool {
        if (n < 2) {
            return false
        }

        # Check divisibility using modulo
        checker: RepeatIterator = utils.repeat(n - 2)

        while (checker.hasNext()) {
            divisor: Int = checker.next() + 2  # Start from 2
            if (divisor * divisor > n) {
                return true  # No divisors found
            }
            if (n % divisor == 0) {
                return false  # Found a divisor
            }
        }

        return true
    }
}

Object Main {
    method main(): Int {
        demo: MathDemo = new MathDemo()
        demo.demonstrateArithmetic()
        io.print("")
        demo.findPrimeNumbers()
        return 0
    }
}
```

### **Simple Calculator**

```obq
import system.io

Object Calculator {
    @external method add(a: Int, b: Int): Int {
        return a + b
    }

    @external method multiply(a: Int, b: Int): Int {
        return a * b
    }
}

Object Main {
    method main(): Int {
        calc: Calculator = new Calculator()

        result1: Int = calc.add(5, 3)
        result2: Int = calc.multiply(4, 7)

        io.print("5 + 3 = %d", result1)
        io.print("4 * 7 = %d", result2)

        return 0
    }
}
```

### **List<T> Collection Management**

```obq
import system.io

Enum PersonalInfoAge {
    YOUNG = 1,
    MIDDLE = 2,
    OLD = 3
}

Record PersonalInfoDTO {
    personalName: Text
    PersonalAge: Text
    ageInfo: PersonalInfoAge
}

Object PersonManager {
    property people: List<PersonalInfoDTO>

    constructor() {
        # Initialize with sample data
        this.people = [
            PersonalInfoDTO(personalName="Alice", PersonalAge="30", ageInfo=PersonalInfoAge.YOUNG),
            PersonalInfoDTO(personalName="Bob", PersonalAge="25", ageInfo=PersonalInfoAge.OLD)
        ]
    }

    @external method addPerson(name: Text, age: Text, category: PersonalInfoAge): Void {
        newPerson: PersonalInfoDTO = PersonalInfoDTO(personalName=name, PersonalAge=age, ageInfo=category)
        this.people.add(newPerson)
        io.print("Added %s to the list. Total people: %d", name, this.people.size())
    }

    @external method removePerson(index: Int): Void {
        if (index >= 0 && index < this.people.size()) {
            removed: PersonalInfoDTO = this.people.get(index)
            this.people.remove(index)
            io.print("Removed %s from the list", removed.personalName)
        }
    }

    @external method showAllPeople(): Void {
        io.print("=== People Management System ===")
        io.print("Total people: %d", this.people.size())

        # Iterate through list
        i: Int = 0
        while (i < this.people.size()) {
            person: PersonalInfoDTO = this.people.get(i)
            io.print("%d. %s (Age: %s, Category: %o)",
                     i + 1, person.personalName, person.PersonalAge, person.ageInfo)
            i = i + 1
        }
    }

    @external method reverseOrder(): Void {
        this.people.reverse()
        io.print("Reversed the order of people")
    }
}

Object Main {
    method main(): Int {
        manager: PersonManager = new PersonManager()

        manager.showAllPeople()
        manager.addPerson("Charlie", "35", PersonalInfoAge.MIDDLE)
        manager.reverseOrder()
        manager.showAllPeople()

        return 0
    }
}
```

### **User Data Manager with Map<K,V>**

```obq
import system.io

Object UserDataManager {
    property user_scores: Map<Text, Int>
    property user_info: Map<Text, Text>

    constructor() {
        # Initialize with sample user data
        this.user_scores = {
            "alice": 1250,
            "bob": 980,
            "charlie": 1100,
            "diana": 1425
        }

        this.user_info = {
            "alice": "Senior Developer",
            "bob": "Junior Developer",
            "charlie": "QA Engineer",
            "diana": "Team Lead"
        }
    }

    @external method addUser(username: Text, score: Int, role: Text): Void {
        this.user_scores.put(username, score)
        this.user_info.put(username, role)
        io.print("Added user %s with score %d and role %s", username, score, role)
    }

    @external method updateScore(username: Text, newScore: Int): Bool {
        if (this.user_scores.contains(username)) {
            oldScore: Int = this.user_scores.get(username)
            this.user_scores.put(username, newScore)
            io.print("Updated %s score from %d to %d", username, oldScore, newScore)
            return true
        }
        io.print("User %s not found", username)
        return false
    }

    @external method getLeaderboard(): Void {
        io.print("=== User Leaderboard ===")

        # Iterate through scores and display with roles
        score_iter: MapIterator = this.user_scores.iterator()
        while (score_iter.hasNext()) {
            entry: MapObject = score_iter.MapItem()
            username: Text = entry.getKey()
            score: Int = entry.getVal()
            role: Text = this.user_info.get(username)

            io.print("%s (%s): %d points", username, role, score)
        }

        io.print("Total users: %d", this.user_scores.size())
    }

    @external method removeUser(username: Text): Bool {
        if (this.user_scores.contains(username)) {
            this.user_scores.remove(username)
            this.user_info.remove(username)
            io.print("Removed user %s", username)
            return true
        }
        io.print("User %s not found", username)
        return false
    }

    @external method findHighScorers(threshold: Int): Void {
        io.print("=== Users with scores above %d ===", threshold)
        high_scorers: List<Text> = []

        score_iter: MapIterator = this.user_scores.iterator()
        while (score_iter.hasNext()) {
            entry: MapObject = score_iter.MapItem()
            username: Text = entry.getKey()
            score: Int = entry.getVal()

            if (score > threshold) {
                high_scorers.add(username)
                role: Text = this.user_info.get(username)
                io.print("⭐ %s (%s): %d points", username, role, score)
            }
        }

        io.print("Found %d high scorers", high_scorers.size())
    }
}

Object Main {
    method main(): Int {
        manager: UserDataManager = new UserDataManager()

        manager.getLeaderboard()
        io.print("")

        manager.addUser("eve", 1350, "DevOps Engineer")
        manager.updateScore("bob", 1050)
        io.print("")

        manager.findHighScorers(1200)
        io.print("")

        manager.getLeaderboard()

        return 0
    }
}
```

### **Configuration Manager**

```obq
import system.io
import system.os

Enum LogLevel {
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4
}

Record AppConfig {
    name: Text
    port: Int
    logLevel: LogLevel
    enabled: Bool
}

Object ConfigManager {
    property config: AppConfig

    constructor() {
        this.config = AppConfig(
            name="MyApp",
            port=8080,
            logLevel=LogLevel.INFO,
            enabled=true
        )
    }

    @external method showConfig(): Void {
        io.print("=== Application Configuration ===")
        io.print("Name: %s", this.config.name)
        io.print("Port: %d", this.config.port)
        io.print("Log Level: %o", this.config.logLevel)
        io.print("Enabled: %o", this.config.enabled)

        env: Text = os.getEnv("USER")
        io.print("Running as user: %s", env)
    }

    @external method isProductionReady(): Bool {
        return this.config.enabled && this.config.port > 1000
    }
}

Object Main {
    method main(): Int {
        manager: ConfigManager = new ConfigManager()
        manager.showConfig()

        if (manager.isProductionReady()) {
            io.print("✅ Application is production ready!")
            return 0
        } else {
            io.print("❌ Application needs configuration")
            return 1
        }
    }
}
```

---

## 🌍 Philosophy in Practice

### **Why No If Statements?**

Traditional `if` statements lead to complex branching logic. O²L encourages object-based dispatch:

**Traditional approach:**

```java
if (user.getType() == UserType.ADMIN) {
    // admin logic
} else if (user.getType() == UserType.REGULAR) {
    // regular logic
} else {
    // guest logic
}
```

**O²L approach:**

```obq
Protocol UserHandler {
    method handleRequest(): Text
}

Object AdminHandler: UserHandler {
    @external method handleRequest(): Text {
        return "Admin access granted"
    }
}

Object RegularHandler: UserHandler {
    @external method handleRequest(): Text {
        return "Regular access granted"
    }
}
```

### **Why No Null?**

Null values are a major source of runtime errors. O²L eliminates them entirely:

```obq
# Instead of nullable values, use explicit optional types
# This forces developers to handle all cases explicitly

Object UserService {
    @external method findUser(id: Int): Optional<User> {
        # Return Optional.empty() if not found
        # Return Optional.of(user) if found
    }

    @external method processUser(id: Int): Text {
        userOpt: Optional<User> = this.findUser(id)
        if (userOpt.isPresent()) {
            user: User = userOpt.get()
            return "Processing: " + user.getName()
        }
        return "User not found"
    }
}
```

### **Why Immutability?**

Immutable objects eliminate entire categories of bugs:

- No concurrent modification exceptions
- No unexpected state changes
- Easier to reason about program behavior
- Natural thread safety
- Cleaner APIs with predictable behavior

---

## 🛣️ Roadmap

### **Current Version (0.0.1)**

- ✅ Full object-oriented language
- ✅ Enums, Records, Protocols
- ✅ Method visibility control
- ✅ System I/O and environment access
- ✅ **Generic Collections**: `List<T>`, `Map<K,V>`, and `Set<T>` with type safety, methods, and iterators
- ✅ **Command Line Arguments**: Full `system.os.args()` and `system.os.argc()` support following C/C++ conventions
- ✅ **Iterator Enhancements**: All iterators support `index()` method for current position tracking
- ✅ **128-bit Integer Support**: Long type with extended precision
- ✅ **Comprehensive Type System**: Int, Long, Float, Double with automatic promotion
- ✅ **While Loops**: Practical iteration with ListIterator and RepeatIterator support
- ✅ **Modulo Operator**: Full arithmetic completeness with `%` operator for all numeric types
- ✅ **System Utilities**: `system.utils` module with RepeatIterator for controlled repetition
- ✅ **Error Handling System**: Complete `throw`/`try`/`catch`/`finally` with `Result<T,E>` and `Error` types
- ✅ Comprehensive test suite with full coverage
- ✅ Debug mode and error reporting

### **Planned Features**

- 📦 **Module System**: Package management and namespaces
- ✅ **Enhanced FFI System**: Complete Foreign Function Interface with libffi integration, advanced type support, and SQLite integration
- 🧪 **Built-in Testing**: Integrated test framework
- ⚡ **Bytecode Compiler**: Improve performance
- 🌐 **WebAssembly**: Run O²L in browsers
- 📊 **Advanced Generic Types**: Advanced parameterized types and Optional<T>
- 🔄 **Async/Await**: Asynchronous programming model

---

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### **Development Setup**

```bash
git clone https://github.com/zombocoder/o2l.git
cd o2l
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

### **Running Tests**

```bash
# Run all 338 tests
cd build
make test

# Run specific test suites
cd build/tests
./o2l_tests --gtest_filter="TextMethodTest.*"      # Text method tests
./o2l_tests --gtest_filter="MathLibraryTest.*"     # Math library tests
./o2l_tests --gtest_filter="JsonLibraryTest.*"     # JSON library tests (16 tests)
./o2l_tests --gtest_filter="HttpClientLibraryTest.*" # HTTP client tests (24 tests)
./o2l_tests --gtest_filter="RuntimeTest.*"         # Runtime and collections tests
./o2l_tests --gtest_filter="IntegrationTest.*"     # End-to-end integration tests
./o2l_tests --gtest_filter="LexerTest.*"           # Lexer and tokenization tests
./o2l_tests --gtest_filter="ParserTest.*"          # Parser and AST tests
./o2l_tests --gtest_filter="RegexpLibraryTest.*"   # Regular expression tests
./o2l_tests --gtest_filter="UrlLibraryTest.*"      # URL library tests
./o2l_tests --gtest_filter="FFISimplifiedTest.*"     # FFI basic types and operations tests
./o2l_tests --gtest_filter="FFILibraryTest.*"        # FFI library functions and integrations tests
./o2l_tests --gtest_filter="ElseIfAndLengthTest.*" # else if and Text.length() tests

# Test coverage summary:
# ✅ 445 total tests - All passing (including 14 FFI tests)
# 📝 27 Lexer tests - Token parsing, keywords, operators
# 🌳 29 Parser tests - AST generation, syntax validation
# ⚡ 44 Runtime tests - Value types, collections, iterators
# 🔌 20 Protocol tests - Interface compliance and signature validation
# 🚀 35 Integration tests - End-to-end execution
# ✨ 12 Text method tests - All 48 string methods including length()
# 📐 12 Math library tests - All mathematical functions and constants
# 🔄 JSON Library - 16 comprehensive tests covering parsing, path navigation, validation, and native collection integration
# 🌐 HTTP Client - 24 extensive tests covering all HTTP methods, authentication, file operations, and error handling
# 🔄 65 Regular expression tests - Complete regexp library functionality
# 🌐 43 URL library tests - Complete URL manipulation and validation
# 🔀 14 Control flow tests - else if syntax and Text.length() method
# 🔄 33 Type conversion tests - Comprehensive conversion methods with error handling
# 📊 20 Extended system tests - DateTime, filesystem, and OS integration
```

### **Code Style**

- Follow modern C++23 practices
- Use clear, descriptive names
- Include comprehensive tests for new features
- Document public APIs

---

## 📄 License

O²L is released under the [Apache License 2.0](LICENSE).

---

## 🙏 Acknowledgments

O²L draws inspiration from:

- **Smalltalk**: Pure object-oriented design
- **Java**: Familiar syntax and structure
- **Rust**: Memory safety and immutability
- **Haskell**: Functional programming concepts
- **Kotlin**: Modern language design

---

## 📞 Support & Community

- 🐛 **Issues**: [GitHub Issues](https://github.com/zombocoder/o2l/issues)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/zombocoder/o2l/discussions)
- 📖 **Documentation**: [Wiki](https://github.com/zombocoder/o2l/wiki)
- 📧 **Contact**: [o2l@zombocoder.com](mailto:o2l@zombocoder.com)

---

_"In O²L, every value is an object, every action is a method call, and every program is a symphony of collaborating objects."_ 🎼
