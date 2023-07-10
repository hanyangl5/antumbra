#pragma once

// standard libraries
#include <memory_resource>
#include <type_traits>
// third party libraries

// project headers
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/singleton/public_singleton.h"
#include "framework/01core/utils/utils.h"

namespace ant::memory {

extern bool b_enable_memory_tracking;

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = false; }

void *amalloc(u64 size);

template <typename T = void, typename... Args> T *alloc(Args &&...args) {
    void *memory = malloc(sizeof(T));
    //if (allocator == nullptr) {
    //    if (default_memory_allocator == nullptr) {
    //        default_memory_allocator = new default_allocator;
    //        allocator = default_memory_allocator;
    //    }
    //}
    //void *memory = allocator->allocate(sizeof(T));
    return new (memory) T(std::forward<Args>(args)...);
}

template <typename T> void afree(T *ptr) {
    if (!ptr) {
        return;
    }
    ptr->~T();
    //allocator->deallocate(ptr, 0);
    free(ptr);
}

// TODO(hyl5): memory pool
// each allocations are associated to a pool, pool defines the budgets of each system(e.g. render, physics).
//#define ANT_MEMORY_POOL(name, allocatorTpe, budget) allocatorTpe name(budget);

//template <typename T>
//constexpr memory_pool *create_memory_pool(const char *pool_name, u64 budget, memory_pool *parent) {
//    return new T(budget, pool_name, parent);
//}
//
//void destroy_memory_pool(memory_pool *pool) { delete pool; }

//ANT_MEMORY_POOL(good, default_allocator, 1024);

#define ACQUIRE_STACK_MEMORY_RESOURCE(name, bytes)                                                                     \
    char __stack_memory__[bytes];                                                                                      \
    std::pmr::monotonic_buffer_resource name((void *)__stack_memory__, bytes);
} // namespace ant::memory