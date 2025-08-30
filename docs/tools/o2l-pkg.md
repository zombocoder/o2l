# o2l-pkg - Package Manager

The `o2l-pkg` tool is O²L's package manager for creating projects, managing dependencies, and organizing code structure.

## Commands

### Initialize New Project

Create a new O²L project with standard structure:

```bash
o2l-pkg init
```

This prompts for:

- Package name (defaults to directory name)
- Version (defaults to "0.1.0")
- Description
- Author name

**Creates:**

- `o2l.toml` - Project configuration
- `src/` directory structure with example code
- `src/main.obq` - Main application entry point
- `src/calc/Calculator.obq` - Example calculator module
- `src/greeters/BasicGreeter.obq` - Example greeter module
- `src/tests/` - Test suite structure
- `.gitignore` - Git ignore file
- `.o2l/` - Package manager directories

### Add Dependencies

Add library dependencies to your project:

```bash
# Simple library
o2l-pkg add collections

# Namespaced library with version
o2l-pkg add com.example.math 1.2.0
```

Libraries are installed to `.o2l/lib/` and can be imported:

```obq
import collections
import com.example.math
```

### Create Objects

Generate new objects with proper directory structure:

```bash
o2l-pkg create com.mycompany.utils Utils
```

Creates:

- `src/com/mycompany/utils/Utils.obq`
- Proper namespace organization
- Template object with basic methods

### List Dependencies

Show installed libraries:

```bash
o2l-pkg list
```

### Clean Cache

Clear the package cache:

```bash
o2l-pkg clean
```

## Project Structure

After `o2l-pkg init`, your project has this structure:

```
my-project/
├── o2l.toml                          # Project configuration
├── .gitignore                        # Git ignore rules
├── .o2l/                            # Package manager
│   ├── lib/                         # Installed libraries
│   └── cache/                       # Download cache
└── src/                             # Source code
    ├── main.obq                     # Main entry point
    ├── calc/
    │   └── Calculator.obq           # Calculator module
    ├── greeters/
    │   └── BasicGreeter.obq         # Greeter module
    └── tests/                       # Test suite
        ├── main_test.obq            # Test runner
        ├── calc/
        │   └── CalculatorTest.obq   # Calculator tests
        └── greeters/
            └── GreeterTest.obq      # Greeter tests
```

## Configuration File (o2l.toml)

The `o2l.toml` file configures your project:

```toml
[package]
name = "my-project"
version = "0.1.0"
description = "An O²L project"
authors = ["Your Name"]
entrypoint = "src/main.obq"

[dependencies]
collections = "latest"
com.example.math = "1.2.0"
```

## Running Your Project

Use the O²L interpreter to run your project:

```bash
# Run main application
o2l run

# Run specific file
o2l run src/main.obq

# Run tests
o2l run src/tests/main_test.obq

# Run specific test suite
o2l run src/tests/calc/CalculatorTest.obq
```

## Example Usage

### 1. Create New Project

```bash
mkdir my-o2l-app
cd my-o2l-app
o2l-pkg init
```

### 2. Add Dependencies

```bash
o2l-pkg add http-client
o2l-pkg add json
```

### 3. Create Custom Objects

```bash
o2l-pkg create com.myapp.models User
o2l-pkg create com.myapp.services ApiClient
```

### 4. Run Your Application

```bash
o2l run
```

## Generated Templates

### Main Application Template

```obq
# my-project - Main Application
import system.io
@import src.calc.Calculator
@import src.greeters.BasicGreeter

Object Main {
    method main(): Int {
        greeter: BasicGreeter = new BasicGreeter()
        io.print(greeter.welcome())

        # Test the Calculator
        calc: Calculator = new Calculator()
        sum: Int = calc.add(5, 3)
        io.print("5 + 3 = %d", sum)

        return 0
    }
}
```

### Calculator Module Template

```obq
# Calculator module - Basic arithmetic operations
import system.io

Object Calculator {
    @external method add(a: Int, b: Int): Int {
        return a + b
    }

    @external method multiply(a: Int, b: Int): Int {
        return a * b
    }

    @external method divide(a: Int, b: Int): Int {
        if (b == 0) {
            io.print("Error: Division by zero!")
            return 0
        }
        return a / b
    }
}
```

## Best Practices

- Always run `o2l-pkg init` in a new directory
- Use namespaced names for reusable libraries (`com.company.module`)
- Organize code in logical packages under `src/`
- Write tests for all modules in `src/tests/`
- Use the `@import` syntax for local modules
- Use regular `import` for external dependencies
