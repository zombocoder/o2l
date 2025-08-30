# Memory Management

O²L Programming Language handles memory management automatically for most operations.

## Object Creation

Objects are created using the `new` keyword and constructors:

```obq
Object Person {
    constructor(name: Text) {
        this.name = name
    }
}

# Create new instance
person: Person = new Person("Alice")
```

## Automatic Memory Management

Memory is managed automatically by the runtime:

- Objects are allocated when created with `new`
- Collections like Lists are automatically resized
- Memory is reclaimed when objects are no longer referenced

## Collection Memory

Lists and other collections manage their own memory:

```obq
numbers: List<Int> = [1, 2, 3]
numbers.add(4)  # List automatically grows
numbers.remove(0)  # Memory is managed internally
```

## Variable Scope

Variables have block scope and are automatically cleaned up:

```obq
method example(): Int {
    if (true) {
        temp: Int = 42  # Scoped to if block
    }
    # temp is no longer accessible here
    return 0
}
```

## Iterator Memory

Iterators are lightweight and don't hold references to collection data:

```obq
iter: ListIterator = numbers.iterator()
while (iter.hasNext()) {
    value: Int = iter.next()
    # Iterator state is managed automatically
}
```