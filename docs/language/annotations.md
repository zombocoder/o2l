# Annotations

O²L Programming Language uses annotations to provide metadata and modify method behavior.

## @external Annotation

The primary annotation in O²L is `@external`, which marks methods for runtime interaction:

```obq
Object Calculator {
    # Regular method
    method internalAdd(a: Int, b: Int): Int {
        return a + b
    }
    
    # External method - can be called by runtime
    @external method add(a: Int, b: Int): Int {
        return this.internalAdd(a, b)
    }
}
```

## Annotation Syntax

Annotations are placed on the line before the method declaration:

```obq
Object Example {
    @external
    method publicMethod(): Int {
        return 0
    }
    
    # Or on the same line
    @external method anotherPublicMethod(): Text {
        return "Hello"
    }
}
```

## Main Method Annotation

Main methods can use the `@external` annotation:

```obq
Object Main {
    @external method main(): Int {
        io.print("Hello, O²L!")
        return 0
    }
}
```

## When to Use @external

Use `@external` for methods that:

- Serve as entry points for your application
- Need to be called by the O²L runtime
- Provide the public API of your objects
- Perform I/O operations or system interactions

```obq
import system.io

Object FileProcessor {
    # Internal helper method
    method validateFilename(filename: Text): Bool {
        return filename.length() > 0
    }
    
    # External API method
    @external method processFile(filename: Text): Int {
        if (this.validateFilename(filename)) {
            io.print("Processing file: %s", filename)
            return 0
        } else {
            io.print("Invalid filename")
            return 1
        }
    }
}
```

## Future Annotations

The annotation system is extensible and may support additional annotations in future versions of O²L for features like:

- Access modifiers
- Validation rules
- Performance hints
- Documentation generation

## Best Practices

- Use `@external` sparingly - only for true entry points
- Keep business logic in regular methods
- Group external methods as the public interface
- Document external methods thoroughly since they're the public API