#pragma once

// standard libraries
#include <atomic>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <type_traits>
#ifdef _MSC_VER 
#include <utility>
#elif __clang__
#include <functional> // std::hash is defined in functional in clang
#endif
// third party libraries

// project headers

#define DELETE_COPY_MOVE(class_name)                                                                                   \
    class_name(const class_name &) = delete;                                                                         \
    class_name(class_name &&) = delete;                                                                              \
    class_name &operator=(const class_name &) = delete;                                                                \
    class_name &operator=(class_name &&) = delete;

#define DEFAULT_COPY_MOVE(class_name)                                                                                  \
    class_name(const class_name &) = default;                                                                        \
    class_name(class_name &&) = default;                                                                             \
    class_name &operator=(const class_name &) = default;                                                               \
    class_name &operator=(class_name &&) = default;

namespace ant {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
#ifdef __APPLE__
// apple define size_t as unsigned long
using u64 = std::size_t;
#else
using u64 = uint64_t;
#endif
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

// add nodiscard for lambda function
// template<typename F> struct NoDiscard
//{
//    F f;
//    NoDiscard(F const &f) : f(f) {}
//    template<typename... T>
//    [[nodiscard]] constexpr auto operator()(T &&...t) const noexcept(noexcept(f(std::forward<T>(t)...)))
//    {
//        return f(std::forward<T>(t)...);
//    }
//};

// [major8, minor8, patch16]
constexpr u32 make_version(u32 major, u32 minor, u32 patch) {
    // 8 bit major, 8 bit minor, 16bit patch
    u32 _major = major << 24;
    u32 _minor = minor << 16;
    u32 _patch = patch;
    return _major | _minor | _patch;
}

template <typename T> void hash_combine(u64 &seed, const T &v) {
    std::hash<T> hasher{};
    u64 hash = hasher(v);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}

//// require c++20
// ant::str to_string(std::filesystem::file_time_type const &ftime) {
//     std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(ftime));
//     ant::str str = std::asctime(std::localtime(&cftime));
//     str.pop_back(); // rm the trailing '\n' put by `asctime`
//     return str;
// }

template <typename TP> std::time_t to_time_t(TP tp) {
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

struct blob {
  public:
    inline void set(void *data, u64 size) {
        m_data = data;
        m_size = size;
    }
    inline void *data() const { return m_data; }
    inline u64 size() const { return m_size; }
    inline void release();
    inline void reset() {
        m_data = nullptr;
        m_size = 0;
    }

  private:
    void *m_data = nullptr;
    u64 m_size = 0;
};

// bytes
constexpr u64 operator"" _b(unsigned long long value) { return static_cast<u64>(value); }
// kilo bytes
constexpr u64 operator"" _kb(unsigned long long value) { return static_cast<u64>(value * 1024); }
// mega bytse
constexpr u64 operator"" _mb(unsigned long long value) { return static_cast<u64>(value * 1024 * 1024); }

template <typename T> T align_up(T a, T size) { return ((a + size - 1) & (~(size - 1))); }
} // namespace ant