# Access Control

O²L Programming Language provides access control mechanisms to manage method visibility and runtime interaction.

## External Methods

The `@external` annotation marks methods that can interact with the runtime and be called from outside the object:

```obq
Object Calculator {
    # Regular method - internal use
    method internalCalculate(a: Int, b: Int): Int {
        return a + b
    }
    
    # External method - can be called from runtime
    @external method calculate(a: Int, b: Int): Int {
        return this.internalCalculate(a, b)
    }
}
```

## Method Visibility

Methods have different visibility levels:

### Internal Methods
Regular methods are only accessible within the object hierarchy:

```obq
Object Helper {
    method protectedOperation(): Void {
        # Only accessible from within this object
    }
}
```

### External Methods
Methods marked with `@external` can be accessed by the runtime:

```obq
Object PublicAPI {
    @external method publicOperation(): Int {
        # Can be called from outside the object
        return 0
    }
}
```

## Access Control Example

```obq
Object BankAccount {
    property balance: Float
    
    constructor(initialBalance: Float) {
        this.balance = initialBalance
    }
    
    # Internal method - protected operation
    method validateAmount(amount: Float): Bool {
        return amount > 0.0
    }
    
    # External method - public API
    @external method deposit(amount: Float): Bool {
        if (this.validateAmount(amount)) {
            this.balance = this.balance + amount
            return true
        }
        return false
    }
    
    # External method - public API  
    @external method getBalance(): Float {
        return this.balance
    }
}
```

## Main Method Requirements

The `main()` method in the `Main` object should typically be external to allow runtime execution:

```obq
Object Main {
    method main(): Int {  # Can be regular or @external
        # Application entry point
        return 0
    }
}
```

## Best Practices

- Use `@external` for methods that need to be called by the runtime
- Keep internal logic in regular methods
- Use external methods as the public API of your objects
- Limit external methods to necessary entry points