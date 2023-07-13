#pragma once

#include <memory_resource>
#if 0
#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>
//#define aligned_alloc(alignment, size) mi_aligned_alloc(alignment, size)
#define aligned_free(ptr) free(ptr)

#else
#include <cstdlib>

#ifdef _MSC_VER
#define aligned_alloc(alignment, size) _aligned_malloc(size, alignment)
#define aligned_free(ptr) _aligned_free(ptr)
#else
// Code for other compilers
#endif
#endif

#include "framework/01core/utils/utils.h"

namespace ant::memory {

extern bool b_enable_memory_tracking;

using memory_pool = std::pmr::memory_resource;

class allocator_base : public memory_pool {
  public:
    allocator_base() noexcept {};
    ~allocator_base() noexcept = default;

  protected:
    virtual void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) = 0;
    virtual void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) = 0;
    virtual bool do_is_equal(const std::pmr::memory_resource &other) const noexcept = 0;
    //virtual void init() = 0;
    // virtual void reset() = 0;

    u64 m_size = 0;
};

} // namespace ant::memory
