# Properties

O²L Programming Language supports object properties for storing state and data.

## Declaring Properties

Properties are declared inside objects and store object state:

```obq
Object Person {
    property name: Text
    property age: Int
    
    constructor(name: Text, age: Int) {
        this.name = name
        this.age = age
    }
}
```

## Accessing Properties

Use `this.propertyName` to access properties within the object:

```obq
Object Calculator {
    property value: Int
    
    constructor(initial: Int) {
        this.value = initial
    }
    
    method getValue(): Int {
        return this.value  # Access property
    }
    
    method setValue(newValue: Int): Void {
        this.value = newValue  # Modify property
    }
}
```

## Properties vs Methods

- **Properties**: Store data and state
- **Methods**: Define behavior and operations

```obq
Object BankAccount {
    property balance: Float      # Data storage
    property accountNumber: Text # Data storage
    
    constructor(number: Text) {
        this.accountNumber = number
        this.balance = 0.0
    }
    
    method deposit(amount: Float): Void {  # Behavior
        this.balance = this.balance + amount
    }
    
    method getBalance(): Float {           # Behavior
        return this.balance
    }
}
```

## Property Initialization

Properties can be initialized in constructors:

```obq
Object Config {
    property maxConnections: Int
    property serverName: Text
    property isEnabled: Bool
    
    constructor() {
        this.maxConnections = 100
        this.serverName = "DefaultServer"
        this.isEnabled = true
    }
}
```

## Property Types

Properties can be any valid O²L type:

```obq
Object DataContainer {
    property numbers: List<Int>
    property settings: Map<Text, Text>
    property flags: Set<Text>
    property processor: Calculator
    
    constructor() {
        this.numbers = [1, 2, 3]
        this.settings = new Map<Text, Text>()
        this.flags = new Set<Text>()
        this.processor = new Calculator(0)
    }
}
```