# HTTP Client Library

The HTTP Client library provides comprehensive HTTP request capabilities for O²L applications, supporting all standard HTTP methods, authentication, file uploads/downloads, and advanced request configuration.

## Import

```obq
import http
```

## Basic HTTP Methods

### `get(url: Text, [headers: Map]) -> HttpResponse`
Performs an HTTP GET request.

```obq
# Simple GET request
response: HttpResponse = http.get("https://api.github.com/users/octocat")

# GET with headers
headers: Map = Map()
headers.put("User-Agent", "O2L-HTTP-Client/1.0")
headers.put("Accept", "application/json")
response: HttpResponse = http.get("https://api.example.com/data", headers)
```

### `post(url: Text, [body: Text], [headers: Map]) -> HttpResponse`
Performs an HTTP POST request.

```obq
# Simple POST with JSON body
json_data: Text = "{\"name\": \"Alice\", \"age\": 30}"
response: HttpResponse = http.post("https://api.example.com/users", json_data)

# POST with headers
headers: Map = Map()
headers.put("Content-Type", "application/json")
headers.put("Authorization", "Bearer your-token")
response: HttpResponse = http.post("https://api.example.com/users", json_data, headers)
```

### `put(url: Text, [body: Text], [headers: Map]) -> HttpResponse`
Performs an HTTP PUT request for updating resources.

```obq
json_data: Text = "{\"id\": 1, \"name\": \"Updated Name\"}"
response: HttpResponse = http.put("https://api.example.com/users/1", json_data)
```

### `delete(url: Text, [headers: Map]) -> HttpResponse`
Performs an HTTP DELETE request.

```obq
response: HttpResponse = http.delete("https://api.example.com/users/1")
```

### `patch(url: Text, [body: Text], [headers: Map]) -> HttpResponse`
Performs an HTTP PATCH request for partial updates.

```obq
patch_data: Text = "{\"status\": \"active\"}"
response: HttpResponse = http.patch("https://api.example.com/users/1", patch_data)
```

### `head(url: Text, [headers: Map]) -> HttpResponse`
Performs an HTTP HEAD request (headers only).

```obq
response: HttpResponse = http.head("https://api.example.com/status")
```

### `options(url: Text, [headers: Map]) -> HttpResponse`
Performs an HTTP OPTIONS request to check allowed methods.

```obq
response: HttpResponse = http.options("https://api.example.com/users")
```

## Advanced Request Methods

### `request(method: Text, url: Text, [body: Text], [headers: Map]) -> HttpResponse`
Generic HTTP request method with custom HTTP method.

```obq
# Custom HTTP method
response: HttpResponse = http.request("PATCH", "https://api.example.com/resource", json_data)

# With all parameters
headers: Map = Map()
headers.put("Content-Type", "application/json")
response: HttpResponse = http.request("POST", "https://api.example.com/data", body_data, headers)
```

### `requestWithConfig(request: HttpRequest) -> HttpResponse`
Execute a configured HTTP request object.

```obq
# Create and configure request
request: HttpRequest = http.createRequest("POST", "https://api.example.com/data")
http.setJsonBody(request, "{\"data\": \"test\"}")
http.setBearerToken(request, "your-token")
http.setTimeout(request, 30)

# Execute configured request
response: HttpResponse = http.requestWithConfig(request)
```

## Request Configuration

### `createRequest(method: Text, url: Text) -> HttpRequest`
Creates a configurable HTTP request object.

```obq
request: HttpRequest = http.createRequest("POST", "https://api.example.com/upload")
```

### Header Configuration

#### `setHeader(request: HttpRequest, name: Text, value: Text) -> Text`
Sets a single header on the request.

```obq
http.setHeader(request, "Content-Type", "application/json")
http.setHeader(request, "User-Agent", "MyApp/1.0")
```

#### `setHeaders(request: HttpRequest, headers: Map) -> Text`
Sets multiple headers from a Map.

```obq
headers: Map = Map()
headers.put("Accept", "application/json")
headers.put("Cache-Control", "no-cache")
http.setHeaders(request, headers)
```

### Query Parameter Configuration

#### `setQueryParam(request: HttpRequest, name: Text, value: Text) -> Text`
Sets a single query parameter.

```obq
http.setQueryParam(request, "page", "1")
http.setQueryParam(request, "limit", "50")
```

#### `setQueryParams(request: HttpRequest, params: Map) -> Text`
Sets multiple query parameters from a Map.

```obq
params: Map = Map()
params.put("sort", "name")
params.put("order", "asc")
params.put("filter", "active")
http.setQueryParams(request, params)
```

### Body Configuration

#### `setBody(request: HttpRequest, body: Text) -> Text`
Sets the request body.

```obq
http.setBody(request, "raw body content")
```

#### `setJsonBody(request: HttpRequest, json: Text) -> Text`
Sets JSON body and automatically sets Content-Type to application/json.

```obq
json_data: Text = "{\"name\": \"Bob\", \"email\": \"bob@example.com\"}"
http.setJsonBody(request, json_data)
```

### Request Options

#### `setTimeout(request: HttpRequest, seconds: Int) -> Text`
Sets the request timeout in seconds.

```obq
http.setTimeout(request, 60)  # 60 second timeout
```

## Authentication Methods

### `setBasicAuth(request: HttpRequest, username: Text, password: Text) -> Text`
Sets HTTP Basic Authentication.

```obq
http.setBasicAuth(request, "myuser", "mypassword")
```

### `setBearerToken(request: HttpRequest, token: Text) -> Text`
Sets Bearer token authentication.

```obq
http.setBearerToken(request, "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...")
```

### `setApiKey(request: HttpRequest, headerName: Text, apiKey: Text) -> Text`
Sets custom API key authentication.

```obq
http.setApiKey(request, "X-API-Key", "your-secret-api-key")
http.setApiKey(request, "Authorization", "ApiKey your-key")
```

## Response Handling

### Status Information

#### `getStatus(response: HttpResponse) -> Int`
Gets the HTTP status code.

```obq
status: Int = http.getStatus(response)  # 200, 404, 500, etc.
```

#### `getStatusMessage(response: HttpResponse) -> Text`
Gets the HTTP status message.

```obq
message: Text = http.getStatusMessage(response)  # "OK", "Not Found", etc.
```

#### `isSuccess(response: HttpResponse) -> Bool`
Checks if the response indicates success (2xx status codes).

```obq
success: Bool = http.isSuccess(response)  # true for 200-299
```

### Response Data

#### `getBody(response: HttpResponse) -> Text`
Gets the response body as text.

```obq
body: Text = http.getBody(response)
```

#### `getJson(response: HttpResponse) -> Text`
Gets the response body if it's JSON, or empty JSON object for non-JSON responses.

```obq
json_data: Text = http.getJson(response)
```

### Response Headers

#### `getHeader(response: HttpResponse, name: Text) -> Text`
Gets a specific response header.

```obq
content_type: Text = http.getHeader(response, "Content-Type")
server: Text = http.getHeader(response, "Server")
```

#### `getHeaders(response: HttpResponse) -> Map`
Gets all response headers as a Map.

```obq
headers: Map = http.getHeaders(response)
all_keys: List = headers.keys()
```

### Error Handling

#### `getError(response: HttpResponse) -> Text`
Gets error message if the request failed.

```obq
error: Text = http.getError(response)
if error.length() > 0 {
    io.print("Request failed: %s", error)
}
```

## URL Utilities

### `urlEncode(text: Text) -> Text`
URL-encodes a string for safe inclusion in URLs.

```obq
encoded: Text = http.urlEncode("hello world & test")  # "hello%20world%20%26%20test"
```

### `urlDecode(text: Text) -> Text`
URL-decodes a string.

```obq
decoded: Text = http.urlDecode("hello%20world%20%26%20test")  # "hello world & test"
```

### `buildUrl(baseUrl: Text, [params: Map]) -> Text`
Builds a URL with query parameters.

```obq
base: Text = "https://api.example.com/search"
params: Map = Map()
params.put("q", "O2L programming")
params.put("page", "1")
params.put("limit", "10")

final_url: Text = http.buildUrl(base, params)
# Result: "https://api.example.com/search?q=O2L%20programming&page=1&limit=10"
```

### `parseUrl(url: Text) -> Map`
Parses a URL into components.

```obq
url: Text = "https://api.example.com/v1/users?page=1&limit=10"
parts: Map = http.parseUrl(url)

protocol: Text = parts.get("protocol")  # "https"
host: Text = parts.get("host")          # "api.example.com"
path: Text = parts.get("path")          # "/v1/users"
query: Text = parts.get("query")        # "page=1&limit=10"
```

## File Operations

### `uploadFile(url: Text, filePath: Text, [formFields: Map]) -> HttpResponse`
Uploads a file using multipart/form-data.

```obq
# Simple file upload
response: HttpResponse = http.uploadFile("https://api.example.com/upload", "/path/to/file.jpg")

# Upload with additional form fields
form_data: Map = Map()
form_data.put("title", "My Photo")
form_data.put("description", "A beautiful sunset")
response: HttpResponse = http.uploadFile("https://api.example.com/upload", "/path/to/photo.jpg", form_data)
```

### `downloadFile(url: Text, destinationPath: Text) -> HttpResponse`
Downloads a file from a URL to local storage.

```obq
response: HttpResponse = http.downloadFile("https://example.com/data.pdf", "/local/path/data.pdf")

if http.isSuccess(response) {
    io.print("File downloaded successfully")
} else {
    error: Text = http.getError(response)
    io.print("Download failed: %s", error)
}
```

## Complete Example

```obq
import http
import json
import system.io

Object HttpExample {
    method demonstrateHttp(): Text {
        io.print("=== HTTP Client Library Example ===")
        
        # Simple GET request
        response: HttpResponse = http.get("https://api.github.com/users/octocat")
        
        if http.isSuccess(response) {
            status: Int = http.getStatus(response)
            body: Text = http.getBody(response)
            io.print("GET successful (status %d)", status)
            
            # Parse JSON response
            user_data: Map = json.parseAuto(body)
            login: Text = user_data.get("login")
            io.print("GitHub user: %s", login)
        } else {
            error: Text = http.getError(response)
            io.print("GET failed: %s", error)
        }
        
        # POST request with authentication
        request: HttpRequest = http.createRequest("POST", "https://api.example.com/data")
        
        # Set authentication
        http.setBearerToken(request, "your-api-token")
        
        # Set JSON body
        post_data: Text = json.createObject("name", "Charlie", "age", 25)
        http.setJsonBody(request, post_data)
        
        # Add custom headers
        http.setHeader(request, "X-Custom-Header", "MyApp/1.0")
        
        # Set timeout
        http.setTimeout(request, 30)
        
        # Execute request
        post_response: HttpResponse = http.requestWithConfig(request)
        
        if http.isSuccess(post_response) {
            result: Text = http.getBody(post_response)
            io.print("POST successful: %s", result)
        }
        
        # URL building example
        search_params: Map = Map()
        search_params.put("q", "O2L programming")
        search_params.put("sort", "relevance")
        
        search_url: Text = http.buildUrl("https://api.example.com/search", search_params)
        io.print("Built URL: %s", search_url)
        
        # URL utilities
        encoded: Text = http.urlEncode("hello world!")
        decoded: Text = http.urlDecode(encoded)
        io.print("URL encode/decode: %s -> %s -> %s", "hello world!", encoded, decoded)
        
        return "HTTP demonstration completed"
    }
    
    method uploadExample(): Text {
        # File upload example
        form_fields: Map = Map()
        form_fields.put("title", "My Document")
        form_fields.put("category", "reports")
        
        upload_response: HttpResponse = http.uploadFile(
            "https://api.example.com/files/upload",
            "/path/to/document.pdf",
            form_fields
        )
        
        if http.isSuccess(upload_response) {
            io.print("File uploaded successfully")
            upload_result: Text = http.getJson(upload_response)
            file_id: Text = json.get(upload_result, "file_id")
            io.print("Uploaded file ID: %s", file_id)
        } else {
            error: Text = http.getError(upload_response)
            io.print("Upload failed: %s", error)
        }
        
        return "Upload example completed"
    }
}
```

## Error Handling

HTTP Client functions may throw errors for:
- Invalid URLs or malformed requests
- Network connectivity issues
- Timeout errors
- Invalid authentication credentials
- File access errors (for upload/download operations)

Always check response status and handle errors appropriately:

```obq
response: HttpResponse = http.get("https://api.example.com/data")

if http.isSuccess(response) {
    # Handle successful response
    data: Text = http.getBody(response)
    # Process data...
} else {
    # Handle error response
    status: Int = http.getStatus(response)
    error: Text = http.getError(response)
    io.print("Request failed with status %d: %s", status, error)
}
```

## Request/Response Objects

### HttpRequest Properties
- `method`: HTTP method (GET, POST, etc.)
- `url`: Target URL
- `headers`: Request headers (Map)
- `body`: Request body content
- `query_params`: URL query parameters (Map)
- `timeout_seconds`: Request timeout
- `follow_redirects`: Whether to follow HTTP redirects
- `verify_ssl`: Whether to verify SSL certificates

### HttpResponse Properties
- `status_code`: HTTP status code (Int)
- `status_message`: HTTP status message (Text)
- `headers`: Response headers (Map)
- `body`: Response body (Text)
- `success`: Whether request was successful (Bool)
- `error_message`: Error details if request failed (Text)

## Performance Notes

- The library supports multiple platforms (Windows, macOS, Linux) with platform-specific implementations
- Requests are thread-safe but use mutex locking for safety
- Default timeout is configurable per request
- SSL certificate verification can be disabled for testing (not recommended for production)
- File uploads use efficient multipart/form-data encoding
- Response bodies are loaded entirely into memory (consider size for large downloads)