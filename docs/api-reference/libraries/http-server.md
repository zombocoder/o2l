# HTTP Server Library

The HTTP Server library provides comprehensive HTTP server capabilities for O²L applications, including routing, middleware, static file serving, and advanced server configuration.

## Import

```obq
import http.server
```

## Server Creation and Lifecycle

### `create() -> HttpServerInstance`
Creates a new HTTP server instance.

```obq
server: HttpServerInstance = http.server.create()
```

### `listen(server: HttpServerInstance) -> Bool`
Starts the server listening on the configured host and port.

```obq
success: Bool = http.server.listen(server)
if success {
    io.print("Server started successfully")
} else {
    io.print("Failed to start server")
}
```

### `stop(server: HttpServerInstance) -> Text`
Stops the server and closes all connections.

```obq
result: Text = http.server.stop(server)
io.print("Server stopped: %s", result)
```

### `isRunning(server: HttpServerInstance) -> Bool`
Checks if the server is currently running.

```obq
running: Bool = http.server.isRunning(server)
```

### `waitForever(server: HttpServerInstance) -> Text`
Blocks execution until the server is stopped (useful for keeping main thread alive).

```obq
# This will block until server.stop() is called
result: Text = http.server.waitForever(server)
```

## Server Configuration

### `setHost(server: HttpServerInstance, host: Text) -> Text`
Sets the host/IP address for the server to bind to.

```obq
http.server.setHost(server, "0.0.0.0")     # Listen on all interfaces
http.server.setHost(server, "127.0.0.1")   # Listen only on localhost
http.server.setHost(server, "192.168.1.100") # Listen on specific IP
```

### `setPort(server: HttpServerInstance, port: Int) -> Text`
Sets the port for the server to listen on.

```obq
http.server.setPort(server, 3000)   # Development port
http.server.setPort(server, 80)     # HTTP default port
http.server.setPort(server, 443)    # HTTPS default port
```

### `setWorkerThreads(server: HttpServerInstance, threads: Int) -> Text`
Sets the number of worker threads for handling requests.

```obq
http.server.setWorkerThreads(server, 8)   # 8 worker threads
http.server.setWorkerThreads(server, 16)  # 16 worker threads (for high load)
```

## Route Registration

### `get(server: HttpServerInstance, pattern: Text, handler: Handler) -> Text`
Registers a GET route handler.

```obq
# Simple text handler
http.server.get(server, "/hello", "greeting_handler")

# Object with handle method
handler_obj: Object = MyHandler()
http.server.get(server, "/users", handler_obj)

# Object with specific method
http.server.get(server, "/users/:id", user_controller, "getUser")
```

### `post(server: HttpServerInstance, pattern: Text, handler: Handler) -> Text`
Registers a POST route handler.

```obq
http.server.post(server, "/api/users", user_controller, "createUser")
http.server.post(server, "/upload", upload_handler)
```

### `put(server: HttpServerInstance, pattern: Text, handler: Handler) -> Text`
Registers a PUT route handler for updates.

```obq
http.server.put(server, "/api/users/:id", user_controller, "updateUser")
```

### `delete(server: HttpServerInstance, pattern: Text, handler: Handler) -> Text`
Registers a DELETE route handler.

```obq
http.server.delete(server, "/api/users/:id", user_controller, "deleteUser")
```

### `patch(server: HttpServerInstance, pattern: Text, handler: Handler) -> Text`
Registers a PATCH route handler for partial updates.

```obq
http.server.patch(server, "/api/users/:id", user_controller, "patchUser")
```

## Route Patterns

Routes support parameter extraction and wildcards:

```obq
# Path parameters (accessible via request.getParam())
http.server.get(server, "/users/:id", handler)           # /users/123
http.server.get(server, "/users/:id/posts/:postId", handler) # /users/123/posts/456

# Wildcards (matches any path)
http.server.get(server, "/api/*", api_handler)          # /api/v1/users, /api/test, etc.

# Exact matches
http.server.get(server, "/about", about_handler)        # Only /about
```

## Static File Serving

### `static(server: HttpServerInstance, urlPath: Text, fsPath: Text) -> Text`
Serves static files from a directory.

```obq
# Serve files from ./public directory at /static URL
http.server.static(server, "/static", "./public")

# Serve website assets
http.server.static(server, "/assets", "./dist/assets")

# Serve documentation
http.server.static(server, "/docs", "./documentation")
```

## Middleware

### `use(server: HttpServerInstance, middleware: Middleware) -> Text`
Registers middleware that runs before route handlers.

```obq
# String middleware (creates logging middleware)
http.server.use(server, "request_logger")

# Object middleware (must have handle method)
cors_middleware: Object = CorsMiddleware()
http.server.use(server, cors_middleware)
```

## Request Handling

Route handlers receive `HttpRequest` and `HttpResponse` objects:

### HttpRequest Object

#### Properties
- `method`: HTTP method (GET, POST, etc.)
- `path`: Request path
- `query_string`: Raw query string
- `body`: Request body content
- `headers`: Request headers (Map)
- `query_params`: Parsed query parameters (Map)
- `path_params`: Path parameters from route pattern (Map)
- `remote_address`: Client IP address
- `remote_port`: Client port number

#### Methods

##### `getMethod() -> Text`
Gets the HTTP method.

```obq
method: Text = request.getMethod()  # "GET", "POST", etc.
```

##### `getPath() -> Text`
Gets the request path.

```obq
path: Text = request.getPath()  # "/api/users/123"
```

##### `getQuery() -> Text`
Gets the raw query string.

```obq
query: Text = request.getQuery()  # "page=1&limit=10"
```

##### `getBody() -> Text`
Gets the request body.

```obq
body: Text = request.getBody()  # JSON, form data, etc.
```

##### `getHeader(name: Text) -> Text`
Gets a specific request header.

```obq
content_type: Text = request.getHeader("Content-Type")
user_agent: Text = request.getHeader("User-Agent")
auth: Text = request.getHeader("Authorization")
```

##### `getParam(name: Text) -> Text`
Gets a path parameter.

```obq
# For route "/users/:id" and request "/users/123"
user_id: Text = request.getParam("id")  # "123"
```

##### `getQueryParam(name: Text) -> Text`
Gets a query parameter.

```obq
# For request "/search?q=test&page=2"
search_query: Text = request.getQueryParam("q")     # "test"
page: Text = request.getQueryParam("page")          # "2"
```

##### `getRemoteAddress() -> Text`
Gets the client IP address.

```obq
ip: Text = request.getRemoteAddress()  # "192.168.1.100"
```

##### `getRemotePort() -> Int`
Gets the client port number.

```obq
port: Int = request.getRemotePort()  # 54321
```

##### `getHeaders() -> Map`
Gets all request headers as a Map.

```obq
headers: Map = request.getHeaders()
all_keys: List = headers.keys()
```

### HttpResponse Object

#### Properties
- `status_code`: HTTP status code
- `status_message`: HTTP status message
- `body`: Response body content
- `headers`: Response headers (Map)
- `sent`: Whether response has been sent
- `chunked`: Whether response uses chunked encoding

#### Methods

##### `setStatus(code: Int) -> Text`
Sets the HTTP status code and message.

```obq
response.setStatus(200)  # OK
response.setStatus(201)  # Created
response.setStatus(400)  # Bad Request
response.setStatus(404)  # Not Found
response.setStatus(500)  # Internal Server Error
```

##### `setHeader(name: Text, value: Text) -> Text`
Sets a response header.

```obq
response.setHeader("Content-Type", "application/json")
response.setHeader("Cache-Control", "no-cache")
response.setHeader("Access-Control-Allow-Origin", "*")
```

##### `setBody(content: Text) -> Text`
Sets the response body.

```obq
response.setBody("Hello, World!")
response.setBody("{\"message\": \"Success\"}")
```

##### `json(content: Text) -> Text`
Sets JSON response body and Content-Type header.

```obq
json_data: Text = "{\"users\": [\"Alice\", \"Bob\"]}"
response.json(json_data)
```

##### `html(content: Text) -> Text`
Sets HTML response body and Content-Type header.

```obq
html_page: Text = "<html><body><h1>Welcome</h1></body></html>"
response.html(html_page)
```

##### `text(content: Text) -> Text`
Sets plain text response body and Content-Type header.

```obq
response.text("Plain text response")
```

##### `redirect(url: Text, [statusCode: Int]) -> Text`
Redirects to another URL.

```obq
response.redirect("/login")          # 302 redirect
response.redirect("/home", 301)      # 301 permanent redirect
```

##### `send(content: Text) -> Text`
Sends response with content (marks as sent).

```obq
response.send("Final response content")
```

##### Utility Methods

```obq
status: Int = response.getStatus()
header_val: Text = response.getHeader("Content-Type")
body: Text = response.getBody()
```

## Complete Handler Examples

### Simple Route Handler

```obq
Object UserController {
    method getUsers(request: HttpRequest, response: HttpResponse): Text {
        # Extract query parameters
        page: Text = request.getQueryParam("page")
        limit: Text = request.getQueryParam("limit")
        
        # Create response data
        users_json: Text = "[{\"id\": 1, \"name\": \"Alice\"}, {\"id\": 2, \"name\": \"Bob\"}]"
        
        # Set response
        response.setStatus(200)
        response.json(users_json)
        
        return "Users retrieved"
    }
    
    method createUser(request: HttpRequest, response: HttpResponse): Text {
        # Get request body
        user_data: Text = request.getBody()
        
        # Process user creation (validation, database, etc.)
        # ...
        
        # Send success response
        response.setStatus(201)
        response.json("{\"message\": \"User created\", \"id\": 123}")
        
        return "User created"
    }
    
    method getUser(request: HttpRequest, response: HttpResponse): Text {
        # Extract path parameter
        user_id: Text = request.getParam("id")
        
        if user_id.length() == 0 {
            response.setStatus(400)
            response.json("{\"error\": \"User ID required\"}")
            return "Bad request"
        }
        
        # Fetch user by ID
        user_json: Text = "{\"id\": " + user_id + ", \"name\": \"User " + user_id + "\"}"
        
        response.setStatus(200)
        response.json(user_json)
        
        return "User retrieved"
    }
}
```

### Middleware Example

```obq
Object CorsMiddleware {
    method handle(request: HttpRequest, response: HttpResponse, next: NextFunction): Text {
        # Add CORS headers
        response.setHeader("Access-Control-Allow-Origin", "*")
        response.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
        response.setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization")
        
        # Handle preflight requests
        method: Text = request.getMethod()
        if method == "OPTIONS" {
            response.setStatus(200)
            response.send("")
            return "Preflight handled"
        }
        
        # Continue to next middleware/handler
        next.call()
        
        return "CORS middleware executed"
    }
}

Object LoggingMiddleware {
    method handle(request: HttpRequest, response: HttpResponse, next: NextFunction): Text {
        # Log request
        method: Text = request.getMethod()
        path: Text = request.getPath()
        ip: Text = request.getRemoteAddress()
        
        io.print("[%s] %s %s", ip, method, path)
        
        # Continue processing
        next.call()
        
        # Log response
        status: Int = response.getStatus()
        io.print("Response: %d", status)
        
        return "Request logged"
    }
}
```

### Complete Server Setup

```obq
import http.server
import system.io

Object WebServer {
    method start(): Text {
        # Create server
        server: HttpServerInstance = http.server.create()
        
        # Configure server
        http.server.setHost(server, "0.0.0.0")
        http.server.setPort(server, 3000)
        http.server.setWorkerThreads(server, 8)
        
        # Add middleware
        cors: Object = CorsMiddleware()
        logger: Object = LoggingMiddleware()
        http.server.use(server, logger)
        http.server.use(server, cors)
        
        # Register routes
        user_controller: Object = UserController()
        http.server.get(server, "/api/users", user_controller, "getUsers")
        http.server.post(server, "/api/users", user_controller, "createUser")
        http.server.get(server, "/api/users/:id", user_controller, "getUser")
        http.server.put(server, "/api/users/:id", user_controller, "updateUser")
        http.server.delete(server, "/api/users/:id", user_controller, "deleteUser")
        
        # Serve static files
        http.server.static(server, "/", "./public")
        
        # Start server
        success: Bool = http.server.listen(server)
        if success {
            io.print("Server running on http://localhost:3000")
            # Keep server running
            http.server.waitForever(server)
        } else {
            io.print("Failed to start server")
        }
        
        return "Server finished"
    }
}
```

## Server Statistics

### `getStats(server: HttpServerInstance) -> Map`
Returns comprehensive server statistics.

```obq
stats: Map = http.server.getStats(server)

total_requests: Int = stats.get("total_requests")
active_connections: Int = stats.get("active_connections")
error_count: Int = stats.get("error_count")
is_running: Bool = stats.get("is_running")
uptime_seconds: Int = stats.get("uptime_seconds")
requests_per_second: Float = stats.get("requests_per_second")
error_rate_percent: Float = stats.get("error_rate_percent")

io.print("Server Stats:")
io.print("  Requests: %d", total_requests)
io.print("  Connections: %d", active_connections)
io.print("  Errors: %d", error_count)
io.print("  Uptime: %d seconds", uptime_seconds)
io.print("  Rate: %.2f req/sec", requests_per_second)
io.print("  Error Rate: %.2f%%", error_rate_percent)
```

## Custom Logging

### `setLogger(server: HttpServerInstance, logger: Logger) -> Text`
Sets a custom logger for the server.

```obq
Object CustomLogger {
    method log(entry: LogEntry): Text {
        timestamp: Text = entry.getTimestamp()
        method: Text = entry.getMethod()
        path: Text = entry.getPath()
        status: Int = entry.getStatus()
        bytes: Int = entry.getBytes()
        
        io.print("[%s] %s %s -> %d (%d bytes)", timestamp, method, path, status, bytes)
        return "Logged"
    }
    
    method logError(error: ErrorLogEntry): Text {
        timestamp: Text = error.getTimestamp()
        level: Text = error.getLevel()
        message: Text = error.getMessage()
        
        io.print("[%s] %s: %s", timestamp, level, message)
        return "Error logged"
    }
}

logger: Object = CustomLogger()
http.server.setLogger(server, logger)
```

## Error Handling

HTTP Server functions may throw errors for:
- Invalid server configuration (invalid ports, hosts)
- Route registration errors (missing handlers, invalid patterns)
- Server startup failures (port already in use, permission issues)
- Invalid handler signatures

Always handle potential errors:

```obq
try {
    success: Bool = http.server.listen(server)
    if !success {
        io.print("Failed to start server")
    }
} catch error {
    io.print("Server error: %s", error.getMessage())
}
```

## Performance Notes

- The server uses a thread pool for handling concurrent requests
- Default configuration handles 1000 concurrent connections with 4 worker threads
- Static file serving includes automatic MIME type detection
- Middleware executes in registration order
- Request/response objects are created per request for thread safety
- URL parameter extraction and query string parsing are optimized
- Server supports keep-alive connections by default
- Platform-specific optimizations for Windows, macOS, and Linux

## Security Considerations

- Always validate and sanitize input data
- Use HTTPS in production environments
- Implement proper authentication and authorization
- Set appropriate CORS headers for cross-origin requests
- Validate file paths for static file serving to prevent directory traversal
- Configure reasonable request size limits
- Use middleware for security headers (CSP, HSTS, etc.)