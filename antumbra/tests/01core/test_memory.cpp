#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
#include <catch2/catch_test_macros.hpp>

#include "framework/01core/memory/allocators/linear_allocator.h"
#include "framework/01core/memory/allocators/pool_allocator.h"
#include "framework/01core/memory/allocators/stack_allocator.h"
using namespace ant::memory;
using namespace ant;
TEST_CASE("malloc") {
    //initialize_memory_system();

    //auto pool = create_memory_pool("test malloc", 10_mb);

    struct A {
        char e[4];
    };
    void *pa = ant::memory::alloc<A>();
    REQUIRE(pa != nullptr);
    ant::memory::afree(pa);

    void *pb = ant::memory::amalloc(sizeof(A));
    REQUIRE(pb != nullptr);
    ant::memory::afree(pb);
    //destroy_memory_system();
}

TEST_CASE("memory") { ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 1024); }

TEST_CASE("container") {
    //initialize_memory_system();
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
    // destroy_memory_system();
}
