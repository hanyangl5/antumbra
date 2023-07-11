#include "stack_allocator.h"

#include "framework/01core/logging/log.h"

namespace ant::memory {

u64 calc_padding_with_header(uintptr_t ptr, uintptr_t alignment, u64 header_size) {
    uintptr_t p, a, modulo, padding, needed_space;

    //assert(is_power_of_two(alignment));

    p = ptr;
    a = alignment;
    modulo = p & (a - 1); // (p % a) as it assumes alignment is a power of two

    padding = 0;
    needed_space = 0;

    if (modulo != 0) { // Same logic as 'align_forward'
        padding = a - modulo;
    }

    needed_space = (uintptr_t)header_size;

    if (padding < needed_space) {
        needed_space -= padding;

        if ((needed_space & (a - 1)) != 0) {
            padding += a * (1 + (needed_space / a));
        } else {
            padding += a * (needed_space / a);
        }
    }

    return (u64)padding;
}

stack_allocator::stack_allocator(u64 pool_size, u64 alignment) noexcept {
    u64 size = align_up(pool_size, alignment);
    m_size = size;
    m_ptr = mi_aligned_alloc(alignment, size);
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: stack allocator initilized at {}, size {}", m_ptr, m_size);
    }
};

stack_allocator::~stack_allocator() noexcept {
    free(m_ptr);
    m_ptr = nullptr;
    if (b_enable_memory_tracking) {
        LOG_DEBUG("[memory]: stack allocator destroyed at {}, size {}", m_ptr, m_size);
    }
};

void stack_allocator::reset() { m_offset = 0; }

void stack_allocator::resize(u64 size, u64 alignment) {
    u64 new_size = align_up(size, alignment);
    void *new_ptr = mi_aligned_alloc(alignment, new_size);
    memcpy(new_ptr, m_ptr, m_size);
    free(m_ptr);
    m_ptr = new_ptr;
    m_size = new_size;
}

void *stack_allocator::do_allocate(u64 bytes, u64 alignment) {

    if (alignment > 128) {
        // As the padding is 8 bits (1 byte), the largest alignment that can
        // be used is 128 bytes
        alignment = 128;
    }

    const u64 current_addr = (u64)m_ptr + m_offset;

    u64 padding = calc_padding_with_header(current_addr, alignment, sizeof(stack_allocator_header));

    if (m_offset + padding + bytes > m_size) {
        if (b_enable_memory_tracking) {
            LOG_DEBUG("[memory]: allocator overflow, expect: {}, max: {}", m_offset + padding + bytes, m_size);
        }
        return nullptr;
    }

    // header end
    const u64 next_addr = current_addr + padding;
    // header start
    const u64 header_addr = next_addr - sizeof(stack_allocator_header);
    //stack_allocator_header allocation_header{padding};
    //stack_allocator_header *header_ptr = (stack_allocator_header *)header_addr;
    //header_ptr = &allocation_header;
    ((stack_allocator_header *)header_addr)->pad = padding;

    ((stack_allocator_header *)header_addr)->prev_offset = m_prev_offset;
    m_prev_offset = m_offset;
    m_offset += padding;
    m_offset += bytes;

    if (b_enable_memory_tracking) {
        LOG_DEBUG("memory::stack_allocator: alloc {} bytes to {} with alignment {}", bytes, (void *)next_addr,
                  alignment);
    }
    return (void *)next_addr;
}

void stack_allocator::do_deallocate(void *ptr, [[maybe_unused]] u64 bytes, [[maybe_unused]] u64 alignment) {
    if (ptr != NULL) {
        uintptr_t start, end, curr_addr;
        stack_allocator_header *header;
        size_t prev_offset;

        start = (uintptr_t)m_ptr;
        end = start + (uintptr_t)m_size;
        curr_addr = (uintptr_t)ptr;

        // invalid ptr, out of bounds
        if (!(start <= curr_addr && curr_addr < end)) {
            if (b_enable_memory_tracking) {
                LOG_DEBUG("[memory]: invalid  memory address passed to stack allocator (free) {}", ptr);
            }
            return;
        }

        // not last allocation
        if (curr_addr >= start + (uintptr_t)m_offset) {
            if (b_enable_memory_tracking) {
                LOG_DEBUG("[memory]: invalid  memory address passed to stack allocator (free) {}", ptr);
            }
            // Allow double frees
            return;
        }

        header = (stack_allocator_header *)(curr_addr - sizeof(stack_allocator_header));
        prev_offset = (size_t)(curr_addr - (uintptr_t)header->pad - start);

        if (prev_offset != m_prev_offset) {
            if (b_enable_memory_tracking) {
                LOG_DEBUG("[memory]: invalid  memory address passed to stack allocator (free) {}", ptr);
            }
            return;
        }

        m_offset = prev_offset;
        m_prev_offset = header->prev_offset;
        if (b_enable_memory_tracking) {
            LOG_DEBUG("[memory]: stack allocator deallocate at {}", ptr);
        }
        ptr = nullptr;
    }
}
bool stack_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }

} // namespace ant::memory