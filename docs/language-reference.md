# Language Reference

Complete syntax reference for O²L Programming Language.

## File Structure

```
program.obq:
  imports*
  object-definitions*
  main-object
```

## Import Statements

```obq
import module-name
import module-name.*
import package.module
```

## Comments

```obq
# Single-line comment

###
Multi-line comment
can span multiple lines
###
```

## Object Definition

```obq
Object ObjectName {
    property propertyName: Type
    
    constructor(param: Type) {
        this.propertyName = param
    }
    
    [@external] method methodName(param: Type): ReturnType {
        # method body
        return value
    }
}
```

## Variable Declarations

```obq
variableName: Type = value
const CONSTANT_NAME: Type = value
```

## Types

### Primitive Types
- `Int` - 32-bit integer
- `Long` - 64-bit integer  
- `Float` - 32-bit floating point
- `Double` - 64-bit floating point
- `Text` - String type
- `Bool` - Boolean (true/false)
- `Void` - No return value

### Generic Types
- `List<T>` - Dynamic array
- `Map<K,V>` - Key-value collection
- `Set<T>` - Unique element collection

### Iterator Types
- `ListIterator` - For List traversal
- `RepeatIterator` - For counting operations

## Control Flow

### If Statements
```obq
if (condition) {
    # statements
} else {
    # statements
}
```

### While Loops
```obq
while (condition) {
    # statements
}
```

### Break and Continue
```obq
while (condition) {
    if (skipCondition) {
        continue
    }
    if (exitCondition) {
        break
    }
}
```

## Operators

### Arithmetic
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo

### Comparison
- `==` Equal
- `!=` Not equal
- `<` Less than
- `>` Greater than
- `<=` Less than or equal
- `>=` Greater than or equal

### Logical
- `&&` Logical AND
- `||` Logical OR
- `!` Logical NOT

## Method Calls

```obq
# Object method call
object.methodName(arguments)

# Constructor call
instance: ClassName = new ClassName(arguments)

# Static-like call (imported modules)
module.function(arguments)
```

## Literals

### Numeric Literals
```obq
intValue: Int = 42
floatValue: Float = 3.14
longValue: Long = 1234567890
doubleValue: Double = 2.718281828
```

### Text Literals
```obq
message: Text = "Hello, World!"
```

### Boolean Literals
```obq
isTrue: Bool = true
isFalse: Bool = false
```

### List Literals
```obq
numbers: List<Int> = [1, 2, 3, 4, 5]
names: List<Text> = ["Alice", "Bob", "Charlie"]
empty: List<Int> = []
```

## Annotations

### @external
Marks methods for runtime access:

```obq
@external method publicMethod(): Int {
    return 0
}
```

## Reserved Keywords

- `Object` - Define object
- `method` - Define method
- `constructor` - Define constructor
- `property` - Define property
- `import` - Import module
- `new` - Create instance
- `this` - Self reference
- `if`, `else` - Conditionals
- `while` - Loop
- `break`, `continue` - Loop control
- `return` - Return value
- `const` - Constant declaration
- `true`, `false` - Boolean literals

## Naming Conventions

- **Objects**: PascalCase (`Calculator`, `BankAccount`)
- **Methods**: camelCase (`calculateTotal`, `getName`)
- **Properties**: camelCase (`firstName`, `accountBalance`)
- **Variables**: camelCase (`userInput`, `totalCount`)
- **Constants**: UPPER_CASE (`MAX_SIZE`, `DEFAULT_VALUE`)

## String Formatting

Use format specifiers in `io.print()`:

- `%s` - Text/String
- `%d` - Integer
- `%f` - Float/Double
- `%o` - Boolean

```obq
io.print("Name: %s, Age: %d, Score: %f, Active: %o", name, age, score, isActive)
```