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
extern slab_allocator *slab_memory_allocator;
extern tlsf_allocator *tlsf_memory_allocator;
extern fixedsize_allocator *fixedsize_memory_allocator;

void initialize_memory_system();

void destroy_memory_system();

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = true; }

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

} // namespace ant::memory