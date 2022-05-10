SHELLCODE_FLAGS:=-O3 -s -std=c++20 -fPIE -ffreestanding -nostdlib -Wall -Werror -Wextra
SHELLCODE_TEST_FLAGS:=-O3 -g -std=c++20 -fPIE -Wall -Werror -Wextra

ifeq ($(OS),Windows_NT)
	SHELLCODE_TEST_FLAGS:=-static-libgcc -static-libstdc++ $(SHELLCODE_TEST_FLAGS)
else
ifneq (,$(findstring mingw, $(CXX)))
	SHELLCODE_TEST_FLAGS:=-static-libgcc -static-libstdc++ $(SHELLCODE_TEST_FLAGS)
endif
endif


SHELLCODES_CPP:=$(wildcard examples/*.cpp)
SHELLCODES_O:=$(patsubst %.cpp,%.o,$(SHELLCODES_CPP))
SHELLCODES_BIN:=$(patsubst %.cpp,%.bin,$(SHELLCODES_CPP))

.PRECIOUS: examples/%.o

all: $(SHELLCODES_BIN) tests/shellcode_test.out

## Shellcodes, force g++, because PE is annoying ###
examples/%.bin: examples/%.o
	g++ $(CPPFLAGS) $(CXXFLAGS) $(SHELLCODE_FLAGS) $(LDFLAGS) \
		-T ./shellcode_linker.ld -o $@ $<

examples/%.o: examples/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) $(SHELLCODE_FLAGS) -c -o $@ $<

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
