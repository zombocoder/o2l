# Changelog

All notable changes to the O²L programming language will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [2024-12-XX] - Variable Mutability & Enhanced Language Features

### Added

#### Variable Mutability System
- **Method-scope variables are now mutable** - Variables declared within methods can be reassigned using `variable = new_value` syntax
- **Object properties remain immutable** - Properties accessed via `this.property` cannot be reassigned after initialization
- **New AST node**: `VariableAssignmentNode` for handling variable reassignment
- **Enhanced Context class**: `reassignVariable()` method with proper validation and scope management

#### Logical Operators
- **Complete logical operator support**: `&&` (AND), `||` (OR), `!` (NOT) with proper C-style syntax
- **Proper precedence**: `!` > `&&` > `||` with left-associativity for same-precedence operators
- **Short-circuit evaluation**: `&&` and `||` operators skip right-side evaluation when result is determined
- **New AST nodes**: `LogicalNode` for binary logical operations, `UnaryNode` for NOT operations
- **Comprehensive precedence handling**: Integration with existing expression parsing hierarchy

#### Filesystem Operations (system.fs)
- **Complete filesystem module** with comprehensive file and directory operations
- **File operations**: `readText()`, `writeText()`, `exists()`, `isFile()`, `deleteFile()`
- **Directory operations**: `createDirectory()`, `isDirectory()`, `listFiles()`
- **Cross-platform implementation**: Works on Windows, macOS, and Linux
- **Proper error handling**: Boolean return values and exceptions for invalid operations

#### Expression Enhancements
- **Parentheses in expressions**: Full support for `(expression)` grouping to override precedence
- **Complex nested expressions**: Unlimited nesting depth with proper parsing
- **Enhanced parseAtomicExpression**: Support for unary operators and parenthesized expressions

### Changed
- **Parser statement recognition**: Added variable assignment detection (`IDENTIFIER ASSIGN` pattern)
- **Expression parsing hierarchy**: Enhanced with `parseLogicalExpression()` method
- **Module system**: Extended to support `system.fs` module registration
- **Context scope management**: Enhanced variable tracking for mutability support

### Technical Implementation
- **136 total tests passing**: Comprehensive test coverage for all new features
- **Parser tests**: Variable assignment parsing in all contexts and control structures
- **Runtime tests**: Context mutability behavior, scoping rules, and error handling
- **Integration tests**: End-to-end variable mutability with logical operators and filesystem operations
- **Performance optimized**: Single-pass parsing with efficient precedence handling

### Documentation
- **Updated CLAUDE.md**: Comprehensive examples of new variable mutability system
- **Updated README.md**: Featured new capabilities with practical code examples
- **New LANGUAGE_REFERENCE.md**: Complete API documentation for all new features
- **Updated examples/README.md**: Highlighted new feature examples and usage patterns

### Examples Added
- **`test_mutability_rules.obq`**: Comprehensive demonstration of variable mutability vs property immutability
- **`test_logical_comprehensive.obq`**: Complete logical operator testing with precedence and De Morgan's laws
- **`test_filesystem_simplified.obq`**: Practical filesystem operations with logical conditions
- **`test_not_edge_cases.obq`**: Edge cases for NOT operator including involution properties
- **`test_integration_advanced.obq`**: Advanced integration of logical operators with filesystem operations

- GitHub Actions CI/CD pipeline for automated building and testing
- Comprehensive code quality checks with clang-format, clang-tidy, and cppcheck
- Performance testing in CI pipeline
- Multi-platform build support (Ubuntu, macOS, Windows)
- Automatic release generation for tagged versions

### Changed

- Updated README.md with comprehensive philosophy and usage guide
- Enhanced CONTRIBUTING.md with detailed development guidelines

## [0.0.1] - 2025-09-20

### Added

- **Core Language Features**

  - Full object-oriented programming model
  - Immutable objects with property and method declarations
  - Constructor support with parameter passing
  - Method visibility control (`@external` for public methods)
  - `this` keyword support for object self-reference

- **Type System**

  - Built-in types: `Int`, `Float`, `Text`, `Bool`, `Char`, `Void`
  - `Enum` declarations with explicit values
  - `Record` declarations for data structures
  - `Protocol` declarations for interfaces
  - Object inheritance and protocol compliance

- **Language Constructs**

  - Object instantiation with `new` keyword
  - Property access and assignment (`this.property = value`)
  - Method calls including internal method calls (`this.method()`)
  - Constant declarations with `const` keyword
  - Variable declarations with type annotations
  - Conditional logic with `if`/`else` statements
  - Import system for system libraries

- **System Libraries**

  - `system.io` for input/output operations with format strings
  - `system.os` for environment variable access
  - Support for formatted printing (`%s`, `%d`, `%f`, `%o`)

- **Developer Tools**

  - Command-line interpreter with `run`, `repl`, `--help`, `--version`
  - Debug mode (`--debug`) with detailed execution traces
  - Comprehensive error messages with stack traces
  - Support for single-line (`#`) and multi-line (`###`) comments

- **Parser and Runtime**
  - Recursive descent parser generating Abstract Syntax Tree (AST)
  - Tree-walking interpreter with context management
  - Support for complex expressions and member access chains
  - Proper handling of method call visibility (internal vs external access)
  - Memory-safe implementation using modern C++23 features

### Testing

- **Comprehensive Test Suite (69 tests total)**

  - Lexer tests (24 tests): Complete tokenization coverage
  - Parser tests (16 tests): AST generation for all language constructs
  - Runtime tests (10 tests): Value system and execution behavior
  - Protocol tests (1 test): Interface compliance validation
  - Integration tests (18 tests): End-to-end program execution

- **Example Programs**
  - Basic object-oriented examples
  - Advanced feature demonstrations
  - Real-world usage patterns
  - Error handling examples

### Technical Achievements

- **Language Design**

  - Elimination of null values and primitives
  - Pure object-oriented approach without procedural constructs
  - Immutability by design for thread safety and predictability
  - Behavior-driven architecture encouraging clean code patterns

- **Implementation Quality**
  - Modern C++23 codebase with smart pointers and RAII
  - Exception-safe error handling with custom exception types
  - Efficient parsing with proper error recovery
  - Clean separation between parsing, AST representation, and execution

### Bug Fixes

- Fixed `this.method()` call parsing that was causing syntax errors
- Resolved issues with property assignment statements in method bodies
- Corrected method visibility checks for internal vs external access
- Fixed member access parsing for chained method calls
- Resolved tokenization issues with negative numbers and whitespace

### Documentation

- Comprehensive README.md with philosophy and practical examples
- Complete language reference with all keywords and operators
- Installation and usage instructions
- Development setup and contribution guidelines
- Example programs demonstrating all major features

---

## Version History Summary

- **v0.0.1**: Initial release with full object-oriented language implementation
- **v0.x.x**: Development versions (not released)

## Migration Guide

### From Development Version to 0.0.1

- Ensure all `enum` and `record` keywords are capitalized (`Enum`, `Record`)
- Mark public methods with `@external` annotation
- Use proper `import` statements for system libraries
- Follow new comment syntax (`#` for single line, `###` for multi-line)

## Future Roadmap

See README.md for planned features including:

- Module system with package management
- FFI bindings for C++/Java integration
- Built-in testing framework
- Bytecode compiler for improved performance
- WebAssembly backend for browser execution
- Generic types and async/await support
