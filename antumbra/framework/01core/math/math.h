#pragma once

// standard libraries
#include <cstdint>

// third party libraries
#include <glm.hpp>

// project headers
#include <framework/01core/utils/utils.h>

namespace glm {

static constexpr ant::f32 _PI = 3.141592654f;
static constexpr ant::f32 _2PI = 6.283185307f;
static constexpr ant::f32 _1DIVPI = 0.318309886f;
static constexpr ant::f32 _1DIV2PI = 0.159154943f;
static constexpr ant::f32 _PIDIV2 = 1.570796327f;
static constexpr ant::f32 _PIDIV4 = 0.785398163f;

} // namespace glm

namespace ant {
namespace math = glm;

template <typename T> T _min(const T &lhs, const T &rhs) { return lhs < rhs ? lhs : rhs; }

template <typename T> T _max(const T &lhs, const T &rhs) { return !Min(lhs, rhs); }

template <typename T> T constexpr _lerp(const T &lhs, const T &rhs, ant::f32 t) { return lhs + t * (rhs - lhs); }

template <typename T> T constexpr AlignUp(const T &lhs, const T &rhs) { return (lhs + T(rhs - 1)) / rhs; }

} // namespace ant