# List API Reference

## Overview

List<T> is O²L's generic dynamic array implementation providing type-safe, ordered collections with efficient access and modification operations. Lists are mutable and support iteration through ListIterator.

## Constructor & Literals

```o2l
# List literal syntax
numbers: List<Int> = [1, 2, 3, 4, 5]

# Empty list
empty: List<Int> = []

# Generic lists with different types
names: List<Text> = ["Alice", "Bob", "Charlie"]
flags: List<Bool> = [true, false, true]
```

## Quick Example

```o2l
numbers: List<Int> = [1, 2, 3]
numbers.add(4)                    // [1, 2, 3, 4]
first: Int = numbers.get(0)       // 1
numbers.reverse()                 // [4, 3, 2, 1]
size: Int = numbers.size()        // 4
```

---

## Core Operations

### `add(element: T) → Int`

Adds an element to the end of the list and returns the new size.

```o2l
numbers: List<Int> = [1, 2, 3]
new_size: Int = numbers.add(4)    // Returns 4
// List is now [1, 2, 3, 4]
```

### `get(index: Int) → T`

Returns the element at the specified index (0-based).

```o2l
numbers: List<Int> = [10, 20, 30]
first: Int = numbers.get(0)       // Returns 10
second: Int = numbers.get(1)      // Returns 20
```

**Error Handling**: Throws `EvaluationError` if index is out of bounds.

### `set(index: Int, element: T) → T`

Sets the element at the specified index and returns the old value.

```o2l
numbers: List<Int> = [1, 2, 3]
old_value: Int = numbers.set(1, 20)  // Returns 2
// List is now [1, 20, 3]
```

### `size() → Int`

Returns the number of elements in the list.

```o2l
numbers: List<Int> = [1, 2, 3, 4, 5]
count: Int = numbers.size()       // Returns 5

empty: List<Int> = []
zero: Int = empty.size()          // Returns 0
```

---

## Modification Operations

### `remove(index: Int) → T`

Removes and returns the element at the specified index.

```o2l
numbers: List<Int> = [10, 20, 30, 40]
removed: Int = numbers.remove(2)  // Returns 30
// List is now [10, 20, 40]
```

### `pop() → T`

Removes and returns the last element in the list.

```o2l
numbers: List<Int> = [1, 2, 3, 4]
last: Int = numbers.pop()         // Returns 4
// List is now [1, 2, 3]
```

### `clear() → Void`

Removes all elements from the list.

```o2l
numbers: List<Int> = [1, 2, 3, 4, 5]
numbers.clear()
// List is now []
size: Int = numbers.size()        // Returns 0
```

### `reverse() → Void`

Reverses the order of elements in the list.

```o2l
numbers: List<Int> = [1, 2, 3, 4, 5]
numbers.reverse()
// List is now [5, 4, 3, 2, 1]
```

---

## Search Operations

### `contains(element: T) → Bool`

Returns true if the list contains the specified element.

```o2l
numbers: List<Int> = [1, 2, 3, 4, 5]
has_three: Bool = numbers.contains(3)  // Returns true
has_ten: Bool = numbers.contains(10)   // Returns false
```

### `indexOf(element: T) → Int`

Returns the index of the first occurrence of the element, or -1 if not found.

```o2l
numbers: List<Int> = [10, 20, 30, 20]
index: Int = numbers.indexOf(20)       // Returns 1 (first occurrence)
missing: Int = numbers.indexOf(99)     // Returns -1
```

### `lastIndexOf(element: T) → Int`

Returns the index of the last occurrence of the element, or -1 if not found.

```o2l
numbers: List<Int> = [10, 20, 30, 20]
last_index: Int = numbers.lastIndexOf(20)  // Returns 3 (last occurrence)
```

---

## Iterator Support

### `iterator() → ListIterator`

Returns an iterator for traversing the list.

```o2l
numbers: List<Int> = [10, 20, 30, 40, 50]
iter: ListIterator = numbers.iterator()

while (iter.hasNext()) {
    value: Int = iter.next()
    index: Int = iter.index()  // Current position after next()
    io.print("Index %d: %d", index, value)
}
```

**ListIterator Methods:**

- `hasNext() → Bool`: Returns true if more elements exist
- `next() → T`: Returns the next element and advances position
- `index() → Int`: Returns current position (after last next() call)
- `reset() → Void`: Resets iterator to the beginning

---

## Bulk Operations

### `addAll(other: List<T>) → Int`

Adds all elements from another list and returns the new size.

```o2l
list1: List<Int> = [1, 2, 3]
list2: List<Int> = [4, 5, 6]
new_size: Int = list1.addAll(list2)   // Returns 6
// list1 is now [1, 2, 3, 4, 5, 6]
```

### `removeAll(elements: List<T>) → Int`

Removes all occurrences of elements in the given list, returns count removed.

```o2l
numbers: List<Int> = [1, 2, 3, 2, 4, 2]
to_remove: List<Int> = [2]
removed_count: Int = numbers.removeAll(to_remove)  // Returns 3
// numbers is now [1, 3, 4]
```

### `retainAll(elements: List<T>) → Int`

Retains only elements in the given list, returns count retained.

```o2l
numbers: List<Int> = [1, 2, 3, 4, 5]
keep: List<Int> = [2, 4, 6]
retained: Int = numbers.retainAll(keep)  // Returns 2
// numbers is now [2, 4]
```

---

## Conversion Operations

### `toArray() → Array<T>`

Returns a fixed-size array with the list's elements.

```o2l
numbers: List<Int> = [1, 2, 3, 4]
array: Array<Int> = numbers.toArray()
```

### `toString() → Text`

Returns a string representation of the list.

```o2l
numbers: List<Int> = [1, 2, 3]
text: Text = numbers.toString()    // "[1, 2, 3]"
```

---

## Advanced Operations

### `sort() → Void`

Sorts the list in ascending order (requires comparable elements).

```o2l
numbers: List<Int> = [5, 2, 8, 1, 9]
numbers.sort()
// numbers is now [1, 2, 5, 8, 9]

names: List<Text> = ["Charlie", "Alice", "Bob"]
names.sort()
// names is now ["Alice", "Bob", "Charlie"]
```

### `sortDescending() → Void`

Sorts the list in descending order.

```o2l
numbers: List<Int> = [5, 2, 8, 1, 9]
numbers.sortDescending()
// numbers is now [9, 8, 5, 2, 1]
```

### `slice(start: Int, end: Int) → List<T>`

Returns a new list containing elements from start to end (exclusive).

```o2l
numbers: List<Int> = [1, 2, 3, 4, 5]
subset: List<Int> = numbers.slice(1, 4)  // [2, 3, 4]
// Original list unchanged
```

### `copy() → List<T>`

Returns a shallow copy of the list.

```o2l
original: List<Int> = [1, 2, 3]
copy: List<Int> = original.copy()
copy.add(4)
// original is still [1, 2, 3]
// copy is [1, 2, 3, 4]
```

---

## Type Safety Examples

```o2l
# ✅ Correct type usage
numbers: List<Int> = [1, 2, 3]
numbers.add(4)                    // Valid: adding Int to List<Int>

names: List<Text> = ["Alice", "Bob"]
names.add("Charlie")              // Valid: adding Text to List<Text>

# ❌ Type errors (caught at compile time)
// numbers.add("text")           // Error: cannot add Text to List<Int>
// mixed: List<Int> = [1, "two"] // Error: mixed types not allowed
```

---

## Complex Usage Examples

### Working with Custom Types

```o2l
Record Person {
    name: Text
    age: Int
}

people: List<Person> = [
    Person(name="Alice", age=30),
    Person(name="Bob", age=25)
]

people.add(Person(name="Charlie", age=35))

# Iterate through custom objects
iter: ListIterator = people.iterator()
while (iter.hasNext()) {
    person: Person = iter.next()
    io.print("Name: %s, Age: %d", person.name, person.age)
}
```

### List of Lists

```o2l
matrix: List<List<Int>> = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

first_row: List<Int> = matrix.get(0)
first_element: Int = first_row.get(0)  // Gets 1

# Add a new row
new_row: List<Int> = [10, 11, 12]
matrix.add(new_row)
```

### List Operations Chaining

```o2l
# Multiple operations in sequence
numbers: List<Int> = [5, 2, 8, 1, 9, 2]
numbers.removeAll([2])      // Remove all 2's
numbers.sort()              // Sort ascending
numbers.reverse()           // Reverse to descending
last: Int = numbers.pop()   // Remove and get last element
```

---

## Performance Characteristics

| Operation             | Time Complexity | Notes                       |
| --------------------- | --------------- | --------------------------- |
| `add(element)`        | O(1) amortized  | May trigger resize          |
| `get(index)`          | O(1)            | Direct array access         |
| `set(index, element)` | O(1)            | Direct array assignment     |
| `remove(index)`       | O(n)            | Shifts elements             |
| `size()`              | O(1)            | Cached value                |
| `contains(element)`   | O(n)            | Linear search               |
| `indexOf(element)`    | O(n)            | Linear search               |
| `reverse()`           | O(n)            | In-place reversal           |
| `sort()`              | O(n log n)      | Efficient sorting algorithm |

---

## Memory Management

- **Dynamic Resizing**: Lists automatically grow as needed
- **Memory Efficiency**: Unused capacity is managed automatically
- **Reference Semantics**: Lists store references to objects, not copies
- **Garbage Collection**: Removed elements are eligible for garbage collection

---

## Error Handling

Common exceptions thrown by List operations:

```o2l
numbers: List<Int> = [1, 2, 3]

try {
    value: Int = numbers.get(10)  // Index out of bounds
} catch (error: EvaluationError) {
    io.print("Error: %s", error.message)
}

try {
    empty: List<Int> = []
    value: Int = empty.pop()      // Pop from empty list
} catch (error: EvaluationError) {
    io.print("Cannot pop from empty list")
}
```

---

## See Also

- [Text API Reference](Text.md) - For working with string collections
- [Map API Reference](Map.md) - For key-value collections
- [Set API Reference](Set.md) - For unique element collections
- [Iterator Pattern](../../tutorials/intermediate/02-collections.md) - Advanced iteration techniques
- [Control Flow](../../language-reference/control-flow.md) - Using lists in loops
