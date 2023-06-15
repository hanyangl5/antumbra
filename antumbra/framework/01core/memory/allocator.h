#pragma once

#include <memory_resource>

#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>

#include "framework/01core/utils/utils.h"

namespace ant {

// Building Night City: The Technology Of CyberPunk 2077
// https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/

// slab for <= 512b
// tlsf for 512b to 512kb
// bigsize for >512kb

class default_allocator : public std::pmr::memory_resource {
  public:
    default_allocator() noexcept = default;
    ~default_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class slab_allocator : public std::pmr::memory_resource {
  public:
    slab_allocator() noexcept = default;
    ~slab_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class tlsf_allocator : public std::pmr::memory_resource {
  public:
    tlsf_allocator() noexcept = default;
    ~tlsf_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class fixedsize_allocator : public std::pmr::memory_resource {
  public:
    fixedsize_allocator() noexcept = default;
    ~fixedsize_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class linear_allocator : public std::pmr::memory_resource {
  public:
    linear_allocator() noexcept = default;
    ~linear_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class buddy_allocator : public std::pmr::memory_resource {
  public:
    buddy_allocator() noexcept = default;
    ~buddy_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class stack_allocator : public std::pmr::memory_resource {
  public:
    stack_allocator(u64 size) noexcept : m_resource(std::pmr::monotonic_buffer_resource(size)){};
    ~stack_allocator() noexcept { m_resource.release(); };

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  protected:
    std::pmr::monotonic_buffer_resource m_resource;
};

class frame_allocator : public std::pmr::memory_resource {
  public:
    frame_allocator() noexcept;
    ~frame_allocator() noexcept;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

} // namespace ant
