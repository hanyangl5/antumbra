#include "allocator.h"

#include <memory_resource>

#include "framework/01core/logging/log.h"

namespace ant::memory {

bool b_enable_memory_tracking = true;
//
//default_allocator *default_memory_allocator = nullptr;
//slab_allocator *slab_memory_allocator = nullptr;
//tlsf_allocator *tlsf_memory_allocator = nullptr;

void *stack_memory_resource::do_allocate(u64 bytes, u64 alignment) {
    void *memory = m_resource.allocate(bytes, alignment);
    if (!memory) {
        LOG_ERROR("memory::default_allocator: failed to allocte memory, {} bytes", bytes);
        return nullptr;
    }
    if (b_enable_memory_tracking) {
        LOG_DEBUG("memory::stack_allocator: alloc {} bytes to {} with alignment {}", bytes, memory, alignment);
    }
    return memory;
}
void stack_memory_resource::do_deallocate(void *ptr, u64 bytes, u64 alignment) {
    m_resource.deallocate(ptr, bytes, alignment);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("memory::stack_allocator: free {} bytes from {} with alignment {}", bytes, (void *)ptr, alignment);
    }
}
bool stack_memory_resource::do_is_equal(const std::pmr::memory_resource &other) const noexcept {
    return this == &other;
}

void *default_allocator::do_allocate(u64 bytes, u64 alignment) {
    void *memory = aligned_alloc(alignment, bytes);
    if (!memory) {
        LOG_ERROR("memory::default_allocator: failed to allocte memory, {} bytes", bytes);
        return nullptr;
    }
    if (b_enable_memory_tracking) {
        // TODO(hyl5):replace with slab/tlsf allocator
        LOG_DEBUG("memory::default_allocator: alloc {} bytes to {} with alignment {}", bytes, memory, alignment);
    }
    return memory;
}

void default_allocator::do_deallocate(void *ptr, u64 bytes, u64 alignment) {
    //mi_free_aligned(ptr, alignment);

    if (b_enable_memory_tracking) {
        LOG_DEBUG("memory::default_allocator: free {} bytes from {} with alignment {}", bytes, (void *)ptr, alignment);
    }
    free(ptr);
    ptr = nullptr;
}

bool default_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

void *default_allocator2::do_allocate(u64 bytes, u64 alignment) {
    if (bytes <= 512) {
        // less than 512b
        return m_slab_memory_allocator->allocate(bytes, alignment);
    } else if (bytes <= 512_kb) {
        // 512b to 512kb
        return m_tlsf_memory_allocator->allocate(bytes, alignment);
    } else {
        void *memory = aligned_alloc(alignment, bytes);
        if (!memory) {
            LOG_ERROR("memory::default_allocator: failed to allocte memory, {} bytes", bytes);
            return nullptr;
        }

        if (b_enable_memory_tracking) {
            // TODO(hyl5):replace with slab/tlsf allocator
            LOG_DEBUG("memory::default_allocator: alloc {} bytes to {} with alignment {}", bytes, memory, alignment);
        }
        return memory;
    }
}
void default_allocator2::do_deallocate(void *ptr, u64 bytes, u64 alignment) {
    if (!ptr) {
        return;
    }
    if (bytes <= 512) {
        // less than 512b
        slab_memory_allocator->deallocate(ptr, bytes, alignment);
    } else if (bytes <= 512_kb) {
        // 512b to 512kb
        tlsf_memory_allocator->deallocate(ptr, bytes, alignment);
    } else {
        free(ptr);
        if (b_enable_memory_tracking) {
            // TODO(hyl5):replace with slab/tlsf allocator
            LOG_DEBUG("memory::default_allocator: free {} bytes from {} with alignment {}", bytes, ptr, alignment);
        }
    }
    ptr = nullptr;
}
bool default_allocator2::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

} // namespace ant::memory
