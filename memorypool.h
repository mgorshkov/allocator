#pragma once

#include <array>
#include <map>
#include <iostream>

#include <assert.h>

const constexpr int DefaultPreallocSize = 10;

// Memory pool for chunked allocator
// Allocates memory by N-bytes chunks
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

    using Chunk = std::array<T, N>;

    T* Alloc(size_t n)
    {
#ifdef DEBUG_PRINT
        std::cout << "allocate: " << n << std::endl;
#endif
        assert (n <= N && "invalid allocation size");
        for (auto& chunk : memoryPool)
        {
            auto& ar = *chunk.first;
            if (chunk.second + n <= N)
            {
                T* ptr = &ar[0] + chunk.second;
#ifdef DEBUG_PRINT
                std::cout << "allocated ready: " << ptr << ", "
                          << "chunk.first: " << chunk.first << ", "
                          << "chunk.second: " << chunk.second << std::endl;
#endif
                chunk.second += n;
                return ptr;
            }
        }
        return AppendNewChunk(n);
    }

    void Dealloc(T* p)
    {
#ifdef DEBUG_PRINT
        std::cout << "deallocate: " << p << std::endl;
#endif
        auto chunk = std::prev(memoryPool.upper_bound(reinterpret_cast<Chunk*>(p)));
#ifdef DEBUG_PRINT
        std::cout << "deallocate, chunk: " << chunk->first << std::endl;
#endif
        if (--chunk->second == 0)
        {
            delete chunk->first;
            memoryPool.erase(chunk);
        }
    }

private:

    MemoryPool()
    {
    }

    ~MemoryPool()
    {
        for (auto& chunk : memoryPool)
        {
            delete chunk->first;
        }
    }

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    MemoryPool& operator = (const MemoryPool&) = delete;
    MemoryPool& operator = (MemoryPool&&) = delete;

    T* AppendNewChunk(size_t n)
    {
        auto newChunk = new Chunk();
        memoryPool.emplace(std::make_pair(newChunk, n));
        auto& ar = *newChunk;
        auto ptr = &ar[0];
#ifdef DEBUG_PRINT
        std::cout << "allocated new: " << ptr << std::endl;
#endif
        return ptr;
    }

private:
    std::map<Chunk*, size_t> memoryPool;  // Chunk -> countIssued, for quick deallocation

    static MemoryPool* instance;
};

template <typename T, size_t N>
MemoryPool<T, N>* MemoryPool<T, N>::instance = nullptr;

