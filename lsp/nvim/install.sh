#!/bin/bash

# O²L Neovim Plugin Installation Script
set -e

echo "Installing O²L Language Support for Neovim..."

# Detect Neovim config directory
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    # Windows
    NVIM_CONFIG_DIR="$HOME/AppData/Local/nvim"
else
    # Unix-like (Linux, macOS)
    NVIM_CONFIG_DIR="${XDG_CONFIG_HOME:-$HOME/.config}/nvim"
fi

# Create plugin directory
PLUGIN_DIR="$NVIM_CONFIG_DIR/pack/plugins/start/o2l-nvim"
mkdir -p "$PLUGIN_DIR"

# Copy plugin files
echo "Copying plugin files to $PLUGIN_DIR..."
cp -r ftdetect/ "$PLUGIN_DIR/" 2>/dev/null || true
cp -r syntax/ "$PLUGIN_DIR/" 2>/dev/null || true
cp -r plugin/ "$PLUGIN_DIR/" 2>/dev/null || true
cp -r lua/ "$PLUGIN_DIR/" 2>/dev/null || true
cp README.md "$PLUGIN_DIR/" 2>/dev/null || true

echo "✅ O²L plugin installed successfully!"
echo ""
echo "Next steps:"
echo "1. Ensure you have nvim-lspconfig installed"
echo "2. Build the O²L interpreter: make -C build"
echo "3. Open any .obq file in Neovim"
echo "4. The LSP will start automatically"
echo ""
echo "Plugin installed at: $PLUGIN_DIR"