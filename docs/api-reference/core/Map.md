# Map API Reference

## Overview

Map<K,V> is O²L's generic hash table implementation providing type-safe key-value associations with efficient lookup, insertion, and deletion operations. Maps are mutable and maintain insertion order, supporting iteration through MapIterator.

## Constructor & Literals

```o2l
# Map literal syntax with type inference
scores: Map<Text, Int> = {
    "Alice": 95,
    "Bob": 87,
    "Charlie": 92
}

# Empty map
empty: Map<Text, Int> = {}

# Different key-value type combinations
user_roles: Map<Text, Text> = {
    "alice": "admin",
    "bob": "user",
    "charlie": "moderator"
}

flags: Map<Text, Bool> = {
    "debug": true,
    "production": false
}
```

## Quick Example

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
scores.put("Charlie", 92)           // Add new entry
alice_score: Int = scores.get("Alice")  // 95
has_bob: Bool = scores.contains("Bob")  // true
size: Int = scores.size()           // 3
```

---

## Core Operations

### `put(key: K, value: V) → V`

Inserts or updates a key-value pair and returns the previous value (or null for new keys).

```o2l
scores: Map<Text, Int> = {"Alice": 95}

# Add new entry
old_value: Optional<Int> = scores.put("Bob", 87)  // Returns null (new key)

# Update existing entry
old_score: Optional<Int> = scores.put("Alice", 100)  // Returns 95 (old value)
```

### `get(key: K) → V`

Returns the value associated with the key.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
alice_score: Int = scores.get("Alice")  // Returns 95
```

**Error Handling**: Throws `EvaluationError` if key is not found.

### `getOrDefault(key: K, defaultValue: V) → V`

Returns the value for the key, or the default value if key not found.

```o2l
scores: Map<Text, Int> = {"Alice": 95}
bob_score: Int = scores.getOrDefault("Bob", 0)  // Returns 0
alice_score: Int = scores.getOrDefault("Alice", 0)  // Returns 95
```

### `contains(key: K) → Bool`

Returns true if the map contains the specified key.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
has_alice: Bool = scores.contains("Alice")  // Returns true
has_charlie: Bool = scores.contains("Charlie")  // Returns false
```

### `size() → Int`

Returns the number of key-value pairs in the map.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87, "Charlie": 92}
count: Int = scores.size()  // Returns 3

empty: Map<Text, Int> = {}
zero: Int = empty.size()    // Returns 0
```

---

## Modification Operations

### `remove(key: K) → V`

Removes the key-value pair and returns the removed value.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87, "Charlie": 92}
removed_score: Int = scores.remove("Bob")  // Returns 87
// Map is now {"Alice": 95, "Charlie": 92}
```

### `clear() → Void`

Removes all key-value pairs from the map.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
scores.clear()
// Map is now {}
size: Int = scores.size()  // Returns 0
```

### `empty() → Bool`

Returns true if the map contains no key-value pairs.

```o2l
scores: Map<Text, Int> = {}
is_empty: Bool = scores.empty()  // Returns true

scores.put("Alice", 95)
is_empty_now: Bool = scores.empty()  // Returns false
```

---

## Collection Views

### `keys() → List<K>`

Returns a list of all keys in the map.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87, "Charlie": 92}
all_keys: List<Text> = scores.keys()  // ["Alice", "Bob", "Charlie"]

key_count: Int = all_keys.size()  // 3
first_key: Text = all_keys.get(0)  // "Alice"
```

### `values() → List<V>`

Returns a list of all values in the map.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87, "Charlie": 92}
all_values: List<Int> = scores.values()  // [95, 87, 92]

value_count: Int = all_values.size()  // 3
first_value: Int = all_values.get(0)  // 95
```

### `entrySet() → List<MapEntry<K,V>>`

Returns a list of all key-value pairs as MapEntry objects.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
entries: List<MapEntry<Text, Int>> = scores.entrySet()

first_entry: MapEntry<Text, Int> = entries.get(0)
key: Text = first_entry.getKey()      // "Alice"
value: Int = first_entry.getValue()   // 95
```

---

## Iterator Support

### `iterator() → MapIterator`

Returns an iterator for traversing the map in multiple ways.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87, "Charlie": 92}
map_iter: MapIterator = scores.iterator()

# Iterate through complete entries
while (map_iter.hasNext()) {
    entry: MapObject = map_iter.MapItem()
    key: Text = entry.getKey()
    value: Int = entry.getVal()
    io.print("%s: %d", key, value)
}
```

**MapIterator Methods:**

#### Key-Only Iteration

```o2l
map_iter: MapIterator = scores.iterator()
while (map_iter.hasNext()) {
    key: Text = map_iter.nextKey()
    io.print("Key: %s", key)
}
```

#### Value-Only Iteration

```o2l
map_iter.reset()  // Reset to beginning
while (map_iter.hasNext()) {
    value: Int = map_iter.nextValue()
    io.print("Value: %d", value)
}
```

#### Entry String Iteration

```o2l
map_iter.reset()
while (map_iter.hasNext()) {
    entry_string: Text = map_iter.nextEntry()  // "key=value" format
    io.print("Entry: %s", entry_string)
}
```

#### Iterator Control Methods

- `hasNext() → Bool`: Returns true if more elements exist
- `reset() → Void`: Resets iterator to the beginning
- `getCurrentIndex() → Int`: Returns current iterator position

---

## Advanced Operations

### `merge(other: Map<K,V>) → Void`

Merges all entries from another map, updating existing keys.

```o2l
map1: Map<Text, Int> = {"Alice": 95, "Bob": 87}
map2: Map<Text, Int> = {"Bob": 90, "Charlie": 92}
map1.merge(map2)
// map1 is now {"Alice": 95, "Bob": 90, "Charlie": 92}
```

### `putAll(other: Map<K,V>) → Void`

Adds all entries from another map (alias for merge).

```o2l
scores: Map<Text, Int> = {"Alice": 95}
additional_scores: Map<Text, Int> = {"Bob": 87, "Charlie": 92}
scores.putAll(additional_scores)
// scores is now {"Alice": 95, "Bob": 87, "Charlie": 92}
```

### `containsValue(value: V) → Bool`

Returns true if the map contains the specified value.

```o2l
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
has_95: Bool = scores.containsValue(95)  // Returns true
has_100: Bool = scores.containsValue(100)  // Returns false
```

---

## Type Safety Examples

```o2l
# ✅ Correct type usage
scores: Map<Text, Int> = {"Alice": 95, "Bob": 87}
scores.put("Charlie", 92)            // Valid: Text key, Int value

user_flags: Map<Text, Bool> = {"admin": true, "guest": false}
user_flags.put("moderator", true)    // Valid: Text key, Bool value

# ❌ Type errors (caught at compile time)
// scores.put("Alice", "ninety-five")  // Error: Text value for Int map
// scores.put(123, 95)                 // Error: Int key for Text map
// mixed_keys: Map<Text, Int> = {1: "one"}  // Error: wrong key type
```

---

## Complex Usage Examples

### Working with Custom Types

```o2l
Record Person {
    name: Text
    age: Int
}

# Map with custom value types
people: Map<Text, Person> = {
    "emp001": Person(name="Alice", age=30),
    "emp002": Person(name="Bob", age=25)
}

alice: Person = people.get("emp001")
people.put("emp003", Person(name="Charlie", age=35))

# Iterate through custom objects
iter: MapIterator = people.iterator()
while (iter.hasNext()) {
    entry: MapObject = iter.MapItem()
    id: Text = entry.getKey()
    person: Person = entry.getVal()
    io.print("ID: %s, Name: %s, Age: %d", id, person.name, person.age)
}
```

### Nested Maps

```o2l
# Map of maps for hierarchical data
departments: Map<Text, Map<Text, Int>> = {
    "engineering": {"alice": 95, "bob": 87},
    "marketing": {"charlie": 92, "diana": 88}
}

engineering_scores: Map<Text, Int> = departments.get("engineering")
alice_score: Int = engineering_scores.get("alice")  // 95

# Add new department
hr_scores: Map<Text, Int> = {"eve": 90}
departments.put("hr", hr_scores)
```

### Map as Configuration

```o2l
config: Map<Text, Text> = {
    "database_url": "localhost:5432",
    "cache_size": "1000",
    "debug_mode": "true",
    "log_level": "info"
}

# Configuration access with defaults
db_url: Text = config.getOrDefault("database_url", "localhost")
cache_size: Text = config.getOrDefault("cache_size", "500")
timeout: Text = config.getOrDefault("timeout", "30")  // Uses default

# Update configuration
config.put("timeout", "60")
config.put("log_level", "debug")
```

### Counting and Frequency Maps

```o2l
# Count word frequencies
words: List<Text> = ["apple", "banana", "apple", "cherry", "banana", "apple"]
word_count: Map<Text, Int> = {}

iter: ListIterator = words.iterator()
while (iter.hasNext()) {
    word: Text = iter.next()
    current_count: Int = word_count.getOrDefault(word, 0)
    word_count.put(word, current_count + 1)
}

# Results: {"apple": 3, "banana": 2, "cherry": 1}
```

---

## Performance Characteristics

| Operation         | Average Case | Worst Case | Notes                   |
| ----------------- | ------------ | ---------- | ----------------------- |
| `put(key, value)` | O(1)         | O(n)       | Hash collision handling |
| `get(key)`        | O(1)         | O(n)       | Hash table lookup       |
| `contains(key)`   | O(1)         | O(n)       | Same as get             |
| `remove(key)`     | O(1)         | O(n)       | Hash table deletion     |
| `size()`          | O(1)         | O(1)       | Cached value            |
| `keys()`          | O(n)         | O(n)       | Creates new list        |
| `values()`        | O(n)         | O(n)       | Creates new list        |
| `clear()`         | O(1)         | O(1)       | Efficient bulk deletion |

---

## Memory Management

- **Hash Table**: Uses efficient hash table implementation
- **Load Factor**: Automatically manages capacity and rehashing
- **Memory Efficiency**: Minimizes overhead per entry
- **Reference Semantics**: Maps store references to key/value objects
- **Garbage Collection**: Removed entries are eligible for collection

---

## Error Handling

Common exceptions thrown by Map operations:

```o2l
scores: Map<Text, Int> = {"Alice": 95}

try {
    bob_score: Int = scores.get("Bob")  // Key not found
} catch (error: EvaluationError) {
    io.print("Key 'Bob' not found: %s", error.message)
}

try {
    removed: Int = scores.remove("Charlie")  // Remove non-existent key
} catch (error: EvaluationError) {
    io.print("Cannot remove non-existent key")
}
```

---

## Best Practices

1. **Use Meaningful Keys**: Choose descriptive, unique keys
2. **Check Existence**: Use `contains()` before `get()` when uncertain
3. **Use Defaults**: Prefer `getOrDefault()` for optional values
4. **Iterator Efficiency**: Reset iterators when reusing
5. **Type Consistency**: Keep consistent key and value types
6. **Memory Management**: Clear maps when no longer needed

---

## See Also

- [List API Reference](List.md) - For ordered collections
- [Set API Reference](Set.md) - For unique element collections
- [Text API Reference](Text.md) - Common key type operations
- [JSON Library](../libraries/json.md) - Converting to/from JSON maps
- [Iterator Patterns](../../tutorials/intermediate/02-collections.md) - Advanced iteration
