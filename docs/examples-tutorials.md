# Examples and Tutorials

Practical examples and tutorials for learning O²L Programming Language.

## Tutorial 1: Hello World

The simplest O²L program:

```obq
import system.io

Object Main {
    method main(): Int {
        io.print("Hello, World!")
        return 0
    }
}
```

## Tutorial 2: Interactive Program

Getting user input and processing it:

```obq
import system.io

Object Interactive {
    @external method greetUser(): Int {
        io.print("What's your name?")
        name: Text = io.readLine("")
        io.print("Nice to meet you, %s!", name)
        return 0
    }
}

Object Main {
    method main(): Int {
        app: Interactive = new Interactive()
        return app.greetUser()
    }
}
```

## Tutorial 3: Working with Objects

Creating and using custom objects:

```obq
import system.io

Object Person {
    property name: Text
    property age: Int
    
    constructor(name: Text, age: Int) {
        this.name = name
        this.age = age
    }
    
    method introduce(): Void {
        io.print("Hi, I'm %s and I'm %d years old", this.name, this.age)
    }
    
    method haveBirthday(): Void {
        this.age = this.age + 1
        io.print("Happy birthday! Now I'm %d", this.age)
    }
}

Object Main {
    method main(): Int {
        alice: Person = new Person("Alice", 25)
        alice.introduce()
        alice.haveBirthday()
        return 0
    }
}
```

## Tutorial 4: Lists and Iteration

Working with collections:

```obq
import system.io

Object ListDemo {
    @external method processGrades(): Int {
        # Create and populate list
        grades: List<Int> = [85, 92, 78, 96, 88]
        io.print("Grades: %s", grades)
        
        # Calculate average using iterator
        iter: ListIterator = grades.iterator()
        total: Int = 0
        count: Int = 0
        
        while (iter.hasNext()) {
            grade: Int = iter.next()
            total = total + grade
            count = count + 1
        }
        
        average: Int = total / count
        io.print("Average grade: %d", average)
        
        return 0
    }
}

Object Main {
    method main(): Int {
        demo: ListDemo = new ListDemo()
        return demo.processGrades()
    }
}
```

## Tutorial 5: Repeat Iterators

Using RepeatIterator for counting operations:

```obq
import system.io
import system.utils

Object CountingDemo {
    @external method createMultiplicationTable(): Int {
        io.print("5x5 Multiplication Table:")
        
        rows: RepeatIterator = utils.repeat(5)
        while (rows.hasNext()) {
            row: Int = rows.next() + 1  # Start from 1
            
            cols: RepeatIterator = utils.repeat(5)
            while (cols.hasNext()) {
                col: Int = cols.next() + 1  # Start from 1
                product: Int = row * col
                io.print("%d x %d = %d", row, col, product)
            }
            io.print("---")
        }
        
        return 0
    }
}

Object Main {
    method main(): Int {
        demo: CountingDemo = new CountingDemo()
        return demo.createMultiplicationTable()
    }
}
```

## Tutorial 6: Calculator Application

A complete calculator with multiple operations:

```obq
import system.io

Object Calculator {
    property currentValue: Float
    
    constructor() {
        this.currentValue = 0.0
    }
    
    method add(value: Float): Void {
        this.currentValue = this.currentValue + value
        io.print("Added %f, result: %f", value, this.currentValue)
    }
    
    method multiply(value: Float): Void {
        this.currentValue = this.currentValue * value
        io.print("Multiplied by %f, result: %f", value, this.currentValue)
    }
    
    method clear(): Void {
        this.currentValue = 0.0
        io.print("Calculator cleared")
    }
    
    method getResult(): Float {
        return this.currentValue
    }
}

Object Main {
    method main(): Int {
        calc: Calculator = new Calculator()
        
        calc.add(10.0)
        calc.multiply(2.5)
        calc.add(5.0)
        
        result: Float = calc.getResult()
        io.print("Final result: %f", result)
        
        return 0
    }
}
```

## Common Patterns

### Error Handling Pattern
```obq
method safeOperation(): Int {
    if (errorCondition) {
        io.print("Error occurred")
        return -1  # Error code
    }
    # Success logic
    return 0  # Success code
}
```

### Builder Pattern
```obq
Object StringBuilder {
    property content: Text
    
    constructor() {
        this.content = ""
    }
    
    method append(text: Text): StringBuilder {
        this.content = this.content + text
        return this
    }
    
    method build(): Text {
        return this.content
    }
}
```

### Iterator Processing Pattern
```obq
method processCollection(items: List<Int>): Int {
    iter: ListIterator = items.iterator()
    result: List<Int> = []
    
    while (iter.hasNext()) {
        item: Int = iter.next()
        processed: Int = item * 2  # Transform
        result.add(processed)
    }
    
    return result.size()
}
```