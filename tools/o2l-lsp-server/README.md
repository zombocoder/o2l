# O²L LSP Server

A Language Server Protocol (LSP) implementation for the O²L programming language, written in C++.

## Overview

This LSP server provides language support for O²L files (`.obq`) by:
- Parsing O²L source files using the O²L interpreter
- Providing real-time diagnostics and error reporting
- Communicating with editors via the LSP protocol

## Features

- **Syntax Error Detection**: Real-time parsing and error reporting
- **LSP Compliance**: Full JSON-RPC communication over stdin/stdout
- **Cross-Platform**: Works on macOS, Linux, and Windows
- **Lightweight**: Minimal dependencies, fast startup

## Building

### Prerequisites
- CMake 3.16 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- O²L interpreter binary (built from parent project)

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# The binary will be created as 'o2l-lsp-server'
```

### Integration with Parent Project

To build as part of the main O²L project, add to the parent CMakeLists.txt:

```cmake
# Add LSP server subdirectory
add_subdirectory(tools/lsp-server)
```

## Usage

### Command Line
The LSP server communicates via stdin/stdout using the LSP protocol:

```bash
./o2l-lsp-server
```

### Editor Integration

#### Neovim with lspconfig

```lua
require('lspconfig').o2l_lsp.setup({
  cmd = { '/path/to/o2l-lsp-server' },
  filetypes = { 'obq' },
  root_dir = require('lspconfig.util').root_pattern('.git', 'o2l.toml'),
  single_file_support = true,
})
```

#### VS Code

Create a VS Code extension with this server configuration:

```json
{
  "contributes": {
    "languages": [{
      "id": "o2l",
      "extensions": [".obq"]
    }],
    "configuration": {
      "title": "O²L",
      "properties": {
        "o2l.lsp.path": {
          "type": "string",
          "default": "o2l-lsp-server",
          "description": "Path to O²L LSP server"
        }
      }
    }
  }
}
```

## Architecture

### Components

1. **LSP Protocol Handler**: Manages JSON-RPC communication
2. **O²L Parser Interface**: Delegates parsing to the o2l binary
3. **Diagnostic Engine**: Converts parse errors to LSP diagnostics
4. **Message Router**: Routes LSP requests to appropriate handlers

### Communication Flow

```
Editor <-> LSP Server <-> O²L Binary
   |           |              |
   |           |              v
   |           |         Parse & Analyze
   |           |              |
   |           v              |
   |    Process Results <------+
   |           |
   v           v
Diagnostics & Responses
```

### Supported LSP Methods

#### Requests
- `initialize` - Initialize LSP connection
- `shutdown` - Graceful shutdown
- `exit` - Terminate server

#### Notifications
- `initialized` - Client ready notification
- `textDocument/didOpen` - File opened
- `textDocument/didChange` - File content changed
- `textDocument/publishDiagnostics` - Send diagnostics to client

#### Capabilities
- `textDocumentSync`: Full document synchronization
- `diagnosticProvider`: Real-time error reporting
- `documentSymbolProvider`: Symbol information (future)

## Configuration

### Binary Discovery
The LSP server automatically searches for the o2l binary in:
1. `../../build/o2l` (relative to LSP server)
2. `../build/o2l`
3. `./build/o2l`
4. `o2l` (in system PATH)

### Custom Binary Path
Set the `O2L_BINARY` environment variable:

```bash
export O2L_BINARY=/custom/path/to/o2l
./o2l-lsp-server
```

## Debugging

### Enable Logging
The LSP server logs to stderr for debugging:

```bash
./o2l-lsp-server 2>lsp.log
```

### Test with O²L Files
Create a test `.obq` file:

```o2l
Object Main {
    method main(): Text {
        "Hello, World!"
    }
}
```

## Contributing

1. Follow the existing code style
2. Add tests for new features
3. Update documentation
4. Ensure cross-platform compatibility

## License

This LSP server is part of the O²L programming language project and follows the same license terms.