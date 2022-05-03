#include <fstream>
#include <iostream>
#include <thread>

#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    static size_t page_size = static_cast<size_t>(getpagesize());

    std::ifstream shellcode_bin("./shellcode.bin",
                                std::ios::binary | std::ios::in);

    if (not shellcode_bin.is_open())
    {
        std::cerr << "could not open shellcode.bin"
                  << "\n";
        return -1;
    }

    shellcode_bin.seekg(0, std::ios::end);
    auto shellcode_size = shellcode_bin.tellg();
    shellcode_bin.seekg(0, std::ios::beg);

    auto allocated = static_cast<char*>(
      mmap(reinterpret_cast<void*>(0x10000),
           (static_cast<std::size_t>(shellcode_size) + page_size)
             & ~(page_size - 1),
           PROT_EXEC | PROT_WRITE | PROT_READ,
           MAP_ANONYMOUS | MAP_PRIVATE,
           -1,
           0));

    if (allocated != reinterpret_cast<char*>(0x10000))
    {
        std::cerr << "could not allocate at 0x10000"
                  << "\n";
        return -1;
    }

    shellcode_bin.read(reinterpret_cast<char*>(allocated),
                       shellcode_size);

    std::jthread(reinterpret_cast<void (*)()>(allocated));

    return 0;
}
