#pragma once

#include <framework/01core/utils/utils.h>

#define DECLARE_GAL_FUNCTION(ret, name, ...)     \
    using gal_pfn_##name = ret (*)(__VA_ARGS__); \
    extern gal_pfn_##name name;

// #ifdef WIN32
// #def ine  DECLARE_GAL_RESOURCE(name) \
//    struct vk_##name;              \
//    struct d3d12_##name;           \
//    struct name                    \
//    {                              \
//        vk_##name *vk;             \
//        d3d12_##name *d3d12;       \
//    };
// #else
// #def ine  DECLARE_GAL_RESOURCE(name) \
//    struct vk_##name;              \
//    struct name                    \
//    {                              \
//        vk_##name *vk;             \
//    };
// #endif// WIN32

// TODO(hyl5): support multiple api?
#define DECLARE_GAL_RESOURCE(name) \
    struct name                    \
    {                              \
        void *handle;              \
    };

namespace ant::gal {
enum class gal_error_code : u32 { success, unsupporte_platform, error };

enum class RenderApi { invalid, vulkan, d3d12};

struct GalDesc
{
    // gal optional feature
    bool b_debug_layer : 1;
    bool b_async_compute : 1;
    bool b_async_transfer : 1;
    bool b_ray_tracing : 1;
    bool b_mesh_shader : 1;
    bool b_variable_rate_shading : 1;
    bool b_swap_chain : 1;
    bool b_surface : 1;
};

struct gal_context
{
    void *handle;
};

enum class QueueType{
    graphcis,
    compute,
    transfer
};

struct command_list
{
    void *handle;
};

[[nodiscard]] gal_error_code init_gal(RenderApi api, gal_context *context);

// TODO(hyl5): how to add [[nodiscard]] attribute to these function?
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_instance, GalDesc *desc, gal_context *context);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_instane, gal_context *context);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, select_gpu, GalDesc *desc, gal_context *context);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_device, GalDesc *gal_desc, gal_context *context);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_device, gal_context *context);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, init_memory_allocator, gal_context *context);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_buffer);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_buffer);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_texture);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_texture);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_sampler);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_sampler);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_swap_chain);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_swap_chain);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_shader);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_shader);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_pipeline);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_pipeline);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, create_surface);

DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, destroy_surface);

// cmds
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, cmd_begin_recording);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, cmd_end_recording);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, cmd_bind_descriptor_set);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, cmd_bind_index_buffer);
DECLARE_GAL_FUNCTION(ant::gal::gal_error_code, cmd_resource_barrier);

}// namespace ant::gal
