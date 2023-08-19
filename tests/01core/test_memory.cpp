#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
//#include <catch2/catch_test_macros.hpp>
#include <doctest.h>
#include <cstddef>

#include "framework/01core/memory/allocators/linear_allocator.h"
#include "framework/01core/memory/allocators/pool_allocator.h"
#include "framework/01core/memory/allocators/stack_allocator.h"
#include "framework/01core/memory/smart_ptr.h"
using namespace ante::memory;
using namespace ante;
TEST_CASE("malloc") {

    struct A {
        char e[4];
    };
    void *pa = ante::memory::alloc<A>(nullptr);
    REQUIRE(pa != nullptr);
    ante::memory::afree(pa);

    void *pb = ante::memory::amalloc(sizeof(A));
    REQUIRE(pb != nullptr);
    ante::memory::afree(pb);
}

TEST_CASE("memory") {
    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 1024);
}

TEST_CASE("container") {
    ante::str str = "test string";
    REQUIRE(str.length() == 11);

    ante::vector<i32> vec;

    for (u32 i = 0; i < 5; i++) {
        vec.push_back(i);
        REQUIRE(vec.back() == static_cast<i32>(i));
        REQUIRE(static_cast<u32>(vec.size()) == i + 1);
    }
    ante::fixed_array<i32, 5> arr{1, 2, 3, 4, 5};
    REQUIRE(arr.size() == 5);
}

TEST_CASE("uniqueptr") {
    class A {
      public:
        A() { LOG_DEBUG("actor"); }
        ~A() { LOG_DEBUG("actor"); }
        int a;
    };
    {
        unique_ptr<A> pi = make_unique<A>();
        pi->a = 1;
        auto pb = std::move(pi);
        unique_ptr<A> pc = make_unique<A>();
        pc->a = 4;
        pb.swap(pc);
        LOG_DEBUG("{} {}", pb->a, pc->a);
    }

    {
        ACQUIRE_STACK_MEMORY_RESOURCE(data, 64);
        unique_ptr<A> pi = make_unique<A>(&data);
    }
}
