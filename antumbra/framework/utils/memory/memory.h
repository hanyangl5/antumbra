#pragma once

// standard libraries
#include <type_traits>
// third party libraries

// project headers
#include "framework/utils/logging/log.h"
#include "framework/utils/memory/container.h"
#include "framework/utils/singleton/public_singleton.h"
#include "framework/utils/types/types.h"

namespace ante::memory {

extern bool b_enable_memory_tracking;

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = false; }

void *amalloc(u64 size);
void *aaligned_alloc(u64 alignment, u64 size);
template <typename T = void, typename... Args> T *alloc(Args &&...args) {
    void *memory = nullptr;
    memory = malloc(sizeof(T));
    if (memory == nullptr) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: memory allocation failed");
        }
        return nullptr;
    }
    return new (memory) T(std::forward<Args>(args)...);
}

template <typename T> void afree(T *ptr) {
    if (!ptr) {
        return;
    }
    if constexpr (!std::is_scalar<T>::value && !std::is_same<T, void>::value) {
        ptr->~T();
    }
    free(ptr);
}
#define ACQUIRE_STACK_MEMORY_RESOURCE(name, bytes)                                                                     \
    ante::fixed_array<char, bytes> __stack_memory__;                                                                   \
    memory::ante_pmr::monotonic_buffer_resource name(__stack_memory__.data(), bytes);

} // namespace ante::memory