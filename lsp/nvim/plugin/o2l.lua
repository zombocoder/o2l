-- O²L language support for Neovim
-- Simple plugin initialization without complex dependencies

-- Set up filetype detection first
vim.filetype.add({
  extension = {
    obq = 'obq',
  },
})

-- Simple LSP setup function
local function setup_o2l_lsp()
  -- Only proceed if lspconfig is available
  local ok, lspconfig = pcall(require, 'lspconfig')
  if not ok then
    return
  end

  -- Simple LSP configuration
  local configs = require('lspconfig.configs')
  
  -- Only configure if not already done
  if not configs.o2l_lsp then
    configs.o2l_lsp = {
      default_config = {
        cmd = { '/Users/zombocoder/dev/zombocoder/o2l/tools/o2l-lsp-server/build/o2l-lsp-server' },
        filetypes = { 'obq' },
        root_dir = lspconfig.util.root_pattern('.git', 'o2l.toml', 'CMakeLists.txt'),
        single_file_support = true,
        name = 'o2l_lsp',
      },
    }
  end

  -- Setup the LSP
  lspconfig.o2l_lsp.setup({})
end

-- Auto-setup when obq file is opened
vim.api.nvim_create_autocmd('FileType', {
  pattern = 'obq',
  callback = function()
    setup_o2l_lsp()
  end,
  group = vim.api.nvim_create_augroup('O2LLSP', { clear = true }),
})