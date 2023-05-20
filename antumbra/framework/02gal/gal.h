#pragma once

#include "framework/01core/memory/container.h"
#include "framework/01core/reflection/enum_reflection.h"
#include "framework/01core/utils/utils.h"
#include "format.h"
// enum included at last
#include "enum.h"

namespace ant::gal {

// intailize gal_context, load function pointer
[[nodiscard]] gal_error_code init_gal(RenderApi api, gal_context *_context);

#ifndef GAL_FUNCTION_DECLARE
#define GAL_FUNCTION_DECLARE
#include "helper/helper_macro.h"
#undef GAL_FUNCTION_DECLARE
#endif // !GAL_FUNCTION_DECLARE

} // namespace ant::gal
