#pragma once

// standard libraries
#if defined(__APPLE__)
#include <boost/container/pmr/memory_resource.hpp>
#else
#include <memory_resource>
#endif
// third party libraries

// project headers
#include "framework/utils/types/types.h"
#include "framework/gal/gal.h"

namespace ante::gal {

#define D3D12_FUNCTION_DECLARE
#include "../helper/helper_macro.h"
#undef D3D12_FUNCTION_DECLARE

} // namespace ante::gal
