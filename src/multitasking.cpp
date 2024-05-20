
#include <multitasking.h>
#include <prints.h>
#include <unixsyscalls.h>

using namespace myos;
using namespace myos::common;

// Initialize the static variable
common::uint32_t TaskManager::nextPID = 0; // Start from 0, assuming PID 0 is reserved for the kernel

common::uint32_t TaskManager::getNextPID() {
    return nextPID++;
}

// Copy constructor for Task class
Task::Task(const Task& parent) {

    // Set other members
    this->ppid = parent.pid;
    this->state = TaskState::READY;
    this->pid = TaskManager::getNextPID();
    this->priority = TaskPriority::MEDIUM;

    for (int i = 0; i < sizeof(parent.stack); i++)
    {
        this->stack[i]=parent.stack[i];
    }

    // Copy all members of CPUState from parent
    *(this->cpustate) = *(parent.cpustate);

}

Task::Task(GlobalDescriptorTable *gdt, common::uint32_t ppid, void entrypoint()) {
    
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
       
    this->pid = TaskManager::getNextPID();
    this->ppid = ppid;
    this->state = TaskState::READY;
    this->priority = TaskPriority::MEDIUM;

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;
    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    cpustate->eflags = 0x202;
}

Task::Task(GlobalDescriptorTable *gdt, common::uint32_t pid, common::uint32_t ppid, void entrypoint()) {
    
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
       
    this->pid = pid;
    this->ppid = ppid;
    this->state = TaskState::READY;
    this->priority = TaskPriority::MEDIUM;

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;
    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    cpustate->eflags = 0x202;
}

Task::~Task() {
}

TaskManager::TaskManager(GlobalDescriptorTable* gdt) {
    numTasks = 0;
    currentTask = -1;
    this->gdt = gdt;
}

TaskManager::~TaskManager() {
}


bool TaskManager::addTask(Task * task) {
    if(numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    return true;
}

CPUState* TaskManager::schedule(CPUState* cpustate)
{
    if (numTasks <= 0)
        return cpustate;

    // Save the state of the current task
    if (currentTask >= 0 && tasks[currentTask]->state == TaskState::RUNNING) {
        tasks[currentTask]->state = TaskState::READY;
        tasks[currentTask]->cpustate = cpustate;
    }

    // Find the next ready task
    int nextTask = currentTask;
    bool foundReadyTask = false;

    for (int i = 0; i < numTasks; ++i) {
        nextTask = (nextTask + 1) % numTasks;
        if (tasks[nextTask]->state == TaskState::READY) {
            foundReadyTask = true;
            break;
        }
    }

    if (foundReadyTask) {
        currentTask = nextTask;
        tasks[currentTask]->state = TaskState::RUNNING;
        
        //printTaskInfo();
        return tasks[currentTask]->cpustate;
    }

    // If no ready task is found, return the current state
    return cpustate;
}

    
common::uint32_t TaskManager::fork(CPUState* cpustate) {
    
    printf("$$ TM_FORK $$");
    // Check if we can create a new task
    if (numTasks >= 256) {
        // Cannot create more tasks, return -1 for error
        return -1;
    }

    // Clone the current task
    Task* parentTask = tasks[currentTask];
    Task* childTask = new Task(*parentTask); // Use the parent task

    common::uint32_t currentTaskOffset=(((common::uint32_t)cpustate - (common::uint32_t) parentTask->stack));
    childTask->cpustate =(CPUState*)(((common::uint32_t) childTask->stack) + currentTaskOffset);

    childTask->cpustate->ecx = 0; // Child process returns 0 from fork
    tasks[numTasks++] = childTask;

    // Return the child process ID (PID) for the parent process
    return childTask->pid;
}

common::uint32_t TaskManager::execve(void (*entrypoint)()) {
    printf("$$ TM_EXECVE $$\n");
    Task * current = tasks[currentTask];
    current = new Task(gdt, current->pid, current->ppid, entrypoint);
    return (uint32_t) current->cpustate;
}

        
uint32_t TaskManager::wait_pid(common::uint32_t pid) {
    Task* parentTask = tasks[currentTask];

    bool validChild = false;
     for (int i = 0; i < numTasks; ++i) {
        if (tasks[i]->pid == pid && tasks[i]->ppid == parentTask->pid) {  
            validChild = true;
            if (tasks[i]->state == TaskState::TERMINATED) {
                return pid; // Child has terminated, return its PID
            }
            break;
        }
    }

/*
    if (!validChild) {
        return -1; // No such child
    }
*/

    // Return 0 if the child process with the specified PID is not found or not terminated
    return 0;  
}


void TaskManager::exit() {
    printf("$$ TM_EXIT $$\n");
    tasks[currentTask]->state = TaskState::TERMINATED;

    uint32_t terminatedPid = tasks[currentTask]->pid;
    for (int i = 0; i < numTasks; ++i) {
        if (tasks[i]->ppid == terminatedPid) {
            tasks[i]->ppid = 0; // Reassign to init process
        }
    }

}

void TaskManager::printTaskInfo() {
    printf("=== Process Table ===\n");
    for (int i = 0; i < numTasks; ++i) {
        Task* task = tasks[i];
        char buffer[20];
        printf("PID: ");
        itoa(task->pid, buffer);
        printf(buffer);

        printf(", PPID: ");
        itoa(task->ppid, buffer);
        printf(buffer);
        printf(", STATE: ");
        switch (task->state) {
            case TaskState::READY:
                printf("READY");
                break;
            case TaskState::RUNNING:
                printf("RUNNING");
                break;
            case TaskState::BLOCKED:
                printf("BLOCKED");
                break;
            case TaskState::TERMINATED:
                printf("TERMINATED");
                break;
        }

        printf(", PRIORITY: ");
        switch (task->priority) {
            case TaskPriority::LOW:
                printf("LOW");
                break;
            case TaskPriority::MEDIUM:
                printf("MEDIUM");
                break;
            case TaskPriority::HIGH:
                printf("HIGH");
                break;
        }

        if (task->cpustate) {
            printf(", EAX: ");
            itoa(task->cpustate->eax, buffer);
            printf(buffer);

            printf(", EBX: ");
            itoa(task->cpustate->ebx, buffer);
            printf(buffer);

            printf(", ECX: ");
            itoa(task->cpustate->ecx, buffer);
            printf(buffer);

            printf(", EIP: ");
            itoa(task->cpustate->eip, buffer, 16);  // Printing eip in hexadecimal
            printf(buffer);
        }

        printf("\n");
    }
    printf("=====================\n");
}


bool TaskManager::areAllTasksTerminated() {
    for (int i = 1; i < numTasks; i++) {
        if (tasks[i]->state != TaskState::TERMINATED) {
            return false;
        }
    }
    return true;
}
