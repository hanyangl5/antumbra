#pragma once

#include "allocator2.h"

namespace ant::memory {

class stack_allocator final : public allocator_base {
  public:
    stack_allocator(u64 pool_size) noexcept;
    ~stack_allocator() noexcept;
    virtual void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) = 0;
    virtual void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) = 0;
    virtual bool do_is_equal(const std::pmr::memory_resource &other) const noexcept = 0;
    void init(u64 pool_size);
    void reset();
  private:
    void *m_ptr = nullptr;
    u64 m_offset = 0;
};

} // namespace ant::memory
