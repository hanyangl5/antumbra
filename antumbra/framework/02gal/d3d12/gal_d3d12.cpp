#include "gal_d3d12.h"

#include <D3D12MemAlloc.h>
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include "framework/01core/logging/log.h"

#define ANT_DX12_API_VERSION D3D_FEATURE_LEVEL_12_1

namespace ant::gal {

struct d3d12_context {
    IDXGIFactory6 *factory = nullptr;
    ID3D12Device *device = nullptr;
    IDXGIAdapter4 *active_gpu = nullptr;
    D3D12MA::Allocator *d3dma_allocator = nullptr;

    ID3D12CommandQueue *graphics_queue = nullptr;
    ID3D12CommandQueue *compute_queue = nullptr;
    ID3D12CommandQueue *transfer_queue = nullptr;
};

struct d3d12_buffer {
    ID3D12Resource *buffer = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
};

struct d3d12_texture {
    ID3D12Resource *texture = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
};

struct d3d12_sampler {
    ID3D12Resource *sampler = nullptr;
};

struct d3d12_rendertargte {};

struct d3d12_swapchain {
    IDXGISwapChain3 *gpu_swap_chain;
};

struct d3d12_fence {};

struct d3d12_semaphore {};

struct d3d12_command_list {
    ID3D12GraphicsCommandList6 *gpu_command_list;
};

D3D12_RESOURCE_STATES util_to_dx12_resource_state(gal_resource_states state) {
    D3D12_RESOURCE_STATES ret = D3D12_RESOURCE_STATE_COMMON;

    // These states cannot be combined with other states so we just do an == check
    if (state == gal_resource_state::rs_common)
        return D3D12_RESOURCE_STATE_COMMON;
    if (state == gal_resource_state::rs_present)
        return D3D12_RESOURCE_STATE_PRESENT;

    if (state & gal_resource_state::rs_vertex_buffer)
        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if (state & gal_resource_state::rs_constant_buffer)
        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if (state & gal_resource_state::rs_index_buffer)
        ret |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
    if (state & gal_resource_state::rs_render_target)
        ret |= D3D12_RESOURCE_STATE_RENDER_TARGET;
    if (state & gal_resource_state::rs_rw_buffer)
        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if (state & gal_resource_state::rs_rw_texture)
        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if (state & gal_resource_state::rs_depth_write)
        ret |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
    if (state & gal_resource_state::rs_depth_read)
        ret |= D3D12_RESOURCE_STATE_DEPTH_READ;
    if (state & gal_resource_state::rs_indiret_buffer)
        ret |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
    if (state & gal_resource_state::rs_copy_dst)
        ret |= D3D12_RESOURCE_STATE_COPY_DEST;
    if (state & gal_resource_state::rs_copy_src)
        ret |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if (state & gal_resource_state::rs_texture)
        ret |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    // TODO(hyl5):rt
    // #ifdef
    //     if (state & RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
    //         ret |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
    // #endif
    //
    // #ifdef ENABLE_VRS
    //     if (state & RESOURCE_STATE_SHADING_RATE_SOURCE) ret |= D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
    // #endif

    return ret;
}

D3D12_RESOURCE_FLAGS util_to_dx12_resource_flags(gal_buffer_desc *desc) {
    D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

    if (desc->resource_types & gal_resource_type::rt_rw_buffer) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (desc->resource_types & gal_resource_type::rt_rw_texture) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (desc->resource_types & gal_resource_type::rt_color_rt) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if (desc->resource_types & gal_resource_type::rt_depth_stencil_rt) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    if (desc->memory_flag == gal_memory_flag::gpu_download) {
        flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }
    return flags;
}

inline gal_texture_format FromDXGI_FORMAT(DXGI_FORMAT fmt) {
    switch (fmt) {
    case DXGI_FORMAT_UNKNOWN:
        return gal_texture_format::UNDEFINED;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return gal_texture_format::R32G32B32A32_SFLOAT;
    case DXGI_FORMAT_R32G32B32A32_UINT:
        return gal_texture_format::R32G32B32A32_UINT;
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return gal_texture_format::R32G32B32A32_SINT;
    case DXGI_FORMAT_R32G32B32_FLOAT:
        return gal_texture_format::R32G32B32_SFLOAT;
    case DXGI_FORMAT_R32G32B32_UINT:
        return gal_texture_format::R32G32B32_UINT;
    case DXGI_FORMAT_R32G32B32_SINT:
        return gal_texture_format::R32G32B32_SINT;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
        return gal_texture_format::R16G16B16A16_SFLOAT;
    case DXGI_FORMAT_R16G16B16A16_UNORM:
        return gal_texture_format::R16G16B16A16_UNORM;
    case DXGI_FORMAT_R16G16B16A16_UINT:
        return gal_texture_format::R16G16B16A16_UINT;
    case DXGI_FORMAT_R16G16B16A16_SNORM:
        return gal_texture_format::R16G16B16A16_SNORM;
    case DXGI_FORMAT_R16G16B16A16_SINT:
        return gal_texture_format::R16G16B16A16_SINT;
    case DXGI_FORMAT_R32G32_FLOAT:
        return gal_texture_format::R32G32_SFLOAT;
    case DXGI_FORMAT_R32G32_UINT:
        return gal_texture_format::R32G32_UINT;
    case DXGI_FORMAT_R32G32_SINT:
        return gal_texture_format::R32G32_SINT;
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        return gal_texture_format::D32_SFLOAT_S8_UINT;
    case DXGI_FORMAT_R10G10B10A2_UNORM:
        return gal_texture_format::R10G10B10A2_UNORM;
    case DXGI_FORMAT_R10G10B10A2_UINT:
        return gal_texture_format::R10G10B10A2_UINT;
    case DXGI_FORMAT_R11G11B10_FLOAT:
        return gal_texture_format::B10G11R11_UFLOAT; // WRONG!
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return gal_texture_format::R8G8B8A8_UNORM;
    case DXGI_FORMAT_R16G16_FLOAT:
        return gal_texture_format::R16G16_SFLOAT;
    case DXGI_FORMAT_R16G16_UNORM:
        return gal_texture_format::R16G16_UNORM;
    case DXGI_FORMAT_R16G16_UINT:
        return gal_texture_format::R16G16_UINT;
    case DXGI_FORMAT_R16G16_SNORM:
        return gal_texture_format::R16G16_SNORM;
    case DXGI_FORMAT_R16G16_SINT:
        return gal_texture_format::R16G16_SINT;
    case DXGI_FORMAT_D32_FLOAT:
        return gal_texture_format::D32_SFLOAT;
    case DXGI_FORMAT_R32_FLOAT:
        return gal_texture_format::R32_SFLOAT;
    case DXGI_FORMAT_R32_UINT:
        return gal_texture_format::R32_UINT;
    case DXGI_FORMAT_R32_SINT:
        return gal_texture_format::R32_SINT;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        return gal_texture_format::D24_UNORM_S8_UINT;
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return gal_texture_format::D24_UNORM_S8_UINT;
    case DXGI_FORMAT_R8G8_UNORM:
        return gal_texture_format::R8G8_UNORM;
    case DXGI_FORMAT_R8G8_UINT:
        return gal_texture_format::R8G8_UINT;
    case DXGI_FORMAT_R8G8_SNORM:
        return gal_texture_format::R8G8_SNORM;
    case DXGI_FORMAT_R8G8_SINT:
        return gal_texture_format::R8G8_SINT;
    case DXGI_FORMAT_R16_FLOAT:
        return gal_texture_format::R16_SFLOAT;
    case DXGI_FORMAT_D16_UNORM:
        return gal_texture_format::D16_UNORM;
    case DXGI_FORMAT_R16_UNORM:
        return gal_texture_format::R16_UNORM;
    case DXGI_FORMAT_R16_UINT:
        return gal_texture_format::R16_UINT;
    case DXGI_FORMAT_R16_SNORM:
        return gal_texture_format::R16_SNORM;
    case DXGI_FORMAT_R16_SINT:
        return gal_texture_format::R16_SINT;
    case DXGI_FORMAT_R8_UNORM:
        return gal_texture_format::R8_UNORM;
    case DXGI_FORMAT_R8_UINT:
        return gal_texture_format::R8_UINT;
    case DXGI_FORMAT_R8_SNORM:
        return gal_texture_format::R8_SNORM;
    case DXGI_FORMAT_R8_SINT:
        return gal_texture_format::R8_SINT;
    case DXGI_FORMAT_A8_UNORM:
        return gal_texture_format::A8_UNORM;
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        return gal_texture_format::E5B9G9R9_UFLOAT;
    case DXGI_FORMAT_BC1_UNORM:
        return gal_texture_format::DXBC1_RGBA_UNORM;
    case DXGI_FORMAT_BC1_UNORM_SRGB:
        return gal_texture_format::DXBC1_RGBA_SRGB;
    case DXGI_FORMAT_BC2_UNORM:
        return gal_texture_format::DXBC2_UNORM;
    case DXGI_FORMAT_BC2_UNORM_SRGB:
        return gal_texture_format::DXBC2_SRGB;
    case DXGI_FORMAT_BC3_UNORM:
        return gal_texture_format::DXBC3_UNORM;
    case DXGI_FORMAT_BC3_UNORM_SRGB:
        return gal_texture_format::DXBC3_SRGB;
    case DXGI_FORMAT_BC4_UNORM:
        return gal_texture_format::DXBC4_UNORM;
    case DXGI_FORMAT_BC4_SNORM:
        return gal_texture_format::DXBC4_SNORM;
    case DXGI_FORMAT_BC5_UNORM:
        return gal_texture_format::DXBC5_UNORM;
    case DXGI_FORMAT_BC5_SNORM:
        return gal_texture_format::DXBC5_SNORM;
    case DXGI_FORMAT_B5G6R5_UNORM:
        return gal_texture_format::R5G6B5_UNORM;
    case DXGI_FORMAT_B5G5R5A1_UNORM:
        return gal_texture_format::R5G5B5A1_UNORM;
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        return gal_texture_format::B8G8R8A8_UNORM;
    case DXGI_FORMAT_B8G8R8X8_UNORM:
        return gal_texture_format::B8G8R8X8_UNORM;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return gal_texture_format::B8G8R8A8_SRGB;
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return gal_texture_format::B8G8R8A8_SRGB; // WRONG should be X8
    case DXGI_FORMAT_BC6H_UF16:
        return gal_texture_format::DXBC6H_UFLOAT;
    case DXGI_FORMAT_BC6H_SF16:
        return gal_texture_format::DXBC6H_SFLOAT;
    case DXGI_FORMAT_BC7_UNORM:
        return gal_texture_format::DXBC7_UNORM;
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return gal_texture_format::DXBC7_SRGB;
    case DXGI_FORMAT_B4G4R4A4_UNORM:
        return gal_texture_format::B4G4R4A4_UNORM;
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return gal_texture_format::R8G8B8A8_SRGB;
    case DXGI_FORMAT_R8G8B8A8_UINT:
        return gal_texture_format::R8G8B8A8_UINT;
    case DXGI_FORMAT_R8G8B8A8_SNORM:
        return gal_texture_format::R8G8B8A8_SNORM;
    case DXGI_FORMAT_R8G8B8A8_SINT:
        return gal_texture_format::R8G8B8A8_SINT;
    //case DXGI_FORMAT_R4G4_UNORM:
    //    return gal_texture_format::R4G4_UNORM;
    case DXGI_FORMAT_R1_UNORM:
        return gal_texture_format::R1_UNORM;

    case DXGI_FORMAT_420_OPAQUE:
        return gal_texture_format::G8_B8_R8_3PLANE_420_UNORM;
    case DXGI_FORMAT_AYUV:
        return gal_texture_format::B8G8R8A8_UNORM;
    case DXGI_FORMAT_Y410:
        return gal_texture_format::A2B10G10R10_UNORM;
    case DXGI_FORMAT_Y416:
        return gal_texture_format::R16G16B16A16_UNORM;
    case DXGI_FORMAT_NV12:
        return gal_texture_format::G8_B8R8_2PLANE_420_UNORM;
    case DXGI_FORMAT_P010:
        return gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
    case DXGI_FORMAT_P016:
        return gal_texture_format::G16_B16R16_2PLANE_420_UNORM;
    case DXGI_FORMAT_YUY2:
        return gal_texture_format::G8B8G8R8_422_UNORM;
    case DXGI_FORMAT_Y210:
        return gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
    case DXGI_FORMAT_Y216:
        return gal_texture_format::G16B16G16R16_422_UNORM;
    case DXGI_FORMAT_P208:
        return gal_texture_format::G8_B8R8_2PLANE_422_UNORM;

    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_NV11:
    case DXGI_FORMAT_AI44:
    case DXGI_FORMAT_IA44:
    case DXGI_FORMAT_P8:
    case DXGI_FORMAT_A8P8:
    case DXGI_FORMAT_V208:
    case DXGI_FORMAT_V408:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
        //case DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
        //case DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
        //case DXGI_FORMAT_D16_UNORM_S8_UINT:
        //case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
        //case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
        //case DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
        return gal_texture_format::UNDEFINED;
    }
    return gal_texture_format::UNDEFINED;
}

inline constexpr DXGI_FORMAT galtextureformat_to_dxgiformat(gal_texture_format fmt) {
    switch (fmt) {
    case gal_texture_format::R1_UNORM:
        return DXGI_FORMAT_R1_UNORM;
    //case gal_texture_format::R4G4_UNORM:
    //    return DXGI_FORMAT_R4G4_UNORM;
    case gal_texture_format::R5G6B5_UNORM:
        return DXGI_FORMAT_B5G6R5_UNORM;
    case gal_texture_format::B5G6R5_UNORM:
        return DXGI_FORMAT_B5G6R5_UNORM;
    case gal_texture_format::B5G5R5A1_UNORM:
        return DXGI_FORMAT_B5G5R5A1_UNORM;
    case gal_texture_format::R8_UNORM:
        return DXGI_FORMAT_R8_UNORM;
    case gal_texture_format::R8_SNORM:
        return DXGI_FORMAT_R8_SNORM;
    case gal_texture_format::R8_UINT:
        return DXGI_FORMAT_R8_UINT;
    case gal_texture_format::R8_SINT:
        return DXGI_FORMAT_R8_SINT;
    case gal_texture_format::R8G8_UNORM:
        return DXGI_FORMAT_R8G8_UNORM;
    case gal_texture_format::R8G8_SNORM:
        return DXGI_FORMAT_R8G8_SNORM;
    case gal_texture_format::R8G8_UINT:
        return DXGI_FORMAT_R8G8_UINT;
    case gal_texture_format::R8G8_SINT:
        return DXGI_FORMAT_R8G8_SINT;
    case gal_texture_format::B4G4R4A4_UNORM:
        return DXGI_FORMAT_B4G4R4A4_UNORM;

    case gal_texture_format::R8G8B8A8_UNORM:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case gal_texture_format::R8G8B8A8_SNORM:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    case gal_texture_format::R8G8B8A8_UINT:
        return DXGI_FORMAT_R8G8B8A8_UINT;
    case gal_texture_format::R8G8B8A8_SINT:
        return DXGI_FORMAT_R8G8B8A8_SINT;
    case gal_texture_format::R8G8B8A8_SRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

    case gal_texture_format::B8G8R8A8_UNORM:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    case gal_texture_format::B8G8R8X8_UNORM:
        return DXGI_FORMAT_B8G8R8X8_UNORM;
    case gal_texture_format::B8G8R8A8_SRGB:
        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

    case gal_texture_format::R10G10B10A2_UNORM:
        return DXGI_FORMAT_R10G10B10A2_UNORM;
    case gal_texture_format::R10G10B10A2_UINT:
        return DXGI_FORMAT_R10G10B10A2_UINT;

    case gal_texture_format::R16_UNORM:
        return DXGI_FORMAT_R16_UNORM;
    case gal_texture_format::R16_SNORM:
        return DXGI_FORMAT_R16_SNORM;
    case gal_texture_format::R16_UINT:
        return DXGI_FORMAT_R16_UINT;
    case gal_texture_format::R16_SINT:
        return DXGI_FORMAT_R16_SINT;
    case gal_texture_format::R16_SFLOAT:
        return DXGI_FORMAT_R16_FLOAT;
    case gal_texture_format::R16G16_UNORM:
        return DXGI_FORMAT_R16G16_UNORM;
    case gal_texture_format::R16G16_SNORM:
        return DXGI_FORMAT_R16G16_SNORM;
    case gal_texture_format::R16G16_UINT:
        return DXGI_FORMAT_R16G16_UINT;
    case gal_texture_format::R16G16_SINT:
        return DXGI_FORMAT_R16G16_SINT;
    case gal_texture_format::R16G16_SFLOAT:
        return DXGI_FORMAT_R16G16_FLOAT;
    case gal_texture_format::R16G16B16A16_UNORM:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    case gal_texture_format::R16G16B16A16_SNORM:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    case gal_texture_format::R16G16B16A16_UINT:
        return DXGI_FORMAT_R16G16B16A16_UINT;
    case gal_texture_format::R16G16B16A16_SINT:
        return DXGI_FORMAT_R16G16B16A16_SINT;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case gal_texture_format::R32_UINT:
        return DXGI_FORMAT_R32_UINT;
    case gal_texture_format::R32_SINT:
        return DXGI_FORMAT_R32_SINT;
    case gal_texture_format::R32_SFLOAT:
        return DXGI_FORMAT_R32_FLOAT;
    case gal_texture_format::R32G32_UINT:
        return DXGI_FORMAT_R32G32_UINT;
    case gal_texture_format::R32G32_SINT:
        return DXGI_FORMAT_R32G32_SINT;
    case gal_texture_format::R32G32_SFLOAT:
        return DXGI_FORMAT_R32G32_FLOAT;
    case gal_texture_format::R32G32B32_UINT:
        return DXGI_FORMAT_R32G32B32_UINT;
    case gal_texture_format::R32G32B32_SINT:
        return DXGI_FORMAT_R32G32B32_SINT;
    case gal_texture_format::R32G32B32_SFLOAT:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case gal_texture_format::R32G32B32A32_UINT:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case gal_texture_format::R32G32B32A32_SINT:
        return DXGI_FORMAT_R32G32B32A32_SINT;
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case gal_texture_format::B10G11R11_UFLOAT:
        return DXGI_FORMAT_R11G11B10_FLOAT;
    case gal_texture_format::E5B9G9R9_UFLOAT:
        return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    case gal_texture_format::D16_UNORM:
        return DXGI_FORMAT_D16_UNORM;
    case gal_texture_format::X8_D24_UNORM:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case gal_texture_format::D32_SFLOAT:
        return DXGI_FORMAT_D32_FLOAT;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return DXGI_FORMAT_BC1_UNORM;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return DXGI_FORMAT_BC1_UNORM_SRGB;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return DXGI_FORMAT_BC1_UNORM;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return DXGI_FORMAT_BC1_UNORM_SRGB;
    case gal_texture_format::DXBC2_UNORM:
        return DXGI_FORMAT_BC2_UNORM;
    case gal_texture_format::DXBC2_SRGB:
        return DXGI_FORMAT_BC2_UNORM_SRGB;
    case gal_texture_format::DXBC3_UNORM:
        return DXGI_FORMAT_BC3_UNORM;
    case gal_texture_format::DXBC3_SRGB:
        return DXGI_FORMAT_BC3_UNORM_SRGB;
    case gal_texture_format::DXBC4_UNORM:
        return DXGI_FORMAT_BC4_UNORM;
    case gal_texture_format::DXBC4_SNORM:
        return DXGI_FORMAT_BC4_SNORM;
    case gal_texture_format::DXBC5_UNORM:
        return DXGI_FORMAT_BC5_UNORM;
    case gal_texture_format::DXBC5_SNORM:
        return DXGI_FORMAT_BC5_SNORM;
    case gal_texture_format::DXBC6H_UFLOAT:
        return DXGI_FORMAT_BC6H_UF16;
    case gal_texture_format::DXBC6H_SFLOAT:
        return DXGI_FORMAT_BC6H_SF16;
    case gal_texture_format::DXBC7_UNORM:
        return DXGI_FORMAT_BC7_UNORM;
    case gal_texture_format::DXBC7_SRGB:
        return DXGI_FORMAT_BC7_UNORM_SRGB;
        //case gal_texture_format::D16_UNORM_S8_UINT:
        //    return DXGI_FORMAT_D16_UNORM_S8_UINT;

    default:
        return DXGI_FORMAT_UNKNOWN;
    }
    return DXGI_FORMAT_UNKNOWN;
}

inline constexpr DXGI_FORMAT dxgiformat_to_typeless(DXGI_FORMAT fmt) {
    switch (fmt) {
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return DXGI_FORMAT_R32G32B32A32_TYPELESS;
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return DXGI_FORMAT_R32G32B32_TYPELESS;

    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
        return DXGI_FORMAT_R16G16B16A16_TYPELESS;

    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
        return DXGI_FORMAT_R32G32_TYPELESS;

    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
        return DXGI_FORMAT_R10G10B10A2_TYPELESS;

    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
        return DXGI_FORMAT_R16G16_TYPELESS;

    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
        return DXGI_FORMAT_R32_TYPELESS;
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
        return DXGI_FORMAT_R8G8_TYPELESS;
    case DXGI_FORMAT_B4G4R4A4_UNORM: // just treats a 16 raw bits
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
        return DXGI_FORMAT_R16_TYPELESS;
    case DXGI_FORMAT_A8_UNORM:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    //case DXGI_FORMAT_R4G4_UNORM:
    case DXGI_FORMAT_R8_SINT:
        return DXGI_FORMAT_R8_TYPELESS;
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
        return DXGI_FORMAT_BC1_TYPELESS;
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
        return DXGI_FORMAT_BC2_TYPELESS;
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
        return DXGI_FORMAT_BC3_TYPELESS;
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        return DXGI_FORMAT_BC4_TYPELESS;
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
        return DXGI_FORMAT_BC5_TYPELESS;
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
        return DXGI_FORMAT_R16_TYPELESS;

    case DXGI_FORMAT_R11G11B10_FLOAT:
        return DXGI_FORMAT_R11G11B10_FLOAT;

    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8X8_TYPELESS;

    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8A8_TYPELESS;

    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
        return DXGI_FORMAT_BC6H_TYPELESS;

    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return DXGI_FORMAT_BC7_TYPELESS;

    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        return DXGI_FORMAT_R32G8X24_TYPELESS;
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return DXGI_FORMAT_R24G8_TYPELESS;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        return DXGI_FORMAT_R24G8_TYPELESS;

        // typeless just return the input format
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC7_TYPELESS:
        return fmt;

    case DXGI_FORMAT_R1_UNORM:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_AYUV:
    case DXGI_FORMAT_Y410:
    case DXGI_FORMAT_Y416:
    case DXGI_FORMAT_NV12:
    case DXGI_FORMAT_P010:
    case DXGI_FORMAT_P016:
    case DXGI_FORMAT_420_OPAQUE:
    case DXGI_FORMAT_YUY2:
    case DXGI_FORMAT_Y210:
    case DXGI_FORMAT_Y216:
    case DXGI_FORMAT_NV11:
    case DXGI_FORMAT_AI44:
    case DXGI_FORMAT_IA44:
    case DXGI_FORMAT_P8:
    case DXGI_FORMAT_A8P8:
    case DXGI_FORMAT_P208:
    case DXGI_FORMAT_V208:
    case DXGI_FORMAT_V408:
    //case DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
    //case DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
    //case DXGI_FORMAT_D16_UNORM_S8_UINT:
    //case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
    case DXGI_FORMAT_UNKNOWN:
        return DXGI_FORMAT_UNKNOWN;
    }
    return DXGI_FORMAT_UNKNOWN;
}

void load_gal_d3d12_functions() {
#define D3D12_LOAD_FUNCTION_PTRS
#include "../helper/helper_macro.h"
#undef D3D12_LOAD_FUNCTION_PTRS
}

void offload_gal_d3d12_functions(){
#define D3D12_OFFLOAD_FUNCTION_PTRS
#include "../helper/helper_macro.h"
#undef D3D12_OFFLOAD_FUNCTION_PTRS
}

gal_error_code d3d12_init_gal(gal_context *context) {
    *context = reinterpret_cast<gal_handle>(new d3d12_context);
    load_gal_d3d12_functions();
    if (*context == gal_null) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}
gal_error_code d3d12_destroy_gal(gal_context context) {
    if (context != gal_null) {
        d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
        delete d3d12_ctx;
        context = gal_null;
        offload_gal_d3d12_functions();
    }
    return gal_error_code::success;
}
gal_error_code d3d12_create_instance(GalDesc *desc, gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);

    u32 dxgi_factory_flags = 0;
    if (desc->b_debug_layer) {
        ID3D12Debug1 *pDebugController;
        if (D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController)) == S_OK) {
            // Enabling GPU Validation without enabling the debug layer does nothing

            pDebugController->EnableDebugLayer();
            pDebugController->SetEnableGPUBasedValidation(true);

            pDebugController->Release();
        }

        dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
    }

    // require directx12.1
    HRESULT hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&d3d12_ctx->factory));
    if (FAILED(hr) || d3d12_ctx->factory == nullptr) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_destroy_instance(gal_context *context) {

    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->factory != nullptr) {
        d3d12_ctx->factory->Release();
        d3d12_ctx->factory = nullptr;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_create_device(GalDesc *gal_desc, gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);

    auto pick_gpu = [&](IDXGIFactory6 *factory, IDXGIAdapter4 **gpu, ID3D12Device **device) -> gal_error_code {
        IDXGIAdapter4 *adapter = NULL;
        IDXGIFactory6 *factory6;

        HRESULT hr;

        hr = (factory->QueryInterface(IID_PPV_ARGS(&factory6)));
        if (FAILED(hr)) {
            return gal_error_code::unsupported_device;
        }

        // Find number of usable GPUs
        // Use DXGI6 interface which lets us specify gpu preference so we dont need
        // to use NVOptimus or AMDPowerExpress exports
        for (u32 i = 0; DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
                                                    i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
             ++i) {
            DXGI_ADAPTER_DESC3 desc;
            adapter->GetDesc3(&desc);
            ID3D12Device *t_device;

            if (SUCCEEDED(D3D12CreateDevice(adapter, ANT_DX12_API_VERSION, IID_PPV_ARGS(&t_device)))) {
                // if (SUCCEEDED(D3D12CreateDevice(adapter, ANT_DX12_API_VERSION, _uuidof(ID3D12Device), nullptr))) {

                auto check_features = [&]() -> gal_error_code {
                    HRESULT hr = 0;
                    D3D12_FEATURE_DATA_D3D12_OPTIONS featureDataOptions = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureDataOptions,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS1 featureDataOptions1 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &featureDataOptions1,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    bool b_wave_ops = featureDataOptions1.WaveOps;
                    if (!b_wave_ops) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS2 featureDataOptions2 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &featureDataOptions2,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS2)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS3 featureDataOptions3 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &featureDataOptions3,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS3)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS4 featureDataOptions4 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &featureDataOptions4,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS4)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureDataOptions5 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureDataOptions5,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    bool b_rt11Supported = (featureDataOptions5.RaytracingTier & D3D12_RAYTRACING_TIER_1_1) != 0;
                    if (!b_rt11Supported) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS6 featureDataOptions6 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &featureDataOptions6,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS6)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    if (gal_desc->b_variable_rate_shading) {
                        bool b_vrs2Supported =
                            (featureDataOptions6.VariableShadingRateTier & D3D12_VARIABLE_SHADING_RATE_TIER_2) != 0;
                        if (!b_vrs2Supported) {
                            return gal_error_code::unsupported_device;
                        }
                    }
                    D3D12_FEATURE_DATA_D3D12_OPTIONS7 featureDataOptions7 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &featureDataOptions7,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS7)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    if (gal_desc->b_mesh_shader) {
                        bool b_mesh_shader = (featureDataOptions7.MeshShaderTier & D3D12_MESH_SHADER_TIER_1) != 0;
                        if (!b_mesh_shader) {
                            return gal_error_code::unsupported_device;
                        }
                    }

                    // FIXME(hyl5): these code may not compile on some windows sdks
                    D3D12_FEATURE_DATA_D3D12_OPTIONS8 featureDataOptions8 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS8, &featureDataOptions8,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS8)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS9 featureDataOptions9 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &featureDataOptions9,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS9)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    if (gal_desc->b_mesh_shader) {
                        bool b_mesh_shader_pipeline = featureDataOptions9.MeshShaderPipelineStatsSupported;
                        if (!b_mesh_shader_pipeline) {
                            return gal_error_code::unsupported_device;
                        }
                    }
                    D3D12_FEATURE_DATA_D3D12_OPTIONS10 featureDataOptions10 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS10, &featureDataOptions10,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS10)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS11 featureDataOptions11 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &featureDataOptions11,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS11)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    return gal_error_code::success;
                };

                if (check_features() != gal_error_code::success) {
                    t_device->Release();
                    continue;
                }
                *gpu = adapter;
                *device = t_device;
                return gal_error_code::success;
            }
        }
        return gal_error_code::unsupported_device;
    };

    gal_error_code result = pick_gpu(d3d12_ctx->factory, &d3d12_ctx->active_gpu, &d3d12_ctx->device);

    if (result != gal_error_code::success || d3d12_ctx->active_gpu == nullptr || d3d12_ctx->device == nullptr) {
        return gal_error_code::unsupported_device;
    }
    // create queue
    D3D12_COMMAND_QUEUE_DESC graphics_command_queue_desc{};
    graphics_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    graphics_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr =
        (d3d12_ctx->device->CreateCommandQueue(&graphics_command_queue_desc, IID_PPV_ARGS(&d3d12_ctx->graphics_queue)));
    if (FAILED(hr) || d3d12_ctx->graphics_queue == nullptr) {
        return gal_error_code::error;
    }
    if (gal_desc->b_async_compute) {
        D3D12_COMMAND_QUEUE_DESC compute_command_queue_desc{};
        compute_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        compute_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;

        hr = (d3d12_ctx->device->CreateCommandQueue(&compute_command_queue_desc,
                                                    IID_PPV_ARGS(&d3d12_ctx->compute_queue)));

        if (FAILED(hr) || d3d12_ctx->compute_queue == nullptr) {
            return gal_error_code::error;
        }
    } else {
        d3d12_ctx->compute_queue = d3d12_ctx->graphics_queue;
    }
    if (gal_desc->b_async_transfer) {
        D3D12_COMMAND_QUEUE_DESC transfer_command_queue_desc{};
        transfer_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        transfer_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;

        hr = (d3d12_ctx->device->CreateCommandQueue(&transfer_command_queue_desc,
                                                    IID_PPV_ARGS(&d3d12_ctx->transfer_queue)));
        if (FAILED(hr) || d3d12_ctx->transfer_queue == nullptr) {
            return gal_error_code::error;
        }
    } else {

        d3d12_ctx->transfer_queue = d3d12_ctx->graphics_queue;
    }
    if (result == gal_error_code::success && d3d12_ctx->active_gpu != nullptr && d3d12_ctx->device != nullptr) {
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}

gal_error_code d3d12_destroy_device(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->device != nullptr) {
        d3d12_ctx->device->Release();
        d3d12_ctx->device = nullptr;
    }
    return gal_error_code::success;
}
gal_error_code d3d12_create_memory_allocator(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    D3D12MA::ALLOCATOR_DESC allocatorDesc{};
    allocatorDesc.pDevice = d3d12_ctx->device;
    allocatorDesc.pAdapter = d3d12_ctx->active_gpu;

    HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &d3d12_ctx->d3dma_allocator);

    if (FAILED(hr) || d3d12_ctx->d3dma_allocator == nullptr) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_destroy_memory_allocator(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->d3dma_allocator != nullptr) {
        d3d12_ctx->d3dma_allocator->Release();
        d3d12_ctx->d3dma_allocator = nullptr;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_create_buffer(gal_context context, gal_buffer_desc *desc, gal_buffer *buffer) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
    *buffer = reinterpret_cast<gal_handle>(new ant::gal::d3d12_buffer);
    d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(*buffer);

    CD3DX12_RESOURCE_DESC buffer_desc =
        CD3DX12_RESOURCE_DESC::Buffer(desc->size, D3D12_RESOURCE_FLAG_NONE, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

    buffer_desc.Flags = util_to_dx12_resource_flags(desc);

    D3D12MA::ALLOCATION_DESC allocation_desc{};

    D3D12_RESOURCE_STATES initial_state = util_to_dx12_resource_state(desc->initial_state);

    if (desc->memory_flag == gal_memory_flag::gpu_dedicated) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
    } else if (desc->memory_flag == gal_memory_flag::cpu_upload) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
        initial_state = D3D12_RESOURCE_STATE_GENERIC_READ; // genric read is a required start state for upload heap
    } else if (desc->memory_flag == gal_memory_flag::gpu_download) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
        initial_state = D3D12_RESOURCE_STATE_COPY_DEST;
    } else {
        return gal_error_code::invalid_parameter;
    }
    if (desc->flags & gal_buffer_flag::bcf_own_memory) {
        allocation_desc.Flags |= D3D12MA::ALLOCATION_FLAGS::ALLOCATION_FLAG_COMMITTED;
    }

    HRESULT hr = (d3d12_ctx->d3dma_allocator->CreateResource(&allocation_desc, &buffer_desc, initial_state, NULL,
                                                             &d3d12_buf->allocation, IID_PPV_ARGS(&d3d12_buf->buffer)));

    if (FAILED(hr) || d3d12_buf->allocation == nullptr) {
        return gal_error_code::error;
    }

    // create descriptor
    //if (!desc->flags & buffer_creation_flag::bcf_no_descriptor_view_creation) {
    //    //TODO(hyl5): create resource descriptor
    //}

    return gal_error_code::success;
}

gal_error_code d3d12_destroy_buffer([[maybe_unused]] gal_context context, gal_buffer buffer) {
    if (buffer != nullptr) {
        d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(buffer);
        d3d12_buf->buffer->Release();
        d3d12_buf->allocation->Release();
        delete d3d12_buf;
        buffer = nullptr;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_create_texture(gal_context context, gal_texture_desc *desc, gal_texture *texture) {
    // TODO(hyl5): finish this 
    if (context)
        return gal_error_code::success;
    else if (texture)
        return gal_error_code::success;
    else if (desc)
        return gal_error_code::success;
    return gal_error_code::success;
}

gal_error_code d3d12_destroy_texture(gal_context context, gal_texture texture) {
    // TODO(hyl5): finish this 
    if (context)
    return gal_error_code::success;
    else if(texture)
    return gal_error_code::success;
    return gal_error_code::success;
}


gal_error_code d3d12_create_sampler() { return gal_error_code::success; }
gal_error_code d3d12_destroy_sampler() { return gal_error_code::success; }
gal_error_code d3d12_create_rendertarget() { return gal_error_code::success; }
gal_error_code d3d12_destroy_rendertarget() { return gal_error_code::success; }
// surface
gal_error_code d3d12_create_swapchain() { return gal_error_code::success; }
gal_error_code d3d12_destroy_swapchain() { return gal_error_code::success; }
gal_error_code d3d12_create_surface() { return gal_error_code::success; }
gal_error_code d3d12_destroy_surface() { return gal_error_code::success; }
// pipeline
gal_error_code d3d12_create_shader() { return gal_error_code::success; }
gal_error_code d3d12_destroy_shader() { return gal_error_code::success; }
gal_error_code d3d12_create_pipeline() { return gal_error_code::success; }
gal_error_code d3d12_destroy_pipeline() { return gal_error_code::success; }
gal_error_code d3d12_create_descriptorpool() { return gal_error_code::success; }
gal_error_code d3d12_destroy_descriptorpool() { return gal_error_code::success; }
gal_error_code d3d12_create_descriptorset() { return gal_error_code::success; }
gal_error_code d3d12_destroy_descriptorset() { return gal_error_code::success; }
gal_error_code d3d12_create_rootsignature() { return gal_error_code::success; }
gal_error_code d3d12_destroy_rootsignature() { return gal_error_code::success; }
// sync
gal_error_code d3d12_create_fence() { return gal_error_code::success; }
gal_error_code d3d12_wait_fence() { return gal_error_code::success; }
gal_error_code d3d12_destroy_fence() { return gal_error_code::success; }
gal_error_code d3d12_wait_gpu() { return gal_error_code::success; }
gal_error_code d3d12_create_semaphore() { return gal_error_code::success; }
gal_error_code d3d12_destroy_semaphore() { return gal_error_code::success; }
// cmds
gal_error_code d3d12_create_commandpool() { return gal_error_code::success; }
gal_error_code d3d12_reset_commandpool() { return gal_error_code::success; }
gal_error_code d3d12_destroy_commandpool() { return gal_error_code::success; }
gal_error_code d3d12_allocate_commandlist() { return gal_error_code::success; }
gal_error_code d3d12_free_commandlist() { return gal_error_code::success; }
gal_error_code d3d12_cmd_begin_recording() { return gal_error_code::success; }
gal_error_code d3d12_cmd_end_recording() { return gal_error_code::success; }
gal_error_code d3d12_cmd_resource_barrier() { return gal_error_code::success; }
gal_error_code d3d12_cmd_bind_descriptor_set() { return gal_error_code::success; }
gal_error_code d3d12_cmd_bind_index_buffer() { return gal_error_code::success; }
gal_error_code d3d12_cmd_bind_vertex_buffer() { return gal_error_code::success; }
gal_error_code d3d12_cmd_bind_descriptorset() { return gal_error_code::success; }
gal_error_code d3d12_cmd_bind_pipeline() { return gal_error_code::success; }
gal_error_code d3d12_cmd_begin_renderpass() { return gal_error_code::success; }
gal_error_code d3d12_cmd_end_renderpass() { return gal_error_code::success; }
gal_error_code d3d12_cmd_dispatch() { return gal_error_code::success; }
gal_error_code d3d12_cmd_dispatch_indirect() { return gal_error_code::success; }
gal_error_code d3d12_cmd_draw_instanced() { return gal_error_code::success; }
gal_error_code d3d12_cmd_draw_indexed_instanced() { return gal_error_code::success; }
gal_error_code d3d12_cmd_draw_indirect_instanced() { return gal_error_code::success; }
gal_error_code d3d12_cmd_draw_indirect_indexed_instanced() { return gal_error_code::success; }
gal_error_code d3d12_cmd_draw_mesh_task() { return gal_error_code::success; }
gal_error_code d3d12_cmd_draw_indirect_mesh_task() { return gal_error_code::success; }
gal_error_code d3d12_cmd_copy_texture() { return gal_error_code::success; }
gal_error_code d3d12_cmd_copy_buffer() { return gal_error_code::success; }
gal_error_code d3d12_cmd_fill_buffer() { return gal_error_code::success; }
gal_error_code d3d12_cmd_fill_texture() { return gal_error_code::success; }
gal_error_code d3d12_cmd_upload_buffer() { return gal_error_code::success; }
gal_error_code d3d12_cmd_upload_texture() { return gal_error_code::success; }
gal_error_code d3d12_cmd_copy_buffer_to_texture() { return gal_error_code::success; }
gal_error_code d3d12_cmd_copy_texture_to_buffer() { return gal_error_code::success; }

} // namespace ant::gal
