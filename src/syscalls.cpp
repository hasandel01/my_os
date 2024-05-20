
#include <syscalls.h>
#include <prints.h>
 
using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;
 
SyscallHandler::SyscallHandler(TaskManager* taskManager, InterruptManager* interruptManager, uint8_t InterruptNumber)
: InterruptHandler(interruptManager, InterruptNumber  + interruptManager->HardwareInterruptOffset())
{
    this->taskManager = taskManager;
}
SyscallHandler::~SyscallHandler()
{
}

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState* cpu = (CPUState*)esp;
    
    switch(cpu->eax)
    {
        case 1:
            taskManager->exit();
            break;
        case 2:
            cpu->ecx = taskManager->fork(cpu);
            esp = (uint32_t) taskManager->schedule(cpu);
            break;
        case 3:
            taskManager->fork(cpu);
            break;
        case 4:
            printf((char*)cpu->ebx);
            break;
        case 7:
            cpu-> ecx = taskManager->wait_pid(cpu->ebx);
            break;
        case 11: 
            esp = taskManager->execve((void (*)()) cpu->ebx);
            break;
        default:
            break;
    }

    return esp;
}





