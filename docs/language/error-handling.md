# Error Handling

O²L Programming Language provides basic error handling through return values and method signatures.

## Return Values

Methods return values to indicate success or failure:

```obq
Object Calculator {
    method divide(a: Int, b: Int): Int {
        if (b == 0) {
            return -1  # Error indicator
        }
        return a / b
    }
}
```

## Method Return Types

Methods must specify their return type in the signature:

```obq
method processData(): Int {
    # Process data and return status code
    return 0  # Success
}

method getName(): Text {
    return "Example"
}
```

## Error Checking Patterns

Check return values to handle errors:

```obq
result: Int = calculator.divide(10, 0)
if (result == -1) {
    io.print("Division by zero error")
} else {
    io.print("Result: %d", result)
}
```

## External Methods

External methods (marked with `@external`) can interact with the runtime and may have different error semantics:

```obq
@external method riskyOperation(): Int {
    # External method that might fail
    return 0
}
```