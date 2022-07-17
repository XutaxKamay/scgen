#include "pch.h"
#include "view_as.h"

void main()
{
    volatile const char hello_world[] = "Hello, World\n";
    kernel::syscall(__NR_write,
                    STDOUT_FILENO,
                    hello_world,
                    sizeof(hello_world));

    kernel::syscall(__NR_exit, 0);
}

extern "C" void _start()
{
    main();
}
