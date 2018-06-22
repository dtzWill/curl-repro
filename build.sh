#!/bin/sh

nix build -f . use_curl_release -o release-tests
nix build -f . use_curl_fix1 -o fix1-tests
nix build -f . use_curl_git -o git-tests
