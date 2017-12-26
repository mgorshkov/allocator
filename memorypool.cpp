#include "memorypool.h"

MemoryPool::MemoryPool()
    : countAllocated(0)
    , ptrAllocated(0)
    , ptrIssued(0)
{
}

MemoryPool& MemoryPool::GetInstance()
{
    if (instance)
        return instance;

    std::unique_lock<std::mutex> lock;

    if (instance)
        return instance;

    instance = MemoryPool();
    return instance;
}

void MemoryPool::AddAlloc(size_t size)
{
    ptrAllocated = realloc(size);
}

void* MemoryPool::GetAlloc(size_t size)
{
}

