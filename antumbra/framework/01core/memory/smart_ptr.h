#pragma once

#include "memory.h"
#include <type_traits>
namespace ant::memory {

template <typename T> class unique_ptr;

template <class _Ty, class... _Types, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
constexpr unique_ptr<_Ty> make_unique(_Types &&..._Args, memory_pool *pool = nullptr) { // make a unique_ptr
    return unique_ptr<_Ty>(ant::memory::alloc<_Ty>(std::forward<_Types>(_Args)..., pool), pool);
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
    unique_ptr() : {}
    // Explicit constructor
    explicit unique_ptr(T *data, memory_pool *pool) : data(data), allocator(pool) {}
    ~unique_ptr() { afree(data, allocator); }

    // Constructor/Assignment that binds to nullptr
    // This makes usage with nullptr cleaner
    unique_ptr(std::nullptr_t) : data(nullptr) {}
    unique_ptr &operator=(std::nullptr_t) {
        reset();
        return *this;
    }

    // Constructor/Assignment that allows move semantics
    unique_ptr(unique_ptr &&moving) noexcept : data(nullptr) {
        moving.swap(*this);
        // In the comments it was pointed out that this
        // does not match the implementation of std::unique_ptr
        // I am going to leave mine the same. But
        // the the standard provides some extra guarantees
        // and probably a more intuitive usage.
    }
    unique_ptr &operator=(unique_ptr &&moving) noexcept {
        moving.swap(*this);
        return *this;
        // See move constructor.
    }

    // Constructor/Assignment for use with types derived from T
    template <typename U> unique_ptr(unique_ptr<U> &&moving) {
        unique_ptr<T> tmp(moving.release());
        tmp.swap(*this);
    }
    template <typename U> unique_ptr &operator=(unique_ptr<U> &&moving) {
        unique_ptr<T> tmp(moving.release());
        tmp.swap(*this);
        return *this;
    }

    // Remove compiler generated copy semantics.
    unique_ptr(unique_ptr const &) = delete;
    unique_ptr &operator=(unique_ptr const &) = delete;

    // Const correct access owned object
    T *operator->() const { return data; }
    T &operator*() const { return *data; }

    // Access to smart pointer state
    T *get() const { return data; }
    explicit operator bool() const { return data; }

    void swap(unique_ptr &_Right) noexcept { std::swap(data, _Right.data); }

    // Modify object state
    T *release() noexcept {
        T *result = nullptr;
        std::swap(result, data);
        return result;
    }
};

} // namespace ant::memory