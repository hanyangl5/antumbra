#include "linear_allocator.h"

#include "framework/01core/logging/log.h"

namespace ant::memory {

linear_allocator::linear_allocator(u64 pool_size) noexcept { init(pool_size); };

linear_allocator::~linear_allocator() noexcept {
    free(m_start_ptr);
    m_start_ptr = nullptr;
};

void linear_allocator::init(u64 pool_size) {
    m_totalSize = pool_size;
    m_start_ptr = malloc(pool_size);
}

void linear_allocator::reset() {
    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}

void *linear_allocator::do_allocate(u64 bytes, u64 alignment) {
    // return current ptr
    void *dst = (char *)m_start_ptr + m_offset;
    // offset bytes with alignment
    u64 offset = (bytes + alignment - 1) / alignment * alignment;
    if (m_offset + offset > m_totalSize) {
        if (b_enable_memory_tracking) {
            LOG_DEBUG("[memory]: allocator overflow, expect: {}, max: {}", m_offset + offset, m_totalSize);
        }
        return nullptr;
    }
    if (b_enable_memory_tracking) {
        LOG_DEBUG("memory::stack_allocator: alloc {} bytes to {} with alignment {}", bytes, dst, alignment);
    }
    m_offset += offset;
    return dst;
}
void linear_allocator::do_deallocate([[maybe_unused]] void *ptr, [[maybe_unused]] u64 bytes,
                                     [[maybe_unused]] u64 alignment) {}
bool linear_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

} // namespace ant::memory