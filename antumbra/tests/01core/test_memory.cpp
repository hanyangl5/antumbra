#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
#include <catch2/catch_test_macros.hpp>

using namespace ant;

TEST_CASE("malloc") {
    initialize_memory_system();
    struct A {
        char e[4];
    };
    //ant::ant_enable_memory_tracking();
    void *pa = ant::ant_alloc<A>(allocator::default_memory_allocator);
    ant::ant_free(pa, allocator::default_memory_allocator);

    void *pb = ant_malloc(sizeof(A), allocator::default_memory_allocator);
    ant::ant_free(pb, allocator::default_memory_allocator);
    //ant::ant_disable_memory_tracking();
    destroy_memory_system();
}

TEST_CASE("memory") {
    initialize_memory_system();
    stack_allocator stack_memory(1024u);
    destroy_memory_system();
}

TEST_CASE("container") {
    initialize_memory_system();
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
    destroy_memory_system();
}
