#pragma once

// standard libraries
#include <memory_resource>
// third party libraries

// project headers
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/allocator.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/utils/utils.h"

namespace ant {

extern bool b_enable_memory_tracking;
extern bool b_enable_memory_tracking;

namespace allocator {
extern default_allocator *default_memory_allocator;
extern slab_allocator *slab_memory_allocator;
extern tlsf_allocator *tlsf_memory_allocator;
extern fixedsize_allocator *fixedsize_memory_allocator;
} // namespace allocator

void initialize_memory_system();

void destroy_memory_system();

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = true; }

void *ant_malloc(u64 size, std::pmr::memory_resource *allocator);
void *ant_calloc(u64 num, u64 size, std::pmr::memory_resource *allocator);

template <typename T = void, typename... Args> T *ant_alloc(Args &&...args, std::pmr::memory_resource *allocator) {
    
    //if (!allocator) {
    //    initialize_memory_system();
    //}
    //void *memory = mi_malloc(sizeof(T));
    void *memory = allocator->allocate(sizeof(T));
    return new (memory) T(std::forward<Args>(args)...);
}
//template <typename T, typename... Args> T *ant_alloc_aligned(Args &&...args) {
//    void *memory = mi_malloc_aligned(sizeof(T), alignof(std::max_align_t));
//    return new (memory) T(std::forward<Args>(args)...);
//}

template <typename T> void ant_free(T *ptr, std::pmr::memory_resource *allocator) {
    if (!ptr) {
        return;
    }
    ptr->~T();
    allocator->deallocate(ptr, 0);
    //mi_free(ptr);

}

//inline std::pmr::monotonic_buffer_resource get_stack_memory_resource(u64 size = 1024) {
//    return std::pmr::monotonic_buffer_resource(size);
//}
//inline std::pmr::memory_resource *TO_MEMORY_RESOURCE(void *p) {
//    return reinterpret_cast<std::pmr::memory_resource *>(p);
//}
} // namespace ant