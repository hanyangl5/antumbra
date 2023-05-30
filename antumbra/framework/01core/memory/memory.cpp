#include "memory.h"

namespace ant {
bool b_enable_memory_tracking = false;

void *ant_malloc(u64 size) {
    if (b_enable_memory_tracking) {
        LOG_DEBUG("alloc {} bytes", size);
    }
    return mi_malloc(size);
}

void *ant_calloc(u64 num, u64 size) {
    if (b_enable_memory_tracking) {
        LOG_DEBUG("alloc {} bytes", size * num);
    }
    return mi_calloc(num, size);
}


} // namespace ant
