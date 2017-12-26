#define BOOST_TEST_MODULE allocator_test

#include <boost/test/unit_test.hpp>

#include "allocatorlib.h"

BOOST_AUTO_TEST_SUITE(allocator_test)

struct Fixture
{
    Fixture()
    {
    }
};

BOOST_FIXTURE_TEST_CASE(test_reverse_lexicographycal_sort, Fixture)
{
}

}
