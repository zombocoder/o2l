# Iterator System

O²L Programming Language provides a powerful iterator system for traversing collections and generating sequences.

## ListIterator

Iterate through List collections using `ListIterator`:

```obq
import system.io

Object IteratorDemo {
    method basicIteration(): Int {
        numbers: List<Int> = [1, 2, 3, 4, 5]
        iter: ListIterator = numbers.iterator()
        
        while (iter.hasNext()) {
            value: Int = iter.next()
            io.print("Value: %d", value)
        }
        
        return 0
    }
}
```

## RepeatIterator

Generate sequences using `RepeatIterator` from `system.utils`:

```obq
import system.io
import system.utils

Object RepeatDemo {
    method basicRepeat(): Int {
        # Create iterator that counts from 0 to 4
        repeater: RepeatIterator = utils.repeat(5)
        
        while (repeater.hasNext()) {
            count: Int = repeater.next()
            io.print("Count: %d", count)
        }
        
        return 0
    }
}
```

## Iterator Methods

All iterators provide these methods:

- `hasNext()`: Returns `Bool` - true if more elements available
- `next()`: Returns next element and advances iterator
- `getCurrentCount()`: Returns current position (RepeatIterator)
- `reset()`: Resets iterator to beginning (RepeatIterator)

## RepeatIterator Features

```obq
import system.io
import system.utils

Object RepeatFeatures {
    method demonstrateFeatures(): Int {
        repeater: RepeatIterator = utils.repeat(3)
        
        # First iteration
        io.print("First run:")
        while (repeater.hasNext()) {
            val: Int = repeater.next()
            io.print("  Value: %d", val)
        }
        
        io.print("Count after first run: %d", repeater.getCurrentCount())
        
        # Reset and iterate again
        repeater.reset()
        io.print("After reset, hasNext: %o", repeater.hasNext())
        
        # Second iteration
        io.print("Second run:")
        while (repeater.hasNext()) {
            val: Int = repeater.next()
            io.print("  Value: %d", val)
        }
        
        return 0
    }
}
```

## Nested Iterators

Iterators can be nested for complex patterns:

```obq
import system.io
import system.utils

Object NestedDemo {
    method nestedIteration(): Int {
        outer: RepeatIterator = utils.repeat(3)
        
        while (outer.hasNext()) {
            i: Int = outer.next()
            io.print("Outer: %d", i)
            
            inner: RepeatIterator = utils.repeat(3)
            while (inner.hasNext()) {
                j: Int = inner.next()
                io.print("  Inner: %d", j)
            }
        }
        
        return 0
    }
}
```

## Iterator with Collections

Build collections while iterating:

```obq
import system.io
import system.utils

Object CollectionBuilder {
    method buildSquares(): Int {
        repeater: RepeatIterator = utils.repeat(5)
        squares: List<Int> = []
        
        while (repeater.hasNext()) {
            n: Int = repeater.next()
            square: Int = n * n
            squares.add(square)
            io.print("%d squared = %d", n, square)
        }
        
        io.print("Squares: %s", squares)
        return 0
    }
}
```

## Iterator Best Practices

- Always check `hasNext()` before calling `next()`
- Use appropriate iterator type for your use case
- Reset RepeatIterators when reusing them
- Prefer iterators over manual indexing for collections