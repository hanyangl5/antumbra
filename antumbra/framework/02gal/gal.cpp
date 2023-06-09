#include "gal.h"

#define DEFINE_GAL_FUNCTION(name) gal_pfn_##name name;

namespace ant::gal {

extern gal_error_code vk_init_gal(gal_context *_context);

#ifdef WIN32
extern gal_error_code d3d12_init_gal(gal_context *_context);
#endif //  WIN32

[[nodiscard]] gal_error_code init_gal(gal_api api, gal_context *_context) {
    switch (api) {
    case ant::gal::gal_api::UNDEFINED:
        break;
    case ant::gal::gal_api::VULKAN:
        return vk_init_gal(_context);
        break;
    case ant::gal::gal_api::D3D12:
#ifdef WIN32
        //return d3d12_init_gal(_context);
#else
        return gal_error_code::unsupported_platform;
#endif
        break;
    default:
        break;
    }

    return gal_error_code::GAL_ERRORCODE_SUCCESS;
}

#define GAL_FUNCTION_DEFINATION
#include "helper/helper_macro.h"
#undef GAL_FUNCTION_DEFINATION
} // namespace ant::gal
