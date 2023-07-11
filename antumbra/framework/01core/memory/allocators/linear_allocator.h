#pragma once

#include "allocator2.h"

namespace ant::memory {

class linear_allocator final : public allocator_base {
  public:
    linear_allocator(u64 pool_size, u64 alignment = alignof(std::max_align_t)) noexcept;
    ~linear_allocator() noexcept;
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
    void reset();
    void resize(u64 size, u64 alignment);
  private:
    void *m_ptr = nullptr;
    u64 m_offset = 0;
};

} // namespace ant::memory
