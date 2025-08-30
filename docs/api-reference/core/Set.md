# Set API Reference

## Overview

Set<T> is O²L's generic hash set implementation providing type-safe collections of unique elements with efficient membership testing, insertion, and deletion. Sets automatically eliminate duplicates and support iteration through SetIterator.

## Constructor & Literals

```o2l
# Set literal syntax (parentheses)
numbers: Set<Int> = (1, 2, 3, 4, 5)

# Empty set
empty: Set<Int> = ()

# Automatic duplicate elimination
with_duplicates: Set<Int> = (1, 2, 2, 3, 3, 3)  // Becomes (1, 2, 3)

# Different element types
names: Set<Text> = ("Alice", "Bob", "Charlie")
flags: Set<Bool> = (true, false)
```

## Quick Example

```o2l
numbers: Set<Int> = (1, 2, 3)
numbers.add(4)                      // (1, 2, 3, 4)
numbers.add(2)                      // Still (1, 2, 3, 4) - no duplicates
has_three: Bool = numbers.contains(3)  // true
size: Int = numbers.size()          // 4
```

---

## Core Operations

### `add(element: T) → Bool`

Adds an element to the set. Returns true if element was added (wasn't already present).

```o2l
numbers: Set<Int> = (1, 2, 3)
was_added: Bool = numbers.add(4)    // Returns true, set becomes (1, 2, 3, 4)
duplicate: Bool = numbers.add(2)    // Returns false, set unchanged
```

### `contains(element: T) → Bool`

Returns true if the set contains the specified element.

```o2l
numbers: Set<Int> = (1, 2, 3, 4, 5)
has_three: Bool = numbers.contains(3)   // Returns true
has_ten: Bool = numbers.contains(10)    // Returns false
```

### `remove(element: T) → Bool`

Removes an element from the set. Returns true if element was removed.

```o2l
numbers: Set<Int> = (1, 2, 3, 4, 5)
was_removed: Bool = numbers.remove(3)   // Returns true, set becomes (1, 2, 4, 5)
not_found: Bool = numbers.remove(10)    // Returns false, set unchanged
```

### `size() → Int`

Returns the number of elements in the set.

```o2l
numbers: Set<Int> = (1, 2, 3, 4, 5)
count: Int = numbers.size()         // Returns 5

empty: Set<Int> = ()
zero: Int = empty.size()            // Returns 0
```

---

## Set Operations

### `union(other: Set<T>) → Set<T>`

Returns a new set containing all elements from both sets.

```o2l
set1: Set<Int> = (1, 2, 3)
set2: Set<Int> = (3, 4, 5)
combined: Set<Int> = set1.union(set2)   // (1, 2, 3, 4, 5)
```

### `intersection(other: Set<T>) → Set<T>`

Returns a new set containing only elements present in both sets.

```o2l
set1: Set<Int> = (1, 2, 3, 4)
set2: Set<Int> = (3, 4, 5, 6)
common: Set<Int> = set1.intersection(set2)  // (3, 4)
```

### `difference(other: Set<T>) → Set<T>`

Returns a new set with elements in this set but not in other.

```o2l
set1: Set<Int> = (1, 2, 3, 4)
set2: Set<Int> = (3, 4, 5)
diff: Set<Int> = set1.difference(set2)     // (1, 2)
```

### `symmetricDifference(other: Set<T>) → Set<T>`

Returns a new set with elements in either set but not in both.

```o2l
set1: Set<Int> = (1, 2, 3)
set2: Set<Int> = (3, 4, 5)
sym_diff: Set<Int> = set1.symmetricDifference(set2)  // (1, 2, 4, 5)
```

---

## Set Relationships

### `isSubsetOf(other: Set<T>) → Bool`

Returns true if all elements of this set are in the other set.

```o2l
small: Set<Int> = (1, 2)
large: Set<Int> = (1, 2, 3, 4)
is_subset: Bool = small.isSubsetOf(large)   // Returns true
```

### `isSupersetOf(other: Set<T>) → Bool`

Returns true if this set contains all elements of the other set.

```o2l
large: Set<Int> = (1, 2, 3, 4)
small: Set<Int> = (1, 2)
is_superset: Bool = large.isSupersetOf(small)  // Returns true
```

### `isDisjointFrom(other: Set<T>) → Bool`

Returns true if the sets have no elements in common.

```o2l
set1: Set<Int> = (1, 2, 3)
set2: Set<Int> = (4, 5, 6)
disjoint: Bool = set1.isDisjointFrom(set2)     // Returns true

set3: Set<Int> = (3, 4, 5)
not_disjoint: Bool = set1.isDisjointFrom(set3) // Returns false
```

---

## Modification Operations

### `clear() → Void`

Removes all elements from the set.

```o2l
numbers: Set<Int> = (1, 2, 3, 4, 5)
numbers.clear()
// Set is now empty ()
size: Int = numbers.size()          // Returns 0
```

### `addAll(collection: List<T>) → Int`

Adds all elements from a list to the set. Returns number of elements added.

```o2l
numbers: Set<Int> = (1, 2)
to_add: List<Int> = [3, 4, 2]  // Note: 2 is duplicate
added_count: Int = numbers.addAll(to_add)   // Returns 2 (only 3 and 4 added)
// Set is now (1, 2, 3, 4)
```

### `removeAll(collection: List<T>) → Int`

Removes all elements found in the collection. Returns number removed.

```o2l
numbers: Set<Int> = (1, 2, 3, 4, 5)
to_remove: List<Int> = [2, 4, 6]  // Note: 6 not in set
removed_count: Int = numbers.removeAll(to_remove)  // Returns 2
// Set is now (1, 3, 5)
```

---

## Iterator Support

### `iterator() → SetIterator`

Returns an iterator for traversing the set.

```o2l
numbers: Set<Int> = (1, 2, 3, 4, 5)
set_iter: SetIterator = numbers.iterator()

while (set_iter.hasNext()) {
    value: Int = set_iter.next()
    io.print("Element: %d", value)
}
```

**SetIterator Methods:**

- `hasNext() → Bool`: Returns true if more elements exist
- `next() → T`: Returns the next element and advances position
- `reset() → Void`: Resets iterator to the beginning

---

## Conversion Operations

### `toList() → List<T>`

Returns a list containing all elements of the set.

```o2l
numbers: Set<Int> = (3, 1, 4, 2, 5)
as_list: List<Int> = numbers.toList()
// Order may vary: [3, 1, 4, 2, 5] or other permutation
```

### `toString() → Text`

Returns a string representation of the set.

```o2l
numbers: Set<Int> = (1, 2, 3)
text: Text = numbers.toString()    // "(1, 2, 3)" or similar
```

---

## Advanced Examples

### Working with Custom Types

```o2l
Record Person {
    name: Text
    age: Int
}

people: Set<Person> = (
    Person(name="Alice", age=30),
    Person(name="Bob", age=25),
    Person(name="Alice", age=30)  # Duplicate - automatically removed
)

has_alice: Bool = people.contains(Person(name="Alice", age=30))
people.add(Person(name="Charlie", age=35))

# Iterate through custom objects
iter: SetIterator = people.iterator()
while (iter.hasNext()) {
    person: Person = iter.next()
    io.print("Person: %s, Age: %d", person.name, person.age)
}
```

### Set-Based Algorithms

```o2l
# Find unique words in text
words: List<Text> = ["the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"]
unique_words: Set<Text> = ()

iter: ListIterator = words.iterator()
while (iter.hasNext()) {
    word: Text = iter.next()
    unique_words.add(word)
}

io.print("Unique words: %d", unique_words.size())  // 8 (no duplicate "the")
```

### Permission System

```o2l
# User permissions using sets
admin_perms: Set<Text> = ("read", "write", "delete", "admin")
user_perms: Set<Text> = ("read", "write")
guest_perms: Set<Text> = ("read")

# Check if user can perform admin action
can_admin: Bool = user_perms.contains("admin")  // false

# Combine permissions
all_perms: Set<Text> = admin_perms.union(user_perms)  // Same as admin_perms

# Find permissions user lacks
missing_perms: Set<Text> = admin_perms.difference(user_perms)  // ("delete", "admin")
```

---

## Performance Characteristics

| Operation             | Average Case | Worst Case | Notes                     |
| --------------------- | ------------ | ---------- | ------------------------- |
| `add(element)`        | O(1)         | O(n)       | Hash collision handling   |
| `contains(element)`   | O(1)         | O(n)       | Hash table lookup         |
| `remove(element)`     | O(1)         | O(n)       | Hash table deletion       |
| `size()`              | O(1)         | O(1)       | Cached value              |
| `union(other)`        | O(n + m)     | O(n + m)   | n, m = set sizes          |
| `intersection(other)` | O(min(n,m))  | O(n + m)   | Optimized for smaller set |
| `clear()`             | O(1)         | O(1)       | Efficient bulk deletion   |

---

## Type Safety Examples

```o2l
# ✅ Correct usage
numbers: Set<Int> = (1, 2, 3)
numbers.add(4)                      // Valid: adding Int to Set<Int>

names: Set<Text> = ("Alice", "Bob")
names.add("Charlie")                // Valid: adding Text to Set<Text>

# Set operations with same types
set1: Set<Int> = (1, 2, 3)
set2: Set<Int> = (3, 4, 5)
combined: Set<Int> = set1.union(set2)  // Valid: same element types

# ❌ Type errors (caught at compile time)
// numbers.add("text")              // Error: cannot add Text to Set<Int>
// mixed: Set<Int> = (1, "two")     // Error: mixed types not allowed
// wrong_union = set1.union(names)  // Error: different element types
```

---

## Memory Management

- **Hash Table**: Efficient hash-based storage
- **Load Factor**: Automatic capacity management
- **Duplicate Prevention**: No memory waste on duplicates
- **Reference Semantics**: Sets store references to objects
- **Garbage Collection**: Removed elements eligible for collection

---

## Error Handling

Sets handle edge cases gracefully:

```o2l
empty: Set<Int> = ()

# Operations on empty sets
empty.remove(1)                     // Returns false, no error
size: Int = empty.size()            // Returns 0

# Iterator on empty set
iter: SetIterator = empty.iterator()
has_next: Bool = iter.hasNext()     // Returns false
```

---

## Best Practices

1. **Unique Elements**: Remember sets automatically eliminate duplicates
2. **Equality**: Ensure custom types have proper equality semantics
3. **Iteration Order**: Don't rely on specific iteration order
4. **Set Operations**: Use mathematical set operations for clean logic
5. **Membership Testing**: Prefer `contains()` over converting to lists
6. **Memory Efficiency**: Use sets when uniqueness is important

---

## See Also

- [List API Reference](List.md) - For ordered collections allowing duplicates
- [Map API Reference](Map.md) - For key-value associations
- [Text API Reference](Text.md) - Common element type operations
- [Iterator Patterns](../../tutorials/intermediate/02-collections.md) - Advanced iteration techniques
