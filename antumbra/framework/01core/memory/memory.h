#pragma once

// standard libraries
#include <memory_resource>
// third party libraries

// project headers
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/allocator.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/utils/utils.h"

namespace ant::memory {

extern bool b_enable_memory_tracking;
extern bool b_enable_memory_tracking;

extern default_allocator *default_memory_allocator;

void initialize_memory_system();

void destroy_memory_system();

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = false; }

void *amalloc(u64 size, std::pmr::memory_resource *allocator);

template <typename T = void, typename... Args> T *alloc(Args &&...args, std::pmr::memory_resource *allocator) {
    void *memory = allocator->allocate(sizeof(T));
    return new (memory) T(std::forward<Args>(args)...);
}

template <typename T> void afree(T *ptr, std::pmr::memory_resource *allocator) {
    if (!ptr) {
        return;
    }
    ptr->~T();
    allocator->deallocate(ptr, 0);
}

// TODO(hyl5): memory pool
// each allocations are associated to a pool, pool defines the budgets of each system(e.g. render, physics).
//#define ANT_MEMORY_POOL(name, allocator_type, budget) allocator_type name(budget);

template <typename T = default_allocator>
constexpr std::pmr::memory_resource create_memory_pool(const char *pool_name, u64 budget) {
    return T(budget, name);
    
}

//ANT_MEMORY_POOL(good, default_allocator, 1024);
} // namespace ant::memory