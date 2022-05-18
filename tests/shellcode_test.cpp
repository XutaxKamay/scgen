#include <fstream>
#include <iostream>

#if defined(__linux__)
    #include <sys/mman.h>
    #include <unistd.h>
#elif defined(_WIN32)
    #include <windows.h>
#else
    #error "OS not supported"
#endif

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

    static auto page_size = static_cast<size_t>(
      []()
      {
#if defined(__linux__)
          return getpagesize();
#elif defined(_WIN32)
          SYSTEM_INFO si;
          GetSystemInfo(&si);
          return si.dwPageSize;
#endif
      }());

    std::ifstream shellcode_bin(argv[1], std::ios::binary | std::ios::in);

    if (not shellcode_bin.is_open())
    {
        std::cerr << "could not open " << argv[1] << "\n";
        return error_code::CANT_OPEN_FILE;
    }

    shellcode_bin.seekg(0, std::ios::end);
    auto shellcode_size = shellcode_bin.tellg();
    shellcode_bin.seekg(0, std::ios::beg);

    auto shellcode_start = reinterpret_cast<void (*)()>(
      [&shellcode_size]()
      {
#if defined(__linux__)
          return mmap(nullptr,
                      (static_cast<std::size_t>(shellcode_size)
                       + page_size)
                        & ~(page_size - 1),
                      PROT_EXEC | PROT_WRITE | PROT_READ,
                      MAP_ANONYMOUS | MAP_PRIVATE,
                      -1,
                      0);
#elif defined(_WIN32)
          return VirtualAlloc(nullptr,
                              (static_cast<std::size_t>(shellcode_size)
                               + page_size)
                                & ~(page_size - 1),
                              MEM_COMMIT | MEM_RESERVE,
                              PAGE_EXECUTE_READWRITE);
#endif
      }());

    if (reinterpret_cast<void*>(shellcode_start) == nullptr)
    {
        std::cerr << "could not allocate memory for shellcode"
                  << "\n";
        return error_code::CANT_ALLOCATE;
    }

    std::cout << "allocated memory for shellcode at: "
              << reinterpret_cast<void*>(shellcode_start) << "\n";

    shellcode_bin.read(reinterpret_cast<char*>(shellcode_start),
                       shellcode_size);

    shellcode_start();

    return error_code::NO_ERRORS;
}
