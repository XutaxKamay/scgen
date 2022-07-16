#include "pch.h"
#include "view_as.h"

void main()
{
    volatile const char hello_world[] = "Hello, World\n";
    kernel::syscall(__NR_write,
                    STDOUT_FILENO,
                    hello_world,
                    sizeof(hello_world));

    *view_as<int*>(0x13370000) = 1337;

    kernel::syscall(__NR_exit, 0);
}

extern "C" void _start()
{
    main();
}
