#pragma once

#include <iostream>
#include <memory>

template <typename T, typename A = std::allocator<T>>
class SinglyLinkedList
{
public:
    struct Node
    {
        T data;
        Node* next;
    };
    SinglyLinkedList(A allocator = A());
    ~SinglyLinkedList();

    void Add(const T& data);

    template <typename U, typename B>
    friend std::ostream& operator << (std::ostream& stream, const SinglyLinkedList<U, B>& list);

    class Iterator
    {
    public:
        Iterator(SinglyLinkedList* list)
            : parent(list)
            , ptr(list->head)
        {
        }
        void operator ++ ()
        {
            if (ptr != nullptr)
                ptr = ptr->next;
        }
        typename SinglyLinkedList::Node& operator * ()
        {
            return *ptr;
        }
        typename SinglyLinkedList::Node* operator -> ()
        {
            return ptr;
        }
        bool IsEnd() const
        {
            return ptr == nullptr;
        }
    private:
        SinglyLinkedList* parent;
        typename SinglyLinkedList::Node* ptr;
    };

    friend class Iterator;

private:
    void Destroy(Node* node);

    Node* head;
    Node* tail;

    using AllocatorTraits = std::allocator_traits<A>;
    using RebindAllocNode = typename AllocatorTraits::template rebind_alloc<Node>;

    A allocData;
    RebindAllocNode allocNode;
};

template <typename T, typename A>
SinglyLinkedList<T, A>::SinglyLinkedList(A allocator)
    : head(nullptr)
    , tail(nullptr)
    , allocData(allocator)
{
}

template <typename T, typename A>
SinglyLinkedList<T, A>::~SinglyLinkedList()
{
    auto ptr = head;
    while (ptr != nullptr)
    {
        auto next = ptr->next;
        Destroy(ptr);
        ptr = next;
    }
}

template <typename T, typename A>
void SinglyLinkedList<T, A>::Destroy(Node* node)
{
    allocData.destroy(node);
    allocNode.deallocate(node, 1);
}

template <typename T, typename A>
void SinglyLinkedList<T, A>::Add(const T& data)
{
    Node* node = allocNode.allocate(1);
    allocData.construct(node, Node{data, nullptr});
    if (tail != nullptr)
        tail->next = node;
    tail = node;
    if (head == nullptr)
        head = node;
}

template <typename T, typename A>
std::ostream& operator << (std::ostream& stream, const SinglyLinkedList<T, A>& list)
{
    for (typename SinglyLinkedList<T, A>::Node* ptr = list.head; ptr != nullptr; ptr = ptr->next)
        stream << ptr->data << std::endl;

    return stream;
}
