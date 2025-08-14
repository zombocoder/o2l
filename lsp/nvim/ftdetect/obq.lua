-- Filetype detection for O2L language (*.obq files)
-- Compatible with older Neovim versions

-- Use vim.api for better compatibility
vim.api.nvim_create_autocmd({"BufRead", "BufNewFile"}, {
  pattern = "*.obq",
  callback = function()
    vim.bo.filetype = "obq"
  end,
})

-- Also add to filetype if available (Neovim 0.7+)
if vim.filetype then
  vim.filetype.add({
    extension = {
      obq = 'obq',
    },
  })
end