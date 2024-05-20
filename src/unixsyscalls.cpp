#include <common/types.h>
#include <unixsyscalls.h>

using namespace myos;
using namespace myos::common;

namespace myos {

    void sys_printf(char * str) {
        asm("int $0x80" : : "a" (4), "b" (str));
    }

    uint32_t sys_fork() {
        uint32_t pid;
    asm volatile("int $0x80" : "=c"(pid) : "a" (2) : "memory");
        return pid;
    }

    void custom_fork() {
        asm ("int $0x80" : : "a" (3));
    }
    
    void sys_execve(void (*entrypoint)()) {
        asm("int $0x80" : : "a" (11), "b" (entrypoint));
    }

    void sys_exit() {
        asm("int $0x80" : : "a" (1));
    }

    uint32_t sys_waitpid(uint32_t pid) {
        uint32_t ret;
        asm("int $0x80" : "=c" (ret): "a" (7), "b"(pid));
        return ret;
    }

}