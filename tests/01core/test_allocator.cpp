#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
#include <catch2/catch_test_macros.hpp>

#include "framework/01core/memory/allocators/linear_allocator.h"
#include "framework/01core/memory/allocators/pool_allocator.h"
#include "framework/01core/memory/allocators/stack_allocator.h"
using namespace ante::memory;
using namespace ante;

ante::fixed_array<u64, 10> allocation = {8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

TEST_CASE("allocator_linear") {
    memory::linear_allocator pool(8192, 8);
    void *ptr[10];
    for (u64 i = 0; i < allocation.size(); i++) {
        ptr[i] = pool.allocate(allocation[i]); // success

        REQUIRE(ptr[i] != nullptr);
    }
    // linear allocator doesn't support free

    void *null = pool.allocate(0xffffffff); // fail
    REQUIRE(null == nullptr);
}

TEST_CASE("allocator_stack") {
    memory::stack_allocator pool(8192 + 10 * stack_allocator::header_size, 8);
    void *ptr[10];

    for (u64 i = 0; i < allocation.size(); i++) {
        ptr[i] = pool.allocate(allocation[i]); // success
        REQUIRE(ptr[i] != nullptr);
    }

    void *null = pool.allocate(0xffffffff); // fail
    REQUIRE(null == nullptr);

    for (u64 i = 0; i < allocation.size() - 1; i++) {
        pool.deallocate(ptr[i], allocation[i]); // fail
    }
    for (int i = (int)allocation.size() - 1; i >= 0; i--) {
        pool.deallocate(ptr[i], allocation[i]); // success
    }
    int x;
    pool.deallocate(&x, 64); // fail
}

TEST_CASE("allocator_pool") {
    memory::pool_allocator pool(8192, 64);
    constexpr u64 chunk_count = 8192 / 64;
    void *ptr[chunk_count];

    for (u64 i = 0; i < chunk_count; i++) {
        ptr[i] = pool.allocate(64); // success
        REQUIRE(ptr[i] != nullptr);
    }

    void *null = pool.allocate(64); // fail
    REQUIRE(null == nullptr);

    for (u64 i = 0; i < chunk_count; i++) {
        pool.deallocate(ptr[i], 64); // fail
    }

    int x;
    pool.deallocate(&x, 64); // fail
}
