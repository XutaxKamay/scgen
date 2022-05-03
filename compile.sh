g++ -O3 -std=c++20 -static-pie -static-libgcc -ffreestanding -nostdlib -T ./linker.ld shellcode.cpp
g++ -O3 -g -std=c++20 -o shellcode_test.out shellcode_test.cpp
