#include <cstddef>

#include "memorypool.h"

template<typename T>
struct ChunkedAllocatorType
{
    typedef T value_type;
};

// Allocates memory by N-byte chunks
// Meets "allocator" requirements of ISO C++ Standard, Section 20.1.5
template <typename T, size_t N = DefaultPreallocSize>
class ChunkedAllocator
{
public:
    typedef typename ChunkedAllocatorType<T>::value_type value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template<typename U>
    struct rebind
    {
        typedef ChunkedAllocator<U, N> other;
    };

    ChunkedAllocator() = default;
    ChunkedAllocator(const ChunkedAllocator&) = default;
    template <typename U, size_t S>
    ChunkedAllocator(const ChunkedAllocator<U, S>&) {}
    ~ChunkedAllocator() {}

    ChunkedAllocator& operator = (const ChunkedAllocator& other) = default;
    pointer allocate(size_type n, const void* /*hint*/ = 0);
    void deallocate(pointer p, size_type /*n*/);

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args);

    template<typename U>
    void destroy(U* p);
};

template <typename T, size_t N>
typename ChunkedAllocator<T, N>::pointer ChunkedAllocator<T, N>::allocate(size_type n, const void* /*hint*/)
{
    return MemoryPool<T, N>::GetInstance().Alloc(n);
}

template <typename T, size_t N>
void ChunkedAllocator<T, N>::deallocate(pointer p, size_type)
{
    MemoryPool<T, N>::GetInstance().Dealloc(p);
}

template <typename T, size_t N>
template <typename U, typename... Args>
void ChunkedAllocator<T, N>::construct(U* p, Args&&... args)
{
    ::new((void*)p) U(std::forward<Args>(args)...);
}

template <typename T, size_t N>
template <typename U>
void ChunkedAllocator<T, N>::destroy(U* p)
{
    p->~U();
}

// Analogous to std::allocator<void>, as defined in ISO C++ Standard, Section 20.4.1
template<size_t N>
class ChunkedAllocator<void, N>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef void value_type;
    template<typename U>
    struct rebind
    {
        typedef ChunkedAllocator<U, N> other;
    };
};

template<typename T, size_t N, typename U, size_t S>
inline bool operator == (const ChunkedAllocator<T, N>&, const ChunkedAllocator<U, S>&)
{
    return true;
}

template<typename T, size_t N, typename U, size_t S>
inline bool operator != (const ChunkedAllocator<T, N>&, const ChunkedAllocator<U, S>&)
{
    return false;
}

int version();
