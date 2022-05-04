SHELLCODE_FLAGS:=-O3 -s -std=c++20 -static-pie -static-libgcc -ffreestanding -nostdlib -Wall -Werror -Wextra
SHELLCODE_TEST_FLAGS:=-O3 -g -std=c++20 -Wall -Werror -Wextra

SHELLCODES_CPP:=$(wildcard examples/*.cpp)
SHELLCODES_O:=$(patsubst %.cpp,%.o,$(SHELLCODES_CPP))
SHELLCODES_BIN:=$(patsubst %.cpp,%.bin,$(SHELLCODES_CPP))

.PRECIOUS: examples/%.o

all: $(SHELLCODES_BIN) tests/shellcode_test.out

examples/%.bin: examples/%.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SHELLCODE_FLAGS) $(LDFLAGS) \
		-T ./shellcode_linker.ld -o $@ $<

examples/%.o: examples/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SHELLCODE_FLAGS) -c -o $@ $<

### Shellcode testing ###
tests/shellcode_test.out : tests/shellcode_test.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SHELLCODE_TEST_FLAGS) $(LDFLAGS) \
		-o $@ $<

tests/shellcode_test.o: tests/shellcode_test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SHELLCODE_TEST_FLAGS) -c \
		-o $@ $<

clean:
	${RM} $(SHELLCODES_O) $(SHELLCODES_BIN)
	${RM} tests/shellcode_test.o examples/shellcode_test.out
