#if not defined(__x86_64__)
    #error "Not supported architecture"
#endif

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

static inline unsigned long long syscall(unsigned long long number,
                                         unsigned long long arg1 = 0,
                                         unsigned long long arg2 = 0,
                                         unsigned long long arg3 = 0,
                                         unsigned long long arg4 = 0,
                                         unsigned long long arg5 = 0,
                                         unsigned long long arg6 = 0,
                                         unsigned long long arg7 = 0)
{
    unsigned long long ret;

    /**
     * TODO:
     * Add support for Windows and others OSes here
     */
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
        : "r"(number),
          "r"(arg1),
          "r"(arg2),
          "r"(arg3),
          "r"(arg4),
          "r"(arg5),
          "r"(arg6),
          "r"(arg7)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "memory");

    return ret;
}

void main(void)
{
    Struct::initialized         = true;
    volatile char hello_world[] = "Hello, World\n";

    auto ret = syscall(1,
                       1,
                       reinterpret_cast<unsigned long long>(hello_world),
                       sizeof(hello_world) - 1);

    if (ret > 0)
    {
        g_struct.test_method(0xFFFFFFFFFFFFFFFF);
    }
}

extern "C" void _start(void)
{
    main();
}
