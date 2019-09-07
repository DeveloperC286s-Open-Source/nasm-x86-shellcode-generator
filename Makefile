# linux-x86-shellcode-generator
# See LICENSE file for copyright and license details.
.POSIX:

CC=gcc

SOURCE=src/shellcode-generator.c
OBJECT=shellcode-generator
CFLAGS=

TEST_SOURCE=tests/shellcode-generator-tests.c
TEST_OBJECT=shellcode-generator-tests
TEST_CFLAGS=-lcunit

PAYLOAD_SOURCE=output.c
PAYLOAD_OBJECT=output
PAYLOAD_CFLAGS=-m32 -fno-stack-protector -z execstack

.PHONY: all
all: $(OBJECT) test

$(OBJECT): $(SOURCE)
	$(CC) -o $(OBJECT) $(SOURCE) $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJECT)
	rm -f $(TEST_OBJECT)
	rm -f $(PAYLOAD_OBJECT)

.PHONY: test
test: $(TEST_OBJECT)	
	./$(TEST_OBJECT)	

$(TEST_OBJECT): $(TEST_SOURCE)
	$(CC) -o $(TEST_OBJECT) $(TEST_SOURCE) $(TEST_CFLAGS)

.PHONY: payload
payload: $(PAYLOAD_OBJECT)	
	./$(PAYLOAD_OBJECT)	

$(PAYLOAD_OBJECT): $(PAYLOAD_SOURCE)
	$(CC) -o $(PAYLOAD_OBJECT) $(PAYLOAD_SOURCE) $(PAYLOAD_CFLAGS)
