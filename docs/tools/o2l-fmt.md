# o2l-fmt - Code Formatter

The `o2l-fmt` tool is O²L's official code formatter that automatically formats O²L source code according to standard conventions.

## Usage

### Format to stdout
```bash
o2l-fmt file.obq
```

### Format in-place
```bash
o2l-fmt -w file.obq
```

### Show differences
```bash
o2l-fmt -d file.obq
```

### List files needing formatting
```bash
o2l-fmt -l .
```

### Process directories recursively
```bash
o2l-fmt -r src/
```

### Read from stdin
```bash
cat file.obq | o2l-fmt -s
```

## Command Line Options

| Option | Description |
|--------|-------------|
| `-w, --write` | Write result to source file instead of stdout |
| `-d, --diff` | Display diffs instead of rewriting files |
| `-l, --list` | List files whose formatting differs from standard |
| `-r, --recursive` | Process directories recursively |
| `-s, --stdin` | Read from stdin |
| `-h, --help` | Show help message |

## Formatting Rules

The formatter applies these standardized formatting rules:

### Indentation
- **4-space indentation** for all code blocks
- Consistent nesting levels

### Spacing
- **Spaces around operators** (`=`, `+`, `-`, `*`, `/`, `==`, etc.)
- **Space after keywords** (`Object`, `method`, `if`, etc.)
- **Proper type annotation spacing** (`name: Type`)

### Newlines
- **3 newlines after import blocks**
- **1 newline between top-level declarations** (Object, Enum, Record, Protocol)
- **1 newline between Object methods**
- **1 newline after Object property blocks**

### Brace Placement
- **Consistent brace placement** with proper line breaks
- **Balanced braces** (validates and throws errors for unmatched braces)

## Examples

### Before Formatting
```obq
import system.io
Object Greeter{property message:Text constructor(greeting:Text){this.message=greeting}@external method greet(name:Text):Text{io.print("%s, %s!",this.message,name)return this.message}}
```

### After Formatting
```obq
import system.io


Object Greeter {
    property message: Text

    constructor(greeting: Text) {
        this.message = greeting
    }

    @external method greet(name: Text): Text {
        io.print("%s, %s!", this.message, name)
        return this.message
    }
}
```

## Integration

### Editor Integration
Most editors can be configured to run `o2l-fmt` on save:

**VS Code** (with shell command):
```json
{
    "files.associations": {
        "*.obq": "o2l"
    },
    "[o2l]": {
        "editor.formatOnSave": true,
        "editor.defaultFormatter": "o2l-fmt"
    }
}
```

**Vim/Neovim**:
```vim
autocmd BufWritePre *.obq !o2l-fmt -w %
```

### Pre-commit Hooks
Add to `.git/hooks/pre-commit`:
```bash
#!/bin/sh
# Format O²L files before commit
o2l-fmt -l . | while read file; do
    echo "Formatting $file"
    o2l-fmt -w "$file"
    git add "$file"
done
```

### CI/CD Integration
```yaml
# GitHub Actions example
- name: Check O²L formatting
  run: |
    o2l-fmt -l . > unformatted_files.txt
    if [ -s unformatted_files.txt ]; then
      echo "Files need formatting:"
      cat unformatted_files.txt
      exit 1
    fi
```

## Building from Source

```bash
cd tools/o2l-fmt
mkdir build && cd build
cmake ..
make
```

The formatter requires:
- C++20 or later
- CMake 3.10+
- Google Test (for running tests)

## Testing

Run the test suite:
```bash
cd build
./o2l-fmt-test          # Basic tests
./o2l-fmt-advanced-test # Advanced tests
```

## Error Handling

The formatter validates:
- **Balanced braces** - throws exception for unmatched `{` or `}`
- **Valid O²L syntax** - preserves semantic meaning
- **String literal integrity** - never modifies content inside quotes

## Best Practices

- **Format before committing** to maintain consistent code style
- **Use in CI/CD** to catch formatting issues early
- **Configure editor integration** for automatic formatting
- **Run on entire codebase** periodically to maintain consistency

## File Support

- Processes files with `.obq` extension
- Skips non-O²L files automatically
- Handles complex nested structures and annotations
- Preserves comments and string literals exactly