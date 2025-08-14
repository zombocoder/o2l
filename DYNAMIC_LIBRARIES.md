# O²L Dynamic Library System

This document describes the comprehensive dynamic library architecture implemented for O²L, enabling native C++ libraries to be loaded and used from O²L code.

## Architecture Overview

The dynamic library system consists of several key components:

### 1. Core Components

- **`NativeLibrary`** - Abstract base class that all native libraries must inherit from
- **`DynamicLibraryManager`** - Manages loading, unloading, and lifecycle of native libraries
- **`NativeMethodRegistry`** - Registry for mapping method names to native function implementations
- **Enhanced `@external` syntax** - Support for native method bindings

### 2. Library Interface

```cpp
class NativeLibrary {
public:
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual void initialize(Context& context) = 0;
    virtual void registerMethods(ObjectInstance* obj) = 0;
    virtual void cleanup() = 0;
    virtual std::map<std::string, std::string> getMetadata() const;
};
```

### 3. Entry Points

Every native library must export these C-style functions:

```cpp
extern "C" {
    NativeLibrary* create_library();
    void destroy_library(NativeLibrary* lib);
    const char* get_abi_version();  // Optional
}
```

## Usage Examples

### Creating a Native Library

```cpp
// MathLibrary.cpp
#include <o2l/NativeLibrary.hpp>

class MathLibrary : public o2l::NativeLibrary {
public:
    std::string getName() const override { return "Math"; }
    std::string getVersion() const override { return "1.0.0"; }
    
    void initialize(Context& context) override {
        // One-time initialization
    }
    
    void registerMethods(ObjectInstance* obj) override {
        // Register sqrt method
        obj->addMethod("sqrt", [](const std::vector<Value>& args, Context& ctx) -> Value {
            if (args.size() != 1) throw EvaluationError("sqrt() expects 1 argument");
            double val = std::get<Double>(args[0]);
            return Double(std::sqrt(val));
        }, true); // true = external
        
        // Register pow method
        obj->addMethod("pow", [](const std::vector<Value>& args, Context& ctx) -> Value {
            if (args.size() != 2) throw EvaluationError("pow() expects 2 arguments");
            double base = std::get<Double>(args[0]);
            double exp = std::get<Double>(args[1]);
            return Double(std::pow(base, exp));
        }, true);
    }
    
    void cleanup() override {
        // Cleanup resources
    }
};

extern "C" {
    o2l::NativeLibrary* create_library() {
        return new MathLibrary();
    }
    
    void destroy_library(o2l::NativeLibrary* lib) {
        delete lib;
    }
}
```

### Using Native Libraries in O²L

```obq
# Load native library (implicit)
import native.math

Object Main {
    method main(): Text {
        # Create native object instance
        math: Math = new Math()
        
        # Call native methods
        result: Double = math.sqrt(16.0)
        power: Double = math.pow(2.0, 3.0)
        
        io.print("sqrt(16) = %f", result)    # 4.0
        io.print("pow(2,3) = %f", power)     # 8.0
        
        "Native library demo complete"
    }
}
```

### Enhanced @external Syntax (Future)

```obq
Object MathWrapper {
    # Direct native binding
    @external(native="libmath.so", function="o2l_sqrt")
    method sqrt(x: Double): Double
    
    # Pure O²L external method
    @external
    method double(x: Double): Double {
        x * 2.0
    }
}
```

## Package Manager Integration

### o2l-pkg Native Library Support

```bash
# Add native library
o2l-pkg add-native math-native 1.0.0

# Create native library template
o2l-pkg create-native com.mycompany.math

# Build native library
o2l-pkg build-native com.mycompany.math --release
```

### Directory Structure

```
project/
├── .o2l/
│   └── lib/
│       └── native/              # Native libraries
│           ├── libmath.so       # Math library
│           ├── libcollections.so
│           └── libnetwork.so
├── src/
│   └── main.obq
└── o2l.toml
```

## Platform Support

### Library Extensions
- **Linux**: `.so` (shared object)
- **macOS**: `.dylib` (dynamic library)  
- **Windows**: `.dll` (dynamic link library)

### Library Naming
- **Unix-like**: `libname.ext` (e.g., `libmath.so`)
- **Windows**: `name.dll` (e.g., `math.dll`)

### Search Paths
1. `.o2l/lib/native/` (project-local)
2. `/usr/local/lib/o2l/` (system-wide Unix)
3. `$PROGRAMFILES/O2L/lib/` (system-wide Windows)

## Error Handling

### Library Loading Errors
- Missing library file
- Invalid library format
- Missing entry points (`create_library`, `destroy_library`)
- ABI version mismatch
- Initialization failures

### Runtime Errors
- Method not found
- Invalid argument types/count
- Native function exceptions
- Library cleanup failures

## ABI Compatibility

### Version Checking
Libraries can export `get_abi_version()` for compatibility:

```cpp
extern "C" const char* get_abi_version() {
    return "1.0.0";  // Must match O²L runtime ABI
}
```

### Breaking Changes
Major ABI changes will increment the version, requiring library recompilation.

## Security Considerations

### Sandboxing
- Native libraries run with full system privileges
- Consider runtime permission system for sensitive operations
- Validate all inputs from O²L code

### Memory Safety
- Proper exception handling in native code
- Avoid raw pointers in public interfaces
- RAII principles for resource management

## Performance Characteristics

### Loading Overhead
- Libraries loaded on first use (lazy loading)
- Cached for subsequent object instantiations
- Unloaded on program exit or explicit cleanup

### Call Overhead
- Direct function pointer calls (minimal overhead)
- Value conversion between O²L and native types
- Exception translation layer

## Development Tools

### CMake Integration
```cmake
# Template for native libraries
add_library(math SHARED MathLibrary.cpp)
target_link_libraries(math o2l_runtime)
set_target_properties(math PROPERTIES PREFIX "lib")
```

### Debugging Support
- GDB/LLDB support for mixed O²L/native debugging
- Stack trace integration across language boundaries
- Memory profiling for native components

## Future Enhancements

### Planned Features
1. **Hot Reloading** - Update libraries without restarting
2. **Dependency Management** - Native library dependencies  
3. **Code Generation** - Auto-generate bindings from headers
4. **WebAssembly Support** - Compile native libraries to WASM
5. **Reflection API** - Runtime inspection of native methods

### Integration Points
- IDE tooling for native library development
- Package registry for sharing native libraries
- Cross-compilation support for multiple platforms
- Automated testing frameworks for native code

## Status: Foundation Complete ✅

The core architecture is implemented and ready for:
- Native library development
- Integration with o2l-pkg
- Testing with example libraries
- Production use cases

The dynamic library system provides a robust foundation for extending O²L with high-performance native code while maintaining the language's object-oriented philosophy and safety guarantees.