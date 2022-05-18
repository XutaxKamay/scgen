#include <cstdint>
#include <tuple>
#include <unistd.h>

#if defined(__linux__) and (defined(__x86_64__) or defined(__i386__))
    #include <sys/syscall.h>

struct Struct
{
    static volatile bool initialized;

    void test_method(unsigned long long blah) volatile
    {
        member = blah;
    }

    volatile unsigned long long member;
};

volatile bool Struct::initialized = false;
volatile static Struct g_struct {};

template <typename T>
concept ValidTypeForRegister = sizeof(T) <= sizeof(std::uintptr_t);

template <ValidTypeForRegister... T, std::size_t N = sizeof...(T)>
inline auto syscall(const auto number, T... args)
{
    std::uintptr_t ret;
    const auto targs = std::forward_as_tuple(args...);

    #if defined(__x86_64__)
    if constexpr (N == 0)
    {
        asm("mov %1, %%rax\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number))
            : "rax");
    }
    else if constexpr (N == 1)
    {
        asm("mov %1, %%rax\n\t"
            "mov %2, %%rdi\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs)))
            : "rax");
    }
    else if constexpr (N == 2)
    {
        asm("mov %1, %%rax\n\t"
            "mov %2, %%rdi\n\t"
            "mov %3, %%rsi\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs)))
            : "rax", "rdi", "rsi");
    }
    else if constexpr (N == 3)
    {
        asm("mov %1, %%rax\n\t"
            "mov %2, %%rdi\n\t"
            "mov %3, %%rsi\n\t"
            "mov %4, %%rdx\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs)))
            : "rax", "rdi", "rsi", "rdx");
    }
    else if constexpr (N == 4)
    {
        asm("mov %1, %%rax\n\t"
            "mov %2, %%rdi\n\t"
            "mov %3, %%rsi\n\t"
            "mov %4, %%rdx\n\t"
            "mov %5, %%rcx\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<3>(targs)))
            : "rax", "rdi", "rsi", "rdx", "rcx");
    }
    else if constexpr (N == 5)
    {
        asm("mov %1, %%rax\n\t"
            "mov %2, %%rdi\n\t"
            "mov %3, %%rsi\n\t"
            "mov %4, %%rdx\n\t"
            "mov %5, %%rcx\n\t"
            "mov %6, %%r8\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<3>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<4>(targs)))
            : "rax", "rdi", "rsi", "rdx", "rcx", "r8");
    }
    else if constexpr (N == 6)
    {
        asm("mov %1, %%rax\n\t"
            "mov %2, %%rdi\n\t"
            "mov %3, %%rsi\n\t"
            "mov %4, %%rdx\n\t"
            "mov %5, %%rcx\n\t"
            "mov %6, %%r8\n\t"
            "mov %7, %%r9\n\t"
            "syscall\n\t"
            "mov %%rax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<3>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<4>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<5>(targs)))
            : "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9");
    }
    #elif defined(__i386__)
    if constexpr (N == 0)
    {
        asm("mov %1, %%eax\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number))
            : "eax");
    }
    else if constexpr (N == 1)
    {
        asm("mov %1, %%eax\n\t"
            "mov %2, %%ebx\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs)))
            : "eax");
    }
    else if constexpr (N == 2)
    {
        asm("mov %1, %%eax\n\t"
            "mov %2, %%ebx\n\t"
            "mov %3, %%ecx\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs)))
            : "eax", "ebx", "ecx");
    }
    else if constexpr (N == 3)
    {
        asm("mov %1, %%eax\n\t"
            "mov %2, %%ebx\n\t"
            "mov %3, %%ecx\n\t"
            "mov %4, %%edx\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs)))
            : "eax", "ebx", "ecx", "edx");
    }
    else if constexpr (N == 4)
    {
        asm("mov %1, %%eax\n\t"
            "mov %2, %%ebx\n\t"
            "mov %3, %%ecx\n\t"
            "mov %4, %%edx\n\t"
            "mov %5, %%esi\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<3>(targs)))
            : "eax", "ebx", "ecx", "edx", "esi");
    }
    else if constexpr (N == 5)
    {
        asm("mov %1, %%eax\n\t"
            "mov %2, %%ebx\n\t"
            "mov %3, %%ecx\n\t"
            "mov %4, %%edx\n\t"
            "mov %5, %%esi\n\t"
            "mov %6, %%edi\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<3>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<4>(targs)))
            : "eax", "ebx", "ecx", "edx", "esi", "edi");
    }
    else if constexpr (N == 6)
    {
        asm("mov %1, %%eax\n\t"
            "mov %2, %%ebx\n\t"
            "mov %3, %%ecx\n\t"
            "mov %4, %%edx\n\t"
            "mov %5, %%esi\n\t"
            "mov %6, %%edi\n\t"
            "mov %7, %%ebp\n\t"
            "int $0x80\n\t"
            "mov %%eax, %0\n\t"
            : "=g"(ret)
            : "g"(static_cast<std::uintptr_t>(number)),
              "g"(static_cast<std::uintptr_t>(std::get<0>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<1>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<2>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<3>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<4>(targs))),
              "g"(static_cast<std::uintptr_t>(std::get<5>(targs)))
            : "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp");
    }
    #endif
    else
    {
        static_assert(N >= 0 and N <= 7,
                      "The argument count for the system call is not "
                      "valid.");
    }

    return ret;
}

void main(void)
{
    Struct::initialized         = true;
    volatile char hello_world[] = "Hello, World\n";

    auto ret = syscall(__NR_write,
                       STDOUT_FILENO,
                       reinterpret_cast<std::uintptr_t>(hello_world),
                       sizeof(hello_world) - 1);

    if (ret > 0)
    {
        g_struct.test_method(0xFFFFFFFFFFFFFFFF);
    }

    ret = syscall(__NR_exit, 0);

    if (ret)
    {
        asm("int3");
    }
}

extern "C" void _start(void)
{
    main();
}
#else
    #error "Not supported architecture"
#endif
