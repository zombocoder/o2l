# Operators API Reference

## Overview

O²L provides a comprehensive set of operators for arithmetic, logical, comparison, assignment, and other operations. All operators are type-safe and follow standard precedence rules. This reference covers all available operators with examples, precedence information, and best practices.

## Quick Reference

```o2l
# Arithmetic operators
sum: Int = 10 + 5       # Addition
diff: Int = 10 - 5      # Subtraction
prod: Int = 10 * 5      # Multiplication
quot: Int = 10 / 5      # Division
rem: Int = 10 % 3       # Modulo

# Comparison operators
equal: Bool = 10 == 10      # Equality
not_equal: Bool = 10 != 5   # Inequality
less: Bool = 5 < 10         # Less than
greater: Bool = 10 > 5      # Greater than

# Logical operators
and_result: Bool = true && false    # Logical AND
or_result: Bool = true || false     # Logical OR
not_result: Bool = !true            # Logical NOT
```

---

## Arithmetic Operators

### `+` (Addition)

Adds two numeric values together.

```o2l
# Integer addition
int_sum: Int = 42 + 18              # 60
negative_sum: Int = -10 + 5         # -5
zero_sum: Int = 100 + (-100)        # 0

# Float addition
float_sum: Float = 3.14f + 2.86f    # 5.99999... (float precision)
mixed_sum: Float = 5 + 2.5f         # 7.5f (Int promoted to Float)

# Double addition
double_sum: Double = 3.14159 + 2.71828  # High precision
precise_sum: Double = 1.0 + 1e-15       # 1.000000000000001

# Long addition
long_sum: Long = 123456789012345 + 987654321098765

io.print("Integer sum: %d", int_sum)
io.print("Float sum: %f", float_sum)
io.print("Double sum: %f", double_sum)
io.print("Long sum: %s", long_sum.toString())

# String concatenation (if supported)
# greeting: Text = "Hello " + "World"     # "Hello World"
```

### `-` (Subtraction and Unary Minus)

Subtracts one numeric value from another, or negates a value.

```o2l
# Binary subtraction
difference: Int = 50 - 20           # 30
negative_diff: Int = 10 - 15        # -5
self_subtract: Int = 42 - 42        # 0

# Unary minus (negation)
positive: Int = 42
negative: Int = -positive           # -42
double_negative: Int = -(-10)       # 10

# Float subtraction
float_diff: Float = 10.5f - 3.2f    # 7.3f
precision_loss: Float = 1.0f - 0.9f # 0.10000002f (float precision)

# Double subtraction
precise_diff: Double = 1.0 - 0.1    # 0.9 (exact in binary)
tiny_diff: Double = 1.0000001 - 1.0000000  # 1e-7

io.print("Subtraction: %d", difference)
io.print("Unary minus: %d", negative)
io.print("Float precision: %f", precision_loss)
io.print("Double precision: %f", precise_diff)
```

### `*` (Multiplication)

Multiplies two numeric values.

```o2l
# Integer multiplication
product: Int = 6 * 7                # 42
zero_product: Int = 42 * 0          # 0
negative_product: Int = -5 * 8      # -40

# Float multiplication
float_product: Float = 3.14f * 2.0f # 6.28f
area: Float = 3.14159f * 5.0f * 5.0f  # Circle area (π * r²)

# Double multiplication (high precision)
precise_product: Double = 3.141592653589793 * 2.718281828459045
very_small: Double = 1.23e-100 * 4.56e-200  # 5.6088e-300

# Long multiplication
big_product: Long = 123456789 * 987654321

# Overflow examples
large1: Int = 2000000000
large2: Int = 2000000000
# overflow_result: Int = large1 * large2  # May overflow!

io.print("Basic product: %d", product)
io.print("Float area: %f", area)
io.print("Precise product: %f", precise_product)
io.print("Very small: %e", very_small)
```

### `/` (Division)

Divides one numeric value by another.

```o2l
# Integer division (truncates toward zero)
int_division: Int = 15 / 4          # 3 (not 3.75)
exact_division: Int = 20 / 4        # 5
negative_div: Int = -15 / 4         # -3 (truncated)

# Float division
float_division: Float = 15.0f / 4.0f    # 3.75f
mixed_division: Float = 15 / 4.0f       # 3.75f (Int promoted)

# Double division (high precision)
precise_division: Double = 22.0 / 7.0  # 3.142857142857143
one_third: Double = 1.0 / 3.0          # 0.3333333333333333

# Division by zero handling
# zero_division: Int = 5 / 0          # Runtime error!
# inf_result: Float = 5.0f / 0.0f     # Positive infinity
# nan_result: Float = 0.0f / 0.0f     # NaN

# Safe division
method safeDivide(numerator: Int, denominator: Int): Int {
    if (denominator == 0) {
        io.error("Division by zero!")
        return 0
    }
    return numerator / denominator
}

safe_result: Int = safeDivide(10, 3)    # 3

io.print("Integer division: %d", int_division)
io.print("Float division: %f", float_division)
io.print("One third: %.15f", one_third)
io.print("Safe division: %d", safe_result)
```

### `%` (Modulo)

Returns the remainder after division.

```o2l
# Basic modulo operations
remainder1: Int = 15 % 4            # 3 (15 = 4*3 + 3)
remainder2: Int = 20 % 6            # 2 (20 = 6*3 + 2)
zero_remainder: Int = 12 % 4        # 0 (exact division)

# Negative numbers
negative_mod1: Int = -15 % 4        # -3 (result has sign of dividend)
negative_mod2: Int = 15 % -4        # 3
both_negative: Int = -15 % -4       # -3

# Common use cases
# Check if even or odd
number: Int = 17
remainder: Int = number % 2
is_even: Bool = remainder == 0
is_odd: Bool = remainder == 1
io.print("%d is even: %o, is odd: %o", number, is_even, is_odd)

# Cycle through values (0, 1, 2, 0, 1, 2, ...)
cycle_values: List<Int> = []
i: Int = 0
while (i < 10) {
    cycle_value: Int = i % 3
    cycle_values.add(cycle_value)
    i = i + 1
}

# Float modulo (if supported)
float_mod: Float = 7.5f % 2.0f      # 1.5f

# Modulo with zero (error)
# error_mod: Int = 10 % 0           # Runtime error!

io.print("Basic remainder: %d", remainder1)
io.print("Negative modulo: %d", negative_mod1)
io.print("Float modulo: %f", float_mod)
```

---

## Comparison Operators

### `==` (Equality)

Tests if two values are equal.

```o2l
# Numeric equality
int_equal: Bool = 42 == 42          # true
int_not_equal: Bool = 42 == 43      # false
zero_equal: Bool = 0 == 0           # true

# Float equality (be careful!)
float_equal: Bool = 3.14f == 3.14f  # true
float_calc: Float = 0.1f + 0.2f
float_expected: Float = 0.3f
float_eq_problem: Bool = float_calc == float_expected  # May be false!

# Double equality
double_equal: Bool = 3.14159 == 3.14159  # true
double_precision: Double = 1.0 / 3.0 * 3.0
double_one: Double = 1.0
double_eq_problem: Bool = double_precision == double_one  # May be false!

# Boolean equality
bool_equal: Bool = true == true     # true
bool_not_equal: Bool = true == false # false

# Text equality (if available)
# text_equal: Bool = "hello" == "hello"    # true
# text_not_equal: Bool = "hello" == "world" # false

# Safe float comparison
method floatEquals(a: Float, b: Float, epsilon: Float): Bool {
    diff: Float = a - b
    if (diff < 0.0f) diff = -diff
    return diff < epsilon
}

epsilon: Float = 0.0001f
safe_float_eq: Bool = floatEquals(float_calc, float_expected, epsilon)

io.print("Integer equality: %o", int_equal)
io.print("Float equality problem: %o", float_eq_problem)
io.print("Safe float equality: %o", safe_float_eq)
```

### `!=` (Inequality)

Tests if two values are not equal.

```o2l
# Basic inequality
not_equal: Bool = 5 != 3            # true
equal_values: Bool = 10 != 10       # false

# Inequality with different types
int_float_ne: Bool = 5 != 5.0f      # false (5 promoted to 5.0f)
precision_ne: Bool = 5 != 5.00001   # true

# Boolean inequality
bool_ne: Bool = true != false       # true
same_bool_ne: Bool = true != true   # false

# Use in conditions
age: Int = 25
legal_age: Int = 18
if (age != legal_age) {
    io.print("Age %d is different from legal age %d", age, legal_age)
}

# Inequality in loops
counter: Int = 0
target: Int = 5
while (counter != target) {
    io.print("Counter: %d", counter)
    counter = counter + 1
}

io.print("Not equal result: %o", not_equal)
io.print("Bool inequality: %o", bool_ne)
```

### `<` (Less Than)

Tests if the left value is less than the right value.

```o2l
# Integer comparison
less_than: Bool = 5 < 10            # true
not_less: Bool = 10 < 5             # false
equal_not_less: Bool = 5 < 5        # false

# Float comparison
float_less: Bool = 3.14f < 3.15f    # true
precision_less: Bool = 1.0001f < 1.0002f  # true

# Negative numbers
negative_less: Bool = -10 < -5      # true (-10 is less than -5)
neg_pos_less: Bool = -5 < 10        # true

# Use in sorting/ordering
numbers: List<Int> = [5, 2, 8, 1, 9]

# Simple bubble sort demonstration
n: Int = numbers.size()
i: Int = 0
while (i < n - 1) {
    j: Int = 0
    while (j < n - i - 1) {
        if (numbers.get(j) > numbers.get(j + 1)) {
            # Swap elements
            temp: Int = numbers.get(j)
            numbers.set(j, numbers.get(j + 1))
            numbers.set(j + 1, temp)
        }
        j = j + 1
    }
    i = i + 1
}

# Range checking
value: Int = 15
min_value: Int = 10
max_value: Int = 20
in_range: Bool = (value >= min_value) && (value <= max_value)

io.print("Less than: %o", less_than)
io.print("Negative comparison: %o", negative_less)
io.print("Value in range: %o", in_range)
```

### `<=` (Less Than or Equal)

Tests if the left value is less than or equal to the right value.

```o2l
# Basic less than or equal
less_equal: Bool = 5 <= 10          # true
equal_less_equal: Bool = 5 <= 5     # true (equal case)
not_less_equal: Bool = 10 <= 5      # false

# Boundary checking
score: Int = 85
passing_grade: Int = 60
max_score: Int = 100

passed: Bool = score >= passing_grade        # true
valid_score: Bool = score <= max_score       # true
perfect_score: Bool = score <= max_score && score >= max_score  # false

# Age validation
age: Int = 18
min_age: Int = 0
max_age: Int = 150
voting_age: Int = 18

valid_age: Bool = age >= min_age && age <= max_age      # true
can_vote: Bool = age >= voting_age                      # true
senior_citizen: Bool = age >= 65                       # false

# Float boundaries
temperature: Float = 23.5f
freezing: Float = 0.0f
boiling: Float = 100.0f

above_freezing: Bool = temperature > freezing          # true
below_boiling: Bool = temperature < boiling            # true
liquid_water: Bool = temperature > freezing && temperature < boiling  # true

io.print("Less than or equal: %o", less_equal)
io.print("Passed test: %o", passed)
io.print("Can vote: %o", can_vote)
io.print("Water is liquid: %o", liquid_water)
```

### `>` (Greater Than)

Tests if the left value is greater than the right value.

```o2l
# Basic greater than
greater: Bool = 10 > 5              # true
not_greater: Bool = 5 > 10          # false
equal_not_greater: Bool = 5 > 5     # false

# Performance comparison
old_time: Double = 1.5              # seconds
new_time: Double = 1.2              # seconds
is_faster: Bool = old_time > new_time  # true (higher time means slower)

# Priority comparison
high_priority: Int = 1
low_priority: Int = 5
is_higher_priority: Bool = high_priority < low_priority  # true (lower number = higher priority)

# Size comparison
file_size: Long = 1024 * 1024 * 50  # 50 MB
size_limit: Long = 1024 * 1024 * 100  # 100 MB
exceeds_limit: Bool = file_size > size_limit  # false

# Temperature monitoring
current_temp: Float = 75.5f
high_temp_threshold: Float = 80.0f
overheating: Bool = current_temp > high_temp_threshold  # false

io.print("Greater than: %o", greater)
io.print("Performance improved: %o", is_faster)
io.print("File exceeds limit: %o", exceeds_limit)
io.print("System overheating: %o", overheating)
```

### `>=` (Greater Than or Equal)

Tests if the left value is greater than or equal to the right value.

```o2l
# Basic greater than or equal
greater_equal: Bool = 10 >= 10      # true (equal case)
clearly_greater: Bool = 15 >= 10    # true
not_greater_equal: Bool = 5 >= 10   # false

# Qualification checking
experience_years: Int = 5
min_experience: Int = 3
qualified: Bool = experience_years >= min_experience  # true

# Version comparison
current_version: Float = 2.1f
required_version: Float = 2.0f
compatible: Bool = current_version >= required_version  # true

# Budget checking
available_budget: Double = 5000.00
project_cost: Double = 4500.00
can_afford: Bool = available_budget >= project_cost  # true

# Inventory levels
stock_level: Int = 25
minimum_stock: Int = 10
reorder_threshold: Int = 20

sufficient_stock: Bool = stock_level >= minimum_stock  # true
needs_reorder: Bool = stock_level <= reorder_threshold  # false (25 > 20)

io.print("Greater or equal: %o", greater_equal)
io.print("Qualified for job: %o", qualified)
io.print("Version compatible: %o", compatible)
io.print("Can afford project: %o", can_afford)
io.print("Needs reorder: %o", needs_reorder)
```

---

## Logical Operators

### `&&` (Logical AND)

Returns true only if both operands are true. Uses short-circuit evaluation.

```o2l
# Basic AND operations
both_true: Bool = true && true      # true
one_false: Bool = true && false     # false
both_false: Bool = false && false   # false

# Conditional logic
age: Int = 25
has_license: Bool = true
can_drive: Bool = (age >= 18) && has_license  # true

# Multiple conditions
temperature: Float = 22.5f
humidity: Int = 45
is_comfortable: Bool = (temperature >= 20.0f && temperature <= 25.0f) && (humidity >= 30 && humidity <= 70)

# Short-circuit evaluation
counter: Int = 0

method incrementCounter(): Bool {
    counter = counter + 1
    return true
}

# This will NOT call incrementCounter() because false && ... short-circuits
result1: Bool = false && incrementCounter()
io.print("Counter after false &&: %d", counter)  # 0

# This WILL call incrementCounter() because true && ... doesn't short-circuit
result2: Bool = true && incrementCounter()
io.print("Counter after true &&: %d", counter)   # 1

# Validation chains
username: Text = "user123"
password: Text = "securepass"

valid_username: Bool = username.length() >= 4
valid_password: Bool = password.length() >= 8
credentials_valid: Bool = valid_username && valid_password

io.print("AND result: %o", both_true)
io.print("Can drive: %o", can_drive)
io.print("Comfortable conditions: %o", is_comfortable)
io.print("Credentials valid: %o", credentials_valid)
```

### `||` (Logical OR)

Returns true if at least one operand is true. Uses short-circuit evaluation.

```o2l
# Basic OR operations
one_true: Bool = true || false      # true
both_true: Bool = true || true      # true
both_false: Bool = false || false   # false

# Alternative conditions
is_weekend: Bool = true
is_holiday: Bool = false
no_work: Bool = is_weekend || is_holiday  # true

# Multiple exit conditions
temperature: Float = 35.0f
battery_low: Bool = false
shutdown_needed: Bool = (temperature > 30.0f) || battery_low  # true

# Short-circuit evaluation
call_count: Int = 0

method trackCall(): Bool {
    call_count = call_count + 1
    return false
}

# This will NOT call trackCall() because true || ... short-circuits
result1: Bool = true || trackCall()
io.print("Call count after true ||: %d", call_count)  # 0

# This WILL call trackCall() because false || ... doesn't short-circuit
result2: Bool = false || trackCall()
io.print("Call count after false ||: %d", call_count)  # 1

# Permission checking (user needs any of these permissions)
is_admin: Bool = false
is_moderator: Bool = true
is_owner: Bool = false
can_delete: Bool = is_admin || is_moderator || is_owner  # true

# Error conditions
disk_full: Bool = false
memory_low: Bool = false
network_error: Bool = true
system_alert: Bool = disk_full || memory_low || network_error  # true

io.print("OR result: %o", one_true)
io.print("No work day: %o", no_work)
io.print("System needs shutdown: %o", shutdown_needed)
io.print("Can delete content: %o", can_delete)
io.print("System alert needed: %o", system_alert)
```

### `!` (Logical NOT)

Returns the opposite boolean value of its operand.

```o2l
# Basic NOT operations
not_true: Bool = !true              # false
not_false: Bool = !false            # true

# Double negation
original: Bool = true
double_not: Bool = !!original       # true (back to original)

# Inverting conditions
is_logged_in: Bool = true
needs_login: Bool = !is_logged_in   # false

# Complex negation
age: Int = 16
has_permission: Bool = false
cannot_access: Bool = !(age >= 18 && has_permission)  # true

# Negating comparisons (De Morgan's laws)
a: Int = 5
b: Int = 10
# !(a > b) is equivalent to (a <= b)
not_greater: Bool = !(a > b)        # true
less_equal: Bool = a <= b           # true
equivalent: Bool = not_greater == less_equal  # true

# !(a == b && c == d) is equivalent to (a != b || c != d)
c: Int = 15
d: Int = 15
not_both_equal: Bool = !(a == b && c == d)  # true (a != b)
either_not_equal: Bool = (a != b) || (c != d)  # true
demorgan_equiv: Bool = not_both_equal == either_not_equal  # true

# Practical usage
system_healthy: Bool = true
show_warning: Bool = !system_healthy  # false

file_exists: Bool = false
show_error: Bool = !file_exists      # true

is_empty: Bool = false
has_content: Bool = !is_empty        # true

io.print("NOT true: %o", not_true)
io.print("Needs login: %o", needs_login)
io.print("Cannot access: %o", cannot_access)
io.print("De Morgan equivalent: %o", demorgan_equiv)
io.print("Show warning: %o", show_warning)
io.print("Has content: %o", has_content)
```

---

## Assignment Operators

### `=` (Simple Assignment)

Assigns a value to a variable.

```o2l
# Basic assignment
number: Int = 42                    # Declare and assign
text: Text = "Hello, World!"       # Text assignment
flag: Bool = true                   # Boolean assignment

# Reassignment (if variable is mutable)
mutable_counter: Int = 0
mutable_counter = 5                 # Reassign to 5
mutable_counter = mutable_counter + 1  # Now 6

# Multiple assignments (copying values)
original: Int = 100
copy: Int = original                # copy gets value 100
another_copy: Int = copy           # another_copy gets value 100

# Assignment in expressions
x: Int = (y: Int = 10) + 5         # If supported: y = 10, x = 15

# Object assignment
# person: Person = new Person("Alice", 30)
# other_person: Person = person    # Same object reference

# Assignment with type conversion
int_value: Int = 42
float_value: Float = int_value.toFloat()  # Explicit conversion
double_value: Double = int_value.toDouble()

io.print("Number: %d", number)
io.print("Mutable counter: %d", mutable_counter)
io.print("Copy value: %d", copy)
io.print("Float value: %f", float_value)
```

### `+=` (Addition Assignment)

Adds a value to a variable and assigns the result back.

```o2l
# Integer addition assignment
counter: Int = 10
counter += 5                        # counter = counter + 5, now 15
counter += 1                        # counter = counter + 1, now 16

# Float addition assignment
temperature: Float = 20.5f
temperature += 2.3f                 # temperature = temperature + 2.3f, now 22.8f

# Accumulator pattern
total: Int = 0
values: List<Int> = [10, 20, 30, 40, 50]

iterator: ListIterator = values.iterator()
while (iterator.hasNext()) {
    value: Int = iterator.next()
    total += value                  # Accumulate sum
}

# String concatenation assignment (if supported)
# message: Text = "Hello"
# message += " World"             # message = message + " World"
# message += "!"                  # message = message + "!", now "Hello World!"

# Compound operations
balance: Double = 1000.00
balance += 50.00                    # Deposit
balance += 25.50                    # Another deposit
# Final balance: 1075.50

io.print("Counter: %d", counter)
io.print("Temperature: %f", temperature)
io.print("Total sum: %d", total)
io.print("Final balance: $%.2f", balance)
```

### `-=` (Subtraction Assignment)

Subtracts a value from a variable and assigns the result back.

```o2l
# Integer subtraction assignment
score: Int = 100
score -= 15                         # score = score - 15, now 85
score -= 5                          # score = score - 5, now 80

# Float subtraction assignment
fuel_level: Float = 45.8f
fuel_level -= 12.3f                 # fuel_level = fuel_level - 12.3f, now 33.5f

# Countdown pattern
countdown: Int = 10
while (countdown > 0) {
    io.print("Countdown: %d", countdown)
    countdown -= 1                  # Decrement by 1
}

# Budget tracking
budget: Double = 5000.00
budget -= 1200.00                   # Expense 1
budget -= 350.75                    # Expense 2
budget -= 89.50                     # Expense 3
remaining_budget: Double = budget   # 3359.75

# Health/damage system
health: Int = 100
health -= 25                        # Take 25 damage, now 75
health -= 10                        # Take 10 damage, now 65

is_alive: Bool = health > 0         # true

io.print("Final score: %d", score)
io.print("Fuel remaining: %f liters", fuel_level)
io.print("Countdown completed")
io.print("Remaining budget: $%.2f", remaining_budget)
io.print("Health: %d, Alive: %o", health, is_alive)
```

### `*=` (Multiplication Assignment)

Multiplies a variable by a value and assigns the result back.

```o2l
# Integer multiplication assignment
multiplier: Int = 5
multiplier *= 3                     # multiplier = multiplier * 3, now 15
multiplier *= 2                     # multiplier = multiplier * 2, now 30

# Float multiplication assignment
radius: Float = 2.5f
radius *= 1.5f                      # radius = radius * 1.5f, now 3.75f

# Compound interest simulation
principal: Double = 1000.00
interest_rate: Double = 1.05        # 5% interest (multiply by 1.05)

year: Int = 0
while (year < 5) {
    principal *= interest_rate      # Apply compound interest
    io.print("Year %d: $%.2f", year + 1, principal)
    year += 1
}

# Scaling values
width: Int = 100
height: Int = 200
scale_factor: Int = 3

width *= scale_factor              # width = width * scale_factor, now 300
height *= scale_factor             # height = height * scale_factor, now 600

# Exponential growth simulation
bacteria_count: Long = 1000
bacteria_count *= 2                # Double every cycle
bacteria_count *= 2                # 4000
bacteria_count *= 2                # 8000

io.print("Final multiplier: %d", multiplier)
io.print("Final radius: %f", radius)
io.print("Final principal: $%.2f", principal)
io.print("Scaled dimensions: %d x %d", width, height)
io.print("Bacteria count: %s", bacteria_count.toString())
```

### `/=` (Division Assignment)

Divides a variable by a value and assigns the result back.

```o2l
# Integer division assignment
value: Int = 100
value /= 4                          # value = value / 4, now 25
value /= 5                          # value = value / 5, now 5

# Float division assignment
distance: Float = 120.0f
distance /= 3.0f                    # distance = distance / 3.0f, now 40.0f

# Average calculation
sum_values: Int = 150
count: Int = 5
sum_values /= count                 # Average: 150 / 5 = 30

# Halving pattern
large_number: Long = 1024
iterations: Int = 0
while (large_number > 1) {
    large_number /= 2               # Divide by 2 each iteration
    iterations += 1
    io.print("Iteration %d: %s", iterations, large_number.toString())
}

# Converting units
bytes: Long = 2048
bytes /= 1024                       # Convert to KB, now 2
# Could continue: bytes /= 1024 for MB, etc.

# Depreciation calculation
asset_value: Double = 10000.00
depreciation_factor: Double = 0.9   # Lose 10% value each year

year: Int = 0
while (year < 3) {
    asset_value *= depreciation_factor  # Note: this is multiplication by 0.9
    io.print("Year %d asset value: $%.2f", year + 1, asset_value)
    year += 1
}

# More direct depreciation with division
equipment_value: Double = 5000.00
equipment_value /= 1.1              # Reduce by dividing by 1.1

io.print("Final value: %d", value)
io.print("Final distance: %f", distance)
io.print("Average: %d", sum_values)
io.print("Binary reduction took %d iterations", iterations)
io.print("Bytes in KB: %s", bytes.toString())
io.print("Equipment value: $%.2f", equipment_value)
```

### `%=` (Modulo Assignment)

Applies modulo operation to a variable and assigns the result back.

```o2l
# Integer modulo assignment
number: Int = 17
number %= 5                         # number = number % 5, now 2

# Cycling through values
index: Int = 0
array_size: Int = 3
items: List<Text> = ["A", "B", "C"]

# Simulate cycling through array indices
cycle_count: Int = 0
while (cycle_count < 10) {
    current_item: Text = items.get(index)
    io.print("Cycle %d: Item[%d] = %s", cycle_count, index, current_item)

    index += 1
    index %= array_size             # Keep index within bounds [0, 1, 2]
    cycle_count += 1
}

# Time calculation (wrap around)
minutes: Int = 75
minutes %= 60                       # Convert to minutes within an hour: 15

seconds: Int = 3665
seconds %= 60                       # Get remaining seconds: 5

# Color cycling (RGB values)
red_value: Int = 300
red_value %= 256                    # Keep within 0-255 range: 44

# Hash bucket calculation
hash_value: Int = 12345
bucket_count: Int = 10
hash_value %= bucket_count          # Get bucket index: 5

# Even/odd checking pattern
test_number: Int = 23
test_number %= 2                    # Result: 1 (odd), 0 would be even
is_odd: Bool = test_number == 1

io.print("Final number: %d", number)
io.print("Minutes within hour: %d", minutes)
io.print("Remaining seconds: %d", seconds)
io.print("RGB red value: %d", red_value)
io.print("Hash bucket: %d", hash_value)
io.print("Is odd: %o", is_odd)
```

---

## Operator Precedence

Operators are evaluated in the following order (highest to lowest precedence):

```o2l
# Demonstration of operator precedence
Object OperatorPrecedence {
    method demonstratePrecedence(): Void {
        # 1. Unary operators: !, -, + (highest precedence)
        result1: Bool = !true && false      # !(true) && false = false && false = false
        result2: Int = -5 + 3               # (-5) + 3 = -2

        # 2. Multiplicative: *, /, %
        result3: Int = 2 + 3 * 4            # 2 + (3 * 4) = 2 + 12 = 14
        result4: Int = 20 / 4 + 2           # (20 / 4) + 2 = 5 + 2 = 7
        result5: Int = 15 - 8 % 3           # 15 - (8 % 3) = 15 - 2 = 13

        # 3. Additive: +, -
        result6: Int = 3 * 4 + 2 * 5        # (3 * 4) + (2 * 5) = 12 + 10 = 22
        result7: Int = 10 - 3 + 2           # (10 - 3) + 2 = 7 + 2 = 9

        # 4. Relational: <, <=, >, >=
        result8: Bool = 3 + 4 < 2 * 5       # (3 + 4) < (2 * 5) = 7 < 10 = true
        result9: Bool = 5 * 2 >= 8 + 1      # (5 * 2) >= (8 + 1) = 10 >= 9 = true

        # 5. Equality: ==, !=
        result10: Bool = 3 + 2 == 2 + 3     # (3 + 2) == (2 + 3) = 5 == 5 = true
        result11: Bool = 4 > 3 == 2 < 5     # (4 > 3) == (2 < 5) = true == true = true

        # 6. Logical AND: &&
        result12: Bool = true || false && false  # true || (false && false) = true || false = true
        result13: Bool = 5 > 3 && 2 < 4     # (5 > 3) && (2 < 4) = true && true = true

        # 7. Logical OR: || (lowest precedence)
        result14: Bool = false && true || true   # (false && true) || true = false || true = true

        # Using parentheses to override precedence
        override1: Int = (2 + 3) * 4        # 5 * 4 = 20 (vs 2 + 12 = 14)
        override2: Bool = (true || false) && false  # true && false = false (vs true || false = true)

        io.print("Precedence demonstrations:")
        io.print("!true && false = %o", result1)
        io.print("-5 + 3 = %d", result2)
        io.print("2 + 3 * 4 = %d", result3)
        io.print("20 / 4 + 2 = %d", result4)
        io.print("15 - 8 %% 3 = %d", result5)
        io.print("3 * 4 + 2 * 5 = %d", result6)
        io.print("10 - 3 + 2 = %d", result7)
        io.print("3 + 4 < 2 * 5 = %o", result8)
        io.print("5 * 2 >= 8 + 1 = %o", result9)
        io.print("3 + 2 == 2 + 3 = %o", result10)
        io.print("4 > 3 == 2 < 5 = %o", result11)
        io.print("true || false && false = %o", result12)
        io.print("5 > 3 && 2 < 4 = %o", result13)
        io.print("false && true || true = %o", result14)

        io.print("With parentheses:")
        io.print("(2 + 3) * 4 = %d", override1)
        io.print("(true || false) && false = %o", override2)
    }
}

# Usage
precedence: OperatorPrecedence = new OperatorPrecedence()
precedence.demonstratePrecedence()
```

---

## Advanced Operator Usage

### Chained Comparisons

```o2l
# Multiple comparison chains
age: Int = 25
valid_age: Bool = 18 <= age && age <= 65    # Age range check

score: Float = 85.5f
grade_a: Bool = score >= 90.0f              # A grade
grade_b: Bool = score >= 80.0f && score < 90.0f  # B grade
grade_c: Bool = score >= 70.0f && score < 80.0f  # C grade

temperature: Double = 23.7
comfort_zone: Bool = temperature >= 18.0 && temperature <= 26.0

io.print("Valid working age: %o", valid_age)
io.print("Grade B: %o", grade_b)
io.print("In comfort zone: %o", comfort_zone)
```

### Conditional Expressions (Ternary-like behavior)

```o2l
# Simulating ternary operator with if-else
method max(a: Int, b: Int): Int {
    if (a > b) {
        return a
    } else {
        return b
    }
}

method min(a: Int, b: Int): Int {
    if (a < b) {
        return a
    } else {
        return b
    }
}

# Usage
x: Int = 15
y: Int = 23
maximum: Int = max(x, y)        # 23
minimum: Int = min(x, y)        # 15

# Conditional assignment pattern
status: Text = ""
is_logged_in: Bool = true
if (is_logged_in) {
    status = "Welcome back!"
} else {
    status = "Please log in"
}

io.print("Maximum: %d", maximum)
io.print("Status: %s", status)
```

### Short-Circuit Evaluation Patterns

```o2l
Object ShortCircuitPatterns {
    property safe_operation_count: Int

    constructor() {
        this.safe_operation_count = 0
    }

    method safeOperation(): Bool {
        this.safe_operation_count += 1
        io.print("Safe operation called, count: %d", this.safe_operation_count)
        return true
    }

    method riskyOperation(): Bool {
        io.print("Risky operation called")
        return false
    }

    method demonstrateShortCircuit(): Void {
        # AND short-circuit: if first is false, second is not evaluated
        result1: Bool = false && this.safeOperation()
        io.print("false && safeOperation() = %o", result1)
        io.print("Safe operation count: %d", this.safe_operation_count)  # Still 0

        # OR short-circuit: if first is true, second is not evaluated
        result2: Bool = true || this.riskyOperation()
        io.print("true || riskyOperation() = %o", result2)
        io.print("Risky operation was not called")

        # Non-short-circuit cases
        result3: Bool = true && this.safeOperation()    # safeOperation() IS called
        io.print("true && safeOperation() = %o", result3)

        result4: Bool = false || this.safeOperation()   # safeOperation() IS called
        io.print("false || safeOperation() = %o", result4)

        io.print("Final safe operation count: %d", this.safe_operation_count)
    }
}

# Usage
patterns: ShortCircuitPatterns = new ShortCircuitPatterns()
patterns.demonstrateShortCircuit()
```

---

## Best Practices

### Type Safety and Conversions

```o2l
# Explicit type conversions for clarity
int_value: Int = 42
float_result: Float = int_value.toFloat() + 3.14f  # Explicit conversion

# Avoid implicit conversions in complex expressions
mixed_calc: Double = 5.toDouble() * 2.718281828  # Clear intent

# Use appropriate types for operations
precise_calc: Double = 1.0 / 3.0           # Use Double for precision
fast_calc: Float = 1.0f / 3.0f             # Use Float for performance
```

### Safe Comparison Patterns

```o2l
# Safe floating-point comparison
method safeFloatEquals(a: Float, b: Float): Bool {
    epsilon: Float = 0.0001f
    diff: Float = a - b
    if (diff < 0.0f) diff = -diff
    return diff < epsilon
}

# Range checking with validation
method isInRange(value: Int, min_val: Int, max_val: Int): Bool {
    return value >= min_val && value <= max_val
}

# Null-safe operations (conceptual - if null supported)
# method safeStringLength(text: Text?): Int {
#     return (text != null) ? text.length() : 0
# }
```

### Operator Chaining Guidelines

```o2l
# Use parentheses for clarity in complex expressions
complex_calc: Int = (a + b) * (c - d) / (e + f)

# Break complex expressions into steps
step1: Int = a + b
step2: Int = c - d
step3: Int = e + f
result: Int = step1 * step2 / step3

# Prefer readable conditions over clever tricks
# Good:
is_valid_user: Bool = is_authenticated && has_permission && account_active

# Less clear:
# all_conditions: Bool = is_authenticated && has_permission && account_active
```

---

## Common Pitfalls

### Floating-Point Comparison

```o2l
# WRONG: Direct floating-point comparison
wrong_comparison: Bool = (0.1f + 0.2f) == 0.3f  # May be false!

# RIGHT: Epsilon-based comparison
method floatEquals(a: Float, b: Float): Bool {
    return (a - b).abs() < 0.0001f  # Assuming abs() method exists
}

correct_comparison: Bool = floatEquals(0.1f + 0.2f, 0.3f)
```

### Integer Division Truncation

```o2l
# Integer division truncates
int_result: Int = 7 / 3         # 2, not 2.333...

# Use float division for fractional results
float_result: Float = 7.0f / 3.0f  # 2.333...
```

### Operator Precedence Confusion

```o2l
# Potentially confusing
confusing: Bool = true || false && false   # true (because && has higher precedence)

# Clear with parentheses
clear1: Bool = true || (false && false)    # true
clear2: Bool = (true || false) && false    # false
```

---

## See Also

- [Int API Reference](Int.md) - Integer type and arithmetic
- [Float API Reference](Float.md) - Floating-point operations
- [Double API Reference](Double.md) - High-precision floating-point
- [Bool API Reference](Bool.md) - Boolean logic and operations
- [Text API Reference](Text.md) - String operations and comparisons
- [Control Flow](../../language-reference/control-flow.md) - Using operators in conditionals and loops
