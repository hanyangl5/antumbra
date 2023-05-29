#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
#include <catch2/catch_test_macros.hpp>

using namespace ant;

TEST_CASE("malloc") {
    struct A {
        char e[4];
    };
    ant::ant_enable_memory_tracking();
    void *pa = ant::ant_alloc<A>();
    ant::ant_free(pa);

    void *pb = ant_malloc(sizeof(A));
    ant::ant_free(pb);
    ant::ant_disable_memory_tracking();
}

TEST_CASE("memory") { auto stack_resource = ant::get_stack_memory_resource(1024); }

TEST_CASE("container") {
    ant::str str = "test string";
    REQUIRE(str.length() == 11);

    ant::vector<i32> vec;

    for (u32 i = 0; i < 5; i++) {
        vec.push_back(i);
        REQUIRE(vec.back() == static_cast<i32>(i));
        REQUIRE(static_cast<u32>(vec.size()) == i + 1);
    }
    ant::fixed_array<i32, 5> arr{1, 2, 3, 4, 5};
    REQUIRE(arr.size() == 5);
}
