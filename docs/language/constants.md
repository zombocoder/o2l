# Constants

O²L Programming Language supports immutable constants using the `const` keyword.

## Declaring Constants

Constants are declared with `const` and cannot be reassigned:

```obq
const PI: Float = 3.14159
const MAX_USERS: Int = 1000
const APP_NAME: Text = "My Application"
```

## Constant Rules

- Constants must be initialized when declared
- Constants cannot be reassigned after initialization
- Attempting to reassign a constant will cause a compilation error

```obq
const VALUE: Int = 42
# VALUE = 50  # Error: Cannot reassign constant
```

## Constants vs Variables

```obq
# Variable - can be reassigned
counter: Int = 0
counter = counter + 1  # Valid

# Constant - cannot be reassigned
const LIMIT: Int = 100
# LIMIT = 200  # Error: Cannot reassign constant
```

## Use Cases

Constants are ideal for:

- Mathematical constants (PI, E, etc.)
- Configuration values
- Maximum/minimum limits
- Application settings

```obq
Object MathUtils {
    const PI: Float = 3.14159
    const E: Float = 2.71828
    
    method circleArea(radius: Float): Float {
        return PI * radius * radius
    }
}
```

## Scope

Constants follow the same scoping rules as variables:

```obq
Object Example {
    const CLASS_CONSTANT: Int = 100
    
    method demo(): Int {
        const LOCAL_CONSTANT: Int = 50
        return LOCAL_CONSTANT + CLASS_CONSTANT
    }
}
```