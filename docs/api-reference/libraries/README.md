# Libraries Reference

O²L provides a comprehensive set of libraries for common programming tasks including mathematics, networking, data processing, and time manipulation.

## Library Overview

### Mathematical and Utility Libraries
- **[Math](math.md)** - Mathematical functions and constants (47 functions)
- **[DateTime](datetime.md)** - Date and time manipulation
- **[JSON](json.md)** - JSON parsing, generation, and manipulation (30 functions)

### Network Libraries
- **[HTTP Client](http-client.md)** - HTTP client for making web requests
- **[HTTP Server](http-server.md)** - HTTP server for building web applications

### Interoperability Libraries
- **[FFI](ffi.md)** - Foreign Function Interface for calling native libraries

## Import Statements

```obq
import math
import datetime
import json
import http          # HTTP client
import http.server   # HTTP server
import ffi           # Foreign Function Interface
```

## Quick Reference

### Mathematical Operations
| Category | Functions | Examples |
|----------|-----------|----------|
| **Basic** | `abs()`, `ceil()`, `floor()`, `round()` | `math.abs(-5)`, `math.ceil(3.2)` |
| **Power** | `pow()`, `sqrt()`, `exp()`, `log()` | `math.pow(2, 3)`, `math.sqrt(16)` |
| **Trigonometry** | `sin()`, `cos()`, `tan()`, `atan2()` | `math.sin(math.pi()/2)` |
| **Constants** | `pi()`, `e()`, `inf()`, `nan()` | `math.pi()`, `math.e()` |

### Date and Time
| Operation | Functions | Examples |
|-----------|-----------|----------|
| **Current** | `now()`, `today()` | `datetime.now()` |
| **Creation** | `create()`, `fromISOString()` | `datetime.create(2024, 6, 15)` |
| **Formatting** | `toString()`, `formatISO()` | `datetime.formatISO(dt)` |
| **Arithmetic** | `addDays()`, `addHours()` | `datetime.addDays(dt, 7)` |

### JSON Processing
| Operation | Functions | Examples |
|-----------|-----------|----------|
| **Parsing** | `parse()`, `parseToMap()`, `parseToList()` | `json.parse(json_string)` |
| **Generation** | `stringify()`, `createObject()`, `createArray()` | `json.stringify(data)` |
| **Manipulation** | `get()`, `set()`, `merge()` | `json.get(data, "user.name")` |
| **Validation** | `isValid()`, `isObject()`, `isArray()` | `json.isValid(text)` |

### HTTP Client
| Operation | Functions | Examples |
|-----------|-----------|----------|
| **Requests** | `get()`, `post()`, `put()`, `delete()` | `http.get("https://api.example.com")` |
| **Configuration** | `createRequest()`, `setHeader()`, `setTimeout()` | `http.setHeader(req, "Auth", token)` |
| **Response** | `getBody()`, `getStatus()`, `isSuccess()` | `http.isSuccess(response)` |
| **Utilities** | `urlEncode()`, `buildUrl()`, `parseUrl()` | `http.urlEncode("hello world")` |

### HTTP Server
| Operation | Functions | Examples |
|-----------|-----------|----------|
| **Lifecycle** | `create()`, `listen()`, `stop()` | `server = http.server.create()` |
| **Routes** | `get()`, `post()`, `put()`, `delete()` | `http.server.get(server, "/api", handler)` |
| **Static Files** | `static()` | `http.server.static(server, "/", "./public")` |
| **Middleware** | `use()` | `http.server.use(server, middleware)` |

## Common Integration Patterns

### Web API with JSON
```obq
import http
import json
import datetime

# Fetch and process API data
response: HttpResponse = http.get("https://api.github.com/users/octocat")

if http.isSuccess(response) {
    user_data: Text = http.getBody(response)
    user: Map = json.parseToMap(user_data)
    
    # Extract information
    name: Text = user.get("name")
    created: Text = user.get("created_at")
    
    # Parse GitHub's ISO date
    created_date: Text = datetime.fromISOString(created)
    days_old: Int = datetime.daysBetween(created_date, datetime.now())
    
    io.print("User %s created %d days ago", name, days_old)
}
```

### Mathematical Data Processing
```obq
import math
import json

# Statistical analysis of data
data_json: Text = "[1.5, 2.3, 4.7, 3.1, 5.9, 2.8]"
numbers: List = json.parseToList(data_json)

# Calculate statistics
sum: Double = numbers.reduce(0.0, (acc, x) -> acc + x)
mean: Double = sum / numbers.size()

# Calculate standard deviation
variance: Double = numbers.map(x -> math.pow(x - mean, 2))
                          .reduce(0.0, (acc, x) -> acc + x) / numbers.size()
std_dev: Double = math.sqrt(variance)

io.print("Mean: %.2f, Std Dev: %.2f", mean, std_dev)
```

### File Processing with DateTime
```obq
import system.fs
import datetime
import json

# Process log files with timestamps
log_entries: List = []

# Read and parse log file
if fs.exists("app.log") {
    content: Text = fs.readFile("app.log")
    lines: List = content.split("\n")
    
    lines.forEach(line -> {
        if line.length() > 0 {
            # Parse timestamp and message
            parts: List = line.split(" ", 2)
            if parts.size() >= 2 {
                timestamp_str: Text = parts.get(0)
                message: Text = parts.get(1)
                
                # Convert to datetime
                log_time: Text = datetime.fromISOString(timestamp_str)
                
                # Create log entry
                entry: Map = Map()
                entry.put("timestamp", log_time)
                entry.put("message", message)
                entry.put("age_hours", datetime.hoursBetween(log_time, datetime.now()))
                
                log_entries.add(entry)
            }
        }
    })
    
    # Save processed data as JSON
    processed_json: Text = json.stringify(log_entries)
    fs.writeFile("processed_logs.json", processed_json)
}
```

### HTTP Server with Math API
```obq
import http.server
import math
import json

Object MathAPIController {
    method calculate(request: HttpRequest, response: HttpResponse): Text {
        # Parse request body
        body: Text = request.getBody()
        calc_request: Map = json.parseToMap(body)
        
        operation: Text = calc_request.get("operation")
        x: Double = calc_request.get("x").toDouble()
        y: Double = calc_request.get("y").toDouble()
        
        result: Double = 0.0
        
        # Perform calculation
        if operation == "add" {
            result = x + y
        } else if operation == "subtract" {
            result = x - y
        } else if operation == "multiply" {
            result = x * y
        } else if operation == "divide" {
            if y != 0.0 {
                result = x / y
            } else {
                response.setStatus(400)
                response.json("{\"error\": \"Division by zero\"}")
                return "Error"
            }
        } else if operation == "power" {
            result = math.pow(x, y)
        } else if operation == "sqrt" {
            if x >= 0.0 {
                result = math.sqrt(x)
            } else {
                response.setStatus(400)
                response.json("{\"error\": \"Square root of negative number\"}")
                return "Error"
            }
        }
        
        # Return result
        result_json: Text = json.createObject("result", result, "timestamp", datetime.now())
        response.json(result_json)
        
        return "Calculation completed"
    }
}

# Set up server
server: HttpServerInstance = http.server.create()
controller: Object = MathAPIController()

http.server.setPort(server, 3000)
http.server.post(server, "/calculate", controller, "calculate")
http.server.listen(server)
```

## Performance Optimization

### JSON Processing
```obq
# Use parseAuto for automatic type conversion
data: Map = json.parseAuto(json_string)  # Faster than parse + parseToMap

# Pre-validate JSON before parsing
if json.isValid(json_string) {
    data: Map = json.parseToMap(json_string)
}

# Use specific parsing for known structures
users: List = json.parseToList(users_json)  # Faster than parseAuto
```

### Math Operations
```obq
# Cache frequently used constants
PI: Double = math.pi()
E: Double = math.e()

# Use appropriate precision
angle_rad: Float = math.radians(45.0)  # Float for angles
distance: Double = math.sqrt(x*x + y*y)  # Double for precision
```

### HTTP Operations
```obq
# Reuse request configurations
request: HttpRequest = http.createRequest("GET", base_url)
http.setHeader(request, "User-Agent", "MyApp/1.0")
http.setTimeout(request, 30)

# Make multiple requests with same config
response1: HttpResponse = http.requestWithConfig(request)
# Modify URL for next request
request.url = another_url
response2: HttpResponse = http.requestWithConfig(request)
```

## Error Handling Best Practices

### Network Operations
```obq
import http
import json

try {
    response: HttpResponse = http.get("https://api.example.com/data")
    
    if http.isSuccess(response) {
        data: Text = http.getBody(response)
        parsed: Map = json.parseToMap(data)
        # Process data...
    } else {
        status: Int = http.getStatus(response)
        error_msg: Text = http.getError(response)
        io.print("HTTP Error %d: %s", status, error_msg)
    }
} catch error {
    io.print("Request failed: %s", error.getMessage())
}
```

### Mathematical Operations
```obq
import math

try {
    # Validate inputs before math operations
    if x >= 0.0 {
        result: Double = math.sqrt(x)
    } else {
        io.print("Error: Cannot take square root of negative number")
    }
    
    # Check for special values
    result: Double = math.log(x)
    if math.isNan(result) || math.isInf(result) {
        io.print("Warning: Mathematical operation produced NaN or Infinity")
    }
} catch error {
    io.print("Math error: %s", error.getMessage())
}
```

## See Also

- **[Core Types](../core/README.md)** - Basic data types used with libraries
- **[System Libraries](../system/README.md)** - Operating system integration
- **[Language Overview](../../language/overview.md)** - O²L language fundamentals