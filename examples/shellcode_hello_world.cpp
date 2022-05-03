#ifndef __x86_64__
#error "Not supported architecture"
#endif

struct Struct
{
    static volatile bool initialized;

    void test_method(unsigned long blah) volatile
    {
        member = *reinterpret_cast<unsigned long*>(0x10000);
        *reinterpret_cast<unsigned long*>(0x10000) = blah;
        initialized                                = true;
    }

    volatile unsigned long member;
};

volatile bool Struct::initialized = false;
volatile static Struct g_struct {};

static inline unsigned long syscall(unsigned long rax,
                                    unsigned long rdi = 0,
                                    unsigned long rsi = 0,
                                    unsigned long rdx = 0,
                                    unsigned long rcx = 0,
                                    unsigned long r8  = 0,
                                    unsigned long r9  = 0,
                                    unsigned long r10 = 0)
{
    unsigned long ret;

    asm(".intel_syntax noprefix;\n\t"
        "mov rax, %1\n\t"
        "mov rdi, %2\n\t"
        "mov rsi, %3\n\t"
        "mov rdx, %4\n\t"
        "mov rcx, %5\n\t"
        "mov r8, %6\n\t"
        "mov r9, %7\n\t"
        "mov r10, %8\n\t"
        "syscall\n\t"
        "mov %0, rax\n\t"
        ".att_syntax prefix;\n\t"
        : "=r"(ret)
        : "r"(rax),
          "r"(rdi),
          "r"(rsi),
          "r"(rdx),
          "r"(rcx),
          "r"(r8),
          "r"(r9),
          "r"(r10)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "memory");

    return ret;
}

extern "C" void _start(void)
{
    volatile char hello_world[] = "Hello, World\n";

    auto ret = syscall(1,
                       1,
                       reinterpret_cast<unsigned long>(hello_world),
                       sizeof(hello_world) - 1);

    if (ret > 0)
    {
        g_struct.test_method(0xFFFFFFFFFFFFFFFF);
    }
}
