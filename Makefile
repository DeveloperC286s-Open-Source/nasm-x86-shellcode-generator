# Linux-x86-Shellcode-Generator
# See LICENSE file for copyright and license details.
.POSIX:

CC=gcc

compile:
	$(CC) Shellcode-Generator.c -o Shellcode-Generator

payload:
	$(CC) -m32 -fno-stack-protector -z execstack output.c -o output
	./output

.PHONY: compile payload 
