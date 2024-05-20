#ifndef __PRINTS_H
#define __PRINTS_H

#include <common/types.h>

namespace myos
{
    void printf(char *str);
    void printfHex(common::uint8_t key);
    void printfHex16(common::uint16_t key);
    void printfHex32(common::uint32_t key);
    void itoa(int num, char* buffer);
    void itoa(myos::common::uint32_t num, char* buffer, int base = 10);
    void itoa(long num, char* buffer);
}

#endif