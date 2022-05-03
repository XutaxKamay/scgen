SHELLCODE_FLAGS:=-O3 -s -std=c++20 -static-pie -static-libgcc -ffreestanding -nostdlib
SHELLCODE_TEST_FLAGS:=-O3 -g -std=c++20


all: examples/shellcode_hello_world.bin tests/shellcode_test.out

examples/shellcode_hello_world.bin : examples/shellcode_hello_world.o
	$(CXX) $(SHELLCODE_FLAGS) -T ./shellcode_linker.ld -o examples/shellcode_hello_world.bin examples/shellcode_hello_world.o

tests/shellcode_test.out : tests/shellcode_test.o
	$(CXX) $(SHELLCODE_TEST_FLAGS) -o tests/shellcode_test.out tests/shellcode_test.o


examples/shellcode_hello_world.o:
	$(CXX) $(SHELLCODE_FLAGS) -c examples/shellcode_hello_world.cpp -o examples/shellcode_hello_world.o

tests/shellcode_test.o:
	$(CXX) $(SHELLCODE_TEST_FLAGS) -c tests/shellcode_test.cpp -o tests/shellcode_test.o


clean:
	${RM} examples/shellcode_hello_world.o examples/shellcode_hello_world.out
	${RM} tests/shellcode_test.o examples/shellcode_test.out
