#include <cstddef>
#include "memorypool.h"

template<typename T>
struct AllocatorType
{
    typedef T value_type;
};

template <typename T>
class Allocator
{
public:
    typedef typename AllocatorType<T>::value_type value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template<typename U> struct rebind
    {
        typedef Allocator<U> other;
    };

    Allocator(size_t preallocated_size = 10);
    ~Allocator();

    pointer allocate(size_type n, const void* /*hint*/ = 0);
    void deallocate(pointer p, size_type /*n*/);

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args);

    template<typename U>
    void destroy(U* p);
};

template <typename T>
Allocator<T>::Allocator(size_t preallocated_size)
{
    MemoryPool::Instance().AddAlloc(preallocated_size);
}

template <typename T>
Allocator<T>::~Allocator()
{

}

template <typename T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type n, const void* /*hint*/)
{
    return MemoryPool::Instance().GetAlloc(n);
}

template <typename T>
void Allocator<T>::deallocate(pointer p, size_type)
{
}

template <typename T>
template<class U, class... Args>
void Allocator<T>::construct(U* p, Args&&... args)
{
    ::new((void*)p) U(std::forward<Args>(args)...);
}

template <typename T>
template<class U>
void Allocator<T>::destroy(U* p)
{
    p->~U();
}

template<>
class Allocator<void>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef void value_type;
    template<class U> struct rebind {
        typedef Allocator<U> other;
    };
};

template<typename T, typename U>
inline bool operator==( const Allocator<T>&, const Allocator<U>& )
{
    return true;
}

template<typename T, typename U>
inline bool operator!=( const Allocator<T>&, const Allocator<U>& )
{
    return false;
}


int version();
