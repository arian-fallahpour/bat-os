
#include <memorymanagement.h>

using namespace batos;
using namespace batos::common;

MemoryManager* MemoryManager::activeMemoryManager = 0;

MemoryManager::MemoryManager(size_t start, size_t size) {
    activeMemoryManager = this;

    if (size < sizeof(MemoryChunk)) {
        first = 0;
    } else {
        first = (MemoryChunk*)start;

        first->allocated = false;
        first->prev = 0;
        first->next = 0;
        first->size = size - sizeof(MemoryChunk);
    }
};

MemoryManager::~MemoryManager() {
    if (activeMemoryManager == this) {
        activeMemoryManager = 0;
    }
};

void* MemoryManager::malloc(size_t size) {
    MemoryChunk* result = 0;

    for (MemoryChunk* chunk = first; chunk != 0 && result == 0;
         chunk = chunk->next) {
        if (chunk->size > size && !chunk->allocated) {
            result = chunk;
            break;
        }
    }

    if (result == 0) {
        return 0;
    }

    if (result->size >= size + sizeof(MemoryChunk) + 1) {
        MemoryChunk* temp
            = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);

        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if (result->next != 0) {
            result->next->prev = temp;
        }

        result->size = size;
        result->next = temp;
    }

    result->allocated = true;
    return (void*)((size_t)result + sizeof(MemoryChunk));
};

void MemoryManager::free(void* ptr) {
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
    chunk->allocated = false;

    if (chunk->prev != 0 && !chunk->prev->allocated) {
        chunk->prev->next = chunk->next;
        chunk->prev->size += sizeof(MemoryChunk) + chunk->size;
        if (chunk->next != 0) {
            chunk->next->prev = chunk->prev;
        }

        chunk = chunk->prev;
    }

    if (chunk->next != 0 && !chunk->next->allocated) {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if (chunk->next != 0) {
            chunk->next->prev = chunk;
        }
    }
};

void* operator new(unsigned size) {
    if (batos::MemoryManager::activeMemoryManager == 0) {
        return 0;
    }

    return batos::MemoryManager::activeMemoryManager->malloc(size);
};

void* operator new[](unsigned size) {
    if (batos::MemoryManager::activeMemoryManager == 0) {
        return 0;
    }

    return batos::MemoryManager::activeMemoryManager->malloc(size);
};

void* operator new(unsigned size, void* ptr) { return ptr; };

void* operator new[](unsigned size, void* ptr) { return ptr; };

void operator delete(void* ptr) {
    if (batos::MemoryManager::activeMemoryManager != 0) {
        batos::MemoryManager::activeMemoryManager->free(ptr);
    }
};

void operator delete[](void* ptr) {
    if (batos::MemoryManager::activeMemoryManager != 0) {
        batos::MemoryManager::activeMemoryManager->free(ptr);
    }
};