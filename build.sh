#!/bin/bash
gcc -m32 -fno-stack-protector -z execstack output.c -o output.out
./output.out
