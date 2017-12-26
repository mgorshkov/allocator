#include <map>
#include <iostream>

#include "allocatorlib.h"

template <typename Map>
static void PrintMap(const Map& map)
{
    for (auto pair : map)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
}

static long long fact(int n)
{
    long long factorial = 1;
    for (int i = 1; i <= n; ++i)
    {
        factorial *= i;
    }
    return factorial;
}

static void TestMapWithStandardAllocator()
{
    using Map = std::map<int, long long>;

    Map map;

    for (int i = 0; i < 10; ++i)
    {
        map[i] = fact(i);
    }

    PrintMap(map);
}

static void TestMapWithCustomAllocator()
{
    using Map = std::map<int, long long, std::less<int>, Allocator<long long>>;

    Map map;

    for (int i = 0; i < 10; ++i)
    {
        map[i] = fact(i);
    }

    PrintMap(map);
}

int main(int, char const **)
{
    try
    {
        TestMapWithStandardAllocator();

        TestMapWithCustomAllocator();

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
