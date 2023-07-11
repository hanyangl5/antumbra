#include "pool_allocator.h"

#include "framework/01core/logging/log.h"

namespace ant::memory {

pool_allocator::pool_allocator(u64 pool_size, u64 chunk_size, u64 alignment) noexcept {

    u64 size = align_up(pool_size, alignment);
    m_size = size;
    m_ptr = mi_aligned_alloc(alignment, size);

    m_chunk_size = align_up(chunk_size, alignment);
    m_chunk_count = m_size / m_chunk_size;
    m_head = nullptr;
    // Set all chunks to be free
    for (u64 i = 0; i < m_chunk_count; i++) {
        void *ptr = (char *)m_ptr + i * m_chunk_size;
        pool_allocator_node *node = (pool_allocator_node *)ptr;
        // Push free node onto thte free list
        node->next = m_head;
        m_head = node;
    }
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: pool allocator initilized at {}, size {}", m_ptr, m_size);
    }
};

pool_allocator::~pool_allocator() noexcept {
    free(m_ptr);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: pool allocator destroyed at {}, size {}", m_ptr, m_size);
    }
    m_ptr = nullptr;
};

void pool_allocator::free_all() {
    // Set all chunks to be free
    for (u64 i = 0; i < m_chunk_count; i++) {
        void *ptr = (char *)m_ptr + i * m_chunk_size;
        pool_allocator_node *node = (pool_allocator_node *)ptr;
        // Push free node onto thte free list
        node->next = m_head;
        m_head = node;
    }
}

void pool_allocator::resize(u64 size, u64 alignment) {
    u64 new_size = align_up(size, alignment);
    void *new_ptr = mi_aligned_alloc(alignment, new_size);
    memcpy(new_ptr, m_ptr, m_size);
    free(m_ptr);
    m_ptr = new_ptr;
    m_size = new_size;
}

void *pool_allocator::do_allocate([[maybe_unused]] u64 bytes, [[maybe_unused]] u64 alignment) {
    // return current ptr
    if (m_head == nullptr) {
        // error
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]:  pool allocator overflow");
        }
        return nullptr;
    }
    void *dst = m_head;
    m_head = m_head->next;

    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: alloc {} bytes to {}", m_chunk_size, dst);
    }

    return dst;
}
void pool_allocator::do_deallocate(void *ptr, [[maybe_unused]] u64 bytes, [[maybe_unused]] u64 alignment) {

    if (ptr == nullptr) {
        // Ignore NULL pointers
        return;
    }

    if (!(m_ptr <= ptr && ptr < (char *)m_ptr + m_size)) {
        if (b_enable_memory_tracking) {
            LOG_ERROR("[memory]: Memory is out of bounds of the buffer in this pool");
        }
        return;
    }
    pool_allocator_node *node;

    // Push free node
    node = (pool_allocator_node *)ptr;
    node->next = m_head;
    m_head = node;

    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: dealloc {} bytes from {} ", m_chunk_size, ptr);
    }
}
bool pool_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

} // namespace ant::memory