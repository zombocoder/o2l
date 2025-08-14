-- Mason package definition for O²L LSP
local M = {}

local package = require("mason-core.package")
local platform = require("mason-core.platform")
local github = require("mason-core.managers.github")

M.o2l_lsp = package.new({
    name = "o2l-lsp",
    desc = "Language Server Protocol implementation for O²L programming language (*.obq files)",
    homepage = "https://github.com/zombocoder/o2l",
    categories = { package.Cat.LSP },
    languages = { "obq" },
    ---@async
    ---@param ctx InstallContext
    install = function(ctx)
        local repo = "zombocoder/o2l"
        local asset_file = platform.is.win and "o2l-windows.exe" or "o2l"
        
        github.download_release_file({
            repo = repo,
            asset_file = asset_file,
            out_file = platform.is.win and "o2l.exe" or "o2l",
        }).with_receipt()
        
        -- The LSP functionality is built into the o2l binary
        -- Usage: o2l parse <file.obq> --json-output
        ctx:link_bin(
            "o2l",
            platform.is.win and ctx:which("o2l.exe") or ctx:which("o2l")
        )
    end,
})

return M