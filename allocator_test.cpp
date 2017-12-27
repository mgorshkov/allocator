#define BOOST_TEST_MODULE allocator_test

#include <boost/test/unit_test.hpp>

#include "allocatorlib.h"
#include "slist.h"

BOOST_AUTO_TEST_SUITE(allocator_test)

BOOST_AUTO_TEST_CASE(test_map_with_allocator)
{
    using Map = std::map<int, long long, std::less<int>, ChunkedAllocator<long long>>;

    Map map;

    for (int i = 0; i < 1000; ++i)
    {
        map[i] = i;
    }

    for (int i = 0; i < 1000; ++i)
    {
        BOOST_CHECK_EQUAL(map[i], i);
    }
}

BOOST_AUTO_TEST_CASE(test_custom_list_with_allocator)
{
    using List = SinglyLinkedList<int, ChunkedAllocator<int>>;

    List list;

    for (int i = 0; i < 1000; ++i)
    {
        list.Add(i);
    }

    List::Iterator it(&list);
    int i = 0;
    while (!it.IsEnd())
    {
        BOOST_CHECK_EQUAL(it->data, i++);
        ++it;
    }
}

}
