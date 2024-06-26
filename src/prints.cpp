#include <prints.h>
#include <common/types.h>

using namespace myos;
using namespace myos::common;

namespace myos {

    void printf(char* str)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;

        static uint8_t x=0,y=0;

        for(int i = 0; str[i] != '\0'; ++i)
        {
            switch(str[i])
            {
                case '\n':
                    x = 0;
                    y++;
                    break;
                default:
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                    x++;
                    break;
            }

            if(x >= 80)
            {
                x = 0;
                y++;
            }

            if(y >= 25)
            {
                for(y = 0; y < 25; y++)
                    for(x = 0; x < 80; x++)
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                x = 0;
                y = 0;
            }
        }
    }

    void itoa(int num, char* buffer) {
        int i = 0;
        if(num == 0) {
            buffer[i++] = '0';
            buffer[i] = '\0';
            return;
        }
        while(num != 0) {
            int rem = num % 10;
            buffer[i++] = rem + '0';
            num = num / 10;
        }
        buffer[i] = '\0';
        int start = 0;
        int end = i - 1;
        while(start < end) {
            char temp = buffer[start];
            buffer[start] = buffer[end];
            buffer[end] = temp;
            start++;
            end--;
        }
    }

        void itoa(long num, char* buffer) {
            
        int i = 0;
        if(num == 0) {
            buffer[i++] = '0';
            buffer[i] = '\0';
            return;
        }
        while(num != 0) {
            int rem = num % 10;
            buffer[i++] = rem + '0';
            num = num / 10;
        }
        buffer[i] = '\0';
        int start = 0;
        int end = i - 1;
        while(start < end) {
            char temp = buffer[start];
            buffer[start] = buffer[end];
            buffer[end] = temp;
            start++;
            end--;
        }
    }

    void itoa(uint32_t num, char* buffer, int base) {
    int i = 0;
    if(num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    while(num != 0) {
        int rem = num % base;
        buffer[i++] = rem + '0';
        num = num / base;
    }
    buffer[i] = '\0';
    int start = 0;
    int end = i - 1;
    while(start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}


    void printfHex(uint8_t key)
    {
        char* foo = "00";
        char* hex = "0123456789ABCDEF";
        foo[0] = hex[(key >> 4) & 0xF];
        foo[1] = hex[key & 0xF];
        printf(foo);
    }
    void printfHex16(uint16_t key)
    {
        printfHex((key >> 8) & 0xFF);
        printfHex( key & 0xFF);
    }
    void printfHex32(uint32_t key)
    {
        printfHex((key >> 24) & 0xFF);
        printfHex((key >> 16) & 0xFF);
        printfHex((key >> 8) & 0xFF);
        printfHex( key & 0xFF);
    }

}