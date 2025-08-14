# O²L Language Support for Neovim

This plugin provides Language Server Protocol (LSP) integration for the O²L programming language in Neovim.

## Features

- **Syntax highlighting** for `.obq` files
- **Filetype detection** for O²L files
- **LSP integration** using the built-in O²L parser
- **Error detection** and diagnostics
- **Code completion** and IntelliSense
- **Go to definition** and find references
- **Hover information** and signature help
- **Symbol navigation** and workspace symbols

## Installation

### Using lazy.nvim

```lua
{
  "zombocoder/o2l",
  dir = "/path/to/o2l/lsp/nvim", -- Local path to this plugin
  ft = "obq",
  dependencies = {
    "neovim/nvim-lspconfig",
  },
  config = function()
    require('o2l').setup_o2l_lsp()
  end,
}
```

### Using packer.nvim

```lua
use {
  "zombocoder/o2l",
  ft = "obq",
  requires = { "neovim/nvim-lspconfig" },
  config = function()
    require('o2l').setup_o2l_lsp()
  end,
}
```

### Manual Installation

1. Copy this `lsp/nvim` directory to your Neovim configuration:
   ```bash
   cp -r /path/to/o2l/lsp/nvim ~/.config/nvim/pack/plugins/start/o2l-nvim
   ```

2. Ensure you have `nvim-lspconfig` installed

3. The plugin will automatically detect `.obq` files and start the LSP

## Requirements

- Neovim 0.8+ 
- `nvim-lspconfig` plugin
- O²L interpreter built at `./build/o2l` (relative to project root)

## Usage

### Building the O²L LSP Server

First, ensure the O²L LSP server is built:

```bash
cd /path/to/o2l/tools/o2l-lsp-server
mkdir build && cd build
cmake ..
make
```

### LSP Integration

The plugin uses a dedicated LSP server that communicates via the Language Server Protocol:

```bash
./tools/o2l-lsp-server/build/o2l-lsp-server
```

The LSP server internally uses the O²L parser for syntax analysis.

### File Types

The plugin automatically detects files with the `.obq` extension as O²L files.

### LSP Commands

Once a `.obq` file is opened, the following LSP features are available:

- `:LspInfo` - Show LSP status
- `:LspStart o2l_lsp` - Manually start O²L LSP
- `:LspStop o2l_lsp` - Stop O²L LSP
- `gd` - Go to definition
- `gr` - Find references
- `K` - Hover information
- `<C-k>` - Signature help

## Configuration

### Custom LSP Settings

You can customize the LSP configuration:

```lua
require('lspconfig').o2l_lsp.setup({
  cmd = { './build/o2l', 'parse', '--json-output' },
  settings = {
    o2l = {
      diagnostics = { enable = true },
      completion = { enable = true },
      -- Add more settings as needed
    },
  },
})
```

### Custom Binary Path

If your O²L binary is in a different location:

```lua
require('lspconfig').o2l_lsp.setup({
  cmd = { '/custom/path/to/o2l', 'parse', '--json-output' },
})
```

## File Structure

```
lsp/nvim/
├── README.md                           # This file
├── ftdetect/
│   └── obq.lua                        # Filetype detection
├── syntax/
│   └── obq.vim                        # Syntax highlighting
├── plugin/
│   └── o2l.lua                        # Plugin initialization
└── lua/
    ├── mason-o2l-lsp/
    │   └── init.lua                   # Mason integration
    └── lspconfig/server_configurations/
        └── o2l_lsp.lua               # LSP configuration
```

## Troubleshooting

### LSP Not Starting

1. Check that the O²L binary exists and is executable:
   ```bash
   ls -la ./build/o2l
   ./build/o2l --help
   ```

2. Verify LSP status:
   ```vim
   :LspInfo
   ```

3. Check logs:
   ```vim
   :LspLog
   ```

### Syntax Highlighting Issues

1. Force reload the filetype:
   ```vim
   :set filetype=obq
   ```

2. Check if the syntax file is loaded:
   ```vim
   :echo b:current_syntax
   ```

## Contributing

Contributions are welcome! Please see the main O²L repository for contribution guidelines.

## License

This plugin is part of the O²L programming language project and follows the same license terms.