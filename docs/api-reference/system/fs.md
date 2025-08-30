# system.fs API Reference

## Overview

The system.fs module provides comprehensive filesystem operations for O²L programs. It handles file I/O, directory management, path manipulation, and file system queries through a clean, cross-platform API. All operations are type-safe and include proper error handling.

## Import

```o2l
import system.fs
# Or import all functions
import system.fs.*
```

## Quick Example

```o2l
import system.fs
import system.io

# Write and read a file
fs.writeText("hello.txt", "Hello, World!")
content: Text = fs.readText("hello.txt")
io.print("File content: %s", content)

# Check file properties
exists: Bool = fs.exists("hello.txt")
is_file: Bool = fs.isFile("hello.txt")
io.print("Exists: %o, Is file: %o", exists, is_file)

# Path manipulation
filename: Text = fs.basename("documents/readme.txt")  // "readme.txt"
directory: Text = fs.dirname("documents/readme.txt")  // "documents"
```

---

## File Operations

### `readText(path: Text) → Text`

Reads the entire contents of a text file and returns it as a string.

```o2l
# Read configuration file
config_content: Text = fs.readText("config.json")
io.print("Config: %s", config_content)

# Read with error handling
try {
    data: Text = fs.readText("data.txt")
    io.print("File size: %d characters", data.length())
} catch (error: Exception) {
    io.error("Failed to read file: %s", error.message)
}
```

### `writeText(path: Text, content: Text) → Bool`

Writes text content to a file, creating it if it doesn't exist or overwriting if it does.

```o2l
# Write simple content
success: Bool = fs.writeText("output.txt", "Hello, World!")
io.print("Write successful: %o", success)

# Write formatted content
data: Text = "User: Alice\nAge: 30\nEmail: alice@example.com"
result: Bool = fs.writeText("user_data.txt", data)

# Write empty file
empty_file: Bool = fs.writeText("empty.txt", "")
```

---

## File System Queries

### `exists(path: Text) → Bool`

Returns true if the specified path exists (file or directory).

```o2l
# Check if file exists before reading
if (fs.exists("important.txt")) {
    content: Text = fs.readText("important.txt")
    io.print("File found: %s", content)
} else {
    io.print("File not found")
}

# Check multiple paths
files: List<Text> = ["config.json", "data.txt", "backup.sql"]
iterator: ListIterator = files.iterator()

while (iterator.hasNext()) {
    file: Text = iterator.next()
    exists: Bool = fs.exists(file)
    io.print("%s: %s", file, exists ? "EXISTS" : "NOT FOUND")
}
```

### `isFile(path: Text) → Bool`

Returns true if the path exists and is a regular file.

```o2l
# Distinguish between files and directories
path: Text = "documents/readme.txt"

if (fs.exists(path)) {
    if (fs.isFile(path)) {
        io.print("%s is a file", path)
        size: Text = fs.readText(path)
        io.print("Content length: %d", size.length())
    } else {
        io.print("%s exists but is not a file", path)
    }
}
```

### `isDirectory(path: Text) → Bool`

Returns true if the path exists and is a directory.

```o2l
# Check directory status
path: Text = "project_folder"

if (fs.exists(path)) {
    if (fs.isDirectory(path)) {
        io.print("%s is a directory", path)
        files: List<Text> = fs.listFiles(path)
        io.print("Contains %d items", files.size())
    } else if (fs.isFile(path)) {
        io.print("%s is a file, not a directory", path)
    }
}
```

---

## Directory Operations

### `listFiles(directory: Text) → List<Text>`

Returns a list of file and directory names in the specified directory.

```o2l
# List current directory contents
current_files: List<Text> = fs.listFiles(".")
io.print("Files in current directory: %d", current_files.size())

iterator: ListIterator = current_files.iterator()
while (iterator.hasNext()) {
    filename: Text = iterator.next()

    # Get full path for type checking
    full_path: Text = fs.join(".", filename)
    if (fs.isFile(full_path)) {
        io.print("  FILE: %s", filename)
    } else if (fs.isDirectory(full_path)) {
        io.print("  DIR:  %s", filename)
    }
}
```

### `createDirectory(path: Text) → Bool`

Creates a directory (and parent directories if needed). Returns true on success.

```o2l
# Create single directory
success: Bool = fs.createDirectory("new_folder")
io.print("Directory created: %o", success)

# Create nested directory structure
nested_success: Bool = fs.createDirectory("project/src/components")
io.print("Nested directories created: %o", nested_success)

# Verify directory creation
if (fs.exists("project/src/components") && fs.isDirectory("project/src/components")) {
    io.print("Directory structure verified")
}
```

### `deleteFile(path: Text) → Bool`

Deletes a file or empty directory. Returns true on success.

```o2l
# Delete a file
delete_file: Bool = fs.deleteFile("temp.txt")
io.print("File deleted: %o", delete_file)

# Delete empty directory
delete_dir: Bool = fs.deleteFile("empty_folder")
io.print("Directory deleted: %o", delete_dir)

# Cleanup example: delete files then directory
files_to_clean: List<Text> = ["temp1.txt", "temp2.txt", "temp3.txt"]
cleanup_iterator: ListIterator = files_to_clean.iterator()

while (cleanup_iterator.hasNext()) {
    file: Text = cleanup_iterator.next()
    if (fs.exists(file)) {
        result: Bool = fs.deleteFile(file)
        io.print("Deleted %s: %o", file, result)
    }
}

# Delete directory after files are removed
fs.deleteFile("temp_folder")
```

---

## Path Manipulation

### `basename(path: Text) → Text`

Extracts the filename from a path (last component).

```o2l
# Extract filenames
filename1: Text = fs.basename("/home/user/document.txt")    // "document.txt"
filename2: Text = fs.basename("project/src/main.o2l")       // "main.o2l"
filename3: Text = fs.basename("/usr/local/bin/program")     // "program"

io.print("Files: %s, %s, %s", filename1, filename2, filename3)

# Handle different path formats
paths: List<Text> = [
    "/absolute/path/file.txt",
    "relative/path/file.txt",
    "simple_file.txt",
    "/directory/",
    "directory"
]

path_iterator: ListIterator = paths.iterator()
while (path_iterator.hasNext()) {
    path: Text = path_iterator.next()
    name: Text = fs.basename(path)
    io.print("Path: %-25s -> Basename: %s", path, name)
}
```

### `dirname(path: Text) → Text`

Extracts the directory path (all components except the last).

```o2l
# Extract directory paths
dir1: Text = fs.dirname("/home/user/document.txt")    // "/home/user"
dir2: Text = fs.dirname("project/src/main.o2l")       // "project/src"
dir3: Text = fs.dirname("/usr/local/bin/program")     // "/usr/local/bin"

io.print("Directories: %s, %s, %s", dir1, dir2, dir3)

# Use dirname for navigation
current_file: Text = "/var/log/app/current.log"
log_directory: Text = fs.dirname(current_file)        // "/var/log/app"
archive_path: Text = fs.join(log_directory, "archive")
io.print("Archive will be stored in: %s", archive_path)
```

### `extname(path: Text) → Text`

Extracts the file extension (including the dot).

```o2l
# Extract extensions
ext1: Text = fs.extname("document.txt")        // ".txt"
ext2: Text = fs.extname("archive.tar.gz")      // ".gz"
ext3: Text = fs.extname("readme")              // ""
ext4: Text = fs.extname(".bashrc")             // ""

io.print("Extensions: '%s', '%s', '%s', '%s'", ext1, ext2, ext3, ext4)

# Filter files by extension
files: List<Text> = fs.listFiles(".")
file_iterator: ListIterator = files.iterator()

while (file_iterator.hasNext()) {
    file: Text = file_iterator.next()
    extension: Text = fs.extname(file)

    if (extension == ".txt") {
        io.print("Text file found: %s", file)
    } else if (extension == ".o2l") {
        io.print("O²L source file: %s", file)
    }
}
```

### `join(...paths: Text) → Text`

Combines multiple path components into a single path using the appropriate separator.

```o2l
# Basic path joining
config_path: Text = fs.join("app", "config", "settings.json")
io.print("Config path: %s", config_path)

# Join with absolute base
system_path: Text = fs.join("/usr", "local", "bin", "myapp")
io.print("System path: %s", system_path)

# Build paths dynamically
user: Text = "alice"
project: Text = "web_app"
file: Text = "index.html"
full_path: Text = fs.join("users", user, "projects", project, "public", file)
io.print("Full path: %s", full_path)

# Handle empty components gracefully
mixed_path: Text = fs.join("/root", "", "documents", "important.txt")
io.print("Mixed path: %s", mixed_path)
```

### `normalize(path: Text) → Text`

Normalizes a path by resolving `.` and `..` components.

```o2l
# Normalize complex paths
complex1: Text = "/usr/./local/../bin/program"
normalized1: Text = fs.normalize(complex1)
io.print("Complex: %s", complex1)
io.print("Normal:  %s", normalized1)  // "/usr/bin/program"

# Normalize relative paths
complex2: Text = "./documents/../photos/./vacation.jpg"
normalized2: Text = fs.normalize(complex2)
io.print("Relative: %s", complex2)
io.print("Normal:   %s", normalized2)  // "photos/vacation.jpg"

# Use in file operations
messy_path: Text = "project/../project/src/./main.o2l"
clean_path: Text = fs.normalize(messy_path)
if (fs.exists(clean_path)) {
    content: Text = fs.readText(clean_path)
    io.print("File found at normalized path: %s", clean_path)
}
```

### `resolve(path: Text) → Text`

Converts a relative path to an absolute path based on the current working directory.

```o2l
# Resolve relative paths
relative_path: Text = "documents/readme.txt"
absolute_path: Text = fs.resolve(relative_path)
io.print("Relative: %s", relative_path)
io.print("Absolute: %s", absolute_path)

# Resolve current directory
current_dir: Text = fs.resolve(".")
io.print("Current directory: %s", current_dir)

# Resolve parent directory
parent_dir: Text = fs.resolve("..")
io.print("Parent directory: %s", parent_dir)

# Use resolved paths for operations
config_file: Text = fs.resolve("config/app.json")
if (fs.exists(config_file)) {
    config_data: Text = fs.readText(config_file)
    io.print("Config loaded from: %s", config_file)
}
```

### `relative(from: Text, to: Text) → Text`

Returns the relative path from one location to another.

```o2l
# Calculate relative paths
from_path: Text = "/home/user/projects/webapp"
to_path: Text = "/home/user/documents/specs.pdf"
relative_path: Text = fs.relative(from_path, to_path)

io.print("From: %s", from_path)
io.print("To:   %s", to_path)
io.print("Relative: %s", relative_path)  // "../../documents/specs.pdf"

# Use for creating portable references
project_root: Text = "/var/www/myapp"
asset_path: Text = "/var/www/myapp/assets/images/logo.png"
relative_asset: Text = fs.relative(project_root, asset_path)
io.print("Asset reference: %s", relative_asset)  // "assets/images/logo.png"
```

### `isAbsolute(path: Text) → Bool`

Returns true if the path is absolute (starts from root).

```o2l
# Check path types
paths: List<Text> = [
    "/usr/local/bin",        # Absolute
    "documents/file.txt",    # Relative
    "./config.json",         # Relative
    "../parent/file.txt",    # Relative
    "/",                     # Absolute root
    ""                       # Empty (relative)
]

path_iterator: ListIterator = paths.iterator()
while (path_iterator.hasNext()) {
    path: Text = path_iterator.next()
    is_abs: Bool = fs.isAbsolute(path)
    type_str: Text = is_abs ? "ABSOLUTE" : "RELATIVE"
    io.print("%-20s -> %s", path, type_str)
}

# Conditionally resolve paths
input_path: Text = "data/input.txt"
final_path: Text = fs.isAbsolute(input_path) ? input_path : fs.resolve(input_path)
io.print("Final path: %s", final_path)
```

### `splitPath(path: Text) → List<Text>`

Splits a path into its individual components.

```o2l
# Split various paths
paths_to_split: List<Text> = [
    "/usr/local/bin/program",
    "documents/projects/webapp/src",
    "simple_file.txt",
    "/",
    ""
]

split_iterator: ListIterator = paths_to_split.iterator()
while (split_iterator.hasNext()) {
    path: Text = split_iterator.next()
    components: List<Text> = fs.splitPath(path)

    io.print("Path: %s", path)
    io.print("Components (%d):", components.size())

    component_iterator: ListIterator = components.iterator()
    index: Int = 0
    while (component_iterator.hasNext()) {
        component: Text = component_iterator.next()
        io.print("  [%d]: '%s'", index, component)
        index = index + 1
    }
    io.print("")
}
```

### `getParent(path: Text) → Text`

Returns the parent directory of the given path (alias for dirname).

```o2l
# Get parent directories
file_path: Text = "/var/log/app/error.log"
parent: Text = fs.getParent(file_path)
io.print("File: %s", file_path)
io.print("Parent: %s", parent)  // "/var/log/app"

# Navigate to parent for operations
log_file: Text = "/var/log/myapp/current.log"
log_dir: Text = fs.getParent(log_file)
archive_dir: Text = fs.join(log_dir, "archive")

# Create archive directory if needed
if (!fs.exists(archive_dir)) {
    success: Bool = fs.createDirectory(archive_dir)
    io.print("Archive directory created: %o", success)
}
```

### `changeExtension(path: Text, new_extension: Text) → Text`

Changes or adds a file extension to a path.

```o2l
# Change file extensions
doc_file: Text = "report.docx"
pdf_file: Text = fs.changeExtension(doc_file, ".pdf")
io.print("Convert: %s -> %s", doc_file, pdf_file)

# Add extension to file without one
script_file: Text = "backup_script"
shell_file: Text = fs.changeExtension(script_file, ".sh")
io.print("Add extension: %s -> %s", script_file, shell_file)

# Remove extension
temp_file: Text = "data.tmp"
final_file: Text = fs.changeExtension(temp_file, "")
io.print("Remove extension: %s -> %s", temp_file, final_file)

# Change extension without dot
log_file: Text = "app.log"
backup_file: Text = fs.changeExtension(log_file, "bak")
io.print("Change extension: %s -> %s", log_file, backup_file)

# Work with full paths
full_path: Text = "/home/user/projects/data.json"
backup_path: Text = fs.changeExtension(full_path, ".backup")
io.print("Full path: %s -> %s", full_path, backup_path)
```

---

## Real-World Examples

### Configuration Management

```o2l
import system.fs
import system.io

Object ConfigManager {
    method loadConfig(): Text {
        # Build configuration path
        config_dir: Text = fs.resolve("config")
        main_config: Text = fs.join(config_dir, "app.json")
        default_config: Text = fs.join(config_dir, "default.json")

        # Try main config first, fall back to default
        config_path: Text = ""
        if (fs.exists(main_config)) {
            config_path = main_config
            io.print("Using main configuration")
        } else if (fs.exists(default_config)) {
            config_path = default_config
            io.print("Using default configuration")
        } else {
            io.error("No configuration found")
            return ""
        }

        # Load and return configuration
        config_content: Text = fs.readText(config_path)
        io.print("Loaded config from: %s", config_path)
        return config_content
    }

    method backupConfig(): Bool {
        config_file: Text = fs.join("config", "app.json")

        if (!fs.exists(config_file)) {
            io.error("No configuration file to backup")
            return false
        }

        # Create backup filename with timestamp (simplified)
        backup_name: Text = fs.changeExtension(config_file, ".backup")

        # Read original and write backup
        original_content: Text = fs.readText(config_file)
        backup_success: Bool = fs.writeText(backup_name, original_content)

        io.print("Configuration backed up: %o", backup_success)
        return backup_success
    }
}
```

### Log File Management

```o2l
Object LogManager {
    property log_directory: Text

    constructor(log_dir: Text) {
        this.log_directory = fs.resolve(log_dir)

        # Ensure log directory exists
        if (!fs.exists(this.log_directory)) {
            created: Bool = fs.createDirectory(this.log_directory)
            io.print("Log directory created: %o", created)
        }
    }

    method writeLog(level: Text, message: Text): Bool {
        # Create log filename
        log_file: Text = fs.join(this.log_directory, "app.log")

        # Format log entry (simplified)
        log_entry: Text = "[" + level + "] " + message + "\n"

        # Append to existing log or create new one
        existing_content: Text = ""
        if (fs.exists(log_file)) {
            existing_content = fs.readText(log_file)
        }

        new_content: Text = existing_content + log_entry
        return fs.writeText(log_file, new_content)
    }

    method archiveLogs(): Bool {
        current_log: Text = fs.join(this.log_directory, "app.log")

        if (!fs.exists(current_log)) {
            io.print("No current log to archive")
            return true
        }

        # Create archive directory
        archive_dir: Text = fs.join(this.log_directory, "archive")
        if (!fs.exists(archive_dir)) {
            fs.createDirectory(archive_dir)
        }

        # Create archived filename (simplified timestamp)
        archived_log: Text = fs.join(archive_dir, "app-archived.log")

        # Move current log to archive
        log_content: Text = fs.readText(current_log)
        archive_success: Bool = fs.writeText(archived_log, log_content)

        if (archive_success) {
            # Clear current log
            fs.writeText(current_log, "")
            io.print("Log archived successfully")
            return true
        }

        return false
    }

    method listLogFiles(): List<Text> {
        if (!fs.exists(this.log_directory)) {
            return []
        }

        all_files: List<Text> = fs.listFiles(this.log_directory)
        log_files: List<Text> = []

        file_iterator: ListIterator = all_files.iterator()
        while (file_iterator.hasNext()) {
            filename: Text = file_iterator.next()
            extension: Text = fs.extname(filename)

            if (extension == ".log") {
                log_files.add(filename)
            }
        }

        return log_files
    }
}
```

### Project File Organization

````o2l
Object ProjectManager {
    method createProjectStructure(project_name: Text): Bool {
        # Create main project directory
        project_root: Text = fs.resolve(project_name)

        if (fs.exists(project_root)) {
            io.error("Project already exists: %s", project_root)
            return false
        }

        # Create directory structure
        directories: List<Text> = [
            project_name,
            fs.join(project_name, "src"),
            fs.join(project_name, "config"),
            fs.join(project_name, "docs"),
            fs.join(project_name, "tests"),
            fs.join(project_name, "build")
        ]

        dir_iterator: ListIterator = directories.iterator()
        while (dir_iterator.hasNext()) {
            dir: Text = dir_iterator.next()
            success: Bool = fs.createDirectory(dir)
            if (!success) {
                io.error("Failed to create directory: %s", dir)
                return false
            }
        }

        # Create initial files
        main_file: Text = fs.join(project_name, "src", "main.o2l")
        main_content: Text = "import system.io\n\nObject Main {\n    method main(): Text {\n        io.print(\"Hello from " + project_name + "!\")\n        return \"Success\"\n    }\n}"

        readme_file: Text = fs.join(project_name, "README.md")
        readme_content: Text = "# " + project_name + "\n\nA new O²L project.\n\n## Getting Started\n\nCompile and run with:\n```\no2l src/main.o2l\n```"

        config_file: Text = fs.join(project_name, "config", "project.json")
        config_content: Text = "{\n  \"name\": \"" + project_name + "\",\n  \"version\": \"1.0.0\",\n  \"main\": \"src/main.o2l\"\n}"

        # Write initial files
        main_success: Bool = fs.writeText(main_file, main_content)
        readme_success: Bool = fs.writeText(readme_file, readme_content)
        config_success: Bool = fs.writeText(config_file, config_content)

        if (main_success && readme_success && config_success) {
            io.print("Project '%s' created successfully at: %s", project_name, project_root)
            return true
        } else {
            io.error("Failed to create some project files")
            return false
        }
    }

    method analyzeProject(project_path: Text): Text {
        project_root: Text = fs.resolve(project_path)

        if (!fs.exists(project_root) || !fs.isDirectory(project_root)) {
            return "Project not found or not a directory: " + project_root
        }

        # Analyze project structure
        analysis: Text = "Project Analysis for: " + fs.basename(project_root) + "\n"
        analysis = analysis + "Location: " + project_root + "\n\n"

        # Check for standard directories
        standard_dirs: List<Text> = ["src", "config", "docs", "tests", "build"]
        dir_iterator: ListIterator = standard_dirs.iterator()

        while (dir_iterator.hasNext()) {
            dir_name: Text = dir_iterator.next()
            dir_path: Text = fs.join(project_root, dir_name)
            exists: Bool = fs.exists(dir_path) && fs.isDirectory(dir_path)

            analysis = analysis + "Directory '" + dir_name + "': " + (exists ? "EXISTS" : "MISSING") + "\n"

            if (exists) {
                files: List<Text> = fs.listFiles(dir_path)
                analysis = analysis + "  Files: " + files.size().toString() + "\n"
            }
        }

        # Count source files
        src_dir: Text = fs.join(project_root, "src")
        if (fs.exists(src_dir)) {
            src_files: List<Text> = fs.listFiles(src_dir)
            o2l_count: Int = 0

            src_iterator: ListIterator = src_files.iterator()
            while (src_iterator.hasNext()) {
                filename: Text = src_iterator.next()
                if (fs.extname(filename) == ".o2l") {
                    o2l_count = o2l_count + 1
                }
            }

            analysis = analysis + "\nSource files (.o2l): " + o2l_count.toString()
        }

        return analysis
    }
}
````

---

## Error Handling

Filesystem operations can fail for various reasons. Always handle potential errors:

```o2l
# Safe file operations with error handling
method safeFileOperation(filename: Text): Bool {
    try {
        # Check if file exists first
        if (!fs.exists(filename)) {
            io.error("File does not exist: %s", filename)
            return false
        }

        # Check if it's actually a file
        if (!fs.isFile(filename)) {
            io.error("Path is not a file: %s", filename)
            return false
        }

        # Read file content
        content: Text = fs.readText(filename)

        # Process content (example)
        processed: Text = content + "\n\nProcessed by O²L"

        # Write back to file
        backup_name: Text = fs.changeExtension(filename, ".backup")
        backup_success: Bool = fs.writeText(backup_name, content)

        if (!backup_success) {
            io.error("Failed to create backup")
            return false
        }

        write_success: Bool = fs.writeText(filename, processed)
        if (!write_success) {
            io.error("Failed to write processed content")
            return false
        }

        io.print("File processed successfully: %s", filename)
        return true

    } catch (error: Exception) {
        io.error("Filesystem operation failed: %s", error.message)
        return false
    }
}
```

---

## Performance Considerations

1. **File Size**: Be aware of memory usage when reading large files with `readText()`
2. **Path Resolution**: Cache resolved paths if used multiple times
3. **Directory Listings**: Filter results at the application level for large directories
4. **Existence Checks**: Use `exists()` before expensive operations
5. **Cross-Platform**: Path operations work consistently across different operating systems

---

## Cross-Platform Notes

- **Path Separators**: All path operations handle both `/` and `\` appropriately
- **Absolute Paths**: Unix-style (`/usr/local`) and Windows-style (`C:\Users`) are supported
- **Case Sensitivity**: Depends on underlying filesystem (case-sensitive on Linux, case-insensitive on Windows/macOS)
- **Path Normalization**: Use `normalize()` for consistent path handling

---

## See Also

- [system.io API Reference](io.md) - Console input/output operations
- [Text API Reference](../core/Text.md) - String manipulation for file content
- [List API Reference](../core/List.md) - Working with file listings
- [Error Handling Guide](../../tutorials/intermediate/03-error-handling.md) - Exception handling patterns
