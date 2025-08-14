#!/bin/bash

# Test script for the O2L LSP server
echo "Testing O2L LSP server..."

# Create a test JSON message
cat << 'EOF' | ./build/o2l-lsp-server
Content-Length: 246

{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"capabilities":{"textDocument":{"synchronization":{"didOpen":true,"didChange":true}}},"processId":null,"rootUri":"file:///tmp","workspaceFolders":null}}
EOF