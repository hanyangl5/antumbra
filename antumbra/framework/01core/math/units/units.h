
#pragma once

// standard libraries

// third party libraries

// project headers
#include "framework/01core/utils/utils.h"

namespace ante {

// length

f32 operator"" _cm(long double);

f32 operator"" _m(long double);

f32 operator"" _km(long double);

// luminousity

f32 operator"" _lux(long double);

f32 operator"" _lm(long double);

f32 operator"" _cd(long double);

// degree

f32 operator"" _rad(long double);

f32 operator"" _deg(long double);

} // namespace ante