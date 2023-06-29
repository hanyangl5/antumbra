#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
#include <catch2/catch_test_macros.hpp>

using namespace ant::memory;
using namespace ant;
TEST_CASE("malloc") {
    initialize_memory_system();

    auto pool = create_memory_pool("test malloc", 10_mb);

    struct A {
        char e[4];
    };
    void *pa = ant::memory::alloc<A>(pool);
    REQUIRE(pa != nullptr);
    ant::memory::afree(pa, pool);

    void *pb = ant::memory::amalloc(sizeof(A), pool);
    REQUIRE(pb != nullptr);
    ant::memory::afree(pb, pool);
    destroy_memory_system();
}

TEST_CASE("memory") {
    stack_memory_resource stack_memory(1024_b);
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
