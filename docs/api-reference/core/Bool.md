# Bool API Reference

## Overview

Bool is O²L's boolean type representing logical truth values: `true` and `false`. Bool values are immutable and support logical operations, comparisons, and type conversions. Bool is fundamental for conditional logic, control flow, and binary state representation throughout O²L programs.

## Declaration & Literals

```o2l
# Boolean literals
is_active: Bool = true
is_complete: Bool = false

# From expressions
age: Int = 25
is_adult: Bool = age >= 18          # true
has_permission: Bool = age >= 21    # true

# From comparisons
x: Int = 10
y: Int = 20
is_equal: Bool = x == y             # false
is_less: Bool = x < y               # true
```

## Quick Example

```o2l
import system.io

# Basic boolean operations
logged_in: Bool = true
has_permission: Bool = false

can_access: Bool = logged_in && has_permission
should_show_login: Bool = !logged_in

io.print("Logged in: %o", logged_in)
io.print("Can access: %o", can_access)
io.print("Should show login: %o", should_show_login)

# Convert to other types
as_text: Text = logged_in.toString()    # "true"
as_int: Int = logged_in.toInt()         # 1
io.print("As text: %s, As int: %d", as_text, as_int)
```

---

## Logical Operations

Bool supports all standard logical operations:

```o2l
# Basic logical operators
a: Bool = true
b: Bool = false

# AND operation
and_result: Bool = a && b           # false (true AND false)
io.print("%o && %o = %o", a, b, and_result)

# OR operation
or_result: Bool = a || b            # true (true OR false)
io.print("%o || %o = %o", a, b, or_result)

# NOT operation
not_a: Bool = !a                    # false (NOT true)
not_b: Bool = !b                    # true (NOT false)
io.print("!%o = %o", a, not_a)
io.print("!%o = %o", b, not_b)
```

### Truth Tables

```o2l
# Demonstrate complete truth tables
Object LogicTables {
    method demonstrateAnd(): Void {
        io.print("AND Truth Table:")
        io.print("true  && true  = %o", true && true)     # true
        io.print("true  && false = %o", true && false)    # false
        io.print("false && true  = %o", false && true)    # false
        io.print("false && false = %o", false && false)   # false
    }

    method demonstrateOr(): Void {
        io.print("OR Truth Table:")
        io.print("true  || true  = %o", true || true)     # true
        io.print("true  || false = %o", true || false)    # true
        io.print("false || true  = %o", false || true)    # true
        io.print("false || false = %o", false || false)   # false
    }

    method demonstrateNot(): Void {
        io.print("NOT Truth Table:")
        io.print("!true  = %o", !true)                    # false
        io.print("!false = %o", !false)                   # true
    }

    method demonstrateXor(a: Bool, b: Bool): Bool {
        # XOR: true when operands are different
        return (a && !b) || (!a && b)
    }
}

# Usage
tables: LogicTables = new LogicTables()
tables.demonstrateAnd()
tables.demonstrateOr()
tables.demonstrateNot()

# XOR examples
xor1: Bool = tables.demonstrateXor(true, false)   # true
xor2: Bool = tables.demonstrateXor(true, true)    # false
io.print("XOR(true, false) = %o", xor1)
io.print("XOR(true, true) = %o", xor2)
```

### Short-Circuit Evaluation

```o2l
# Logical operators use short-circuit evaluation
Object ShortCircuitDemo {
    property counter: Int

    constructor() {
        this.counter = 0
    }

    method incrementAndReturnTrue(): Bool {
        this.counter = this.counter + 1
        io.print("Method called, counter now: %d", this.counter)
        return true
    }

    method demonstrateAndShortCircuit(): Void {
        io.print("Testing AND short-circuit:")
        this.counter = 0

        # This should NOT call the method (false && ...)
        result1: Bool = false && this.incrementAndReturnTrue()
        io.print("false && method() = %o, counter = %d", result1, this.counter)

        # This SHOULD call the method (true && ...)
        result2: Bool = true && this.incrementAndReturnTrue()
        io.print("true && method() = %o, counter = %d", result2, this.counter)
    }

    method demonstrateOrShortCircuit(): Void {
        io.print("Testing OR short-circuit:")
        this.counter = 0

        # This should NOT call the method (true || ...)
        result1: Bool = true || this.incrementAndReturnTrue()
        io.print("true || method() = %o, counter = %d", result1, this.counter)

        # This SHOULD call the method (false || ...)
        result2: Bool = false || this.incrementAndReturnTrue()
        io.print("false || method() = %o, counter = %d", result2, this.counter)
    }
}

# Usage
demo: ShortCircuitDemo = new ShortCircuitDemo()
demo.demonstrateAndShortCircuit()
demo.demonstrateOrShortCircuit()
```

---

## Comparison Operations

```o2l
# Boolean equality and inequality
bool1: Bool = true
bool2: Bool = false
bool3: Bool = true

# Equality comparison
equal1: Bool = bool1 == bool3       # true
equal2: Bool = bool1 == bool2       # false
not_equal: Bool = bool1 != bool2    # true

io.print("%o == %o is %o", bool1, bool3, equal1)
io.print("%o == %o is %o", bool1, bool2, equal2)
io.print("%o != %o is %o", bool1, bool2, not_equal)

# Boolean values in comparisons
values: List<Bool> = [true, false, true, false, true]
true_count: Int = 0
false_count: Int = 0

iterator: ListIterator = values.iterator()
while (iterator.hasNext()) {
    value: Bool = iterator.next()
    if (value == true) {
        true_count = true_count + 1
    } else {
        false_count = false_count + 1
    }
}

io.print("True values: %d, False values: %d", true_count, false_count)
```

---

## Type Conversion Methods

### `toString() → Text`

Converts the boolean to its string representation ("true" or "false").

```o2l
# String conversion
bool_values: List<Bool> = [true, false]
bool_iterator: ListIterator = bool_values.iterator()

while (bool_iterator.hasNext()) {
    bool_val: Bool = bool_iterator.next()
    text_val: Text = bool_val.toString()
    io.print("Bool %o as text: '%s'", bool_val, text_val)
}

# Use in string building
is_logged_in: Bool = true
status_message: Text = "User logged in: " + is_logged_in.toString()
io.print(status_message)  # "User logged in: true"

# Conditional messages based on boolean
method formatStatus(active: Bool): Text {
    return "System is " + active.toString()
}

system_active: Bool = false
message: Text = formatStatus(system_active)
io.print(message)  # "System is false"
```

### `toInt() → Int`

Converts the boolean to an integer (false = 0, true = 1).

```o2l
# Boolean to integer conversion
bool_states: List<Bool> = [true, false, true, true, false]
sum: Int = 0

state_iterator: ListIterator = bool_states.iterator()
while (state_iterator.hasNext()) {
    state: Bool = state_iterator.next()
    int_value: Int = state.toInt()
    sum = sum + int_value
    io.print("Bool %o as Int: %d", state, int_value)
}

io.print("Sum of boolean values: %d", sum)  # Count of true values

# Use in calculations
success_rate: Bool = true
failure_rate: Bool = false
total_attempts: Int = 10

successes: Int = success_rate.toInt() * 5  # 5 if true, 0 if false
failures: Int = failure_rate.toInt() * 5   # 0 if false, 5 if true

io.print("Successes: %d, Failures: %d", successes, failures)
```

### `toLong() → Long`

Converts the boolean to a Long integer (false = 0, true = 1).

```o2l
# Boolean to Long conversion
bool_flags: List<Bool> = [true, false, true]
flag_iterator: ListIterator = bool_flags.iterator()

while (flag_iterator.hasNext()) {
    flag: Bool = flag_iterator.next()
    long_value: Long = flag.toLong()
    io.print("Bool %o as Long: %s", flag, long_value.toString())
}

# Use in large-scale calculations
event_occurred: Bool = true
timestamp_multiplier: Long = event_occurred.toLong() * 1609459200  # Unix timestamp
io.print("Timestamp multiplier: %s", timestamp_multiplier.toString())
```

### `toFloat() → Float`

Converts the boolean to a float (false = 0.0, true = 1.0).

```o2l
# Boolean to Float conversion
conditions: List<Bool> = [true, false, true, false]
weight_total: Float = 0.0f

condition_iterator: ListIterator = conditions.iterator()
while (condition_iterator.hasNext()) {
    condition: Bool = condition_iterator.next()
    weight: Float = condition.toFloat() * 2.5f  # Each true condition worth 2.5
    weight_total = weight_total + weight
    io.print("Condition %o contributes weight: %f", condition, weight)
}

io.print("Total weight: %f", weight_total)

# Use in percentage calculations
feature_enabled: Bool = true
feature_weight: Float = feature_enabled.toFloat()  # 1.0 if enabled, 0.0 if disabled
percentage: Float = feature_weight * 100.0f
io.print("Feature usage: %f%%", percentage)
```

### `toDouble() → Double`

Converts the boolean to a double (false = 0.0, true = 1.0).

```o2l
# Boolean to Double conversion for high-precision calculations
measurements: List<Bool> = [true, true, false, true, false, false, true]
precision_sum: Double = 0.0

measurement_iterator: ListIterator = measurements.iterator()
while (measurement_iterator.hasNext()) {
    measurement: Bool = measurement_iterator.next()
    double_value: Double = measurement.toDouble()
    precision_sum = precision_sum + double_value
    io.print("Measurement %o as Double: %f", measurement, double_value)
}

total_measurements: Double = measurements.size().toDouble()
success_rate: Double = precision_sum / total_measurements * 100.0

io.print("Success rate: %f%% (%f out of %f)", success_rate, precision_sum, total_measurements)

# Scientific calculations with boolean states
experiment_success: Bool = true
success_factor: Double = experiment_success.toDouble()  # 1.0 or 0.0
confidence_interval: Double = success_factor * 0.95    # 95% if successful, 0% if failed
io.print("Confidence interval: %f", confidence_interval)
```

---

## Practical Examples

### Conditional Logic and Flow Control

```o2l
Object AccessControl {
    property is_authenticated: Bool
    property has_admin_role: Bool
    property account_active: Bool
    property within_hours: Bool

    constructor(auth: Bool, admin: Bool, active: Bool, hours: Bool) {
        this.is_authenticated = auth
        this.has_admin_role = admin
        this.account_active = active
        this.within_hours = hours
    }

    method canAccessSystem(): Bool {
        return this.is_authenticated && this.account_active
    }

    method canAccessAdminPanel(): Bool {
        return this.canAccessSystem() && this.has_admin_role
    }

    method canPerformAction(): Bool {
        return this.canAccessSystem() && this.within_hours
    }

    method getAccessLevel(): Text {
        if (!this.is_authenticated) {
            return "No access - not authenticated"
        }

        if (!this.account_active) {
            return "No access - account inactive"
        }

        if (this.has_admin_role) {
            return "Full admin access"
        }

        if (this.within_hours) {
            return "Standard user access"
        } else {
            return "Limited access - outside business hours"
        }
    }
}

# Usage
# Normal user during business hours
user1: AccessControl = new AccessControl(true, false, true, true)
io.print("User 1 can access system: %o", user1.canAccessSystem())
io.print("User 1 can access admin: %o", user1.canAccessAdminPanel())
io.print("User 1 access level: %s", user1.getAccessLevel())

# Admin user with inactive account
user2: AccessControl = new AccessControl(true, true, false, true)
io.print("User 2 can access system: %o", user2.canAccessSystem())
io.print("User 2 access level: %s", user2.getAccessLevel())
```

### State Machines and Flags

```o2l
Object DeviceStateMachine {
    property power_on: Bool
    property connected: Bool
    property calibrated: Bool
    property error_state: Bool

    constructor() {
        this.power_on = false
        this.connected = false
        this.calibrated = false
        this.error_state = false
    }

    method powerOn(): Bool {
        if (this.error_state) {
            io.print("Cannot power on - device in error state")
            return false
        }

        this.power_on = true
        io.print("Device powered on")
        return true
    }

    method connect(): Bool {
        if (!this.power_on) {
            io.print("Cannot connect - device not powered on")
            return false
        }

        this.connected = true
        io.print("Device connected")
        return true
    }

    method calibrate(): Bool {
        if (!this.connected) {
            io.print("Cannot calibrate - device not connected")
            return false
        }

        this.calibrated = true
        io.print("Device calibrated")
        return true
    }

    method isReady(): Bool {
        return this.power_on && this.connected && this.calibrated && !this.error_state
    }

    method getStatus(): Text {
        status: Text = "Device Status: "
        status = status + "Power=" + this.power_on.toString()
        status = status + ", Connected=" + this.connected.toString()
        status = status + ", Calibrated=" + this.calibrated.toString()
        status = status + ", Error=" + this.error_state.toString()
        status = status + ", Ready=" + this.isReady().toString()
        return status
    }

    method setError(error: Bool): Void {
        this.error_state = error
        if (error) {
            io.print("Device entered error state")
        } else {
            io.print("Device error cleared")
        }
    }
}

# Usage
device: DeviceStateMachine = new DeviceStateMachine()
io.print(device.getStatus())

# Attempt operations in sequence
device.powerOn()
io.print(device.getStatus())

device.connect()
io.print(device.getStatus())

device.calibrate()
io.print(device.getStatus())

io.print("Device ready for operation: %o", device.isReady())
```

### Validation and Error Checking

```o2l
Object DataValidator {
    method validateEmail(email: Text): Bool {
        # Simplified email validation
        has_at: Bool = email.contains("@")
        has_dot: Bool = email.contains(".")
        not_empty: Bool = email.length() > 0
        not_too_short: Bool = email.length() >= 5

        return has_at && has_dot && not_empty && not_too_short
    }

    method validatePassword(password: Text): Bool {
        min_length: Bool = password.length() >= 8
        has_upper: Bool = password != password.toLowerCase()  # Simplified check
        has_digit: Bool = password.contains("1") || password.contains("2") || password.contains("3") ||
                          password.contains("4") || password.contains("5") || password.contains("6") ||
                          password.contains("7") || password.contains("8") || password.contains("9") ||
                          password.contains("0")

        return min_length && has_upper && has_digit
    }

    method validateAge(age: Int): Bool {
        min_age: Bool = age >= 0
        max_age: Bool = age <= 150
        return min_age && max_age
    }

    method validateUserData(email: Text, password: Text, age: Int): Bool {
        email_valid: Bool = this.validateEmail(email)
        password_valid: Bool = this.validatePassword(password)
        age_valid: Bool = this.validateAge(age)

        io.print("Email valid: %o", email_valid)
        io.print("Password valid: %o", password_valid)
        io.print("Age valid: %o", age_valid)

        return email_valid && password_valid && age_valid
    }
}

# Usage
validator: DataValidator = new DataValidator()

# Test valid data
valid_result: Bool = validator.validateUserData("user@example.com", "SecurePass123", 25)
io.print("Valid user data: %o", valid_result)

# Test invalid data
invalid_result: Bool = validator.validateUserData("invalid-email", "weak", 200)
io.print("Invalid user data: %o", invalid_result)
```

### Feature Flags and Configuration

```o2l
Object FeatureFlags {
    property experimental_ui: Bool
    property beta_features: Bool
    property debug_mode: Bool
    property analytics_enabled: Bool
    property cache_enabled: Bool

    constructor() {
        # Default configuration
        this.experimental_ui = false
        this.beta_features = false
        this.debug_mode = false
        this.analytics_enabled = true
        this.cache_enabled = true
    }

    method enableExperimentalFeatures(): Void {
        this.experimental_ui = true
        this.beta_features = true
        this.debug_mode = true
        io.print("Experimental features enabled")
    }

    method enableProductionMode(): Void {
        this.experimental_ui = false
        this.beta_features = false
        this.debug_mode = false
        this.analytics_enabled = true
        this.cache_enabled = true
        io.print("Production mode enabled")
    }

    method shouldShowFeature(feature_name: Text): Bool {
        if (feature_name == "new_ui") {
            return this.experimental_ui
        } else if (feature_name == "advanced_analytics") {
            return this.beta_features && this.analytics_enabled
        } else if (feature_name == "debug_panel") {
            return this.debug_mode
        } else if (feature_name == "performance_cache") {
            return this.cache_enabled
        }
        return false  # Unknown feature
    }

    method getActiveFeatureCount(): Int {
        count: Int = 0

        if (this.experimental_ui) count = count + 1
        if (this.beta_features) count = count + 1
        if (this.debug_mode) count = count + 1
        if (this.analytics_enabled) count = count + 1
        if (this.cache_enabled) count = count + 1

        return count
    }

    method getConfigSummary(): Text {
        summary: Text = "Feature Configuration: "
        summary = summary + "UI=" + this.experimental_ui.toString()
        summary = summary + ", Beta=" + this.beta_features.toString()
        summary = summary + ", Debug=" + this.debug_mode.toString()
        summary = summary + ", Analytics=" + this.analytics_enabled.toString()
        summary = summary + ", Cache=" + this.cache_enabled.toString()

        active_count: Int = this.getActiveFeatureCount()
        summary = summary + " (" + active_count.toString() + " features active)"

        return summary
    }
}

# Usage
features: FeatureFlags = new FeatureFlags()
io.print(features.getConfigSummary())

io.print("Should show new UI: %o", features.shouldShowFeature("new_ui"))
io.print("Should show debug panel: %o", features.shouldShowFeature("debug_panel"))

# Enable experimental features
features.enableExperimentalFeatures()
io.print(features.getConfigSummary())

io.print("Should show new UI (after enabling): %o", features.shouldShowFeature("new_ui"))
io.print("Should show debug panel (after enabling): %o", features.shouldShowFeature("debug_panel"))
```

---

## Error Handling

```o2l
# Boolean operations are generally safe and don't throw exceptions
# However, here are some validation patterns

method safeBooleanOperation(condition1: Bool, condition2: Bool, operation: Text): Bool {
    if (operation == "and") {
        return condition1 && condition2
    } else if (operation == "or") {
        return condition1 || condition2
    } else if (operation == "xor") {
        return (condition1 && !condition2) || (!condition1 && condition2)
    } else {
        io.error("Unknown boolean operation: %s", operation)
        return false
    }
}

# Safe parsing from text
method parseBooleanSafely(text: Text): Bool {
    try {
        return text.toBool()
    } catch (error: Exception) {
        io.error("Invalid boolean format: %s", text)
        return false  # Default value
    }
}

# Validate boolean flags in configuration
method validateConfiguration(flags: List<Bool>): Bool {
    if (flags.size() == 0) {
        io.error("Configuration cannot be empty")
        return false
    }

    # Check for at least one true value
    has_true: Bool = false
    iterator: ListIterator = flags.iterator()
    while (iterator.hasNext()) {
        if (iterator.next() == true) {
            has_true = true
            break
        }
    }

    if (!has_true) {
        io.warn("Configuration has all false values")
    }

    return true
}
```

---

## Performance Considerations

1. **Memory Usage**: Bool uses 1 byte (8 bits) per value
2. **Arithmetic Speed**: Boolean operations are extremely fast
3. **Comparison Speed**: Boolean comparisons are among the fastest operations
4. **Logical Operations**: AND, OR, NOT are single CPU instructions
5. **Short-Circuit Evaluation**: Can improve performance by avoiding unnecessary evaluations
6. **Conversion Costs**: All boolean conversions are very low cost

---

## Best Practices

1. **Meaningful Names**: Use descriptive names (`is_valid`, `has_permission`, not `flag1`, `x`)
2. **Positive Logic**: Prefer positive boolean names (`is_enabled` vs `is_disabled`)
3. **Short-Circuit Advantage**: Order conditions in logical expressions for optimal performance
4. **Explicit Comparisons**: Write `if (condition)` not `if (condition == true)`
5. **Avoid Double Negatives**: Use positive logic where possible for readability
6. **State Management**: Use booleans for binary states in state machines
7. **Feature Flags**: Excellent for feature toggles and configuration options

---

## Common Patterns

```o2l
# Guard clauses
method processData(data: Text): Text {
    is_valid: Bool = data.length() > 0
    if (!is_valid) {
        return "Invalid data"
    }

    # Process data...
    return "Processed: " + data
}

# Boolean flags as method parameters
method createUser(name: Text, send_welcome_email: Bool, activate_immediately: Bool): Text {
    result: Text = "Created user: " + name

    if (send_welcome_email) {
        result = result + " (welcome email sent)"
    }

    if (activate_immediately) {
        result = result + " (activated)"
    } else {
        result = result + " (pending activation)"
    }

    return result
}

# Boolean accumulation patterns
method allTrue(conditions: List<Bool>): Bool {
    iterator: ListIterator = conditions.iterator()
    while (iterator.hasNext()) {
        if (!iterator.next()) {
            return false
        }
    }
    return true
}

method anyTrue(conditions: List<Bool>): Bool {
    iterator: ListIterator = conditions.iterator()
    while (iterator.hasNext()) {
        if (iterator.next()) {
            return true
        }
    }
    return false
}
```

---

## See Also

- [Int API Reference](Int.md) - Integer type for boolean conversions
- [Long API Reference](Long.md) - Long integer type
- [Float API Reference](Float.md) - Floating-point type
- [Double API Reference](Double.md) - Double-precision type
- [Text API Reference](Text.md) - String representation and parsing
- [Control Flow](../../language-reference/control-flow.md) - Using booleans in conditionals
