 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{

    enum class TaskState
    {
        RUNNING,
        READY,
        BLOCKED,
        TERMINATED
    };

    enum class TaskPriority
    {
        LOW,
        MEDIUM,
        HIGH
    };
    
    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;        
    } __attribute__((packed));
    
    
    class Task
    {
    friend class TaskManager;
    private:
        common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;
        TaskState state;
        TaskPriority priority; 
        common::uint32_t pid;
        common::uint32_t ppid;
    public:
        Task(GlobalDescriptorTable *gdt, common::uint32_t ppid, void entrypoint());
        Task(GlobalDescriptorTable *gdt, common::uint32_t pid, common::uint32_t ppid,  void entrypoint());
        // Copy constructor
        Task(const Task& parent);
        ~Task();
    };
    
    
    class TaskManager
    {
    private:
        Task* tasks[256];
        int numTasks;
        int currentTask;
        GlobalDescriptorTable* gdt; // Add a member variable to store a reference to the GDT
        static common::uint32_t nextPID; // Static variable to track the next available PID
    public:
        TaskManager(GlobalDescriptorTable* gdt);
        ~TaskManager();
        bool addTask(Task * task);
        common::uint32_t fork(CPUState * cpustate);
        common::uint32_t execve(void (* entrypoint)());
        common::uint32_t wait_pid(common::uint32_t pid);
        void exit();
        static common::uint32_t getNextPID();
        CPUState* schedule(CPUState* cpustate);
        void printTaskInfo();
        bool areAllTasksTerminated();
    };
    
    
    
}

#endif