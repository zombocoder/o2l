# Text API Reference

## Overview

The Text type is O²L's immutable string implementation with 48+ methods for string manipulation, validation, and formatting. All Text operations return new Text instances, maintaining immutability.

## Constructor

```o2l
text: Text = "Hello, World!"
```

## Quick Example

```o2l
text: Text = "  Hello World  "
cleaned: Text = text.strip().upper()  // "HELLO WORLD"
length: Int = cleaned.length()        // 11
```

---

## String Information

### `length() → Int`

Returns the number of characters in the string.

```o2l
text: Text = "Hello World"
len: Int = text.length()  // Returns 11
```

---

## Case Conversion Methods

### `capitalize() → Text`

Returns a copy with the first character capitalized and the rest lowercased.

```o2l
text: Text = "hello world"
result: Text = text.capitalize()  // "Hello world"
```

### `upper() → Text`

Returns a copy with all characters converted to uppercase.

```o2l
text: Text = "hello world"
result: Text = text.upper()  // "HELLO WORLD"
```

### `lower() → Text`

Returns a copy with all characters converted to lowercase.

```o2l
text: Text = "HELLO WORLD"
result: Text = text.lower()  // "hello world"
```

### `caseFold() → Text`

Returns a casefolded copy (aggressive lowercase for case-insensitive comparisons).

```o2l
text: Text = "HELLO WORLD"
result: Text = text.caseFold()  // "hello world"
```

### `swapCase() → Text`

Returns a copy with uppercase characters converted to lowercase and vice versa.

```o2l
text: Text = "Hello World"
result: Text = text.swapCase()  // "hELLO wORLD"
```

### `title() → Text`

Returns a titlecased version where words start with uppercase.

```o2l
text: Text = "hello world test"
result: Text = text.title()  // "Hello World Test"
```

---

## Search Methods

### `find(substring: Text) → Int`

Returns the lowest index where substring is found, or -1 if not found.

```o2l
text: Text = "Hello World"
index: Int = text.find("World")  // Returns 6
missing: Int = text.find("xyz")  // Returns -1
```

### `rfind(substring: Text) → Int`

Returns the highest index where substring is found, or -1 if not found.

```o2l
text: Text = "Hello Hello"
index: Int = text.rfind("Hello")  // Returns 6
```

### `index(substring: Text) → Int`

Like find(), but throws an exception if substring is not found.

```o2l
text: Text = "Hello World"
index: Int = text.index("World")  // Returns 6
// text.index("xyz")  // Throws EvaluationError
```

### `rindex(substring: Text) → Int`

Like rfind(), but throws an exception if substring is not found.

```o2l
text: Text = "Hello Hello"
index: Int = text.rindex("Hello")  // Returns 6
```

### `count(substring: Text) → Int`

Returns the number of non-overlapping occurrences of substring.

```o2l
text: Text = "Hello Hello Hello"
count: Int = text.count("Hello")  // Returns 3
```

---

## Boolean Check Methods

### `startswith(prefix: Text) → Bool`

Returns true if the string starts with the specified prefix.

```o2l
text: Text = "Hello World"
starts: Bool = text.startswith("Hello")  // Returns true
```

### `endswith(suffix: Text) → Bool`

Returns true if the string ends with the specified suffix.

```o2l
text: Text = "Hello World"
ends: Bool = text.endswith("World")  // Returns true
```

---

## Character Type Checking

### `isAlnum() → Bool`

Returns true if all characters are alphanumeric.

```o2l
text: Text = "Hello123"
result: Bool = text.isAlnum()  // Returns true
```

### `isAlpha() → Bool`

Returns true if all characters are alphabetic.

```o2l
text: Text = "Hello"
result: Bool = text.isAlpha()  // Returns true
```

### `isDigit() → Bool`

Returns true if all characters are digits.

```o2l
text: Text = "12345"
result: Bool = text.isDigit()  // Returns true
```

### `isLower() → Bool`

Returns true if all characters are lowercase.

```o2l
text: Text = "hello"
result: Bool = text.isLower()  // Returns true
```

### `isUpper() → Bool`

Returns true if all characters are uppercase.

```o2l
text: Text = "HELLO"
result: Bool = text.isUpper()  // Returns true
```

### `isSpace() → Bool`

Returns true if all characters are whitespace.

```o2l
text: Text = "   \t\n"
result: Bool = text.isSpace()  // Returns true
```

### `isPrintable() → Bool`

Returns true if all characters are printable.

```o2l
text: Text = "Hello World!"
result: Bool = text.isPrintable()  // Returns true
```

### `isAscii() → Bool`

Returns true if all characters are ASCII.

```o2l
text: Text = "Hello"
result: Bool = text.isAscii()  // Returns true
```

### `isIdentifier() → Bool`

Returns true if the string is a valid identifier.

```o2l
text: Text = "hello_world"
result: Bool = text.isIdentifier()  // Returns true
```

### `isDecimal() → Bool`

Returns true if all characters are decimal digits.

```o2l
text: Text = "12345"
result: Bool = text.isDecimal()  // Returns true
```

### `isNumeric() → Bool`

Returns true if all characters are numeric.

```o2l
text: Text = "123.45"
result: Bool = text.isNumeric()  // Returns true
```

### `isTitle() → Bool`

Returns true if the string is in title case.

```o2l
text: Text = "Hello World"
result: Bool = text.isTitle()  // Returns true
```

---

## String Manipulation

### `strip() → Text`

Returns a copy with leading and trailing whitespace removed.

```o2l
text: Text = "  Hello World  "
result: Text = text.strip()  // "Hello World"
```

### `lstrip() → Text`

Returns a copy with leading whitespace removed.

```o2l
text: Text = "  Hello World  "
result: Text = text.lstrip()  // "Hello World  "
```

### `rstrip() → Text`

Returns a copy with trailing whitespace removed.

```o2l
text: Text = "  Hello World  "
result: Text = text.rstrip()  // "  Hello World"
```

### `replace(old: Text, new: Text) → Text`

Returns a copy with all occurrences of old replaced by new.

```o2l
text: Text = "Hello World"
result: Text = text.replace("World", "Universe")  // "Hello Universe"
```

### `split(separator: Text) → List<Text>`

Returns a list of substrings split by the separator.

```o2l
text: Text = "Hello,World,Test"
parts: List<Text> = text.split(",")  // ["Hello", "World", "Test"]
```

### `splitlines() → List<Text>`

Returns a list of lines, breaking at line boundaries.

```o2l
text: Text = "Line1\nLine2\nLine3"
lines: List<Text> = text.splitlines()  // ["Line1", "Line2", "Line3"]
```

---

## String Positioning

### `center(width: Int) → Text`

Returns a centered string in a field of given width.

```o2l
text: Text = "Hi"
result: Text = text.center(10)  // "    Hi    "
```

### `ljust(width: Int) → Text`

Returns a left-justified string in a field of given width.

```o2l
text: Text = "Hi"
result: Text = text.ljust(10)  // "Hi        "
```

### `rjust(width: Int) → Text`

Returns a right-justified string in a field of given width.

```o2l
text: Text = "Hi"
result: Text = text.rjust(10)  // "        Hi"
```

### `zfill(width: Int) → Text`

Returns a copy left-padded with zeros to the given width.

```o2l
text: Text = "42"
result: Text = text.zfill(5)  // "00042"

negative: Text = "-42"
result2: Text = negative.zfill(5)  // "-0042"
```

---

## String Formatting

### `format(arg0, arg1, ...) → Text`

Returns a formatted string using positional arguments.

```o2l
template: Text = "Hello {0}, you are {1} years old!"
result: Text = template.format("Alice", 25)  // "Hello Alice, you are 25 years old!"

bool_template: Text = "The answer is {0}"
bool_result: Text = bool_template.format(true)  // "The answer is true"
```

### `formatMap(map: Map<Text, Value>) → Text`

Returns a formatted string using named arguments from a map.

```o2l
template: Text = "Hello {name}, welcome to {place}!"
args: Map<Text, Text> = {"name": "Bob", "place": "O²L"}
result: Text = template.formatMap(args)  // "Hello Bob, welcome to O²L!"
```

### `join(list: List<Value>) → Text`

Returns a string concatenating the list elements with this string as separator.

```o2l
separator: Text = ", "
items: List<Text> = ["Hello", "World", "Test"]
result: Text = separator.join(items)  // "Hello, World, Test"

# Works with mixed types
mixed: List<Value> = ["Number:", 42, true]
mixed_result: Text = " ".join(mixed)  // "Number: 42 true"
```

---

## String Partitioning

### `partition(separator: Text) → List<Text>`

Splits the string at the first occurrence of separator, returning a 3-element list.

```o2l
text: Text = "name=value"
parts: List<Text> = text.partition("=")  // ["name", "=", "value"]

# If separator not found
not_found: List<Text> = text.partition("@")  // ["name=value", "", ""]
```

### `rpartition(separator: Text) → List<Text>`

Splits the string at the last occurrence of separator, returning a 3-element list.

```o2l
text: Text = "a.b.c"
parts: List<Text> = text.rpartition(".")  // ["a.b", ".", "c"]
```

---

## Translation Methods

### `makeTrans(from: Text, to: Text) → Map<Text, Text>`

Returns a translation table for use with translate().

```o2l
text: Text = "abc"
trans_table: Map<Text, Text> = text.makeTrans("abc", "xyz")
```

### `translate(table: Map<Text, Text>) → Text`

Returns a copy with characters mapped through the translation table.

```o2l
text: Text = "abcdef"
trans_table: Map<Text, Text> = "abc".makeTrans("abc", "xyz")
result: Text = text.translate(trans_table)  // "xyzdef"
```

---

## Type Conversions

### `toString() → Text`

Returns the string itself (identity operation).

```o2l
text: Text = "Hello"
same: Text = text.toString()  // "Hello"
```

### `toInt() → Int`

Parses the string as an integer.

```o2l
text: Text = "42"
number: Int = text.toInt()  // 42
```

### `toLong() → Long`

Parses the string as a long integer.

```o2l
text: Text = "1234567890"
number: Long = text.toLong()  // 1234567890L
```

### `toFloat() → Float`

Parses the string as a float.

```o2l
text: Text = "3.14"
number: Float = text.toFloat()  // 3.14f
```

### `toDouble() → Double`

Parses the string as a double.

```o2l
text: Text = "3.141592653589793"
number: Double = text.toDouble()  // 3.141592653589793d
```

### `toBool() → Bool`

Converts the string to a boolean ("true" → true, everything else → false).

```o2l
text1: Text = "true"
bool1: Bool = text1.toBool()  // true

text2: Text = "false"
bool2: Bool = text2.toBool()  // false
```

---

## Performance Notes

- **Immutability**: All Text operations return new instances; original strings are unchanged
- **Memory Efficiency**: O²L optimizes string operations for minimal memory allocation
- **Method Chaining**: Operations can be chained for fluent programming
- **Type Safety**: All methods are type-safe at compile time

## Error Handling

Methods that can fail (like `index()`, `rindex()`, numeric conversions) throw `EvaluationError` exceptions:

```o2l
try {
    result: Int = "not_a_number".toInt()
} catch (error: EvaluationError) {
    io.print("Conversion failed: %s", error.message)
}
```

## See Also

- [List API Reference](../core/List.md) - For working with string collections
- [Map API Reference](../core/Map.md) - For key-value string operations
- [system.io](../system/io.md) - For string input/output operations
- [Control Flow](../../language-reference/control-flow.md) - Using strings in conditionals
