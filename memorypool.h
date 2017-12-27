#pragma once

#include <mutex>

#include <list>
#include <array>

#include <assert.h>

const constexpr int DefaultPreallocSize = 10;

template <typename T, size_t N = DefaultPreallocSize>
class MemoryPool
{
public:
    static MemoryPool& GetInstance()
    {
        if (instance)
            return *instance;

        std::unique_lock<std::mutex> lock;

        if (instance)
            return *instance;

        instance = new MemoryPool();
        return *instance;
    }

    T* Alloc(size_t n)
    {
        assert (n <= N && "incorrect allocation size");
        auto& lastChunk = memoryPool.back();
        T* lastChunkStart = &lastChunk[0];
        T* ptr = lastChunkStart + countIssued;
        if (countIssued + n > N)
        {
            AppendNewChunk();
            auto& newLastChunk = memoryPool.back();
            ptr = &newLastChunk[0];
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

private:
    using Chunk = std::array<T, N>;
    std::list<Chunk> memoryPool;
    size_t countIssued;

    static MemoryPool* instance;
};

template <typename T, size_t N>
MemoryPool<T, N>* MemoryPool<T, N>::instance = nullptr;
