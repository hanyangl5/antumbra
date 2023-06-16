#include "memory.h"

namespace ant::memory {

void initialize_memory_system() {
    default_memory_allocator = new default_allocator(1024); // TODO(hyl5): use smart pointer?
    if (b_enable_memory_tracking) {
        LOG_DEBUG("initialize memory system");
    }
}

void destroy_memory_system() {
    if (default_memory_allocator) {
        delete default_memory_allocator;
    }
    if (b_enable_memory_tracking) {
        LOG_DEBUG("destroy memory system");
    }
}

void *amalloc(u64 size, std::pmr::memory_resource *allocator) { return allocator->allocate(size); }

//void *ant_calloc(u64 num, u64 size, std::pmr::memory_resource *allocator) {
//    if (b_enable_memory_tracking) {
//        LOG_DEBUG("alloc {} bytes", size * num);
//    }
//    allocator->allocate()
//    return mi_calloc(num, size);
//}

} // namespace ant::memory
