#pragma once

#include <memory_resource>

#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>

#include "framework/01core/utils/utils.h"

namespace ant::memory {

extern bool b_enable_memory_tracking;

using memory_pool = std::pmr::memory_resource;

// Building Night City: The Technology Of CyberPunk 2077
// https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/

// slab for <= 512b
// tlsf for 512b to 512kb
// bigsize for >512kb

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

    u64 m_size;
};

} // namespace ant::memory
