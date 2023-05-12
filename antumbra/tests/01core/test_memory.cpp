#include <catch2/catch_test_macros.hpp>
#include <framework/01core/memory/memory.h>
#include <framework/01core/utils/utils.h>

using namespace ant;

TEST_CASE("memory") { auto stack_resource = ant::GetStackMemoryResource(1024); }

TEST_CASE("container")
{
    ant::str str = "test string";
    REQUIRE(str.length() == 11);

    ant::vector<i32> vec;

    for (u32 i = 0; i < 5; i++) {
        vec.push_back(i);
        REQUIRE(vec.back() == static_cast<i32>(i));
        REQUIRE(static_cast<u32>(vec.size()) == i + 1);
    }
    ant::fixed_array<i32, 5> arr{ 1, 2, 3, 4, 5 };
    REQUIRE(arr.size() == 5);
}
