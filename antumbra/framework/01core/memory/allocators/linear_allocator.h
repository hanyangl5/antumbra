#pragma once

#include "allocator2.h"

namespace ante::memory {

// The Linear Allocator is an efficient memory management tool characterized by its simplicity and speed.
// It operates by incrementing a pointer within a pre-allocated memory block during allocation.
// Upon allocation, it returns the current pointer position and subsequently increments the pointer by the size of the requested allocation.
// Memory deallocation does not immediately free up memory; instead, it waits until all memory is deallocated simultaneously.
// This makes the Linear Allocator highly efficient in scenarios where memory allocation and deallocation follow a predictable pattern.
// However, it is not recommended for scenarios requiring arbitrary order memory deallocation.

class linear_allocator final : public allocator_base {
  public:
    linear_allocator(u64 pool_size, u64 alignment = alignof(std::max_align_t)) noexcept;
    virtual ~linear_allocator() noexcept;
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    bool do_is_equal(const ante_pmr::memory_resource &other) const noexcept override;
    void reset();
    void resize(u64 size, u64 alignment);

  private:
    void *m_ptr = nullptr;
    u64 m_offset = 0;
};

} // namespace ante::memory
