# Linux-x86-Shellcode-Generator
# See LICENSE file for copyright and license details.
.POSIX:

CC=gcc
CFLAGS=
SOURCES=Shellcode-Generator.c

all: Shellcode-Generator

Shellcode-Generator: $(SOURCES)
	$(CC) -o Shellcode-Generator $(SOURCES) $(CFLAGS)

payload:
	$(CC) -m32 -fno-stack-protector -z execstack output.c -o output
	./output

.PHONY: payload 
