#include "memory.h"

namespace ant::memory {

void *amalloc(u64 size, memory_pool *pool) {
    void *ptr;
    if (pool == nullptr) {
        ptr = malloc(size);
    } else {
        ptr = pool->allocate(size);
    }
    if (ptr == nullptr) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: memory allocation failed");
        }
        return nullptr;
    }
    return ptr;
}
//void *ant_calloc(u64 num, u64 size, std::pmr::memory_resource *allocator) {
//    if (b_enable_memory_tracking) {
//        LOG_DEBUG("alloc {} bytes", size * num);
//    }
//    allocator->allocate()
//    return mi_calloc(num, size);
//}

} // namespace ant::memory
