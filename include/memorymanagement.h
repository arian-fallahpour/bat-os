#ifndef __BATOS__MEMORYMANAGEMENT_H
#define __BATOS__MEMORYMANAGEMENT_H

#include <common/types.h>

namespace batos {
    struct MemoryChunk {
            MemoryChunk* next;
            MemoryChunk* prev;
            bool allocated;
            batos::common::size_t size;
    };

    class MemoryManager {
        protected:
            MemoryChunk* first;

        public:
            static MemoryManager* activeMemoryManager;

            MemoryManager(
                batos::common::size_t start,
                batos::common::size_t size
            );
            ~MemoryManager();

            void* malloc(batos::common::size_t size);
            void free(void* ptr);
    };
}  // namespace batos

void* operator new(unsigned size);
void* operator new[](unsigned size);

// placement new
void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* pts);
void operator delete[](void* pts);

#endif