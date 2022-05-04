#if not defined(__x86_64__) and not defined(__i386__)
    #error "Not supported architecture"
#endif

#include <fstream>
#include <iostream>

#include <sys/mman.h>
#include <unistd.h>

namespace error_code
{
    enum : int
    {
        NO_ERRORS        = 0,
        ARGUMENT_MISSING = -1,
        CANT_OPEN_FILE   = -2,
        CANT_ALLOCATE    = -3
    };
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "missing arguments, usage: ./shellcode_test.out "
                     "</path/to/shellcode>"
                  << "\n";
        return error_code::ARGUMENT_MISSING;
    }

    static auto page_size = static_cast<size_t>(getpagesize());

    std::ifstream shellcode_bin(argv[1], std::ios::binary | std::ios::in);

    if (not shellcode_bin.is_open())
    {
        std::cerr << "could not open shellcode.bin"
                  << "\n";
        return error_code::CANT_OPEN_FILE;
    }

    shellcode_bin.seekg(0, std::ios::end);
    auto shellcode_size = shellcode_bin.tellg();
    shellcode_bin.seekg(0, std::ios::beg);

    auto shellcode_start = reinterpret_cast<void (*)()>(
      mmap(reinterpret_cast<void*>(0x10000),
           (static_cast<std::size_t>(shellcode_size) + page_size)
             & ~(page_size - 1),
           PROT_EXEC | PROT_WRITE | PROT_READ,
           MAP_ANONYMOUS | MAP_PRIVATE,
           -1,
           0));

    if (reinterpret_cast<void*>(shellcode_start)
        != reinterpret_cast<void*>(0x10000))
    {
        std::cerr << "could not allocate at 0x10000"
                  << "\n";
        return error_code::CANT_ALLOCATE;
    }

    shellcode_bin.read(reinterpret_cast<char*>(shellcode_start),
                       shellcode_size);

    shellcode_start();

    return error_code::NO_ERRORS;
}
