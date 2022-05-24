#include "pch.h"

void main()
{
    const char hello_world[] = "Hello, World";
    Kernel::syscall(__NR_write,
                    STDOUT_FILENO,
                    hello_world,
                    sizeof(hello_world));
}

extern "C" void _start()
{
    main();
}
