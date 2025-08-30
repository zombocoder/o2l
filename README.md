# 🌟 O²L Programming Language

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/zombocoder/o2l) [![License: Apache 2.0](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![C++ Version](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp/23) [![Tests](https://img.shields.io/badge/tests-445%20passing-brightgreen.svg)](https://github.com/zombocoder/o2l) [![FFI System](https://img.shields.io/badge/FFI-SQLite%20integrated-brightgreen.svg)](https://github.com/zombocoder/o2l) [![Text Methods](https://img.shields.io/badge/text%20methods-48-blue.svg)](https://github.com/zombocoder/o2l) [![HTTP Client](https://img.shields.io/badge/http%20methods-30+-brightgreen.svg)](https://github.com/zombocoder/o2l) [![JSON Library](https://img.shields.io/badge/json%20methods-30+-brightgreen.svg)](https://github.com/zombocoder/o2l) [![URL Library](https://img.shields.io/badge/url%20methods-26-blue.svg)](https://github.com/zombocoder/o2l) [![Regexp Library](https://img.shields.io/badge/regexp%20methods-12-blue.svg)](https://github.com/zombocoder/o2l) [![Math Library](https://img.shields.io/badge/math%20functions-40+-brightgreen.svg)](https://github.com/zombocoder/o2l) [![DateTime Library](https://img.shields.io/badge/datetime%20functions-65+-blue.svg)](https://github.com/zombocoder/o2l)

**O²L** is a modern object-oriented programming language that balances pure object-oriented design with practical programming constructs. Built with C++23, O²L eliminates primitives and null values while providing essential control flow like while loops, comprehensive arithmetic operations, and extensive string manipulation capabilities for real-world programming needs.

📚 **[Complete Documentation](https://docs.o2l.dev/)** | 🚀 **[Getting Started Guide](https://docs.o2l.dev/#/getting-started)** | 📖 **[Language Reference](https://docs.o2l.dev/#/language-reference)**

### 🆕 **Latest Features**

- **🔗 Enhanced Foreign Function Interface (FFI)**: Complete FFI system with advanced type support, SQLite integration, and comprehensive C library interoperability
- **🌐 HTTP Client Library**: Complete `http.client` library with 30+ methods, multipart file upload, platform-specific implementations (Windows/Linux/macOS), and cross-platform libcurl fallback
- **🔄 JSON Library**: Revolutionary `json` library with 30+ methods including auto-detection parsing, fixed path navigation bug, and seamless native Map/List integration
- **🌐 URL Library**: Complete `url` library with 26 methods for URL parsing, construction, manipulation, and validation
- **🔄 Regular Expressions**: Complete `regexp` library with 12 methods for pattern matching, text searching, and string manipulation
- **🔀 Control Flow Enhancement**: `else if` syntax support for complex conditional logic with proper AST chaining
- **🛑 Break Statements**: `break` statement for early exit from while loops, enabling search patterns and safety limits
- **⏭️ Continue Statements**: `continue` statement for skipping iterations in while loops, perfect for filtering and validation
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
- ✅ **Break Statements**: Early exit from while loops for search patterns and safety limits
- ✅ **Continue Statements**: Skip iterations in while loops for filtering and validation patterns
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
