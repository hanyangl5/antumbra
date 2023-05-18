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
enum gal_resource_type : u32 {
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

using gal_resource_types = u32;

enum gal_resource_state : u32 {
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

using gal_resource_states = u32;

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
using gal_commandlist = gal_handle;
using gal_buffer = gal_handle;
using gal_texture = gal_handle;
using gal_sampler = gal_handle;
using gal_rendertarget = gal_handle;
using gal_fence = gal_handle;
using gal_semaphore = gal_handle;
using gal_swapchain = gal_handle;

enum gal_buffer_flag {
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

using gal_buffer_flags = u32;

enum gal_texture_flag {
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

struct gal_buffer_desc {
    u64 size;                          // size
    gal_resource_types resource_types; // descriptor types of buffer
    gal_resource_states initial_state; // initial state of buffer
    memory_flag memory_flags;
    gal_buffer_flags flags;
};

// https://github.com/DeanoC/tiny_imageformat/blob/master/include/tiny_imageformat/tinyimageformat.h
enum class gal_texture_format {
    UNDEFINED = 0,
    R1_UNORM = 1,
    R2_UNORM = 2,
    R4_UNORM = 3,
    R4G4_UNORM = 4,
    G4R4_UNORM = 5,
    A8_UNORM = 6,
    R8_UNORM = 7,
    R8_SNORM = 8,
    R8_UINT = 9,
    R8_SINT = 10,
    R8_SRGB = 11,
    B2G3R3_UNORM = 12,
    R4G4B4A4_UNORM = 13,
    R4G4B4X4_UNORM = 14,
    B4G4R4A4_UNORM = 15,
    B4G4R4X4_UNORM = 16,
    A4R4G4B4_UNORM = 17,
    X4R4G4B4_UNORM = 18,
    A4B4G4R4_UNORM = 19,
    X4B4G4R4_UNORM = 20,
    R5G6B5_UNORM = 21,
    B5G6R5_UNORM = 22,
    R5G5B5A1_UNORM = 23,
    B5G5R5A1_UNORM = 24,
    A1B5G5R5_UNORM = 25,
    A1R5G5B5_UNORM = 26,
    R5G5B5X1_UNORM = 27,
    B5G5R5X1_UNORM = 28,
    X1R5G5B5_UNORM = 29,
    X1B5G5R5_UNORM = 30,
    B2G3R3A8_UNORM = 31,
    R8G8_UNORM = 32,
    R8G8_SNORM = 33,
    G8R8_UNORM = 34,
    G8R8_SNORM = 35,
    R8G8_UINT = 36,
    R8G8_SINT = 37,
    R8G8_SRGB = 38,
    R16_UNORM = 39,
    R16_SNORM = 40,
    R16_UINT = 41,
    R16_SINT = 42,
    R16_SFLOAT = 43,
    R16_SBFLOAT = 44,
    R8G8B8_UNORM = 45,
    R8G8B8_SNORM = 46,
    R8G8B8_UINT = 47,
    R8G8B8_SINT = 48,
    R8G8B8_SRGB = 49,
    B8G8R8_UNORM = 50,
    B8G8R8_SNORM = 51,
    B8G8R8_UINT = 52,
    B8G8R8_SINT = 53,
    B8G8R8_SRGB = 54,
    R8G8B8A8_UNORM = 55,
    R8G8B8A8_SNORM = 56,
    R8G8B8A8_UINT = 57,
    R8G8B8A8_SINT = 58,
    R8G8B8A8_SRGB = 59,
    B8G8R8A8_UNORM = 60,
    B8G8R8A8_SNORM = 61,
    B8G8R8A8_UINT = 62,
    B8G8R8A8_SINT = 63,
    B8G8R8A8_SRGB = 64,
    R8G8B8X8_UNORM = 65,
    B8G8R8X8_UNORM = 66,
    R16G16_UNORM = 67,
    G16R16_UNORM = 68,
    R16G16_SNORM = 69,
    G16R16_SNORM = 70,
    R16G16_UINT = 71,
    R16G16_SINT = 72,
    R16G16_SFLOAT = 73,
    R16G16_SBFLOAT = 74,
    R32_UINT = 75,
    R32_SINT = 76,
    R32_SFLOAT = 77,
    A2R10G10B10_UNORM = 78,
    A2R10G10B10_UINT = 79,
    A2R10G10B10_SNORM = 80,
    A2R10G10B10_SINT = 81,
    A2B10G10R10_UNORM = 82,
    A2B10G10R10_UINT = 83,
    A2B10G10R10_SNORM = 84,
    A2B10G10R10_SINT = 85,
    R10G10B10A2_UNORM = 86,
    R10G10B10A2_UINT = 87,
    R10G10B10A2_SNORM = 88,
    R10G10B10A2_SINT = 89,
    B10G10R10A2_UNORM = 90,
    B10G10R10A2_UINT = 91,
    B10G10R10A2_SNORM = 92,
    B10G10R10A2_SINT = 93,
    B10G11R11_UFLOAT = 94,
    E5B9G9R9_UFLOAT = 95,
    R16G16B16_UNORM = 96,
    R16G16B16_SNORM = 97,
    R16G16B16_UINT = 98,
    R16G16B16_SINT = 99,
    R16G16B16_SFLOAT = 100,
    R16G16B16_SBFLOAT = 101,
    R16G16B16A16_UNORM = 102,
    R16G16B16A16_SNORM = 103,
    R16G16B16A16_UINT = 104,
    R16G16B16A16_SINT = 105,
    R16G16B16A16_SFLOAT = 106,
    R16G16B16A16_SBFLOAT = 107,
    R32G32_UINT = 108,
    R32G32_SINT = 109,
    R32G32_SFLOAT = 110,
    R32G32B32_UINT = 111,
    R32G32B32_SINT = 112,
    R32G32B32_SFLOAT = 113,
    R32G32B32A32_UINT = 114,
    R32G32B32A32_SINT = 115,
    R32G32B32A32_SFLOAT = 116,
    R64_UINT = 117,
    R64_SINT = 118,
    R64_SFLOAT = 119,
    R64G64_UINT = 120,
    R64G64_SINT = 121,
    R64G64_SFLOAT = 122,
    R64G64B64_UINT = 123,
    R64G64B64_SINT = 124,
    R64G64B64_SFLOAT = 125,
    R64G64B64A64_UINT = 126,
    R64G64B64A64_SINT = 127,
    R64G64B64A64_SFLOAT = 128,
    D16_UNORM = 129,
    X8_D24_UNORM = 130,
    D32_SFLOAT = 131,
    S8_UINT = 132,
    D16_UNORM_S8_UINT = 133,
    D24_UNORM_S8_UINT = 134,
    D32_SFLOAT_S8_UINT = 135,
    DXBC1_RGB_UNORM = 136,
    DXBC1_RGB_SRGB = 137,
    DXBC1_RGBA_UNORM = 138,
    DXBC1_RGBA_SRGB = 139,
    DXBC2_UNORM = 140,
    DXBC2_SRGB = 141,
    DXBC3_UNORM = 142,
    DXBC3_SRGB = 143,
    DXBC4_UNORM = 144,
    DXBC4_SNORM = 145,
    DXBC5_UNORM = 146,
    DXBC5_SNORM = 147,
    DXBC6H_UFLOAT = 148,
    DXBC6H_SFLOAT = 149,
    DXBC7_UNORM = 150,
    DXBC7_SRGB = 151,
    PVRTC1_2BPP_UNORM = 152,
    PVRTC1_4BPP_UNORM = 153,
    PVRTC2_2BPP_UNORM = 154,
    PVRTC2_4BPP_UNORM = 155,
    PVRTC1_2BPP_SRGB = 156,
    PVRTC1_4BPP_SRGB = 157,
    PVRTC2_2BPP_SRGB = 158,
    PVRTC2_4BPP_SRGB = 159,
    ETC2_R8G8B8_UNORM = 160,
    ETC2_R8G8B8_SRGB = 161,
    ETC2_R8G8B8A1_UNORM = 162,
    ETC2_R8G8B8A1_SRGB = 163,
    ETC2_R8G8B8A8_UNORM = 164,
    ETC2_R8G8B8A8_SRGB = 165,
    ETC2_EAC_R11_UNORM = 166,
    ETC2_EAC_R11_SNORM = 167,
    ETC2_EAC_R11G11_UNORM = 168,
    ETC2_EAC_R11G11_SNORM = 169,
    ASTC_4x4_UNORM = 170,
    ASTC_4x4_SRGB = 171,
    ASTC_5x4_UNORM = 172,
    ASTC_5x4_SRGB = 173,
    ASTC_5x5_UNORM = 174,
    ASTC_5x5_SRGB = 175,
    ASTC_6x5_UNORM = 176,
    ASTC_6x5_SRGB = 177,
    ASTC_6x6_UNORM = 178,
    ASTC_6x6_SRGB = 179,
    ASTC_8x5_UNORM = 180,
    ASTC_8x5_SRGB = 181,
    ASTC_8x6_UNORM = 182,
    ASTC_8x6_SRGB = 183,
    ASTC_8x8_UNORM = 184,
    ASTC_8x8_SRGB = 185,
    ASTC_10x5_UNORM = 186,
    ASTC_10x5_SRGB = 187,
    ASTC_10x6_UNORM = 188,
    ASTC_10x6_SRGB = 189,
    ASTC_10x8_UNORM = 190,
    ASTC_10x8_SRGB = 191,
    ASTC_10x10_UNORM = 192,
    ASTC_10x10_SRGB = 193,
    ASTC_12x10_UNORM = 194,
    ASTC_12x10_SRGB = 195,
    ASTC_12x12_UNORM = 196,
    ASTC_12x12_SRGB = 197,
    CLUT_P4 = 198,
    CLUT_P4A4 = 199,
    CLUT_P8 = 200,
    CLUT_P8A8 = 201,
};

enum class gal_texture_dimension { none, td_1D, td_2D, td_3D };

enum class gal_texture_sample_count {
    tsc_1,
    tsc_2,
    tsc_4,
    tsc_8,
    tsc_16,
    tsc_32,
};

struct gal_texture_desc {
    u32 width;
    u32 height;
    u32 depth;
    u32 array_size;
    u32 mip_level;
    memory_flag memory_flags;
    gal_texture_dimension dimension;
    gal_texture_sample_count sample_count;
    gal_texture_format format;
    gal_resource_types resource_types; // descriptor types of buffer
    gal_resource_states initial_state; // initial state of buffer
};

// intailize gal_context, load function pointer
[[nodiscard]] gal_error_code init_gal(RenderApi api, gal_context *context);

#ifndef GAL_FUNCTION_DECLARE
#define GAL_FUNCTION_DECLARE
#include "helper/helper_macro.h"
#undef GAL_FUNCTION_DECLARE
#endif // !GAL_FUNCTION_DECLARE

} // namespace ant::gal
