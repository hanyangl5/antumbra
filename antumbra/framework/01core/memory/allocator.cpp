#include "allocator.h"

#include <memory_resource>

#include "framework/01core/logging/log.h"

namespace ant {

bool b_enable_memory_tracking = true;
namespace allocator {
default_allocator *default_memory_allocator = nullptr;
slab_allocator *slab_memory_allocator = nullptr;
tlsf_allocator *tlsf_memory_allocator = nullptr;
fixedsize_allocator *fixedsize_memory_allocator = nullptr;
} // namespace allocator
void *stack_allocator::do_allocate(u64 bytes, u64 alignment) {
    void *memory = m_resource.allocate(bytes, alignment);
    if (!memory) {
        LOG_ERROR("allocator::default_allocator: failed to allocte memory, {} bytes", bytes);
        return nullptr;
    }
    if (b_enable_memory_tracking) {
        LOG_DEBUG("allocator::stack_allocator: alloc {} bytes to {} with alignment {}", bytes, memory, alignment);
    }
    return memory;
}
void stack_allocator::do_deallocate(void *ptr, u64 bytes, u64 alignment) {
    m_resource.deallocate(ptr, bytes, alignment);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("allocator::stack_allocator: free {} bytes from {} with alignment {}", bytes, (void *)ptr, alignment);
    }
}
bool stack_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

void *default_allocator::do_allocate(u64 bytes, u64 alignment) {
    void *memory = mi_aligned_alloc(bytes, alignment);
    if (!memory) {
        LOG_ERROR("allocator::default_allocator: failed to allocte memory, {} bytes", bytes);
        return nullptr;
    }
    if (b_enable_memory_tracking) {
        // TODO(hyl5):replace with slab/tlsf allocator
        LOG_DEBUG("allocator::default_allocator: alloc {} bytes to {} with alignment {}", bytes, memory, alignment);
    }
    return memory;
}

void default_allocator::do_deallocate(void *ptr, u64 bytes, u64 alignment) {
    //mi_free_aligned(ptr, alignment);

    if (b_enable_memory_tracking) {
        LOG_DEBUG("allocator::default_allocator: free {} bytes from {} with alignment {}", bytes, (void *)ptr,
                  alignment);
    }
    mi_free_aligned(ptr, alignment);
}
bool default_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }
} // namespace ant
