# Import System

O²L Programming Language provides a flexible import system for using external modules and libraries.

## Basic Import Syntax

Import specific modules using the `import` statement:

```obq
import system.io
import system.utils
```

## Wildcard Imports

Import all exports from a module using `*`:

```obq
import system.io.*
```

## System Libraries

Common system libraries available for import:

```obq
import system.io     # Input/output operations
import system.fs     # File system operations  
import system.os     # Operating system utilities
import system.utils  # Utility functions and iterators
```

## Using Imported Modules

After importing, use the module's functions and classes:

```obq
import system.io
import system.utils

Object Example {
    method demo(): Int {
        # Use io module
        io.print("Hello World")
        
        # Use utils module
        repeater: RepeatIterator = utils.repeat(5)
        
        return 0
    }
}
```

## Package Imports

Import from packages using dot notation:

```obq
import mypackage.PackageObj
import company.utils.Calculator
```

## Multiple Imports

Import multiple modules in separate statements:

```obq
import system.io
import system.fs
import system.utils
import mypackage.Helper
```

## Import Examples

### Complete Example

```obq
import system.io.*
import system.utils

Object ImportDemo {
    @external method runDemo(): Int {
        # Use imported io functions
        io.print("Starting demo...")
        
        # Use imported utils
        repeater: RepeatIterator = utils.repeat(3)
        while (repeater.hasNext()) {
            count: Int = repeater.next()
            io.print("Count: %d", count)
        }
        
        io.print("Demo complete!")
        return 0
    }
}

Object Main {
    method main(): Int {
        demo: ImportDemo = new ImportDemo()
        return demo.runDemo()
    }
}
```

## Import Best Practices

- Import only what you need
- Use specific imports rather than wildcards when possible
- Group system imports first, then external packages
- Place all imports at the top of the file