#pragma once

#include <framework/01core/utils/utils.h>

namespace ant::gal {
enum class gal_error_code : u32 { success, unsupporte_platform, error, unsupported_device, invalid_parameter };

enum class RenderApi { none, vulkan, d3d12 };

struct GalDesc {
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

enum class QueueType { none, graphcis, compute, transfer };

// Enum for descriptor types
enum resource_type : u32 {
    rt_none = 0,
    // constant buffer
    rt_constant_buffer = 1 << 0,
    // read write buffer
    rt_rw_buffer = 1 << 1,
    // read only texture
    rt_texture = 1 << 2,
    // read write texture
    rt_rw_texture = 1 << 3,
    rt_texture_cube = 1 << 4,
    rt_sampler = 1 << 5,
    rt_push_constant = 1 << 6,
    rt_vertex_buffer = 1 << 7,
    rt_index_buffer = 1 << 8,
    rt_indirect_buffer = 1 << 9,
    rt_color_rt = 1 << 10,
    rt_depth_stencil_rt = 1 << 11,
};

using resource_types = u32;

enum resource_state : u32 {
    rs_none = 0,
    // Your application should transition to this state only for accessing a resource across differentgraphics engine
    // types.
    rs_common = 1 << 0,
    rs_vertex_buffer = 1 << 1,
    rs_index_buffer = 1 << 2,
    rs_indiret_buffer = 1 << 3,
    // shader resource
    rs_constant_buffer = 1 << 4,
    rs_rw_buffer = 1 << 5,
    rs_texture = 1 << 6,
    rs_rw_texture = 1 << 7,
    rs_copy_src = 1 << 8,
    rs_copy_dst = 1 << 9,
    rs_cpu_read = 1 << 10,
    rs_cpu_write = 1 << 11,
    rs_render_target = 1 << 12,
    rs_present = 1 << 13,
    rs_depth_read = 1 << 14,
    rs_depth_write = 1 << 15,
    // rt_acce_structure,
    // shading_rate_src
};

using resource_states = u32;

enum class memory_flag {
    none,
    gpu_dedicated, // gpu dedicated memory
    cpu_upload,    // cpu upload memory
    gpu_download   // gpu download
};

// declare gal handle
struct gal_handle_T;
using gal_handle = gal_handle_T *;
constexpr gal_handle gal_null = nullptr;
using gal_context = gal_handle;
using commandlist = gal_handle;
using buffer = gal_handle;
using texture = gal_handle;
using sampler = gal_handle;
using rendertarget = gal_handle;
using fence = gal_handle;
using semaphore = gal_handle;
using swapchain = gal_handle;

enum buffer_creation_flag {
    /// Default flag (Buffer will use aliased memory, buffer will not be cpu accessible until mapBuffer is called)
    bcf_none = 0,
    /// Buffer will allocate its own memory (COMMITTED resource)
    bcf_own_memory = 1 << 0,
    /// Buffer will be persistently mapped
    bcf_persistent_map = 1 << 1,
    /// Use ESRAM to store this buffer
    bcf_esram = 1 << 2,
    /// Flag to specify not to allocate descriptors for the resource
    bcf_no_descriptor_view_creation = 1 << 3
};

using buffer_creation_flags = u32;
enum texture_creation_flag {
    none = 0,
    /// Texture will allocate its own memory (COMMITTED resource)
    TEXTURE_CREATION_FLAG_OWN_MEMORY_BIT = 0x01,
    /// Texture will be allocated in memory which can be shared among multiple processes
    /// Texture will be imported from a handle created in another process
    TEXTURE_CREATION_FLAG_IMPORT_BIT = 0x08,
    /// Use ESRAM to store this texture
    TEXTURE_CREATION_FLAG_ESRAM = 0x10,
    /// Use on-tile memory to store this texture
    TEXTURE_CREATION_FLAG_ON_TILE = 0x20,
    /// Prevent compression meta data from generating (XBox)
    TEXTURE_CREATION_FLAG_NO_COMPRESSION = 0x40,
    /// Force 2D instead of automatically determining dimension based on width, height, depth
    TEXTURE_CREATION_FLAG_FORCE_2D = 0x80,
    /// Force 3D instead of automatically determining dimension based on width, height, depth
    TEXTURE_CREATION_FLAG_FORCE_3D = 0x100,
    /// Display target
    TEXTURE_CREATION_FLAG_ALLOW_DISPLAY_TARGET = 0x200,
    /// Create an sRGB texture.
    TEXTURE_CREATION_FLAG_SRGB = 0x400,
    /// Create a normal map texture
    TEXTURE_CREATION_FLAG_NORMAL_MAP = 0x800,
    /// Fast clear
    TEXTURE_CREATION_FLAG_FAST_CLEAR = 0x1000,
    /// Fragment mask
    TEXTURE_CREATION_FLAG_FRAG_MASK = 0x2000,
};

struct buffer_desc {
    u64 size;                      // size
    resource_types resource_types; // descriptor types of buffer
    resource_states initial_state; // initial state of buffer
    memory_flag memory_flags;
    buffer_creation_flags flags;
};

// intailize gal_context, load function pointer
[[nodiscard]] gal_error_code init_gal(RenderApi api, gal_context *context);

#ifndef GAL_FUNCTION_DECLARE
#define GAL_FUNCTION_DECLARE
#include "helper/helper_macro.h"
#undef GAL_FUNCTION_DECLARE
#endif // !GAL_FUNCTION_DECLARE

} // namespace ant::gal
