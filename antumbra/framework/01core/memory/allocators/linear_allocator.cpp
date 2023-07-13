#include "linear_allocator.h"

#include "framework/01core/logging/log.h"
#include "framework/01core/math/math.h"

namespace ant::memory {

linear_allocator::linear_allocator(u64 pool_size, u64 alignment) noexcept {
    u64 size = align_up(pool_size, alignment);
    m_size = size;
    m_ptr = aligned_alloc(alignment, size);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: linear allocator initilized at {}, size {}", m_ptr, m_size);
    }
};

linear_allocator::~linear_allocator() noexcept {
    aligned_free(m_ptr);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: linear allocator destroyed at {}, size {}", m_ptr, m_size);
    }
    m_ptr = nullptr;
};

void linear_allocator::reset() { m_offset = 0; }

void linear_allocator::resize(u64 size, u64 alignment) {
    u64 new_size = align_up(size, alignment);
    void *new_ptr = aligned_alloc(alignment, new_size);
    if (new_size < m_size) {
        if (b_enable_memory_tracking) {
            LOG_WARN("[memory]: new size of stack allocator is smaller than the old one");
        }
    }
    memcpy(new_ptr, m_ptr, _min(m_size, new_size));
    aligned_free(m_ptr);
    m_ptr = new_ptr;
    m_size = new_size;
}

void *linear_allocator::do_allocate(u64 bytes, u64 alignment) {
    // return current ptr
    void *dst = (char *)m_ptr + m_offset;
    // offset bytes with alignment
    u64 offset = align_up(bytes, alignment);
    if (m_offset + offset > m_size) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: allocator overflow, expect: {}, max: {}", m_offset + offset, m_size);
        }
        return nullptr;
    }
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: alloc {} bytes to {} with alignment {}", bytes, dst, alignment);
    }
    m_offset += offset;
    return dst;
}
void linear_allocator::do_deallocate([[maybe_unused]] void *ptr, [[maybe_unused]] u64 bytes,
                                     [[maybe_unused]] u64 alignment) {}
bool linear_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

} // namespace ant::memory