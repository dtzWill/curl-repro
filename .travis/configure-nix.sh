#!/bin/sh

# enable sandbox
mkdir -p ~/.config/nix
cat > ~/.config/nix/nix.conf <<EOF
sandbox = true
EOF
