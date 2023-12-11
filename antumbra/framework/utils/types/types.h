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

namespace ante {

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

} // namespace ante