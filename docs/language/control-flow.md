# Control Flow

O²L Programming Language provides standard control flow constructs with clear syntax.

## If Statements

Conditional statements use parentheses around conditions:

```obq
if (value > 10) {
    io.print("Value is large")
} else {
    io.print("Value is small")
}
```

### Comparison Operators

```obq
if (a == b) { io.print("Equal") }
if (a != b) { io.print("Not equal") }
if (a > b) { io.print("Greater") }
if (a < b) { io.print("Less") }
if (a >= b) { io.print("Greater or equal") }
if (a <= b) { io.print("Less or equal") }
```

## While Loops

While loops require parentheses around the condition:

```obq
# Basic while loop
count: Int = 0
while (count < 5) {
    io.print("Count: %d", count)
    count = count + 1
}
```

### While with Iterators

Common pattern for iterating through collections:

```obq
numbers: List<Int> = [1, 2, 3, 4, 5]
iter: ListIterator = numbers.iterator()

while (iter.hasNext()) {
    value: Int = iter.next()
    io.print("Processing: %d", value)
}
```

### Nested Loops

```obq
outer: RepeatIterator = utils.repeat(3)
while (outer.hasNext()) {
    i: Int = outer.next()
    
    inner: RepeatIterator = utils.repeat(3)
    while (inner.hasNext()) {
        j: Int = inner.next()
        io.print("%d x %d = %d", i, j, i * j)
    }
}
```

## Break and Continue

Control loop execution with break and continue statements:

```obq
count: Int = 0
while (count < 10) {
    count = count + 1
    
    if (count == 5) {
        continue  # Skip iteration 5
    }
    
    if (count == 8) {
        break     # Exit loop at 8
    }
    
    io.print("Count: %d", count)
}
```