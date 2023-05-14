#include "gal.h"

#define DEFINE_GAL_FUNCTION(name) gal_pfn_##name name;

namespace ant::gal {

extern gal_error_code vk_init_gal(gal_context *context);

#ifdef WIN32
extern gal_error_code d3d12_init_gal(gal_context *context);
#endif//  WIN32

[[nodiscard]] gal_error_code init_gal(RenderApi api, gal_context *context)
{
    switch (api) {
    case ant::gal::RenderApi::invalid:
        break;
    case ant::gal::RenderApi::vulkan:
        return vk_init_gal(context);
        break;
    case ant::gal::RenderApi::d3d12:
#ifdef WIN32
        return d3d12_init_gal(context);
#else
        return gal_error_code::unsupported_platform;
#endif
        break;
    default:
        break;
    }

    return gal_error_code();
}

DEFINE_GAL_FUNCTION(create_instance);
DEFINE_GAL_FUNCTION(destroy_instane);

DEFINE_GAL_FUNCTION(select_gpu);
DEFINE_GAL_FUNCTION(create_device);
DEFINE_GAL_FUNCTION(destroy_device);

DEFINE_GAL_FUNCTION(create_buffer);

DEFINE_GAL_FUNCTION(destroy_buffer);

DEFINE_GAL_FUNCTION(create_texture);

DEFINE_GAL_FUNCTION(destroy_texture);

DEFINE_GAL_FUNCTION(create_sampler);

DEFINE_GAL_FUNCTION(destroy_sampler);

DEFINE_GAL_FUNCTION(create_swap_chain);

DEFINE_GAL_FUNCTION(destroy_swap_chain);

DEFINE_GAL_FUNCTION(create_shader);

DEFINE_GAL_FUNCTION(destroy_shader);

DEFINE_GAL_FUNCTION(create_pipeline);

DEFINE_GAL_FUNCTION(destroy_pipeline);

DEFINE_GAL_FUNCTION(create_surface);

DEFINE_GAL_FUNCTION(destroy_surface);

// cmds
DEFINE_GAL_FUNCTION(cmd_begin_recording);
DEFINE_GAL_FUNCTION(cmd_end_recording);
DEFINE_GAL_FUNCTION(cmd_bind_descriptor_set);
DEFINE_GAL_FUNCTION(cmd_bind_index_buffer);

}// namespace ant::gal
