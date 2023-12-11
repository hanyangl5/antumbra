#pragma once

// standard libraries
// third party libraries
#include <glm.hpp>

// project headers
#include "framework/utils/types/types.h"

namespace glm {

static constexpr ante::f32 ANTE_PI = 3.141592654f;
static constexpr ante::f32 ANTE_2PI = 6.283185307f;
static constexpr ante::f32 ANTE_1DIVPI = 0.318309886f;
static constexpr ante::f32 ANTE_1DIV2PI = 0.159154943f;
static constexpr ante::f32 ANTE_PIDIV2 = 1.570796327f;
static constexpr ante::f32 ANTE_PIDIV4 = 0.785398163f;

} // namespace glm

namespace ante {
namespace math = glm;

template <typename T> T _min(const T &lhs, const T &rhs) { return lhs < rhs ? lhs : rhs; }

template <typename T> T _max(const T &lhs, const T &rhs) { return lhs > rhs ? lhs : rhs; }

template <typename T> T constexpr _lerp(const T &lhs, const T &rhs, ante::f32 t) { return lhs + t * (rhs - lhs); }

template <typename T> T constexpr AlignUp(const T &lhs, const T &rhs) { return (lhs + T(rhs - 1)) / rhs; }

} // namespace ante