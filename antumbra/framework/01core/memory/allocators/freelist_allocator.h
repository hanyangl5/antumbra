#pragma once

#include "allocator2.h"

namespace ante::memory {

enum class freelist_allocator_mode { LINK_LIST, RB_TREE };

class freelist_allocator final : public allocator_base {
  public:
    freelist_allocator(freelist_allocator_mode mode, u64 pool_size, u64 alignment = alignof(std::max_align_t)) noexcept;
    virtual ~freelist_allocator() noexcept;
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
    void reset();
    void resize(u64 size, u64 alignment);
    void free_all();

  private:
    struct freelist_allocator_node {
        freelist_allocator_node *next;
    };

    freelist_allocator_node *m_head;


    void *m_ptr = nullptr;
    u64 m_offset = 0;
};

} // namespace ante::memory
