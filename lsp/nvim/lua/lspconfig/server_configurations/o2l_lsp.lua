local util = require 'lspconfig.util'

local bin_name = './build/o2l'

return {
  default_config = {
    cmd = { '/Users/zombocoder/dev/zombocoder/o2l/tools/o2l-lsp-server/build/o2l-lsp-server' },
    filetypes = { 'obq' },
    root_dir = util.root_pattern('.git', 'o2l.toml', 'package.json'),
    single_file_support = true,
    settings = {
      o2l = {
        -- Enable diagnostics
        diagnostics = {
          enable = true,
        },
        -- Enable completion
        completion = {
          enable = true,
        },
        -- Enable hover information
        hover = {
          enable = true,
        },
        -- Enable signature help
        signatureHelp = {
          enable = true,
        },
        -- Enable go to definition
        definition = {
          enable = true,
        },
        -- Enable references
        references = {
          enable = true,
        },
        -- Enable rename
        rename = {
          enable = true,
        },
        -- Enable formatting
        formatting = {
          enable = true,
        },
        -- Enable code actions
        codeAction = {
          enable = true,
        },
        -- Enable symbol information
        documentSymbol = {
          enable = true,
        },
        workspaceSymbol = {
          enable = true,
        },
      },
    },
  },
  docs = {
    description = [[
https://github.com/zombocoder/o2l

Language Server Protocol implementation for O²L programming language.

O²L is an interpreted, fully object-oriented programming language with immutable objects and no primitives.
File extension: *.obq

The language server provides:
- Syntax highlighting and error detection
- Code completion and IntelliSense
- Go to definition and find references
- Hover information and signature help
- Code formatting and refactoring
- Symbol navigation

To use the O²L LSP server:
1. Build the O²L interpreter: `make -C build`
2. The LSP uses: `./build/o2l parse <file.obq> --json-output`

Or install manually by downloading from the releases page and ensuring the binary is in your PATH.
]],
  },
}