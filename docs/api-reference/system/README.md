# System Libraries Reference

O²L provides comprehensive system libraries for interacting with the operating system, file system, and I/O operations.

## System Libraries Overview

### Core System Libraries
- **[system.io](io.md)** - Input/output operations and console interaction
- **[system.fs](fs.md)** - File system operations and directory management
- **[system.os](os.md)** - Operating system interaction and process management

## Import Statements

```obq
import system.io
import system.fs  
import system.os
```

## Quick Reference

### Input/Output Operations
| Function | Purpose | Example |
|----------|---------|---------|
| `io.print()` | Print formatted text | `io.print("Hello %s", name)` |
| `io.println()` | Print with newline | `io.println("Debug info")` |
| `io.readLine()` | Read user input | `input: Text = io.readLine()` |
| `io.flush()` | Flush output buffer | `io.flush()` |

### File System Operations  
| Function | Purpose | Example |
|----------|---------|---------|
| `fs.readFile()` | Read file contents | `content: Text = fs.readFile("data.txt")` |
| `fs.writeFile()` | Write file contents | `fs.writeFile("output.txt", data)` |
| `fs.exists()` | Check if path exists | `exists: Bool = fs.exists("myfile.txt")` |
| `fs.createDir()` | Create directory | `fs.createDir("new_folder")` |
| `fs.listDir()` | List directory contents | `files: List = fs.listDir(".")` |

### Operating System Interaction
| Function | Purpose | Example |
|----------|---------|---------|
| `os.args()` | Command line arguments | `args: List = os.args()` |
| `os.getEnv()` | Environment variables | `path: Text = os.getEnv("PATH")` |
| `os.systemInfo()` | System information | `info: Map = os.systemInfo()` |
| `os.execute()` | Run system command | `result: Int = os.execute("ls -la")` |

## Common Use Cases

### File Processing
```obq
import system.fs
import system.io

# Read and process a configuration file
if fs.exists("config.txt") {
    config: Text = fs.readFile("config.txt")
    lines: List = config.split("\n")
    
    # Process each line
    lines.forEach(line -> {
        if !line.startsWith("#") {  # Skip comments
            io.println("Config: %s", line)
        }
    })
} else {
    io.println("Config file not found")
}
```

### Directory Operations
```obq
import system.fs
import system.io

# Create a backup directory structure
backup_dir: Text = "backup_" + datetime.now().format("yyyy-MM-dd")
fs.createDir(backup_dir)

# List and copy important files
files: List = fs.listDir(".")
files.forEach(file -> {
    if file.endsWith(".txt") {
        content: Text = fs.readFile(file)
        backup_path: Text = backup_dir + "/" + file
        fs.writeFile(backup_path, content)
        io.println("Backed up: %s", file)
    }
})
```

### System Information Gathering
```obq
import system.os
import system.io

# Display system information
info: Map = os.systemInfo()
memory: Map = os.memoryInfo()
cpu: Map = os.cpuInfo()

io.println("=== System Information ===")
io.println("OS: %s %s", info.get("name"), info.get("version"))
io.println("Architecture: %s", info.get("arch"))
io.println("Hostname: %s", info.get("hostname"))

io.println("\n=== Memory Information ===")
io.println("Total: %d MB", memory.get("total_mb"))
io.println("Available: %d MB", memory.get("available_mb"))
io.println("Used: %.1f%%", memory.get("used_percent"))

io.println("\n=== CPU Information ===")
io.println("Cores: %d", cpu.get("cores"))
io.println("Logical CPUs: %d", cpu.get("logical_cpus"))
io.println("Usage: %.1f%%", cpu.get("usage_percent"))
```

### Command Line Processing
```obq
import system.os
import system.io

# Process command line arguments
args: List = os.args()
argc: Int = os.argc()

if argc < 2 {
    io.println("Usage: %s <filename>", args.get(0))
    return
}

filename: Text = args.get(1)
io.println("Processing file: %s", filename)

# Check for optional flags
verbose: Bool = args.contains("--verbose")
if verbose {
    io.println("Verbose mode enabled")
}
```

### Environment Configuration
```obq
import system.os
import system.io

# Read environment configuration
home: Text = os.getEnv("HOME")
path: Text = os.getEnv("PATH")
user: Text = os.getEnv("USER")

io.println("User: %s", user)
io.println("Home Directory: %s", home)

# Check if development tools are available
path_dirs: List = path.split(":")
has_node: Bool = path_dirs.any(dir -> fs.exists(dir + "/node"))
has_python: Bool = path_dirs.any(dir -> fs.exists(dir + "/python"))

io.println("Node.js available: %s", has_node)
io.println("Python available: %s", has_python)
```

## Error Handling Patterns

### File Operations
```obq
import system.fs
import system.io

try {
    content: Text = fs.readFile("important.txt")
    io.println("File contents: %s", content)
} catch error {
    error_type: Text = error.getType()
    if error_type == "FileNotFound" {
        io.println("File does not exist")
    } else if error_type == "PermissionDenied" {
        io.println("No permission to read file")
    } else {
        io.println("Error reading file: %s", error.getMessage())
    }
}
```

### System Commands
```obq
import system.os
import system.io

try {
    exit_code: Int = os.execute("ls /nonexistent")
    if exit_code != 0 {
        io.println("Command failed with exit code: %d", exit_code)
    }
} catch error {
    io.println("Failed to execute command: %s", error.getMessage())
}
```

## Cross-Platform Considerations

### Path Handling
```obq
import system.os
import system.fs

# Use OS-appropriate path separators
separator: Text = os.pathSeparator()  # "/" on Unix, "\" on Windows
config_path: Text = home + separator + ".config" + separator + "app.conf"

# Or use path joining utilities
config_path: Text = fs.joinPath([home, ".config", "app.conf"])
```

### System Commands
```obq
import system.os

# Cross-platform directory listing
info: Map = os.systemInfo()
os_name: Text = info.get("name")

if os_name.contains("Windows") {
    os.execute("dir")
} else {
    os.execute("ls -la")
}
```

## Performance Considerations

### File I/O
- **Batch Operations**: Read/write files in chunks for large files
- **Buffer Management**: Use appropriate buffer sizes for streaming
- **Resource Cleanup**: Ensure file handles are properly closed

### System Calls
- **Command Execution**: Minimize system command usage for performance
- **Environment Access**: Cache frequently accessed environment variables
- **Process Information**: Avoid polling system info in tight loops

## Security Notes

### File Operations
- **Path Validation**: Always validate file paths to prevent directory traversal
- **Permission Checks**: Verify file permissions before operations
- **Temporary Files**: Use secure temporary file creation methods

### System Commands
- **Input Sanitization**: Sanitize all inputs to system commands
- **Command Injection**: Use parameterized commands when possible
- **Privilege Escalation**: Run with minimal required privileges

## See Also

- **[Core Types](../core/README.md)** - Data types used with system operations
- **[HTTP Libraries](../libraries/README.md)** - Network programming
- **[JSON Library](../libraries/json.md)** - Data serialization for system integration
- **[DateTime Library](../libraries/datetime.md)** - Time-based operations for logging and scheduling