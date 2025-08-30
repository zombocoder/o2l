# FFI (Foreign Function Interface)

The FFI library enables O²L programs to call functions from native shared libraries (`.so`, `.dylib`, `.dll`), providing interoperability with C/C++ code and system libraries.

## Quick Start

```obq
import ffi
import system.io

Object Main {
    method main(): Int {
        # Load a shared library
        libResult: Result<Value, Error> = ffi.load("/path/to/library.so")
        if (libResult.isError()) {
            io.print("Failed to load library")
            return 1
        }
        
        lib: Value = libResult.getResult()
        
        # Get a function symbol with signature
        fnResult: Result<Value, Error> = lib.symbol("my_function", "i32,i32->i32")
        if (fnResult.isError()) {
            io.print("Failed to get function symbol")
            return 1
        }
        
        fn: Value = fnResult.getResult()
        
        # Call the function
        result: Result<Value, Error> = fn.call(10, 20)
        if (result.isSuccess()) {
            value: Value = result.getResult()
            io.print("Result: %s", value.toString())
        }
        
        return 0
    }
}
```

## Core Functions

### ffi.load(path: Text) -> Result<Library, Error>

Loads a shared library from the specified path.

**Parameters:**
- `path`: Path to the shared library (`.so`, `.dylib`, `.dll`)

**Returns:**
- `Result<Library, Error>`: Success with Library object or Error on failure

**Example:**
```obq
libResult: Result<Value, Error> = ffi.load("/usr/lib/libm.so")
if (libResult.isSuccess()) {
    lib: Value = libResult.getResult()
    # Use library...
}
```

### ffi.getLastErrno() -> Int

Returns the last system error number from FFI operations.

**Returns:**
- `Int`: The errno value (0 indicates no error)

### ffi.ptr(address: Int) -> Value

Creates a pointer from an integer address.

**Parameters:**
- `address`: Memory address as integer

**Returns:**
- `Value`: Pointer object

### ffi.nullPtr() -> Value

Creates a null pointer.

**Returns:**
- `Value`: Null pointer object

## Library Object Methods

### lib.symbol(name: Text, signature: Text) -> Result<NativeFn, Error>

Gets a function symbol from the loaded library.

**Parameters:**
- `name`: Function name in the library
- `signature`: Function signature (see [Type Signatures](#type-signatures))

**Returns:**
- `Result<NativeFn, Error>`: Success with NativeFn object or Error on failure

**Example:**
```obq
# Get a function that takes two integers and returns an integer
fnResult: Result<Value, Error> = lib.symbol("add_numbers", "i32,i32->i32")
```

### lib.close() -> Void

Closes the library and releases resources.

## NativeFn Object Methods

### fn.call(...args) -> Result<Value, Error>

Calls the native function with the provided arguments.

**Parameters:**
- `...args`: Variable arguments matching the function signature

**Returns:**
- `Result<Value, Error>`: Success with return value or Error on failure

**Example:**
```obq
result: Result<Value, Error> = fn.call(10, 20)
if (result.isSuccess()) {
    value: Value = result.getResult()
    io.print("Result: %s", value.toString())
}
```

## Type Signatures

FFI function signatures specify parameter and return types using this format:
```
param1,param2,param3->return_type
```

### Supported Types

| O²L Type | FFI Signature | C Type | Description |
|----------|---------------|---------|-------------|
| `Int` | `i32` | `int32_t` | 32-bit signed integer |
| `Long` | `i64` | `int64_t` | 64-bit signed integer |
| `Float` | `f32` | `float` | 32-bit floating point |
| `Double` | `f64` | `double` | 64-bit floating point |
| `Bool` | `i32` | `int` | Boolean (0=false, 1=true) |
| `Text` | `ptr` | `char*` | C string pointer |
| `Void` | `void` | `void` | No return value |

### Signature Examples

```obq
# No parameters, returns integer
"->i32"

# Two integers, returns integer  
"i32,i32->i32"

# Float and double, returns double
"f32,f64->f64"

# Takes integer, returns nothing
"i32->void"

# Complex signature with multiple types
"i32,f64,ptr->ptr"
```

## Advanced Features

### Pointer Operations

```obq
# Create pointers
nullPtr: Value = ffi.nullPtr()
ptr: Value = ffi.ptr(0x12345678)

# Convert pointers to values
intVal: Int = ffi.ptrToInt(ptr)
doubleVal: Double = ffi.ptrToDouble(ptr)
floatVal: Float = ffi.ptrToFloat(ptr)
boolVal: Bool = ffi.ptrToBool(ptr)
strVal: Text = ffi.ptrToString(ptr)
```

### C String Handling

```obq
# Convert O²L Text to C string pointer
cstr: Value = ffi.cstring("Hello, World!")

# Convert C string pointer back to O²L Text
text: Text = ffi.ptrToString(cstr)
```

### Working with libm (Math Library)

```obq
import ffi
import system.io

Object MathExample {
    method main(): Int {
        # Load system math library
        libResult: Result<Value, Error> = ffi.load("/usr/lib/libm.so")
        if (libResult.isError()) {
            return 1
        }
        
        lib: Value = libResult.getResult()
        
        # Get sin function
        sinResult: Result<Value, Error> = lib.symbol("sin", "f64->f64")
        if (sinResult.isSuccess()) {
            sinFn: Value = sinResult.getResult()
            result: Result<Value, Error> = sinFn.call(3.14159)
            if (result.isSuccess()) {
                value: Value = result.getResult()
                io.print("sin(π) = %s", value.toString())
            }
        }
        
        return 0
    }
}
```

## Security and Safety

### Library Path Restrictions

FFI includes security policies to restrict which libraries can be loaded:

```obq
# Set allowed library paths (requires administrator privileges)
ffi.setPolicy("allowed_paths", ["/usr/lib/", "/opt/myapp/lib/"])
```

### Symbol Restrictions

Certain dangerous symbols may be blocked by default for security.

### Memory Safety

- **Pointer validation**: FFI validates pointer operations where possible
- **Type checking**: Function signatures are enforced at runtime
- **Error handling**: All FFI operations return Result types for proper error handling

## Error Handling

All FFI operations return `Result<Value, Error>` types. Always check for errors:

```obq
result: Result<Value, Error> = ffi.load("/path/to/lib.so")
if (result.isError()) {
    error: Value = result.getError()
    io.print("FFI Error: %s", error.getMessage())
    return 1
}

# Safely get the result
lib: Value = result.getResult()
```

## Common Patterns

### Library Wrapper Object

```obq
Object MathLibWrapper {
    property lib: Value
    
    constructor() {
        libResult: Result<Value, Error> = ffi.load("/usr/lib/libm.so")
        if (libResult.isError()) {
            throw Error("Failed to load math library")
        }
        this.lib = libResult.getResult()
    }
    
    method sin(x: Double): Double {
        fnResult: Result<Value, Error> = this.lib.symbol("sin", "f64->f64")
        if (fnResult.isError()) {
            throw Error("Failed to get sin function")
        }
        
        fn: Value = fnResult.getResult()
        result: Result<Value, Error> = fn.call(x)
        if (result.isError()) {
            throw Error("Failed to call sin function")
        }
        
        return result.getResult().toDouble()
    }
}
```

### Function Caching

```obq
Object FFICache {
    property functions: Map<Text, Value>
    
    constructor() {
        this.functions = new Map<Text, Value>()
    }
    
    method getFunction(lib: Value, name: Text, signature: Text): Result<Value, Error> {
        key: Text = name + ":" + signature
        if (this.functions.containsKey(key)) {
            return Result.success(this.functions.get(key))
        }
        
        fnResult: Result<Value, Error> = lib.symbol(name, signature)
        if (fnResult.isSuccess()) {
            fn: Value = fnResult.getResult()
            this.functions.put(key, fn)
        }
        
        return fnResult
    }
}
```

## Platform Support

### Library Extensions by Platform

| Platform | Extension | Example Path |
|----------|-----------|--------------|
| Linux | `.so` | `/usr/lib/libm.so` |
| macOS | `.dylib` | `/usr/lib/libm.dylib` |
| Windows | `.dll` | `C:\Windows\System32\kernel32.dll` |

### System Library Paths

**Linux:**
- `/usr/lib/`
- `/lib/`
- `/usr/local/lib/`

**macOS:**
- `/usr/lib/`
- `/opt/homebrew/lib/`
- `/usr/local/lib/`

**Windows:**
- `C:\Windows\System32\`
- Program-specific DLL directories

## Limitations

- **No callback support from C to O²L**: C functions cannot call back into O²L code
- **Limited pointer arithmetic**: Basic pointer operations only
- **Type safety**: No automatic type conversion between incompatible types
- **Memory management**: O²L cannot free memory allocated by C libraries
- **Thread safety**: FFI calls must be made from the main O²L thread

## Best Practices

1. **Always check Result types** for errors
2. **Cache function symbols** to avoid repeated lookups
3. **Use wrapper objects** to encapsulate FFI complexity
4. **Handle platform differences** in library paths
5. **Validate inputs** before passing to native functions
6. **Document function signatures** clearly in your code
7. **Test thoroughly** on all target platforms

## Troubleshooting

### Common Issues

**Library not found:**
```
Failed to load library: /path/to/lib.so
```
- Check if library exists at the specified path
- Verify library permissions
- Check platform-specific extensions

**Symbol not found:**
```
Failed to get symbol: function_name
```
- Verify function name (check for C++ name mangling)
- Use `nm` or `objdump` to list available symbols
- Check if function is exported

**Call failed:**
```
Failed to call function
```
- Verify function signature matches actual C function
- Check argument types and count
- Ensure library is still loaded

### Debugging Tips

```obq
# Check errno for system-level errors
errno: Int = ffi.getLastErrno()
io.print("Last errno: %d", errno)

# Use detailed error messages
if (result.isError()) {
    error: Value = result.getError()
    io.print("Detailed error: %s", error.getMessage())
}
```