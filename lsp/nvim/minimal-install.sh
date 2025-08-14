#!/bin/bash

# Minimal O²L Neovim Plugin Installation
set -e

echo "Installing minimal O²L support for Neovim..."

NVIM_CONFIG_DIR="${XDG_CONFIG_HOME:-$HOME/.config}/nvim"

# Create directories
mkdir -p "$NVIM_CONFIG_DIR/ftdetect"
mkdir -p "$NVIM_CONFIG_DIR/syntax" 
mkdir -p "$NVIM_CONFIG_DIR/after/plugin"

# Copy essential files only
cp ftdetect/obq.lua "$NVIM_CONFIG_DIR/ftdetect/"
cp syntax/obq.vim "$NVIM_CONFIG_DIR/syntax/"
cp plugin/o2l.lua "$NVIM_CONFIG_DIR/after/plugin/"

echo "✅ Minimal O²L support installed!"
echo ""
echo "Files installed:"
echo "- $NVIM_CONFIG_DIR/ftdetect/obq.lua"
echo "- $NVIM_CONFIG_DIR/syntax/obq.vim"
echo "- $NVIM_CONFIG_DIR/after/plugin/o2l.lua"
echo ""
echo "Restart Neovim and open any .obq file to test."