#!/usr/bin/env sh

set -o errexit
set -o xtrace

gcc -o "shellcode-generator" "src/shellcode-generator.c"
