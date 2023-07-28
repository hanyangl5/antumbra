#pragma once

// standard libraries
#include <memory_resource>

// third party libraries

// project headers
#include "framework/01core/utils/utils.h"
#include "framework/02gal/gal.h"

namespace ante::gal {

#define D3D12_FUNCTION_DECLARE
#include "../helper/helper_macro.h"
#undef D3D12_FUNCTION_DECLARE

} // namespace ante::gal
