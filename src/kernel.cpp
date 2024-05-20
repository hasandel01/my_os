
#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <drivers/ata.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>
#include <unixsyscalls.h>
#include <programs.h>
#include <drivers/amd_am79c973.h>
#include <prints.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

    uint32_t wait_pid(uint32_t pid) {
        uint32_t ret = 0;
        while (ret == 0) {
            ret = sys_waitpid(pid);
        }
        return ret;
    }

    void ep_fork() {
        sys_printf("BEFORE FORK\n");
        custom_fork();
        sys_printf("AFTER FORK\n");
        while(true);
    }

    void ep_execve() {
        sys_execve(ep_collatz);
        printf("SHOULDN'T BE EXECUTED.");
        while(true);
    }

    void ep_waitpid() {
        sys_printf("WAIT_PID PROCESS\n");
        uint32_t pid = 3;
        uint32_t ret = wait_pid(pid);
        
        if (ret == pid) 
            printf("CHILD PID 3 IS TERMINATED\n");
        else if (ret == (uint32_t)-1) 
            printf("No such child process\n");
    
        sys_exit();
        while(true);
    }

    void ep_exit(){
        sys_exit();
        while(true);
    }


// Define a function type for the entry points
typedef void (*EntryPoint)();

// Define the number of entry points
const int count = 4; // Adjust this based on your requirement

// Define an array of entry points
EntryPoint entryPoints[count] = {ep_long_running_program, ep_collatz, ep_binary_search, ep_linear_search}; // Replace ep_func1, ep_func2, ep_func3 with your actual function names


void init_process()
{
    sys_printf("INIT PROCESS\n");
    while(true);
}

void ep_add_task() {

    sys_printf("BEFORE FORK\n");
    uint32_t pid = sys_fork();

    if (pid == 0) {
        // Child process
        sys_printf("AFTER FORK - CHILD\n");
        sys_execve(ep_linear_search);
    } else if (pid > 0) {
        // Parent process
        sys_printf("AFTER FORK - PARENT\n");
        wait_pid(pid);
    } else {
        // Fork failed
        sys_printf("FORK FAILED\n");
    }
    while (true);
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    GlobalDescriptorTable gdt;
    TaskManager taskManager(&gdt);
        
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    SyscallHandler syscalls(&taskManager,&interrupts, 0x80);
    
    Task init(&gdt,0, init_process);
    taskManager.addTask(&init);

/*
    Task forkTask(&gdt,0,ep_fork);
    taskManager.addTask(&forkTask);

    Task execve(&gdt, 0, ep_execve);
    Task linearSearchTask(&gdt,0,ep_linear_search);
    Task waitPid(&gdt,0,ep_waitpid);
    Task binaryTask(&gdt,2,ep_binary_search);
    
    Task collatzTask(&gdt,0, ep_collatz);
    Task longTask(&gdt,0,ep_long_running_program);
    Task exit(&gdt,0,ep_exit);


   // taskManager.addTask(&waitPid);
   // taskManager.addTask(&linearSearchTask);
    //taskManager.addTask(&binaryTask);
    taskManager.addTask(&execve);

    taskManager.addTask(&collatzTask);
    taskManager.addTask(&longTask);
    taskManager.addTask(&exit);
*/


    Task addTaskFork(&gdt, 0, ep_add_task);
    taskManager.addTask(&addTaskFork);

    interrupts.Activate();

    while(true);
}

