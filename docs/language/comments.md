# Comments

O²L Programming Language supports both single-line and multi-line comments for code documentation.

## Single-Line Comments

Use `#` for single-line comments:

```obq
# This is a single-line comment
import system.io

Object Calculator {
    method add(a: Int, b: Int): Int {
        # Calculate the sum of two numbers
        result: Int = a + b  # Inline comment
        return result
    }
}
```

## Multi-Line Comments

Use `###` to start and end multi-line comments:

```obq
###
This is a multi-line comment
that can span multiple lines
and include documentation
###

Object Example {
    ###
    This method demonstrates
    multi-line comment usage
    inside object definitions
    ###
    method demo(): Int {
        value: Int = 42
        
        ###
        Multi-line comments can appear
        anywhere in the code
        ###
        
        return value
    }
}
```

## Comment Best Practices

- Use single-line comments for brief explanations
- Use multi-line comments for detailed documentation
- Place comments above the code they describe
- Use inline comments sparingly for clarification

## Example with Mixed Comments

```obq
# Main application entry point
import system.io

###
Calculator object provides basic
arithmetic operations for demonstration
###
Object Calculator {
    # Addition method
    method add(a: Int, b: Int): Int {
        return a + b  # Simple addition
    }
    
    ###
    Division method with error handling
    Returns -1 on division by zero
    ###
    method divide(a: Int, b: Int): Int {
        if (b == 0) {
            return -1  # Error indicator
        }
        return a / b
    }
}
```