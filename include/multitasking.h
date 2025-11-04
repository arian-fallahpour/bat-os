#ifndef __BATOS__MULTITASKING_H
#define __BATOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace batos {
    namespace multitasking {
        struct CPUState {
            batos::common::uint32_t eax;
            batos::common::uint32_t ebx;
            batos::common::uint32_t ecx;
            batos::common::uint32_t edx;
            
            batos::common::uint32_t esi;
            batos::common::uint32_t edi;
            batos::common::uint32_t ebp;

            // batos::common::uint32_t gs;
            // batos::common::uint32_t fs;
            // batos::common::uint32_t es;
            // batos::common::uint32_t ds;

            // batos::common::uint32_t error;
            
            batos::common::uint32_t eip;
            batos::common::uint32_t cs;
            batos::common::uint32_t eflags;
            batos::common::uint32_t esp;
            batos::common::uint32_t ss;
        } __attribute__((packed));

        class Task {
            friend class TaskManager;

            private:
                batos::common::uint8_t stack[4096]; // 8KiB
                CPUState* cpustate;
            
            public:
                Task(GlobalDescriptorTable* gdt, void entrypoint());
                ~Task();
        };

        class TaskManager {
            private:
                Task* tasks[256];
                int numTasks;
                int currentTask;
            
            public:
                TaskManager();
                ~TaskManager();
                bool AddTask(Task* task);
                CPUState* Schedule(CPUState* cpustate);
        };
    }
}

#endif