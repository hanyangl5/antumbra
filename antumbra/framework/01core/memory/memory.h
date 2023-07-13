#pragma once

// standard libraries
#include <memory_resource>
#include <type_traits>
// third party libraries
#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>


// project headers
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/allocators/allocator2.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/singleton/public_singleton.h"
#include "framework/01core/utils/utils.h"

namespace ant::memory {

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = false; }

void *amalloc(u64 size, memory_pool *pool = nullptr);
template <typename T = void, typename... Args> T *alloc(Args &&...args, memory_pool *pool = nullptr) {
    void *memory;
    if (pool == nullptr) {
        memory = mi_malloc(sizeof(T));
    } else {
        memory = pool->allocate(sizeof(T));
    }
    if (memory == nullptr) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: memory allocation failed");
        }
        return false;
    }
    return new (memory) T(std::forward<Args>(args)...);
}

template <typename T> void afree(T *ptr, memory_pool *pool = nullptr) {
    if (!ptr) {
        return;
    }
    ptr->~T();
    if (pool == nullptr) {
        free(ptr);
    } else {
        pool->deallocate(ptr, 0);
    }
}

#define ACQUIRE_STACK_MEMORY_RESOURCE(name, bytes)                                                                     \
    char __stack_memory__[bytes];                                                                                      \
    std::pmr::monotonic_buffer_resource name((void *)__stack_memory__, bytes);
} // namespace ant::memory