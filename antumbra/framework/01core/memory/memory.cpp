#include "memory.h"

namespace ant {

void initialize_memory_system() {

    allocator::default_memory_allocator = new default_allocator; // TODO(hyl5): use smart pointer?
    if (b_enable_memory_tracking) {
        LOG_DEBUG("initialize memory system");
    }
}

void destroy_memory_system() { delete allocator::default_memory_allocator; }

void *ant_malloc(u64 size, std::pmr::memory_resource *allocator) {
    //if (!allocator) {
    //    initialize_memory_system();
    //}
    return allocator->allocate(size);
}

//void *ant_calloc(u64 num, u64 size, std::pmr::memory_resource *allocator) {
//    if (b_enable_memory_tracking) {
//        LOG_DEBUG("alloc {} bytes", size * num);
//    }
//    allocator->allocate()
//    return mi_calloc(num, size);
//}

} // namespace ant
