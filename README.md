## Introduction
There's currently not a lot of ways to make easily shellcodes,<br>
you either write in assembly or extract from PE/ELF the things you need,<br>
but sometimes that is just not enough, especially for variables that are not allocated on the stack.<br>
But there's actually a very easy way to make shellcodes,<br>
it is known feature called "freestanding" environment.
The contrary by default is "host" environment, where it targets mostly for your specific machine, OS, etc.<br>
It is generally used for very low-level (Embedded system, BIOS, Firmware, Kernel, OS ...) programming.<br>
It permits to write possibly code that is compatible with all platforms,<br>
as long you have the same architecture.
## Requirements
- GNU/Linux:
    - g++ 
- For Windows you need MinGW with g++
## How to use
Simply modify shellcode.cpp to your needs.
## TODO
- Do more examples by using nolibc etc.
- Makefile
