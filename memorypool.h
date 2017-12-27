#pragma once

#include <list>
#include <array>

#include <assert.h>

const constexpr int DefaultPreallocSize = 10;

// does not work in multithreaded environment
template <typename T, size_t N = DefaultPreallocSize>
class MemoryPool
{
public:
    static MemoryPool& GetInstance()
    {
        if (instance)
            return *instance;

        instance = new MemoryPool();
        return *instance;
    }

    T* Alloc(size_t n)
    {
        assert (n <= N && "incorrect allocation size");
        T* ptr = GetLastChunkStart() + countIssued;
        if (countIssued + n > N)
        {
            AppendNewChunk();
            ptr = GetLastChunkStart();
        }
        countIssued += n;
        return ptr;
    }

private:

    MemoryPool()
        : countIssued(0)
    {
        AppendNewChunk();
    }

    void AppendNewChunk()
    {
        Chunk newChunk;
        memoryPool.push_back(newChunk);
        countIssued = 0;
    }

    T* GetLastChunkStart()
    {
        auto& newLastChunk = memoryPool.back();
        return &newLastChunk[0];
    }
private:
    using Chunk = std::array<T, N>;

    std::list<Chunk> memoryPool;
    size_t countIssued;

    static MemoryPool* instance;
};

template <typename T, size_t N>
MemoryPool<T, N>* MemoryPool<T, N>::instance = nullptr;
