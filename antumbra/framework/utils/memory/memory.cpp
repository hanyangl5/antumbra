#include "memory.h"

namespace ante::memory {

void *amalloc(u64 size) {
    void *ptr = nullptr;
    ptr = malloc(size);
    if (ptr == nullptr) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: memory allocation failed");
        }
        return nullptr;
    }
    return ptr;
}
void *aaligned_alloc(u64 alignment, u64 size) {
    void *ptr = nullptr;
    ptr = aligned_alloc(alignment, size);

    if (ptr == nullptr) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: memory allocation failed");
        }
        return nullptr;
    }
    return ptr;
}
//void *ant_calloc(u64 num, u64 size, ante_pmr::memory_resource *allocator) {
//    if (b_enable_memory_tracking) {
//        LOG_DEBUG("alloc {} bytes", size * num);
//    }
//    allocator->allocate()
//    return mi_calloc(num, size);
//}

} // namespace ante::memory
