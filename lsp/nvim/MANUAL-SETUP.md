# Manual Setup for O²L Neovim Support

If you're experiencing plugin manager conflicts, you can manually set up O²L support in your Neovim configuration.

## Method 1: Direct File Copy (Recommended)

Run the minimal installation script:
```bash
cd /path/to/o2l/lsp/nvim
./minimal-install.sh
```

## Method 2: Manual Configuration

### Step 1: Add to your `init.lua`

Add this to your `~/.config/nvim/init.lua`:

```lua
-- O²L language support
vim.filetype.add({
  extension = { obq = 'obq' }
})

-- LSP setup for O²L (only if lspconfig is available)
vim.api.nvim_create_autocmd('FileType', {
  pattern = 'obq',
  callback = function()
    local ok, lspconfig = pcall(require, 'lspconfig')
    if not ok then return end
    
    local configs = require('lspconfig.configs')
    if not configs.o2l_lsp then
      configs.o2l_lsp = {
        default_config = {
          cmd = { './build/o2l', 'parse', '--json-output' },
          filetypes = { 'obq' },
          root_dir = lspconfig.util.root_pattern('.git', 'o2l.toml', 'CMakeLists.txt'),
          single_file_support = true,
          name = 'o2l_lsp',
        },
      }
    end
    
    lspconfig.o2l_lsp.setup({})
  end,
})
```

### Step 2: Add Syntax Highlighting

Create `~/.config/nvim/syntax/obq.vim`:

```vim
" Vim syntax file for O²L
if exists("b:current_syntax")
  finish
endif

syn keyword obqKeyword Object method property constructor import this return
syn keyword obqKeyword if else while for enum record protocol const
syn keyword obqKeyword true false null
syn keyword obqType Text Int Float Bool Char Double Long List Map Set

syn match obqOperator "="
syn match obqOperator "+"
syn match obqOperator "\*"
syn match obqDelimiter "("
syn match obqDelimiter ")"
syn match obqDelimiter "{"
syn match obqDelimiter "}"
syn match obqDelimiter "\["
syn match obqDelimiter "\]"

syn match obqNumber "\<\d\+\>"
syn match obqNumber "\<\d\+\.\d*\>"
syn region obqString start='"' end='"'
syn match obqComment "//.*$"
syn region obqComment start="/\*" end="\*/"

hi def link obqKeyword Keyword
hi def link obqType Type
hi def link obqOperator Operator
hi def link obqDelimiter Delimiter
hi def link obqNumber Number
hi def link obqString String
hi def link obqComment Comment

let b:current_syntax = "obq"
```

### Step 3: Add Filetype Detection

Create `~/.config/nvim/ftdetect/obq.lua`:

```lua
vim.filetype.add({
  extension = { obq = 'obq' }
})
```

## Method 3: Plugin Manager Configuration

### For lazy.nvim users

If you want to use lazy.nvim, create a local plugin:

```lua
-- In your lazy.nvim config
{
  name = "o2l-local",
  dir = "/path/to/o2l/lsp/nvim",
  ft = "obq",
  dependencies = { "neovim/nvim-lspconfig" },
  config = function()
    -- Simple setup without complex dependencies
    vim.filetype.add({ extension = { obq = 'obq' } })
  end,
}
```

### For packer.nvim users

```lua
use {
  "/path/to/o2l/lsp/nvim",
  ft = "obq",
  config = function()
    vim.filetype.add({ extension = { obq = 'obq' } })
  end,
}
```

## Troubleshooting

### "Too many rounds of missing plugins" Error

This usually means there's a circular dependency. Use Method 1 or Method 2 above to avoid plugin manager issues.

### LSP Not Starting

1. Check if the O²L binary exists:
   ```bash
   ls -la ./build/o2l
   ```

2. Test the command manually:
   ```bash
   ./build/o2l parse test.obq --json-output
   ```

3. Check LSP status in Neovim:
   ```vim
   :LspInfo
   ```

### Syntax Highlighting Not Working

1. Check filetype detection:
   ```vim
   :echo &filetype
   ```

2. Manually set filetype:
   ```vim
   :set filetype=obq
   ```

## Testing

Create a test file `test.obq`:

```o2l
Object Main {
    method main(): Text {
        message: Text = "Hello, O²L!"
        return message
    }
}
```

Open it in Neovim and verify:
- Syntax highlighting works
- `:echo &filetype` shows `obq`
- `:LspInfo` shows o2l_lsp if configured

## Next Steps

Once basic setup is working:
1. Configure LSP keybindings
2. Add completion settings
3. Customize syntax highlighting colors