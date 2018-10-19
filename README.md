# Linux-86-Shellcode-Generator

This program provides you with the utility to generate Linux x86 shellcode corresponding to your provided bash command and arguments. The assembly produced pushes the provided bash command and arguments onto the stack and uses them as arguments in an execve system call. The assembly generated uses various tricks to not include any null bytes so that you can use the assembly in a buffer overflow attack. Additionally, there are tricks used to allow odd length commands/parameters without using null bytes and to reduce the shellcode length.

Embedded into a char * is the hexadecimal representation of the generated assembly in an outputted proof of concept C file, output.c, along with the NASM assembly equivalent as comments. Through the execution of this proof of concept C file, it proves the shellcode's functionality.

## Benefits
<ul>
  <li>The payload can include commands which can't be called natively through a system call in assembly.</li>
  <li>Faster and less error-prone than handcrafting shellcode.</li>
  <li>Generates shellcode for lengthy/complex bash which would be difficult to do by hand.</li>
  <li>Through the use of <b>./Shellcode-Generator.out /bin/bash -c "<- commands ->"</b> you can execute multiple commands at once. Instead of singular command execution payload shellcode.</li>
</ul>

## Limations
<ul>
  <li>The smallest possible shellcode may not be generated. Especially for specific bash commands as it embeds it inside an execve call, instead of calling natively through a system call.</li>
</ul>

## Installation

<ul>
  <li>Ubuntu: <b>apt-get install gcc</b> - Needed to be able to compile C programs.</li>
  <li>Arch: <b>pacman -Sy gcc</b></li>
  <li>Ubuntu: <b>apt-get install libc6-dev-i386</b> - Needed to be able to compile in 32bit architecture in gcc.</li>
  <li>Arch: <b>pacman -Sy lib32-gcc-libs lib32-glibc</b></li>
  <li><b>gcc Shellcode-Generator.c -o Shellcode-Generator.out</b> - Compiles the generator 'Shellcode-Generator.c' and outputs the binary to Shellcode-Generator.out.</li>
</ul>

## Usage
<ol>
  <li><b>./Shellcode-Generator.out <- desired command -> <- arguments -></b> Invokes the generator which takes the provide bash commmand and arguments and generates the corresponding shellcode, outputing the result to output.c.</li>
  <li><b>./build.sh</b> Compiles output.c with the relevant flags for stack execution of the char * and then executes the outputed binary to prove the functionality of the shellcode.</li>  
</ol>

## Example Usage and Tips

Because of the techiques used to push strings whose length is not a multiple of four strings with no null bytes, the shellcode to push a non multiple can be larger than the shellcode to push a larger strings which is a multiple of four. Strings can be padded so as to not affect the behaviour but make them a multiple of four.

Below is an example to generate shellcode to call /usr/bin/whoami. Running './build.sh' to build the output.c and execute it we can see the length of the shellcode is 39 bytes. As '/usr/bin/whoami' is 15 characters in length some techiques have to be used to push the non multiple.
<table>
  <tr>
    <th>./Shellcode-Generator.out /usr/bin/whoami;</th>
    <th>Generated output.c - pastebin.com/VjPrTH5B</th>
  </tr>
</table>

However full paths can be padded with additional '/'s at the start without affecting the path. In the example below one addition '/' is added to get the length to 16. As we can now see even though the string length has increased the shellcode bytesize has recuded by 3 bytes, around an 8% reduction. 

<table>
  <tr>
    <th>./Shellcode-Generator.out //usr/bin/whoami;</th>
    <th>Generated output.c - pastebin.com/6s9XAM8E</th>
  </tr>
</table>



<table>
  <tr>
    <th>./Shellcode-Generator.out /bin/bash -c "echo test > test.txt;ls;cat test.txt"</th>
    <th>Generated output.c - https://i.imgur.com/um2wvvD.png</th>
  </tr>
</table>

## Recommended Resources
<ul>
  <li>http://insecure.org/stf/smashstack.html</li>
</ul>
