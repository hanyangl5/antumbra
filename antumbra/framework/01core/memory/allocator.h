#pragma once

#include <memory_resource>

#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>

#include "framework/01core/utils/utils.h"

namespace ant::memory {

using memory_pool = std::pmr::memory_resource;
//
//class default_pool final : public memory_pool{public :
//    default_pool(,memory_pool *upstream);
//
//};

// Building Night City: The Technology Of CyberPunk 2077
// https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/

// slab for <= 512b
// tlsf for 512b to 512kb
// bigsize for >512kb

class global_allocator final : public std::pmr::memory_resource {
  public:
    global_allocator() noexcept {};
    ~global_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
};

class default_allocator2 final : public std::pmr::memory_resource {
  public:
    default_allocator2(u64 budget, const char *name = "anonymous allocator") noexcept
        : m_budget(budget), m_remain(0), m_name(name){

                                         };
    ~default_allocator2() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    slab_allocator *m_slab_memory_allocator = nullptr;
    tlsf_allocator *m_tlsf_memory_allocator = nullptr;

    u64 m_budget;
    u64 m_remain;
    const char *m_name;
};

class slab_allocator final : public std::pmr::memory_resource {
  public:
    slab_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~slab_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

class tlsf_allocator final : public std::pmr::memory_resource {
  public:
    tlsf_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~tlsf_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

class fixedsize_allocator final : public std::pmr::memory_resource {
  public:
    fixedsize_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~fixedsize_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

class linear_allocator final : public std::pmr::memory_resource {
  public:
    linear_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~linear_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

class buddy_allocator final : public std::pmr::memory_resource {
  public:
    buddy_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~buddy_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

class stack_allocator final : public std::pmr::memory_resource {
  public:
    stack_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~stack_allocator() noexcept = default;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

class frame_allocator : public std::pmr::memory_resource {
  public:
    frame_allocator(u64 budget) noexcept : m_budget(budget), m_remain(0){};
    ~frame_allocator() noexcept;

  protected:
    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

  private:
    u64 m_budget = 0;
    u64 m_remain = 0;
};

// name, bytes, upstream
#define ACQUIRE_STACK_MEMORY_RESOURCE(name, bytes, pool)                                                                     \
    char __stack_memory__[bytes];                                                                                      \
    std::pmr::monotonic_buffer_resource name((void *)__stack_memory__, bytes, pool);
//
//class stack_memory_resource : public std::pmr::memory_resource {
//  public:
//    stack_memory_resource(u64 size) noexcept
//        : m_budget(size), m_remain(0), m_resource(std::pmr::monotonic_buffer_resource(size)){};
//    ~stack_memory_resource() noexcept { m_resource.release(); };
//
//  protected:
//    void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
//
//    void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
//
//    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
//
//  protected:
//    u64 m_budget = 0;
//    u64 m_remain = 0;
//    std::pmr::monotonic_buffer_resource m_resource;
//};

//Block Allocator
//

} // namespace ant::memory
