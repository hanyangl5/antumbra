#pragma once

#include "allocator2.h"

namespace ant::memory {

// The stack_allocator class provides a memory allocation strategy that is based on a stack.
// It allocates memory in a last-in, first-out (LIFO) order. This means that the last block of memory
// allocated is the first one to be deallocated, making it an efficient memory allocation strategy for
// certain types of data and algorithms.

class stack_allocator final : public allocator_base {
  public:
    stack_allocator(u64 pool_size, u64 alignment = alignof(std::max_align_t)) noexcept;
    virtual ~stack_allocator() noexcept;
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
    void reset();
    void resize(u64 size, u64 alignment);

  private:
    struct stack_allocator_header {
        u64 pad;
        u64 prev_offset;
    };

    void *m_ptr = nullptr;
    u64 m_offset = 0;
    u64 m_prev_offset;

  public:
    static constexpr u32 header_size = sizeof(stack_allocator_header);
};

} // namespace ant::memory
