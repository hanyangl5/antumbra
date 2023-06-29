#pragma once

// standard libraries
#include <memory_resource>
#include <type_traits>
// third party libraries

// project headers
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/allocator.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/singleton/public_singleton.h"
#include "framework/01core/utils/utils.h"

namespace ant::memory {

extern bool b_enable_memory_tracking;

inline void ant_enable_memory_tracking() { b_enable_memory_tracking = true; }
inline void ant_disable_memory_tracking() { b_enable_memory_tracking = false; }

void *amalloc(u64 size, std::pmr::memory_resource *allocator);

template <typename T = void, typename... Args> T *alloc(Args &&...args, std::pmr::memory_resource *allocator) {
    void *memory = allocator->allocate(sizeof(T));
    return new (memory) T(std::forward<Args>(args)...);
}

template <typename T> void afree(T *ptr, std::pmr::memory_resource *allocator) {
    if (!ptr) {
        return;
    }
    ptr->~T();
    allocator->deallocate(ptr, 0);
}

// TODO(hyl5): memory pool
// each allocations are associated to a pool, pool defines the budgets of each system(e.g. render, physics).
//#define ANT_MEMORY_POOL(name, allocatorTpe, budget) allocatorTpe name(budget);

template <typename T>
constexpr memory_pool *create_memory_pool(const char *pool_name, u64 budget, memory_pool *parent) {
    return new T(budget, pool_name, parent);
}

void destroy_memory_pool(memory_pool *pool) { delete pool; }

//template <class T> class uniqptr { // non-copyable T* to an object
//  public:
//    using namespace std;
//
//    constexpr uniqptr(T *rhs, memory_pool *allocator) noexcept : m_resource(rhs), m_allocator(allocator) = default;
//    constexpr uniqptr() noexcept = default;
//    uniqptr &operator=(nullptr_t) noexcept {
//        reset();
//        return *this;
//    }
//    uniqptr(uniqptr &&_Right) noexcept : { 
//        m_allocator = _Right.m_allocator;
//        m_resource = _Right.release(); }
//
//    uniqptr(uniqptr<T2, _Dx2> &&_Right) noexcept
//        : _Mypair(_One_then_variadic_args_t{}, _STD forward<_Dx2>(_Right.get_deleter()), _Right.release()) {}
//
//    uniqptr &operator=(uniqptr<T2> &&_Right) noexcept {
//        reset(_Right.release());
//        _Mypair._Get_first() = _STD forward<_Dx2>(_Right._Mypair._Get_first());
//        return *this;
//    }
//
//    uniqptr &operator=(uniqptr &&_Right) noexcept {
//        if (this != std::addressof(_Right)) {
//            reset(_Right.release());
//
//            _Mypair._Get_first() = _STD forward<_Dx>(_Right._Mypair._Get_first());
//        }
//        return *this;
//    }
//
//    void swap(uniqptr &_Right) noexcept {
//        std::exchange(m_resource, _Right.m_resource);
//        std::exchange(m_allocator, _Right.m_allocator);
//        _Swap_adl(_Mypair._Myval2, _Right._Mypair._Myval2);
//        _Swap_adl(_Mypair._Get_first(), _Right._Mypair._Get_first());
//    }
//
//    ~uniqptr() noexcept {
//        if (_Mypair._Myval2) {
//            _Mypair._Get_first()(_Mypair._Myval2);
//        }
//    }
//
//    T *operator->() const noexcept { return m_resource; }
//
//    T *get() const noexcept { return m_resource; }
//
//    T *release() noexcept { return std::exchange(m_resource, nullptr); }
//
//    void reset(T *_Ptr = nullptr) noexcept {
//        T *_Old = std::exchange(_Mypair._Myval2, _Ptr);
//        if (_Old) {
//            _Mypair._Get_first()(_Old);
//        }
//    }
//
//    uniqptr(const uniqptr &) = delete;
//    uniqptr &operator=(const uniqptr &) = delete;
//
//  private:
//    T *m_resource = nullptr;
//    memory_pool *m_allocator = nullptr;
//};
//
//template <class T, class... Tpes, std::enable_if_t<!std::is_array_v<T>, int> = 0>
//uniqptr<T> make_uniq(Tpes &&..._Args, memory_pool *allocator) { // make a unique_ptr
//    return uniqptr<T>(alloc(std::forward<Tpes>(_Args)..., allocator));
//}
//
//// smart pointer with ref counting
//template<typename T> class ref_ptr {
//
//};

//ANT_MEMORY_POOL(good, default_allocator, 1024);
} // namespace ant::memory