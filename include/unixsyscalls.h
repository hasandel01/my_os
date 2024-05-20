#ifndef _UNIXSYSCALLS_H
#define _UNIXSYSCALLS_H

#include <common/types.h>

using namespace myos;
using namespace myos::common;

namespace myos {
    void sys_printf(char * str);
    uint32_t sys_fork();
    void custom_fork();
    void sys_execve(void (*entrypoint)());
    void sys_exit();
    uint32_t sys_waitpid(uint32_t pid);
}

#endif