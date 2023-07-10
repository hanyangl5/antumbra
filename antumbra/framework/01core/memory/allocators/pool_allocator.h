#pragma once

#include "allocator2.h"

namespace ant::memory {

class pool_allocator final : public allocator_base {
  public:
    pool_allocator(u64 pool_size, u64 chunk_size, u64 alignment = alignof(std::max_align_t)) noexcept;
    ~pool_allocator() noexcept;
    virtual void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) = 0;
    virtual void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) = 0;
    virtual bool do_is_equal(const std::pmr::memory_resource &other) const noexcept = 0;
    void reset();
    void resize(u64 size, u64 alignment);

  private:
    struct pool_allocator_node {
        pool_allocator_node *next;
    };
    u64 chunk_size;
    void *m_ptr = nullptr;
    u64 m_offset = 0;
};

} // namespace ant::memory
