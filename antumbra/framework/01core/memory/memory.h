#pragma once

// standard libraries
#include <memory_resource>

// third party libraries
#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>

// project headers
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/utils/utils.h"

namespace ant {

extern bool b_enable_memory_tracking;

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = false; }

void *ant_malloc(u64 size);
void *ant_calloc(u64 num, u64 size);

template <typename T, typename... Args> T *ant_alloc(Args &&...args) {
    void *memory = mi_malloc(sizeof(T));
    if (b_enable_memory_tracking) {
        LOG_DEBUG("alloc {} bytes to {}", sizeof(T), memory);
    }
    return new (memory) T(std::forward<Args>(args)...);
}
//template <typename T, typename... Args> T *ant_alloc_aligned(Args &&...args) {
//    void *memory = mi_malloc_aligned(sizeof(T), alignof(std::max_align_t));
//    return new (memory) T(std::forward<Args>(args)...);
//}

template <typename T> void ant_free(T *ptr) {
    if (!ptr) {
        return;
    }
    ptr->~T();
    mi_free(ptr);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("free {}", (void *)ptr);
    }
}

inline std::pmr::monotonic_buffer_resource get_stack_memory_resource(u64 size = 1024) {
    return std::pmr::monotonic_buffer_resource(size);
}

} // namespace ant