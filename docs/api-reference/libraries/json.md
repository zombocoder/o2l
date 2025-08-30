# JSON Library

The JSON library provides comprehensive JSON parsing, generation, and manipulation capabilities for O²L applications.

## Import

```obq
import json
```

## Parsing Functions

### `parse(jsonString: Text) -> Text`
Parses a JSON string and returns it as a normalized JSON string.

```obq
json_str: Text = "{\"name\": \"Alice\", \"age\": 30}"
parsed: Text = json.parse(json_str)
```

### `parseAuto(jsonString: Text) -> Value`
Parses JSON and automatically converts to native O²L types (Map for objects, List for arrays).

```obq
# Object becomes Map
obj_json: Text = "{\"name\": \"Bob\", \"score\": 95}"
obj_map: Map = json.parseAuto(obj_json)
name: Text = obj_map.get("name")  # "Bob"

# Array becomes List
arr_json: Text = "[1, 2, 3]"
arr_list: List = json.parseAuto(arr_json)
first: Int = arr_list.get(0)  # 1

# Primitives remain as-is
num: Int = json.parseAuto("42")        # 42
str: Text = json.parseAuto("\"hello\"") # "hello"
bool_val: Bool = json.parseAuto("true") # true
```

### `parseToMap(jsonString: Text) -> Map`
Parses a JSON object string to an O²L Map. Throws error if not an object.

```obq
json_obj: Text = "{\"x\": 10, \"y\": 20}"
coords: Map = json.parseToMap(json_obj)
x: Int = coords.get("x")  # 10
```

### `parseToList(jsonString: Text) -> List`
Parses a JSON array string to an O²L List. Throws error if not an array.

```obq
json_arr: Text = "[\"red\", \"green\", \"blue\"]"
colors: List = json.parseToList(json_arr)
first_color: Text = colors.get(0)  # "red"
```

## Validation Functions

### `isValid(jsonString: Text) -> Bool`
Checks if a string is valid JSON.

```obq
valid: Bool = json.isValid("{\"test\": 123}")    # true
invalid: Bool = json.isValid("{invalid}")        # false
```

### Type Detection Functions

#### `isObject(jsonString: Text) -> Bool`
Checks if JSON string represents an object.

```obq
is_obj: Bool = json.isObject("{\"key\": \"value\"}")  # true
not_obj: Bool = json.isObject("[1, 2, 3]")           # false
```

#### `isArray(jsonString: Text) -> Bool`
Checks if JSON string represents an array.

```obq
is_arr: Bool = json.isArray("[1, 2, 3]")        # true
not_arr: Bool = json.isArray("{\"key\": 1}")    # false
```

#### `isString(jsonString: Text) -> Bool`
Checks if JSON string represents a string value.

```obq
is_str: Bool = json.isString("\"hello\"")  # true
not_str: Bool = json.isString("123")       # false
```

#### `isNumber(jsonString: Text) -> Bool`
Checks if JSON string represents a number.

```obq
is_num: Bool = json.isNumber("456")     # true
not_num: Bool = json.isNumber("\"hi\"") # false
```

#### `isBoolean(jsonString: Text) -> Bool`
Checks if JSON string represents a boolean.

```obq
is_bool: Bool = json.isBoolean("true")   # true
not_bool: Bool = json.isBoolean("123")   # false
```

#### `isNull(jsonString: Text) -> Bool`
Checks if JSON string represents null.

```obq
is_null: Bool = json.isNull("null")      # true
not_null: Bool = json.isNull("\"null\"") # false
```

## Access and Navigation

### `get(jsonString: Text, path: Text) -> Value`
Retrieves a value from JSON using dot notation path.

```obq
json_data: Text = "{\"user\": {\"name\": \"Alice\", \"age\": 30}}"

# Simple property access
name: Text = json.get(json_data, "user.name")  # "Alice"
age: Int = json.get(json_data, "user.age")     # 30

# Array access
arr_json: Text = "{\"items\": [\"first\", \"second\", \"third\"]}"
first: Text = json.get(arr_json, "items.0")  # "first"
```

### `getType(jsonString: Text, path: Text) -> Text`
Returns the type of a value at the specified path.

```obq
data: Text = "{\"name\": \"Bob\", \"age\": 25, \"active\": true}"
name_type: Text = json.getType(data, "name")     # "string"
age_type: Text = json.getType(data, "age")       # "number"
active_type: Text = json.getType(data, "active") # "boolean"
```

### `hasKey(jsonString: Text, key: Text) -> Bool`
Checks if a JSON object has a specific key.

```obq
obj: Text = "{\"name\": \"Charlie\", \"age\": 35}"
has_name: Bool = json.hasKey(obj, "name")    # true
has_email: Bool = json.hasKey(obj, "email")  # false
```

## Generation Functions

### `stringify(value: Value, [indent: Int]) -> Text`
Converts an O²L value to JSON string with optional formatting.

```obq
# Basic stringify
result: Text = json.stringify("hello")        # "\"hello\""
result2: Text = json.stringify(42)            # "42"
result3: Text = json.stringify(true)          # "true"

# With indentation (pretty print)
pretty: Text = json.stringify(data, 2)        # Formatted with 2-space indent
```

### `create(value: Value) -> Text`
Creates a JSON string from an O²L value.

```obq
json_str: Text = json.create("test")  # "\"test\""
json_num: Text = json.create(123)     # "123"
```

### `createArray(...values: Value) -> Text`
Creates a JSON array from arguments.

```obq
arr: Text = json.createArray("apple", "banana", "cherry")
# Result: ["apple","banana","cherry"]

nums: Text = json.createArray(1, 2, 3, 4)
# Result: [1,2,3,4]
```

### `createObject(key1: Text, value1: Value, ...) -> Text`
Creates a JSON object from key-value pairs.

```obq
obj: Text = json.createObject("name", "David", "age", 28, "active", true)
# Result: {"name":"David","age":28,"active":true}
```

## Manipulation Functions

### `set(jsonString: Text, path: Text, value: Value) -> Text`
Sets a value at the specified path in JSON.

```obq
original: Text = "{\"count\": 5, \"user\": {\"name\": \"Eve\"}}"
updated: Text = json.set(original, "count", 10)
# Result has count updated to 10

nested: Text = json.set(original, "user.name", "Eva")
# Result has nested name updated to "Eva"
```

### `remove(jsonString: Text, path: Text) -> Text`
Removes a value at the specified path from JSON.

```obq
data: Text = "{\"a\": 1, \"b\": 2, \"c\": 3}"
removed: Text = json.remove(data, "b")
# Result: {"a":1,"c":3}
```

### `merge(jsonString1: Text, jsonString2: Text) -> Text`
Merges two JSON objects (second object overwrites first on key conflicts).

```obq
obj1: Text = "{\"a\": 1, \"b\": 2}"
obj2: Text = "{\"b\": 3, \"c\": 4}"
merged: Text = json.merge(obj1, obj2)
# Result: {"a":1,"b":3,"c":4}
```

## Object Information Functions

### `keys(jsonString: Text) -> List<Text>`
Returns list of keys from a JSON object.

```obq
obj: Text = "{\"name\": \"Frank\", \"age\": 40, \"city\": \"NYC\"}"
key_list: List = json.keys(obj)
# Result: ["name", "age", "city"]
```

### `values(jsonString: Text) -> List<Value>`
Returns list of values from a JSON object.

```obq
obj: Text = "{\"x\": 10, \"y\": 20}"
value_list: List = json.values(obj)
# Result: [10, 20]
```

### `size(jsonString: Text) -> Int`
Returns the size of a JSON object or array.

```obq
obj_size: Int = json.size("{\"a\": 1, \"b\": 2}")  # 2
arr_size: Int = json.size("[1, 2, 3, 4, 5]")       # 5
```

## Array Operations

### `push(jsonArrayString: Text, value: Value) -> Text`
Adds a value to the end of a JSON array.

```obq
arr: Text = "[1, 2, 3]"
new_arr: Text = json.push(arr, 4)
# Result: [1,2,3,4]
```

### `pop(jsonArrayString: Text) -> Text`
Removes the last element from a JSON array.

```obq
arr: Text = "[1, 2, 3, 4]"
popped: Text = json.pop(arr)
# Result: [1,2,3]
```

### `slice(jsonArrayString: Text, start: Int, [end: Int]) -> Text`
Extracts a section of a JSON array.

```obq
arr: Text = "[10, 20, 30, 40, 50]"
sliced: Text = json.slice(arr, 1, 4)
# Result: [20,30,40]

# Without end parameter (slice to end)
tail: Text = json.slice(arr, 2)
# Result: [30,40,50]
```

### `indexOf(jsonArrayString: Text, value: Value) -> Int`
Returns the index of a value in a JSON array, or -1 if not found.

```obq
arr: Text = "[\"red\", \"green\", \"blue\"]"
green_idx: Int = json.indexOf(arr, "green")  # 1
missing_idx: Int = json.indexOf(arr, "yellow")  # -1
```

## Utility Functions

### `prettyPrint(jsonString: Text) -> Text`
Returns a pretty-formatted JSON string with indentation.

```obq
compact: Text = "{\"name\":\"Grace\",\"data\":[1,2,3]}"
pretty: Text = json.prettyPrint(compact)
# Result: formatted with proper indentation and line breaks
```

### `minify(jsonString: Text) -> Text`
Returns a minified JSON string with no unnecessary whitespace.

```obq
formatted: Text = `{
  "name": "Henry",
  "age": 45
}`
minified: Text = json.minify(formatted)
# Result: {"name":"Henry","age":45}
```

### `equals(jsonString1: Text, jsonString2: Text) -> Bool`
Compares two JSON strings for structural equality.

```obq
json1: Text = "{\"a\": 1, \"b\": 2}"
json2: Text = "{\"b\": 2, \"a\": 1}"  # Different order
json3: Text = "{\"a\": 1, \"b\": 3}"  # Different value

equal12: Bool = json.equals(json1, json2)  # true (order doesn't matter)
equal13: Bool = json.equals(json1, json3)  # false (different values)
```

### `clone(jsonString: Text) -> Text`
Creates a deep copy of a JSON structure.

```obq
original: Text = "{\"data\": [1, 2, {\"nested\": true}]}"
cloned: Text = json.clone(original)
# Result: identical structure, completely independent
```

## Complete Example

```obq
import json
import system.io

Object JsonExample {
    method demonstrateJson(): Text {
        io.print("=== JSON Library Example ===")
        
        # Create JSON data
        user_data: Text = json.createObject(
            "name", "Isabella",
            "age", 32,
            "skills", json.createArray("JavaScript", "O²L", "Python"),
            "active", true
        )
        
        io.print("Created user: %s", json.prettyPrint(user_data))
        
        # Parse and access data
        name: Text = json.get(user_data, "name")
        age: Int = json.get(user_data, "age")
        first_skill: Text = json.get(user_data, "skills.0")
        
        io.print("Name: %s, Age: %d, First skill: %s", name, age, first_skill)
        
        # Auto-parse to native types
        user_map: Map = json.parseAuto(user_data)
        skills_list: List = user_map.get("skills")
        skill_count: Int = skills_list.size()
        
        io.print("User has %d skills", skill_count)
        
        # Modify JSON
        updated: Text = json.set(user_data, "age", 33)
        new_age: Int = json.get(updated, "age")
        io.print("Updated age: %d", new_age)
        
        # Array operations
        more_skills: Text = json.push(json.get(user_data, "skills"), "React")
        skills_size: Int = json.size(more_skills)
        io.print("Skills after adding React: %d", skills_size)
        
        # Validation and type checking
        is_valid: Bool = json.isValid(user_data)
        is_object: Bool = json.isObject(user_data)
        has_email: Bool = json.hasKey(user_data, "email")
        
        io.print("Valid: %s, Is object: %s, Has email: %s", 
                 is_valid, is_object, has_email)
        
        # Get all keys
        keys: List = json.keys(user_data)
        io.print("User data has %d keys", keys.size())
        
        return "JSON demonstration completed"
    }
}
```

## Error Handling

JSON functions may throw `EvaluationError` for:
- Invalid JSON syntax in parsing functions
- Path not found in navigation functions
- Type mismatches (e.g., using `parseToMap` on an array)
- Invalid array indices
- Wrong argument types or counts

Always validate JSON strings and handle potential parsing errors when working with external data.

## Performance Notes

- Use `parseAuto()` for convenient conversion to native O²L types
- Use `parseToMap()` and `parseToList()` when you know the expected structure
- The library handles nested structures and maintains type safety
- Path navigation uses dot notation: `"user.profile.name"` or `"items.0"`
- Array indices in paths are zero-based: `"arr.0"`, `"arr.1"`, etc.

## Type Mapping

| JSON Type | O²L Type (parseAuto) | Notes |
|-----------|---------------------|--------|
| `null` | Text("null") | Represented as string |
| `true`/`false` | Bool | Direct mapping |
| Numbers | Int or Double | Integers become Int, decimals become Double |
| Strings | Text | Direct mapping |
| Arrays | List | Recursive conversion of elements |
| Objects | Map | Keys become Text, values converted recursively |