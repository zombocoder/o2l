# Contributing to O²L

Thank you for your interest in contributing to the O²L programming language! This document provides guidelines for contributing to the project.

## 🚀 Getting Started

### Prerequisites

- **C++23 compatible compiler** (GCC 12+, Clang 15+, or MSVC 2022+)
- **CMake 3.20+**
- **Google Test** (for running tests)
- **Git**

### Setting Up Development Environment

1. **Fork and clone the repository:**
   ```bash
   git clone https://github.com/your-username/o2l.git
   cd o2l
   ```

2. **Create a development build:**
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   make
   ```

3. **Run tests to ensure everything works:**
   ```bash
   cd tests
   ./o2l_tests
   ```

4. **Test the interpreter:**
   ```bash
   cd ..
   ./o2l run ../examples/minimal_test.obq
   ```

## 🎯 How to Contribute

### Types of Contributions

- **Bug fixes**: Fix issues in the interpreter, parser, or runtime
- **Feature development**: Add new language features or system libraries
- **Documentation**: Improve README, add examples, write tutorials
- **Testing**: Add test cases, improve test coverage
- **Performance**: Optimize parsing, runtime, or memory usage
- **Tooling**: Improve build system, CI/CD, or development tools

### Development Workflow

1. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following the coding standards below

3. **Write or update tests** for your changes

4. **Run the full test suite:**
   ```bash
   cd build/tests
   ./o2l_tests
   ```

5. **Test with example programs:**
   ```bash
   cd build
   ./o2l run ../examples/test_final_demo.obq
   ```

6. **Format your code:**
   ```bash
   clang-format -i src/**/*.cpp src/**/*.hpp
   ```

7. **Commit your changes:**
   ```bash
   git add .
   git commit -m "feat: add support for new feature"
   ```

8. **Push and create a pull request:**
   ```bash
   git push origin feature/your-feature-name
   ```

## 📝 Coding Standards

### C++ Code Style

- **Follow the `.clang-format` configuration** provided in the repository
- **Use modern C++23 features** where appropriate
- **Prefer smart pointers** (`std::unique_ptr`, `std::shared_ptr`) over raw pointers
- **Use meaningful variable and function names**
- **Add comments for complex logic**, but let code be self-documenting
- **Follow RAII principles**

### Example:
```cpp
// Good
auto parseExpression() -> std::unique_ptr<ASTNode> {
    auto node = std::make_unique<ExpressionNode>();
    // ... implementation
    return node;
}

// Avoid
ASTNode* parseExpression() {
    ASTNode* node = new ExpressionNode();
    // ... implementation
    return node;  // Memory leak potential
}
```

### O²L Code Style

- **Use clear, descriptive names** for objects and methods
- **Follow the existing patterns** in example files
- **Use proper capitalization**: `Object`, `Enum`, `Record`, `Protocol`
- **Mark public methods** with `@external`
- **Include system imports** at the top of files

### Example:
```o2l
import system.io

Object Calculator {
    property value: Int
    
    constructor(initialValue: Int) {
        this.value = initialValue
    }
    
    @external method add(amount: Int): Int {
        return this.value + amount
    }
    
    method internalValidation(): Bool {
        return this.value >= 0
    }
}
```

## 🧪 Testing Guidelines

### Writing Tests

- **Every new feature** should include comprehensive tests
- **Bug fixes** should include regression tests
- **Use descriptive test names** that explain what is being tested

### Test Categories

1. **Unit Tests** (`test_*.cpp`):
   - Test individual components (lexer, parser, runtime)
   - Fast execution, no I/O dependencies
   - Use Google Test framework

2. **Integration Tests** (`test_integration.cpp`):
   - Test complete program execution
   - Use actual `.obq` example files
   - Verify expected behavior and error handling

3. **Example Programs** (`examples/*.obq`):
   - Real-world usage examples
   - Should compile and run successfully
   - Used in integration testing

### Test Structure

```cpp
// Unit test example
TEST_F(ParserTest, ParseObjectDeclaration) {
    auto nodes = parse(R"(
        Object TestObject {
            @external method test(): Int {
                return 42
            }
        }
    )");
    
    ASSERT_EQ(nodes.size(), 1);
    auto object_node = dynamic_cast<ObjectNode*>(nodes[0].get());
    ASSERT_NE(object_node, nullptr);
    EXPECT_EQ(object_node->getName(), "TestObject");
}
```

## 🐛 Reporting Issues

### Bug Reports

When reporting bugs, please include:

- **O²L version** (`./o2l --version`)
- **Operating system** and compiler version
- **Minimal reproducible example**
- **Expected vs actual behavior**
- **Error messages** and stack traces

### Example Bug Report

```markdown
**Bug Description:**
Parser fails when parsing nested record access

**Environment:**
- O²L version: 0.0.1
- OS: Ubuntu 22.04
- Compiler: GCC 12.2

**Minimal Example:**
```o2l
Record Point { x: Int, y: Int }
Record Line { start: Point, end: Point }

Object Main {
    method main(): Int {
        line: Line = Line(start=Point(x=0, y=0), end=Point(x=10, y=10))
        return line.start.x  # This fails
    }
}
```

**Expected:** Should return 0
**Actual:** Syntax error: "Unexpected token '.'"
```

### Feature Requests

- **Describe the use case** and motivation
- **Provide examples** of how the feature would be used
- **Consider alternatives** and explain why they're insufficient
- **Check existing issues** to avoid duplicates

## 🏗️ Architecture Overview

### Project Structure

```
/src/
  ├── main.cpp              # CLI entry point
  ├── Lexer.{cpp,hpp}       # Tokenization
  ├── Parser.{cpp,hpp}      # AST generation
  ├── Interpreter.{cpp,hpp} # Program execution
  ├── AST/                  # AST node implementations
  │   ├── Node.hpp          # Base AST node
  │   ├── ObjectNode.{cpp,hpp}
  │   ├── MethodCallNode.{cpp,hpp}
  │   └── ...
  └── Runtime/              # Runtime system
      ├── Value.{cpp,hpp}   # Value types
      ├── Context.{cpp,hpp} # Execution context
      ├── ObjectInstance.{cpp,hpp}
      └── ...

/tests/
  ├── test_lexer.cpp        # Lexer unit tests
  ├── test_parser.cpp       # Parser unit tests
  ├── test_runtime.cpp      # Runtime unit tests
  └── test_integration.cpp  # Integration tests

/examples/
  ├── minimal_test.obq      # Basic functionality
  ├── test_final_demo.obq   # Advanced features
  └── ...
```

### Key Components

- **Lexer**: Converts source code into tokens
- **Parser**: Builds Abstract Syntax Tree (AST) from tokens
- **AST Nodes**: Represent language constructs (objects, methods, etc.)
- **Runtime**: Executes AST with context management
- **Value System**: Handles O²L's object-oriented values

## 🎨 Adding New Features

### Language Features

When adding new language features:

1. **Update the lexer** to recognize new keywords/syntax
2. **Add parser support** for the new construct
3. **Create AST nodes** to represent the feature
4. **Implement runtime behavior** in the interpreter
5. **Add comprehensive tests**
6. **Update documentation** and examples

### Example: Adding a new keyword

```cpp
// 1. Lexer.cpp - Add token type
enum class TokenType {
    // ... existing tokens
    ASYNC,  // New token
};

// 2. Parser.cpp - Handle new syntax
if (token.type == TokenType::ASYNC) {
    return parseAsyncMethod();
}

// 3. Create AsyncMethodNode.{cpp,hpp}
class AsyncMethodNode : public ASTNode {
    // ... implementation
};

// 4. Add runtime support in Interpreter.cpp
// 5. Add tests in test_parser.cpp and test_integration.cpp
```

## 🚀 Release Process

### Version Numbering

We follow [Semantic Versioning](https://semver.org/):
- **MAJOR.MINOR.PATCH** (e.g., 0.0.1)
- **Major**: Breaking changes to language syntax or semantics
- **Minor**: New features, backward compatible
- **Patch**: Bug fixes, no new features

### Release Checklist

- [ ] All tests pass
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] Version numbers updated
- [ ] Create release tag
- [ ] GitHub Actions builds successfully
- [ ] Release artifacts generated

## 📞 Getting Help

- **GitHub Discussions**: For general questions and ideas
- **GitHub Issues**: For bug reports and feature requests
- **Code Review**: All pull requests are reviewed by maintainers
- **Community**: Join our community discussions

## 🏆 Recognition

Contributors are recognized in:
- **CONTRIBUTORS.md** file
- **Release notes** for major contributions
- **Git commit history**

## 📄 License

By contributing to O²L, you agree that your contributions will be licensed under the [Apache License 2.0](LICENSE).

Thank you for contributing to O²L! 🎉