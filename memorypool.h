#pragma once

class MemoryPool
{
public:
    static MemoryPool& GetInstance();

    void AddAlloc(size_t size);
    void* GetAlloc(size_t size);

private:
    MemoryPool();

    size_t countAllocated;
    void* ptrAllocated;
    void* ptrIssued;
};
