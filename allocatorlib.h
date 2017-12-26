#include <cstddef>
#include <memory>

template<typename T>
struct AllocatorType
{
    typedef T value_type;
};


template <typename T, size_t Size>
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
    template<typename U, size_t S> struct rebind
    {
        typedef Allocator<U, S> other;
    };

    Allocator();
    ~Allocator();

    T* allocate(std::size_t n, const void * hint);
    void deallocate( T* p, std::size_t n );

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args);

    template<typename U>
    void destroy(U* p);
};

template <typename T, size_t Size>
Allocator<T, Size>::Allocator()
{

}

template <typename T, size_t Size>
Allocator<T, Size>::~Allocator()
{

}

template <typename T, size_t Size>
T* Allocator<T, Size>::allocate(std::size_t n, const void * hint)
{
    return ::operator new(n);
}

template <typename T, size_t Size>
void Allocator<T, Size>::deallocate(T* p, std::size_t)
{
    ::operator delete(p);
}

template <typename T, size_t Size>
template<class U, class... Args>
void Allocator<T, Size>::construct(U* p, Args&&... args)
{
    ::new((void*)p) U(std::forward<Args>(args)...);
}

template <typename T, size_t Size>
template<class U>
void Allocator<T, Size>::destroy(U* p)
{
    p->~U();
}


int version();
