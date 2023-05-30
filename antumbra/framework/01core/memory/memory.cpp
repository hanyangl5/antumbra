#include "memory.h"

namespace ant {
bool b_enable_memory_tracking = false;

void *ant_malloc(u64 size) {
    if (b_enable_memory_tracking) {
        LOG_DEBUG("alloc {} bytes", size);
    }
    return mi_malloc(size);
}

} // namespace ant
