#pragma once

#include "allocator2.h"

namespace ant::memory {

// The pool_allocator is a memory management object that allocates memory in chunks from a predefined memory pool.
// It is designed to be efficient in situations where the same size of memory block is frequently allocated and deallocated.

class pool_allocator final : public allocator_base {
  public:
    pool_allocator(u64 pool_size, u64 chunk_size, u64 alignment = alignof(std::max_align_t)) noexcept;
    ~pool_allocator() noexcept;
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
    void reset();
    void resize(u64 pool_size, u64 alignment);
    void free_all();

  private:
    struct pool_allocator_node {
        pool_allocator_node *next;
    };

    pool_allocator_node *m_head;
    u64 m_chunk_size;
    u64 m_chunk_count;

    void *m_ptr = nullptr;
    // u64 m_offset = 0;
};

} // namespace ant::memory
