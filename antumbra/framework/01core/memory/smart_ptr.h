#pragma once

#include "memory.h"
#include <type_traits>
#include <utility>

namespace ante::memory {

template <typename T> class unique_ptr;

template <class _Ty, class... _Types, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
constexpr unique_ptr<_Ty> make_unique(_Types &&..._Args, memory_pool *pool = nullptr) { // make a unique_ptr
    return unique_ptr<_Ty>(ante::memory::alloc<_Ty>(std::forward<_Types>(_Args)..., pool), pool);
}

//template <class _Ty, std::enable_if_t<std::is_array_v<_Ty> && std::extent_v<_Ty> == 0, int> = 0>
//constexpr unique_ptr<_Ty> make_unique(const size_t _Size, memory_pool *pool = nullptr) { // make a unique_ptr
//    using _Elem = std::remove_extent_t<_Ty>;
//    return unique_ptr<_Ty>(new _Elem[_Size]());
//}

template <class _Ty, class... _Types, std::enable_if_t<std::extent_v<_Ty> != 0, int> = 0>
void make_unique(_Types &&...) = delete;

// a simple unique ptr implementation
// https://codereview.stackexchange.com/questions/163854/my-implementation-for-stdunique-ptr
template <typename T> class unique_ptr {
    T *data = nullptr;
    memory_pool *allocator = nullptr;

  public:
    unique_ptr() noexcept {}
    // Explicit constructor
    explicit unique_ptr(T *data, memory_pool *pool) noexcept : data(data), allocator(pool) {}
    ~unique_ptr() noexcept { afree(data, allocator); }

    unique_ptr(std::nullptr_t) noexcept {}
    unique_ptr &operator=(std::nullptr_t) noexcept {
        //reset(); // TODO(hyl5): complete reset
        return *this;
    }

    unique_ptr(unique_ptr &&rhs) noexcept { rhs.swap(*this); }
    unique_ptr &operator=(unique_ptr &&rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    template <typename U> unique_ptr(unique_ptr<U> &&rhs) {
        unique_ptr<T> tmp(rhs.release(), rhs.allocator);
        tmp.swap(*this);
    }
    template <typename U> unique_ptr &operator=(unique_ptr<U> &&rhs) {
        unique_ptr<T> tmp(rhs.release(), rhs.allocator);
        tmp.swap(*this);
        return *this;
    }

    unique_ptr(unique_ptr const &) = delete;
    unique_ptr &operator=(unique_ptr const &) = delete;

    T *operator->() const noexcept { return data; }
    T &operator*() const noexcept { return *data; }

    T *get() const noexcept { return data; }
    explicit operator bool() const noexcept { return data; }

    void swap(unique_ptr &rhs) noexcept {
        std::swap(data, rhs.data);
        std::swap(allocator, rhs.allocator);
    }

    // Modify object state
    T *release() noexcept {
        T *result = nullptr;
        std::swap(result, data);

        return result;
    }
};

} // namespace ante::memory