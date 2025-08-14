# O²L Language Reference

## Variable Mutability System

### Overview
O²L implements a dual mutability system that balances object immutability principles with practical programming needs:

- **Object Properties**: Immutable after initialization (accessed via `this.property`)
- **Method Variables**: Mutable within method scope (standard variable assignments)

### Object Properties (Immutable)

**Declaration:**
```o2l
Object MyObject {
    property value: Int
    property name: Text
    
    constructor(initial_value: Int, initial_name: Text) {
        this.value = initial_value    # ✅ Initialization allowed
        this.name = initial_name      # ✅ Initialization allowed
    }
    
    @external method getValue(): Int {
        # ❌ this.value = 100         # Error: Cannot reassign property
        return this.value             # ✅ Reading allowed
    }
}
```

**Rules:**
- Properties declared with `property name: Type` syntax
- Can only be assigned during object construction via `this.property = value`
- Cannot be reassigned after initialization
- Always accessible via `this.property` syntax
- Maintains object immutability principle

### Method Variables (Mutable)

**Declaration and Assignment:**
```o2l
Object Calculator {
    @external method compute(): Int {
        # Variable declaration with initialization
        result: Int = 0
        counter: Int = 1
        message: Text = "Starting"
        active: Bool = true
        
        # ✅ All reassignments allowed
        result = result + 10
        counter = counter * 2
        message = message + "..."
        active = !active
        
        # ✅ Reassignment in control structures
        while (counter <= 5) {
            result = result + counter
            counter = counter + 1
        }
        
        if (active) {
            result = result * 2
        }
        
        return result
    }
}
```

**Rules:**
- Variables declared with `name: Type = value` syntax
- Can be reassigned with `name = new_value` syntax
- Scoped to method execution context
- Support all value types (Int, Text, Bool, Float, etc.)
- Can be modified in control structures (if, while)

### Error Handling

**Attempting to reassign property:**
```o2l
Object Test {
    property value: Int
    
    constructor() { this.value = 42 }
    
    @external method tryReassign(): Int {
        this.value = 100  # ❌ Runtime Error: Cannot reassign constant variable 'value'
        return this.value
    }
}
```

**Attempting to reassign undefined variable:**
```o2l
Object Test {
    @external method tryUndefined(): Int {
        undefined_var = 42  # ❌ Runtime Error: Cannot assign to undefined variable 'undefined_var'
        return 0
    }
}
```

## Logical Operators

### Supported Operators

| Operator | Name | Precedence | Associativity | Description |
|----------|------|------------|---------------|-------------|
| `!` | NOT | 1 (highest) | Right | Logical negation |
| `&&` | AND | 2 | Left | Logical conjunction |
| `||` | OR | 3 (lowest) | Left | Logical disjunction |

### Usage Examples

**Basic Operations:**
```o2l
Object LogicDemo {
    @external method basicLogic(): Bool {
        a: Bool = true
        b: Bool = false
        
        # Basic operators
        not_a: Bool = !a           # false
        a_and_b: Bool = a && b     # false  
        a_or_b: Bool = a || b      # true
        
        # Multiple NOT operations
        double_not: Bool = !!a     # true (involution property)
        triple_not: Bool = !!!a    # false
        
        return a_or_b
    }
}
```

**Precedence Rules:**
```o2l
Object PrecedenceDemo {
    @external method precedenceTest(): Bool {
        a: Bool = true
        b: Bool = false
        c: Bool = true
        
        # Without parentheses (precedence: ! > && > ||)
        expr1: Bool = !a && b || c    # ((!a) && b) || c = true
        expr2: Bool = a || b && !c    # a || (b && (!c)) = true
        
        # With parentheses (override precedence)
        expr3: Bool = !(a && b) || c  # (!(a && b)) || c = true
        expr4: Bool = (a || b) && c   # (a || b) && c = true
        
        return expr1 && expr2 && expr3 && expr4
    }
}
```

**Short-Circuit Evaluation:**
```o2l
Object ShortCircuitDemo {
    @external method shortCircuit(): Bool {
        # AND short-circuit: right side not evaluated if left is false
        result1: Bool = false && someExpensiveOperation()  # someExpensiveOperation() not called
        
        # OR short-circuit: right side not evaluated if left is true  
        result2: Bool = true || anotherExpensiveOperation()  # anotherExpensiveOperation() not called
        
        return result1 || result2
    }
}
```

### De Morgan's Laws

O²L logical operators follow De Morgan's laws:

```o2l
Object DeMorganDemo {
    @external method verifyDeMorgan(): Bool {
        a: Bool = true
        b: Bool = false
        
        # De Morgan's Law 1: !(A && B) = (!A || !B)
        law1_left: Bool = !(a && b)        # true
        law1_right: Bool = !a || !b        # true
        law1_holds: Bool = law1_left == law1_right  # true
        
        # De Morgan's Law 2: !(A || B) = (!A && !B)  
        law2_left: Bool = !(a || b)        # false
        law2_right: Bool = !a && !b        # false
        law2_holds: Bool = law2_left == law2_right  # true
        
        return law1_holds && law2_holds
    }
}
```

## Filesystem Operations (system.fs)

### Module Import
```o2l
import system.fs
```

### Available Methods

#### File Operations

**`fs.readText(path: Text): Text`**
- Reads entire file content as text
- Throws error if file doesn't exist or cannot be read
- Example: `content: Text = fs.readText("data.txt")`

**`fs.writeText(path: Text, content: Text): Bool`**  
- Writes text content to file (overwrites existing)
- Returns `true` on success, `false` on failure
- Creates file if it doesn't exist
- Example: `success: Bool = fs.writeText("output.txt", "Hello World")`

#### Path Operations

**`fs.exists(path: Text): Bool`**
- Checks if file or directory exists
- Returns `true` if path exists, `false` otherwise
- Example: `exists: Bool = fs.exists("myfile.txt")`

**`fs.isFile(path: Text): Bool`**
- Checks if path points to a regular file
- Returns `false` if path doesn't exist or is not a file
- Example: `is_file: Bool = fs.isFile("document.pdf")`

**`fs.isDirectory(path: Text): Bool`**
- Checks if path points to a directory
- Returns `false` if path doesn't exist or is not a directory  
- Example: `is_dir: Bool = fs.isDirectory("my_folder")`

#### Directory Operations

**`fs.createDirectory(path: Text): Bool`**
- Creates directory (and parent directories if needed)
- Returns `true` on success, `false` on failure
- Example: `created: Bool = fs.createDirectory("path/to/new/dir")`

**`fs.listFiles(path: Text): List`**
- Lists contents of directory
- Returns List of Text containing file/directory names
- Throws error if path doesn't exist or is not a directory
- Example: `files: List = fs.listFiles(".")`

**`fs.deleteFile(path: Text): Bool`**
- Deletes file or empty directory
- Returns `true` on success, `false` on failure
- Cannot delete non-empty directories
- Example: `deleted: Bool = fs.deleteFile("temp.txt")`

### Complete Example

```o2l
import system.io
import system.fs

Object FileProcessor {
    @external method processDirectory(): Int {
        base_dir: Text = "processing_temp"
        input_file: Text = base_dir + "/input.txt" 
        output_file: Text = base_dir + "/output.txt"
        processed_count: Int = 0
        
        # Create working directory
        dir_created: Bool = fs.createDirectory(base_dir)
        if (!dir_created) {
            io.print("Failed to create directory")
            return 0
        }
        
        # Write initial data
        initial_content: Text = "Data to process\nLine 2\nLine 3"
        write_success: Bool = fs.writeText(input_file, initial_content)
        
        # Verify file was created
        file_exists: Bool = fs.exists(input_file)
        is_file: Bool = fs.isFile(input_file)
        
        # Process only if all conditions met
        if (write_success && file_exists && is_file) {
            # Read and process content
            content: Text = fs.readText(input_file)
            processed_content: Text = "PROCESSED: " + content
            
            # Write processed result
            output_success: Bool = fs.writeText(output_file, processed_content)
            if (output_success) {
                processed_count = processed_count + 1
            }
        }
        
        # List directory contents
        files: List = fs.listFiles(base_dir)
        file_count: Int = files.size()
        io.print("Directory contains %d files", file_count)
        
        # Cleanup
        input_deleted: Bool = fs.deleteFile(input_file)
        output_deleted: Bool = fs.deleteFile(output_file)
        dir_deleted: Bool = fs.deleteFile(base_dir)
        
        cleanup_success: Bool = input_deleted && output_deleted && dir_deleted
        
        if (cleanup_success) {
            io.print("Processing completed successfully")
            return processed_count
        } else {
            io.print("Cleanup failed")
            return 0
        }
    }
}

Object Main {
    method main(): Int {
        processor: FileProcessor = new FileProcessor()
        return processor.processDirectory()
    }
}
```

## Expression Enhancements

### Parentheses Support

**Arithmetic Expressions:**
```o2l
Object MathProcessor {
    @external method calculate(): Int {
        x: Int = 10
        y: Int = 20
        z: Int = 5
        
        # Without parentheses (standard precedence)
        result1: Int = x + y * z        # 10 + (20 * 5) = 110
        
        # With parentheses (override precedence)  
        result2: Int = (x + y) * z      # (10 + 20) * 5 = 150
        
        # Complex nested expressions
        complex: Int = ((x + y) * z) - (x * (y - z))  # 150 - 150 = 0
        
        return complex
    }
}
```

**Logical Expression Grouping:**
```o2l
Object LogicalGrouping {
    @external method evaluate(): Bool {
        a: Bool = true
        b: Bool = false  
        c: Bool = true
        d: Bool = false
        
        # Complex logical grouping
        expr1: Bool = (a && b) || (c && d)           # false || false = false
        expr2: Bool = (a || b) && (c || d)           # true && true = true
        expr3: Bool = !((a && b) || (c && d))        # !(false || false) = true
        expr4: Bool = !(a && b) && !(c && d)         # true && true = true
        
        # De Morgan's law verification with parentheses
        demorgan_left: Bool = !(a && b)              # true
        demorgan_right: Bool = (!a || !b)            # true  
        demorgan_equal: Bool = demorgan_left == demorgan_right  # true
        
        return expr2 && expr3 && demorgan_equal
    }
}
```

### Operator Precedence Summary

**Complete precedence table (highest to lowest):**

1. **Parentheses**: `(` `)`
2. **Unary**: `!` (NOT), `-` (unary minus)  
3. **Multiplicative**: `*`, `/`, `%`
4. **Additive**: `+`, `-`
5. **Comparison**: `<`, `<=`, `>`, `>=`, `==`, `!=`
6. **Logical AND**: `&&`
7. **Logical OR**: `||`
8. **Assignment**: `=`

**Example demonstrating full precedence:**
```o2l
Object PrecedenceComplete {
    @external method fullTest(): Bool {
        x: Int = 10
        y: Int = 5
        z: Int = 2
        
        # Complex expression with mixed operators
        # Evaluates as: (x + (y * z)) > ((x - y) / z) && (x != (y + z))
        complex: Bool = x + y * z > (x - y) / z && x != y + z
        
        # Equivalent with explicit parentheses  
        explicit: Bool = ((x + (y * z)) > ((x - y) / z)) && (x != (y + z))
        
        # Should be equal
        return complex == explicit
    }
}
```