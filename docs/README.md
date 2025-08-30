# O²L Programming Language Documentation

Welcome to the comprehensive documentation for the O²L programming language. This documentation covers all core types, system libraries, and built-in functionality.

## Table of Contents

### Core Language

- **[Language Overview](language/overview.md)** - Introduction to O²L syntax and concepts
- **[Core Types](api-reference/core/README.md)** - Built-in data types and their methods
- **[Operators](api-reference/core/Operators.md)** - All operator types and precedence

### Core Types Reference

- **[Text](api-reference/core/Text.md)** - String manipulation and text processing (48+ methods)
- **[List](api-reference/core/List.md)** - Dynamic arrays and list operations
- **[Map](api-reference/core/Map.md)** - Key-value data structures
- **[Set](api-reference/core/Set.md)** - Unique value collections
- **[Numeric Types](api-reference/core/Numeric.md)** - Int, Long, Float, Double
- **[Bool](api-reference/core/Bool.md)** - Boolean type and operations

### System Libraries

#### I/O and File System
- **[system.io](api-reference/system/io.md)** - Input/output operations
- **[system.fs](api-reference/system/fs.md)** - File system operations
- **[system.os](api-reference/system/os.md)** - Operating system interaction

#### Utility Libraries
- **[Math](api-reference/libraries/math.md)** - Mathematical functions and constants (47 functions)
- **[DateTime](api-reference/libraries/datetime.md)** - Date and time manipulation
- **[JSON](api-reference/libraries/json.md)** - JSON parsing and generation (30 functions)

#### Network Libraries
- **[HTTP Client](api-reference/libraries/http-client.md)** - HTTP client for making requests
- **[HTTP Server](api-reference/libraries/http-server.md)** - HTTP server for building web applications

## Quick Reference

### Common Use Cases

| Task | Library | Key Functions |
|------|---------|---------------|
| String manipulation | [Text](api-reference/core/Text.md) | `substring()`, `split()`, `replace()`, `format()` |
| Working with arrays | [List](api-reference/core/List.md) | `add()`, `get()`, `size()`, `filter()`, `map()` |
| File operations | [system.fs](api-reference/system/fs.md) | `readFile()`, `writeFile()`, `exists()`, `createDir()` |
| HTTP requests | [HTTP Client](api-reference/libraries/http-client.md) | `get()`, `post()`, `put()`, `delete()` |
| Web servers | [HTTP Server](api-reference/libraries/http-server.md) | `create()`, `listen()`, `get()`, `post()` |
| JSON processing | [JSON](api-reference/libraries/json.md) | `parse()`, `stringify()`, `get()`, `set()` |
| Math calculations | [Math](api-reference/libraries/math.md) | `pow()`, `sqrt()`, `sin()`, `cos()`, `abs()` |
| Date/time | [DateTime](api-reference/libraries/datetime.md) | `now()`, `create()`, `format()`, `addDays()` |

### Type Conversions

| From | To | Method |
|------|----|---------| 
| Text | Int | `Text.toInt()` |
| Text | Float | `Text.toFloat()` |
| Text | List | `Text.split()` |
| List | Text | `List.join()` |
| Any | Text | `toString()` |
| Text | JSON | `json.parse()` |
| Object | JSON | `json.stringify()` |

### Common Patterns

#### Error Handling
```obq
try {
    result: Text = fs.readFile("/path/to/file.txt")
    io.print("File contents: %s", result)
} catch error {
    io.print("Error reading file: %s", error.getMessage())
}
```

#### HTTP API Client
```obq
import http
import json

response: HttpResponse = http.get("https://api.example.com/users")
if http.isSuccess(response) {
    users_json: Text = http.getBody(response)
    users: List = json.parseToList(users_json)
    io.print("Found %d users", users.size())
}
```

#### Simple Web Server
```obq
import http.server

server: HttpServerInstance = http.server.create()
http.server.setPort(server, 3000)
http.server.get(server, "/hello", "greeting_handler")
http.server.listen(server)
```

## API Documentation Structure

Each API reference page includes:

- **Overview** - Purpose and key features
- **Import Statement** - How to include the library
- **Method Reference** - Complete function signatures and descriptions
- **Examples** - Practical usage examples
- **Error Handling** - Common error scenarios
- **Performance Notes** - Optimization tips
- **Cross-references** - Related functions and types

## Getting Started

1. **New to O²L?** Start with the [Language Overview](language/overview.md)
2. **Need specific functionality?** Use the Quick Reference table above
3. **Building a web application?** Check out the [HTTP Server](api-reference/libraries/http-server.md) guide
4. **Working with data?** Explore the [JSON](api-reference/libraries/json.md) and [List](api-reference/core/List.md) documentation
5. **File processing?** See the [system.fs](api-reference/system/fs.md) reference

## Documentation Conventions

- **Required parameters** are shown without brackets: `param: Type`
- **Optional parameters** are shown with brackets: `[param: Type]`
- **Return types** are indicated with `-> Type`
- **Code examples** use the `.obq` file extension
- **Cross-references** are linked between related functions
- **Version compatibility** is noted where applicable

---

*This documentation covers O²L version 1.0. For the latest updates, visit the [O²L GitHub repository](https://github.com/zombocoder/o2l).*