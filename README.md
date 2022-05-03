## Introduction
There's currently not a lot of ways to make easily shellcodes,<br>
you either write in assembly or extract from PE/ELF the things you need,<br>
but sometimes that is just not enough, especially for variables that are not allocated on the stack.<br>
But there's actually a very easy way to make shellcodes,<br>
it is known feature called "freestanding" environment.
The contrary by default is "host" environment, where it targets mostly for your specific machine, OS, etc.<br>
It is generally used for very low-level (Embedded system, BIOS, Firmware, Kernel, OS ...) programming.<br>
It permits to write possibly code that is compatible with all platforms,<br>
as long you have the same architecture.<br><br>
TL;DR, you can now inject your stuffs without having to parse or manual map the binary output format (ELF/PE).
## Requirements
- g++
- make
## How to use
Check how to write your own shellcode by looking at:
- `examples/shellcode_hello_world.cpp`
- `shellcode_linker.ld`
- `tests/shellcode_test.cpp`

To run the current shellcode test:
- Type `make -j$(nproc)`
- Pick your example in `examples` folder by copying it:
	- `cp examples/shellcode_hello_world.bin tests/shellcode.bin`
- Go to `tests` folder
- Run, `./shellcode_test.out`
## x86-64 (shellcode) binary output example
Running `rz-asm -a x86 -b 64 -d "$(hexdump -e '16/1 "%02X"' ./examples/shellcode_hello_world.bin)"`,<br>
should give:
```asm
movabs rax, 0x57202c6f6c6c6548          ; .text section starts here, writing the start of Hello, World
push r12                                ;
xor r11d, r11d                          ;
mov r12d, 0xd                           ; Hello, World length
push rbp                                ;
push rbx                                ;
mov ebx, 1                              ;
mov qword [rsp - 0xe], rax              ;
mov eax, 0xa                            ;
lea rbp, [rsp - 0xe]                    ;
mov dword [rsp - 6], 0x646c726f         ;
mov word [rsp - 2], ax                  ;
mov rax, rbx                            ;
mov rdi, rbx                            ;
mov rsi, rbp                            ;
mov rdx, r12                            ;
mov rcx, r11                            ;
mov r8, r11                             ;
mov r9, r11                             ;
mov r10, r11                            ;
syscall                                 ; write linux syscall on STDOUT
mov r11, rax                            ;
test r11, r11                           ;
je 0x7c                                 ;
mov rax, qword [0x10000]                ;
mov qword [rip + 0x27], rax             ;
mov qword [0x10000], 0xffffffffffffffff ;
mov byte [rip + 0xc], 1                 ;
pop rbx                                 ;
pop rbp                                 ;
pop r12                                 ;
ret                                     ; .text section ends here
nop dword [rax]                         ; .data starts here with Struct::initialized
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; Struct::member
add byte [rax], al                      ; .data ends here
```
The last opcodes that has failed to disassemble represents just .data/.bss section.
## TODO
- Do more examples by using nolibc etc.
- (better) Makefile
- Force entrypoint to be at the start of the shellcode.
