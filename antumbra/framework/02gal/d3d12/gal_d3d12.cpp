//#include "gal_d3d12.h"
//
//#include <D3D12MemAlloc.h>
//#include <Windows.h>
//#include <d3dx12.h>
//#include <dxgi1_6.h>
//
//#include "framework/01core/logging/log.h"
//
//#define ANT_DX12_API_VERSION D3D_FEATURE_LEVEL_12_1
//
//namespace ante::gal {
//
//struct d3d12_context {
//    IDXGIFactory6 *factory = nullptr;
//    ID3D12Device *device = nullptr;
//    IDXGIAdapter4 *active_gpu = nullptr;
//    D3D12MA::Allocator *d3dma_allocator = nullptr;
//
//    ID3D12CommandQueue *graphics_queue = nullptr;
//    ID3D12CommandQueue *compute_queue = nullptr;
//    ID3D12CommandQueue *transfer_queue = nullptr;
//};
//
//struct d3d12_buffer {
//    ID3D12Resource *buffer = nullptr;
//    D3D12MA::Allocation *allocation = nullptr;
//};
//
//struct d3d12_texture {
//    ID3D12Resource *texture = nullptr;
//    D3D12MA::Allocation *allocation = nullptr;
//};
//
//struct d3d12_sampler {
//    ID3D12Resource *sampler = nullptr;
//    D3D12MA::Allocation *allocation = nullptr;
//};
//
//struct d3d12_rendertargte {};
//
//struct d3d12_swap_chain {
//    IDXGISwapChain3 *gpu_swap_chain;
//};
//
//struct d3d12_fence {
//    ID3D12Fence *fence = nullptr;
//    HANDLE wait_idle_fence_event = nullptr;
//    UINT64 fence_value = 0;
//};
//
//struct d3d12_semaphore {};
//
//struct d3d12_command_list {
//    ID3D12GraphicsCommandList6 *command_list;
//};
//
//constexpr D3D12_RESOURCE_STATES util_to_dx12_resource_state(gal_resource_state state) {
//    D3D12_RESOURCE_STATES ret = D3D12_RESOURCE_STATE_COMMON;
//
//    // These states cannot be combined with other states so we just do an == check
//    if (state == gal_resource_state::COMMON)
//        return D3D12_RESOURCE_STATE_COMMON;
//    if (state == gal_resource_state::PRESENT)
//        return D3D12_RESOURCE_STATE_PRESENT;
//
//    if ((state & gal_resource_state::VERTEX_BUFFER) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
//    if ((state & gal_resource_state::CONSTANT_BUFFER) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
//    if ((state & gal_resource_state::INDEX_BUFFER) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
//    if ((state & gal_resource_state::RENDER_TARGET) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_RENDER_TARGET;
//    if ((state & gal_resource_state::RW_BUFFER) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
//    if ((state & gal_resource_state::RW_TEXTURE) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
//    if ((state & gal_resource_state::DEPTH_WRITE) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
//    if ((state & gal_resource_state::DEPTH_READ) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_DEPTH_READ;
//    if ((state & gal_resource_state::INDIRECT_BUFFER) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
//    if ((state & gal_resource_state::COPY_DST) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_COPY_DEST;
//    if ((state & gal_resource_state::COPY_SRC) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_COPY_SOURCE;
//    if ((state & gal_resource_state::TEXTURE) != gal_resource_state::UNDEFINIED)
//        ret |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
//    // TODO(hyl5):rt
//    // #ifdef
//    //     if (state & RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
//    //         ret |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
//    // #endif
//    //
//    // #ifdef ENABLE_VRS
//    //     if (state & RESOURCE_STATE_SHADING_RATE_SOURCE) ret |= D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
//    // #endif
//
//    return ret;
//}
//
//constexpr D3D12_RESOURCE_FLAGS util_to_dx12_resource_flags(gal_buffer_desc *desc) {
//    D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
//
//    if ((desc->resource_types & gal_descriptor_type::RW_BUFFER) != gal_descriptor_type::UNDEFINED) {
//        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
//    }
//    if ((desc->resource_types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED) {
//        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
//    }
//    if ((desc->resource_types & gal_descriptor_type::COLOR_RT) != gal_descriptor_type::UNDEFINED) {
//        flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
//    }
//    if ((desc->resource_types & gal_descriptor_type::DEPTH_STENCIL_RT) != gal_descriptor_type::UNDEFINED) {
//        flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
//    }
//    if ((desc->memory_flags & gal_memory_flag::GPU_DOWNLOAD) != gal_memory_flag::UNDEFINED) {
//        flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
//    }
//    return flags;
//}
//
//constexpr gal_texture_format FromDXGI_FORMAT(DXGI_FORMAT fmt) {
//    switch (fmt) {
//    case DXGI_FORMAT_UNKNOWN:
//        return gal_texture_format::UNDEFINED;
//    case DXGI_FORMAT_R32G32B32A32_FLOAT:
//        return gal_texture_format::R32G32B32A32_SFLOAT;
//    case DXGI_FORMAT_R32G32B32A32_UINT:
//        return gal_texture_format::R32G32B32A32_UINT;
//    case DXGI_FORMAT_R32G32B32A32_SINT:
//        return gal_texture_format::R32G32B32A32_SINT;
//    case DXGI_FORMAT_R32G32B32_FLOAT:
//        return gal_texture_format::R32G32B32_SFLOAT;
//    case DXGI_FORMAT_R32G32B32_UINT:
//        return gal_texture_format::R32G32B32_UINT;
//    case DXGI_FORMAT_R32G32B32_SINT:
//        return gal_texture_format::R32G32B32_SINT;
//    case DXGI_FORMAT_R16G16B16A16_FLOAT:
//        return gal_texture_format::R16G16B16A16_SFLOAT;
//    case DXGI_FORMAT_R16G16B16A16_UNORM:
//        return gal_texture_format::R16G16B16A16_UNORM;
//    case DXGI_FORMAT_R16G16B16A16_UINT:
//        return gal_texture_format::R16G16B16A16_UINT;
//    case DXGI_FORMAT_R16G16B16A16_SNORM:
//        return gal_texture_format::R16G16B16A16_SNORM;
//    case DXGI_FORMAT_R16G16B16A16_SINT:
//        return gal_texture_format::R16G16B16A16_SINT;
//    case DXGI_FORMAT_R32G32_FLOAT:
//        return gal_texture_format::R32G32_SFLOAT;
//    case DXGI_FORMAT_R32G32_UINT:
//        return gal_texture_format::R32G32_UINT;
//    case DXGI_FORMAT_R32G32_SINT:
//        return gal_texture_format::R32G32_SINT;
//    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
//        return gal_texture_format::D32_SFLOAT_S8_UINT;
//    case DXGI_FORMAT_R10G10B10A2_UNORM:
//        return gal_texture_format::R10G10B10A2_UNORM;
//    case DXGI_FORMAT_R10G10B10A2_UINT:
//        return gal_texture_format::R10G10B10A2_UINT;
//    case DXGI_FORMAT_R11G11B10_FLOAT:
//        return gal_texture_format::B10G11R11_UFLOAT; // WRONG!
//    case DXGI_FORMAT_R8G8B8A8_UNORM:
//        return gal_texture_format::R8G8B8A8_UNORM;
//    case DXGI_FORMAT_R16G16_FLOAT:
//        return gal_texture_format::R16G16_SFLOAT;
//    case DXGI_FORMAT_R16G16_UNORM:
//        return gal_texture_format::R16G16_UNORM;
//    case DXGI_FORMAT_R16G16_UINT:
//        return gal_texture_format::R16G16_UINT;
//    case DXGI_FORMAT_R16G16_SNORM:
//        return gal_texture_format::R16G16_SNORM;
//    case DXGI_FORMAT_R16G16_SINT:
//        return gal_texture_format::R16G16_SINT;
//    case DXGI_FORMAT_D32_FLOAT:
//        return gal_texture_format::D32_SFLOAT;
//    case DXGI_FORMAT_R32_FLOAT:
//        return gal_texture_format::R32_SFLOAT;
//    case DXGI_FORMAT_R32_UINT:
//        return gal_texture_format::R32_UINT;
//    case DXGI_FORMAT_R32_SINT:
//        return gal_texture_format::R32_SINT;
//    case DXGI_FORMAT_D24_UNORM_S8_UINT:
//        return gal_texture_format::D24_UNORM_S8_UINT;
//    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
//        return gal_texture_format::D24_UNORM_S8_UINT;
//    case DXGI_FORMAT_R8G8_UNORM:
//        return gal_texture_format::R8G8_UNORM;
//    case DXGI_FORMAT_R8G8_UINT:
//        return gal_texture_format::R8G8_UINT;
//    case DXGI_FORMAT_R8G8_SNORM:
//        return gal_texture_format::R8G8_SNORM;
//    case DXGI_FORMAT_R8G8_SINT:
//        return gal_texture_format::R8G8_SINT;
//    case DXGI_FORMAT_R16_FLOAT:
//        return gal_texture_format::R16_SFLOAT;
//    case DXGI_FORMAT_D16_UNORM:
//        return gal_texture_format::D16_UNORM;
//    case DXGI_FORMAT_R16_UNORM:
//        return gal_texture_format::R16_UNORM;
//    case DXGI_FORMAT_R16_UINT:
//        return gal_texture_format::R16_UINT;
//    case DXGI_FORMAT_R16_SNORM:
//        return gal_texture_format::R16_SNORM;
//    case DXGI_FORMAT_R16_SINT:
//        return gal_texture_format::R16_SINT;
//    case DXGI_FORMAT_R8_UNORM:
//        return gal_texture_format::R8_UNORM;
//    case DXGI_FORMAT_R8_UINT:
//        return gal_texture_format::R8_UINT;
//    case DXGI_FORMAT_R8_SNORM:
//        return gal_texture_format::R8_SNORM;
//    case DXGI_FORMAT_R8_SINT:
//        return gal_texture_format::R8_SINT;
//    case DXGI_FORMAT_A8_UNORM:
//        return gal_texture_format::A8_UNORM;
//    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
//        return gal_texture_format::E5B9G9R9_UFLOAT;
//    case DXGI_FORMAT_BC1_UNORM:
//        return gal_texture_format::DXBC1_RGBA_UNORM;
//    case DXGI_FORMAT_BC1_UNORM_SRGB:
//        return gal_texture_format::DXBC1_RGBA_SRGB;
//    case DXGI_FORMAT_BC2_UNORM:
//        return gal_texture_format::DXBC2_UNORM;
//    case DXGI_FORMAT_BC2_UNORM_SRGB:
//        return gal_texture_format::DXBC2_SRGB;
//    case DXGI_FORMAT_BC3_UNORM:
//        return gal_texture_format::DXBC3_UNORM;
//    case DXGI_FORMAT_BC3_UNORM_SRGB:
//        return gal_texture_format::DXBC3_SRGB;
//    case DXGI_FORMAT_BC4_UNORM:
//        return gal_texture_format::DXBC4_UNORM;
//    case DXGI_FORMAT_BC4_SNORM:
//        return gal_texture_format::DXBC4_SNORM;
//    case DXGI_FORMAT_BC5_UNORM:
//        return gal_texture_format::DXBC5_UNORM;
//    case DXGI_FORMAT_BC5_SNORM:
//        return gal_texture_format::DXBC5_SNORM;
//    case DXGI_FORMAT_B5G6R5_UNORM:
//        return gal_texture_format::R5G6B5_UNORM;
//    case DXGI_FORMAT_B5G5R5A1_UNORM:
//        return gal_texture_format::R5G5B5A1_UNORM;
//    case DXGI_FORMAT_B8G8R8A8_UNORM:
//        return gal_texture_format::B8G8R8A8_UNORM;
//    case DXGI_FORMAT_B8G8R8X8_UNORM:
//        return gal_texture_format::B8G8R8X8_UNORM;
//    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
//        return gal_texture_format::B8G8R8A8_SRGB;
//    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
//        return gal_texture_format::B8G8R8A8_SRGB; // WRONG should be X8
//    case DXGI_FORMAT_BC6H_UF16:
//        return gal_texture_format::DXBC6H_UFLOAT;
//    case DXGI_FORMAT_BC6H_SF16:
//        return gal_texture_format::DXBC6H_SFLOAT;
//    case DXGI_FORMAT_BC7_UNORM:
//        return gal_texture_format::DXBC7_UNORM;
//    case DXGI_FORMAT_BC7_UNORM_SRGB:
//        return gal_texture_format::DXBC7_SRGB;
//    case DXGI_FORMAT_B4G4R4A4_UNORM:
//        return gal_texture_format::B4G4R4A4_UNORM;
//    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
//        return gal_texture_format::R8G8B8A8_SRGB;
//    case DXGI_FORMAT_R8G8B8A8_UINT:
//        return gal_texture_format::R8G8B8A8_UINT;
//    case DXGI_FORMAT_R8G8B8A8_SNORM:
//        return gal_texture_format::R8G8B8A8_SNORM;
//    case DXGI_FORMAT_R8G8B8A8_SINT:
//        return gal_texture_format::R8G8B8A8_SINT;
//    //case DXGI_FORMAT_R4G4_UNORM:
//    //    return gal_texture_format::R4G4_UNORM;
//    case DXGI_FORMAT_R1_UNORM:
//        return gal_texture_format::R1_UNORM;
//
//    case DXGI_FORMAT_420_OPAQUE:
//        return gal_texture_format::G8_B8_R8_3PLANE_420_UNORM;
//    case DXGI_FORMAT_AYUV:
//        return gal_texture_format::B8G8R8A8_UNORM;
//    case DXGI_FORMAT_Y410:
//        return gal_texture_format::A2B10G10R10_UNORM;
//    case DXGI_FORMAT_Y416:
//        return gal_texture_format::R16G16B16A16_UNORM;
//    case DXGI_FORMAT_NV12:
//        return gal_texture_format::G8_B8R8_2PLANE_420_UNORM;
//    case DXGI_FORMAT_P010:
//        return gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
//    case DXGI_FORMAT_P016:
//        return gal_texture_format::G16_B16R16_2PLANE_420_UNORM;
//    case DXGI_FORMAT_YUY2:
//        return gal_texture_format::G8B8G8R8_422_UNORM;
//    case DXGI_FORMAT_Y210:
//        return gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
//    case DXGI_FORMAT_Y216:
//        return gal_texture_format::G16B16G16R16_422_UNORM;
//    case DXGI_FORMAT_P208:
//        return gal_texture_format::G8_B8R8_2PLANE_422_UNORM;
//
//    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
//    case DXGI_FORMAT_R32G32B32_TYPELESS:
//    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
//    case DXGI_FORMAT_R32G32_TYPELESS:
//    case DXGI_FORMAT_R32G8X24_TYPELESS:
//    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
//    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
//    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
//    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
//    case DXGI_FORMAT_R16G16_TYPELESS:
//    case DXGI_FORMAT_R32_TYPELESS:
//    case DXGI_FORMAT_R24G8_TYPELESS:
//    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
//    case DXGI_FORMAT_R8G8_TYPELESS:
//    case DXGI_FORMAT_R16_TYPELESS:
//    case DXGI_FORMAT_R8_TYPELESS:
//    case DXGI_FORMAT_BC1_TYPELESS:
//    case DXGI_FORMAT_BC2_TYPELESS:
//    case DXGI_FORMAT_BC3_TYPELESS:
//    case DXGI_FORMAT_BC4_TYPELESS:
//    case DXGI_FORMAT_BC5_TYPELESS:
//    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
//    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
//    case DXGI_FORMAT_BC6H_TYPELESS:
//    case DXGI_FORMAT_BC7_TYPELESS:
//    case DXGI_FORMAT_NV11:
//    case DXGI_FORMAT_AI44:
//    case DXGI_FORMAT_IA44:
//    case DXGI_FORMAT_P8:
//    case DXGI_FORMAT_A8P8:
//    case DXGI_FORMAT_V208:
//    case DXGI_FORMAT_V408:
//    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
//    case DXGI_FORMAT_R8G8_B8G8_UNORM:
//    case DXGI_FORMAT_G8R8_G8B8_UNORM:
//        //case DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
//        //case DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
//        //case DXGI_FORMAT_D16_UNORM_S8_UINT:
//        //case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
//        //case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
//        //case DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
//        return gal_texture_format::UNDEFINED;
//    }
//    return gal_texture_format::UNDEFINED;
//}
//
//constexpr DXGI_FORMAT galtextureformat_to_dxgiformat(gal_texture_format fmt) {
//    switch (fmt) {
//    case gal_texture_format::R1_UNORM:
//        return DXGI_FORMAT_R1_UNORM;
//    //case gal_texture_format::R4G4_UNORM:
//    //    return DXGI_FORMAT_R4G4_UNORM;
//    case gal_texture_format::R5G6B5_UNORM:
//        return DXGI_FORMAT_B5G6R5_UNORM;
//    case gal_texture_format::B5G6R5_UNORM:
//        return DXGI_FORMAT_B5G6R5_UNORM;
//    case gal_texture_format::B5G5R5A1_UNORM:
//        return DXGI_FORMAT_B5G5R5A1_UNORM;
//    case gal_texture_format::R8_UNORM:
//        return DXGI_FORMAT_R8_UNORM;
//    case gal_texture_format::R8_SNORM:
//        return DXGI_FORMAT_R8_SNORM;
//    case gal_texture_format::R8_UINT:
//        return DXGI_FORMAT_R8_UINT;
//    case gal_texture_format::R8_SINT:
//        return DXGI_FORMAT_R8_SINT;
//    case gal_texture_format::R8G8_UNORM:
//        return DXGI_FORMAT_R8G8_UNORM;
//    case gal_texture_format::R8G8_SNORM:
//        return DXGI_FORMAT_R8G8_SNORM;
//    case gal_texture_format::R8G8_UINT:
//        return DXGI_FORMAT_R8G8_UINT;
//    case gal_texture_format::R8G8_SINT:
//        return DXGI_FORMAT_R8G8_SINT;
//    case gal_texture_format::B4G4R4A4_UNORM:
//        return DXGI_FORMAT_B4G4R4A4_UNORM;
//
//    case gal_texture_format::R8G8B8A8_UNORM:
//        return DXGI_FORMAT_R8G8B8A8_UNORM;
//    case gal_texture_format::R8G8B8A8_SNORM:
//        return DXGI_FORMAT_R8G8B8A8_SNORM;
//    case gal_texture_format::R8G8B8A8_UINT:
//        return DXGI_FORMAT_R8G8B8A8_UINT;
//    case gal_texture_format::R8G8B8A8_SINT:
//        return DXGI_FORMAT_R8G8B8A8_SINT;
//    case gal_texture_format::R8G8B8A8_SRGB:
//        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//
//    case gal_texture_format::B8G8R8A8_UNORM:
//        return DXGI_FORMAT_B8G8R8A8_UNORM;
//    case gal_texture_format::B8G8R8X8_UNORM:
//        return DXGI_FORMAT_B8G8R8X8_UNORM;
//    case gal_texture_format::B8G8R8A8_SRGB:
//        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
//
//    case gal_texture_format::R10G10B10A2_UNORM:
//        return DXGI_FORMAT_R10G10B10A2_UNORM;
//    case gal_texture_format::R10G10B10A2_UINT:
//        return DXGI_FORMAT_R10G10B10A2_UINT;
//
//    case gal_texture_format::R16_UNORM:
//        return DXGI_FORMAT_R16_UNORM;
//    case gal_texture_format::R16_SNORM:
//        return DXGI_FORMAT_R16_SNORM;
//    case gal_texture_format::R16_UINT:
//        return DXGI_FORMAT_R16_UINT;
//    case gal_texture_format::R16_SINT:
//        return DXGI_FORMAT_R16_SINT;
//    case gal_texture_format::R16_SFLOAT:
//        return DXGI_FORMAT_R16_FLOAT;
//    case gal_texture_format::R16G16_UNORM:
//        return DXGI_FORMAT_R16G16_UNORM;
//    case gal_texture_format::R16G16_SNORM:
//        return DXGI_FORMAT_R16G16_SNORM;
//    case gal_texture_format::R16G16_UINT:
//        return DXGI_FORMAT_R16G16_UINT;
//    case gal_texture_format::R16G16_SINT:
//        return DXGI_FORMAT_R16G16_SINT;
//    case gal_texture_format::R16G16_SFLOAT:
//        return DXGI_FORMAT_R16G16_FLOAT;
//    case gal_texture_format::R16G16B16A16_UNORM:
//        return DXGI_FORMAT_R16G16B16A16_UNORM;
//    case gal_texture_format::R16G16B16A16_SNORM:
//        return DXGI_FORMAT_R16G16B16A16_SNORM;
//    case gal_texture_format::R16G16B16A16_UINT:
//        return DXGI_FORMAT_R16G16B16A16_UINT;
//    case gal_texture_format::R16G16B16A16_SINT:
//        return DXGI_FORMAT_R16G16B16A16_SINT;
//    case gal_texture_format::R16G16B16A16_SFLOAT:
//        return DXGI_FORMAT_R16G16B16A16_FLOAT;
//    case gal_texture_format::R32_UINT:
//        return DXGI_FORMAT_R32_UINT;
//    case gal_texture_format::R32_SINT:
//        return DXGI_FORMAT_R32_SINT;
//    case gal_texture_format::R32_SFLOAT:
//        return DXGI_FORMAT_R32_FLOAT;
//    case gal_texture_format::R32G32_UINT:
//        return DXGI_FORMAT_R32G32_UINT;
//    case gal_texture_format::R32G32_SINT:
//        return DXGI_FORMAT_R32G32_SINT;
//    case gal_texture_format::R32G32_SFLOAT:
//        return DXGI_FORMAT_R32G32_FLOAT;
//    case gal_texture_format::R32G32B32_UINT:
//        return DXGI_FORMAT_R32G32B32_UINT;
//    case gal_texture_format::R32G32B32_SINT:
//        return DXGI_FORMAT_R32G32B32_SINT;
//    case gal_texture_format::R32G32B32_SFLOAT:
//        return DXGI_FORMAT_R32G32B32_FLOAT;
//    case gal_texture_format::R32G32B32A32_UINT:
//        return DXGI_FORMAT_R32G32B32A32_UINT;
//    case gal_texture_format::R32G32B32A32_SINT:
//        return DXGI_FORMAT_R32G32B32A32_SINT;
//    case gal_texture_format::R32G32B32A32_SFLOAT:
//        return DXGI_FORMAT_R32G32B32A32_FLOAT;
//    case gal_texture_format::B10G11R11_UFLOAT:
//        return DXGI_FORMAT_R11G11B10_FLOAT;
//    case gal_texture_format::E5B9G9R9_UFLOAT:
//        return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
//    case gal_texture_format::D16_UNORM:
//        return DXGI_FORMAT_D16_UNORM;
//    case gal_texture_format::X8_D24_UNORM:
//        return DXGI_FORMAT_D24_UNORM_S8_UINT;
//    case gal_texture_format::D32_SFLOAT:
//        return DXGI_FORMAT_D32_FLOAT;
//    case gal_texture_format::D24_UNORM_S8_UINT:
//        return DXGI_FORMAT_D24_UNORM_S8_UINT;
//    case gal_texture_format::D32_SFLOAT_S8_UINT:
//        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
//    case gal_texture_format::DXBC1_RGB_UNORM:
//        return DXGI_FORMAT_BC1_UNORM;
//    case gal_texture_format::DXBC1_RGB_SRGB:
//        return DXGI_FORMAT_BC1_UNORM_SRGB;
//    case gal_texture_format::DXBC1_RGBA_UNORM:
//        return DXGI_FORMAT_BC1_UNORM;
//    case gal_texture_format::DXBC1_RGBA_SRGB:
//        return DXGI_FORMAT_BC1_UNORM_SRGB;
//    case gal_texture_format::DXBC2_UNORM:
//        return DXGI_FORMAT_BC2_UNORM;
//    case gal_texture_format::DXBC2_SRGB:
//        return DXGI_FORMAT_BC2_UNORM_SRGB;
//    case gal_texture_format::DXBC3_UNORM:
//        return DXGI_FORMAT_BC3_UNORM;
//    case gal_texture_format::DXBC3_SRGB:
//        return DXGI_FORMAT_BC3_UNORM_SRGB;
//    case gal_texture_format::DXBC4_UNORM:
//        return DXGI_FORMAT_BC4_UNORM;
//    case gal_texture_format::DXBC4_SNORM:
//        return DXGI_FORMAT_BC4_SNORM;
//    case gal_texture_format::DXBC5_UNORM:
//        return DXGI_FORMAT_BC5_UNORM;
//    case gal_texture_format::DXBC5_SNORM:
//        return DXGI_FORMAT_BC5_SNORM;
//    case gal_texture_format::DXBC6H_UFLOAT:
//        return DXGI_FORMAT_BC6H_UF16;
//    case gal_texture_format::DXBC6H_SFLOAT:
//        return DXGI_FORMAT_BC6H_SF16;
//    case gal_texture_format::DXBC7_UNORM:
//        return DXGI_FORMAT_BC7_UNORM;
//    case gal_texture_format::DXBC7_SRGB:
//        return DXGI_FORMAT_BC7_UNORM_SRGB;
//        //case gal_texture_format::D16_UNORM_S8_UINT:
//        //    return DXGI_FORMAT_D16_UNORM_S8_UINT;
//
//    default:
//        return DXGI_FORMAT_UNKNOWN;
//    }
//    return DXGI_FORMAT_UNKNOWN;
//}
//
//constexpr DXGI_FORMAT dxgiformat_to_typeless(DXGI_FORMAT fmt) {
//    switch (fmt) {
//    case DXGI_FORMAT_R32G32B32A32_FLOAT:
//    case DXGI_FORMAT_R32G32B32A32_UINT:
//    case DXGI_FORMAT_R32G32B32A32_SINT:
//        return DXGI_FORMAT_R32G32B32A32_TYPELESS;
//    case DXGI_FORMAT_R32G32B32_FLOAT:
//    case DXGI_FORMAT_R32G32B32_UINT:
//    case DXGI_FORMAT_R32G32B32_SINT:
//        return DXGI_FORMAT_R32G32B32_TYPELESS;
//
//    case DXGI_FORMAT_R16G16B16A16_FLOAT:
//    case DXGI_FORMAT_R16G16B16A16_UNORM:
//    case DXGI_FORMAT_R16G16B16A16_UINT:
//    case DXGI_FORMAT_R16G16B16A16_SNORM:
//    case DXGI_FORMAT_R16G16B16A16_SINT:
//        return DXGI_FORMAT_R16G16B16A16_TYPELESS;
//
//    case DXGI_FORMAT_R32G32_FLOAT:
//    case DXGI_FORMAT_R32G32_UINT:
//    case DXGI_FORMAT_R32G32_SINT:
//        return DXGI_FORMAT_R32G32_TYPELESS;
//
//    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
//    //case DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
//    case DXGI_FORMAT_R10G10B10A2_UNORM:
//    case DXGI_FORMAT_R10G10B10A2_UINT:
//        return DXGI_FORMAT_R10G10B10A2_TYPELESS;
//
//    case DXGI_FORMAT_R8G8B8A8_UNORM:
//    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
//    case DXGI_FORMAT_R8G8B8A8_UINT:
//    case DXGI_FORMAT_R8G8B8A8_SNORM:
//    case DXGI_FORMAT_R8G8B8A8_SINT:
//        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
//    case DXGI_FORMAT_R16G16_FLOAT:
//    case DXGI_FORMAT_R16G16_UNORM:
//    case DXGI_FORMAT_R16G16_UINT:
//    case DXGI_FORMAT_R16G16_SNORM:
//    case DXGI_FORMAT_R16G16_SINT:
//        return DXGI_FORMAT_R16G16_TYPELESS;
//
//    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
//    case DXGI_FORMAT_D32_FLOAT:
//    case DXGI_FORMAT_R32_FLOAT:
//    case DXGI_FORMAT_R32_UINT:
//    case DXGI_FORMAT_R32_SINT:
//        return DXGI_FORMAT_R32_TYPELESS;
//    case DXGI_FORMAT_R8G8_UNORM:
//    case DXGI_FORMAT_R8G8_UINT:
//    case DXGI_FORMAT_R8G8_SNORM:
//    case DXGI_FORMAT_R8G8_SINT:
//        return DXGI_FORMAT_R8G8_TYPELESS;
//    case DXGI_FORMAT_B4G4R4A4_UNORM: // just treats a 16 raw bits
//    case DXGI_FORMAT_D16_UNORM:
//    case DXGI_FORMAT_R16_FLOAT:
//    case DXGI_FORMAT_R16_UNORM:
//    case DXGI_FORMAT_R16_UINT:
//    case DXGI_FORMAT_R16_SNORM:
//    case DXGI_FORMAT_R16_SINT:
//        return DXGI_FORMAT_R16_TYPELESS;
//    case DXGI_FORMAT_A8_UNORM:
//    case DXGI_FORMAT_R8_UNORM:
//    case DXGI_FORMAT_R8_UINT:
//    case DXGI_FORMAT_R8_SNORM:
//    //case DXGI_FORMAT_R4G4_UNORM:
//    case DXGI_FORMAT_R8_SINT:
//        return DXGI_FORMAT_R8_TYPELESS;
//    case DXGI_FORMAT_BC1_UNORM:
//    case DXGI_FORMAT_BC1_UNORM_SRGB:
//        return DXGI_FORMAT_BC1_TYPELESS;
//    case DXGI_FORMAT_BC2_UNORM:
//    case DXGI_FORMAT_BC2_UNORM_SRGB:
//        return DXGI_FORMAT_BC2_TYPELESS;
//    case DXGI_FORMAT_BC3_UNORM:
//    case DXGI_FORMAT_BC3_UNORM_SRGB:
//        return DXGI_FORMAT_BC3_TYPELESS;
//    case DXGI_FORMAT_BC4_UNORM:
//    case DXGI_FORMAT_BC4_SNORM:
//        return DXGI_FORMAT_BC4_TYPELESS;
//    case DXGI_FORMAT_BC5_UNORM:
//    case DXGI_FORMAT_BC5_SNORM:
//        return DXGI_FORMAT_BC5_TYPELESS;
//    case DXGI_FORMAT_B5G6R5_UNORM:
//    case DXGI_FORMAT_B5G5R5A1_UNORM:
//        return DXGI_FORMAT_R16_TYPELESS;
//
//    case DXGI_FORMAT_R11G11B10_FLOAT:
//        return DXGI_FORMAT_R11G11B10_FLOAT;
//
//    case DXGI_FORMAT_B8G8R8X8_UNORM:
//    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
//        return DXGI_FORMAT_B8G8R8X8_TYPELESS;
//
//    case DXGI_FORMAT_B8G8R8A8_UNORM:
//    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
//        return DXGI_FORMAT_B8G8R8A8_TYPELESS;
//
//    case DXGI_FORMAT_BC6H_UF16:
//    case DXGI_FORMAT_BC6H_SF16:
//        return DXGI_FORMAT_BC6H_TYPELESS;
//
//    case DXGI_FORMAT_BC7_UNORM:
//    case DXGI_FORMAT_BC7_UNORM_SRGB:
//        return DXGI_FORMAT_BC7_TYPELESS;
//
//    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
//    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
//        return DXGI_FORMAT_R32G8X24_TYPELESS;
//    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
//        return DXGI_FORMAT_R24G8_TYPELESS;
//    case DXGI_FORMAT_D24_UNORM_S8_UINT:
//        return DXGI_FORMAT_R24G8_TYPELESS;
//
//        // typeless just return the input format
//    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
//    case DXGI_FORMAT_R32G32B32_TYPELESS:
//    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
//    case DXGI_FORMAT_R32G32_TYPELESS:
//    case DXGI_FORMAT_R32G8X24_TYPELESS:
//    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
//    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
//    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
//    case DXGI_FORMAT_R16G16_TYPELESS:
//    case DXGI_FORMAT_R32_TYPELESS:
//    case DXGI_FORMAT_R24G8_TYPELESS:
//    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
//    case DXGI_FORMAT_R8G8_TYPELESS:
//    case DXGI_FORMAT_R16_TYPELESS:
//    case DXGI_FORMAT_R8_TYPELESS:
//    case DXGI_FORMAT_BC1_TYPELESS:
//    case DXGI_FORMAT_BC2_TYPELESS:
//    case DXGI_FORMAT_BC3_TYPELESS:
//    case DXGI_FORMAT_BC4_TYPELESS:
//    case DXGI_FORMAT_BC5_TYPELESS:
//    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
//    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
//    case DXGI_FORMAT_BC6H_TYPELESS:
//    case DXGI_FORMAT_BC7_TYPELESS:
//        return fmt;
//
//    case DXGI_FORMAT_R1_UNORM:
//    case DXGI_FORMAT_R8G8_B8G8_UNORM:
//    case DXGI_FORMAT_G8R8_G8B8_UNORM:
//    case DXGI_FORMAT_AYUV:
//    case DXGI_FORMAT_Y410:
//    case DXGI_FORMAT_Y416:
//    case DXGI_FORMAT_NV12:
//    case DXGI_FORMAT_P010:
//    case DXGI_FORMAT_P016:
//    case DXGI_FORMAT_420_OPAQUE:
//    case DXGI_FORMAT_YUY2:
//    case DXGI_FORMAT_Y210:
//    case DXGI_FORMAT_Y216:
//    case DXGI_FORMAT_NV11:
//    case DXGI_FORMAT_AI44:
//    case DXGI_FORMAT_IA44:
//    case DXGI_FORMAT_P8:
//    case DXGI_FORMAT_A8P8:
//    case DXGI_FORMAT_P208:
//    case DXGI_FORMAT_V208:
//    case DXGI_FORMAT_V408:
//    //case DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
//    //case DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
//    //case DXGI_FORMAT_D16_UNORM_S8_UINT:
//    //case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
//    //case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
//    case DXGI_FORMAT_UNKNOWN:
//        return DXGI_FORMAT_UNKNOWN;
//    }
//    return DXGI_FORMAT_UNKNOWN;
//}
//
//constexpr void load_gal_d3d12_functions() {
//    //#define D3D12_LOAD_FUNCTION_PTRS
//    //#include "../helper/helper_macro.h"
//    //#undef D3D12_LOAD_FUNCTION_PTRS
//}
//
//constexpr void offload_gal_d3d12_functions() {
//    //#define D3D12_OFFLOAD_FUNCTION_PTRS
//    //#include "../helper/helper_macro.h"
//    //#undef D3D12_OFFLOAD_FUNCTION_PTRS
//}
//
//gal_error_code d3d12_init_gal(gal_context *context) {
//    *context = reinterpret_cast<gal_context>(new d3d12_context);
//    load_gal_d3d12_functions();
//    if (*context == gal_null) {
//        return gal_error_code::ERR;
//    }
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_gal(gal_context context) {
//    if (context != gal_null) {
//        d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
//        delete d3d12_ctx;
//        context = gal_null;
//        offload_gal_d3d12_functions();
//    }
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_create_instance(gal_desc *desc, gal_context *context) {
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
//
//    u32 dxgi_factory_flags = 0;
//    if (desc->b_debug_layer) {
//        ID3D12Debug1 *pDebugController;
//        if (D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController)) == S_OK) {
//            // Enabling GPU Validation without enabling the debug layer does nothing
//
//            pDebugController->EnableDebugLayer();
//            pDebugController->SetEnableGPUBasedValidation(true);
//
//            pDebugController->Release();
//        }
//
//        dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
//    }
//
//    // require directx12.1
//    HRESULT hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&d3d12_ctx->factory));
//    if (FAILED(hr) || d3d12_ctx->factory == nullptr) {
//        return gal_error_code::ERR;
//    }
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_destroy_instance(gal_context *context) {
//
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
//    if (d3d12_ctx->factory != nullptr) {
//        d3d12_ctx->factory->Release();
//        d3d12_ctx->factory = nullptr;
//    }
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_create_device(gal_desc *gal_desc, gal_context *context) {
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
//
//    auto pick_gpu = [&](IDXGIFactory6 *factory, IDXGIAdapter4 **gpu, ID3D12Device **device) -> gal_error_code {
//        IDXGIAdapter4 *adapter = NULL;
//        IDXGIFactory6 *factory6;
//
//        HRESULT hr;
//
//        hr = (factory->QueryInterface(IID_PPV_ARGS(&factory6)));
//        if (FAILED(hr)) {
//            return gal_error_code::unsupported_device;
//        }
//
//        // Find number of usable GPUs
//        // Use DXGI6 interface which lets us specify gpu preference so we dont need
//        // to use NVOptimus or AMDPowerExpress exports
//        for (u32 i = 0; DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
//                                                    i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
//             ++i) {
//            DXGI_ADAPTER_DESC3 desc;
//            adapter->GetDesc3(&desc);
//            ID3D12Device *t_device;
//
//            if (SUCCEEDED(D3D12CreateDevice(adapter, ANT_DX12_API_VERSION, IID_PPV_ARGS(&t_device)))) {
//                // if (SUCCEEDED(D3D12CreateDevice(adapter, ANT_DX12_API_VERSION, _uuidof(ID3D12Device), nullptr))) {
//
//                auto check_features = [&]() -> gal_error_code {
//                    HRESULT hr = 0;
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS featureDataOptions = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureDataOptions,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS1 featureDataOptions1 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &featureDataOptions1,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    bool b_wave_ops = featureDataOptions1.WaveOps;
//                    if (!b_wave_ops) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS2 featureDataOptions2 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &featureDataOptions2,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS2)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS3 featureDataOptions3 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &featureDataOptions3,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS3)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS4 featureDataOptions4 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &featureDataOptions4,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS4)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureDataOptions5 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureDataOptions5,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    bool b_rt11Supported = (featureDataOptions5.RaytracingTier & D3D12_RAYTRACING_TIER_1_1) != 0;
//                    if (!b_rt11Supported) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS6 featureDataOptions6 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &featureDataOptions6,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS6)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    if (gal_desc->b_variable_rate_shading) {
//                        bool b_vrs2Supported =
//                            (featureDataOptions6.VariableShadingRateTier & D3D12_VARIABLE_SHADING_RATE_TIER_2) != 0;
//                        if (!b_vrs2Supported) {
//                            return gal_error_code::unsupported_device;
//                        }
//                    }
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS7 featureDataOptions7 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &featureDataOptions7,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS7)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    if (gal_desc->b_mesh_shader) {
//                        bool b_mesh_shader = (featureDataOptions7.MeshShaderTier & D3D12_MESH_SHADER_TIER_1) != 0;
//                        if (!b_mesh_shader) {
//                            return gal_error_code::unsupported_device;
//                        }
//                    }
//
//                    // FIXME(hyl5): these code may not compile on some windows sdks
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS8 featureDataOptions8 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS8, &featureDataOptions8,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS8)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS9 featureDataOptions9 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &featureDataOptions9,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS9)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    if (gal_desc->b_mesh_shader) {
//                        bool b_mesh_shader_pipeline = featureDataOptions9.MeshShaderPipelineStatsSupported;
//                        if (!b_mesh_shader_pipeline) {
//                            return gal_error_code::unsupported_device;
//                        }
//                    }
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS10 featureDataOptions10 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS10, &featureDataOptions10,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS10)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    D3D12_FEATURE_DATA_D3D12_OPTIONS11 featureDataOptions11 = {};
//                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &featureDataOptions11,
//                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS11)));
//                    if (FAILED(hr)) {
//                        return gal_error_code::unsupported_device;
//                    }
//
//                    return gal_error_code::SUC;
//                };
//
//                if (check_features() != gal_error_code::SUC) {
//                    t_device->Release();
//                    continue;
//                }
//                *gpu = adapter;
//                *device = t_device;
//                return gal_error_code::SUC;
//            }
//        }
//        return gal_error_code::unsupported_device;
//    };
//
//    gal_error_code result = pick_gpu(d3d12_ctx->factory, &d3d12_ctx->active_gpu, &d3d12_ctx->device);
//
//    if (result != gal_error_code::SUC || d3d12_ctx->active_gpu == nullptr || d3d12_ctx->device == nullptr) {
//        return gal_error_code::unsupported_device;
//    }
//    // create queue
//    D3D12_COMMAND_QUEUE_DESC graphics_command_queue_desc{};
//    graphics_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//    graphics_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//
//    HRESULT hr =
//        (d3d12_ctx->device->CreateCommandQueue(&graphics_command_queue_desc, IID_PPV_ARGS(&d3d12_ctx->graphics_queue)));
//    if (FAILED(hr) || d3d12_ctx->graphics_queue == nullptr) {
//        return gal_error_code::ERR;
//    }
//    if (gal_desc->b_async_compute) {
//        D3D12_COMMAND_QUEUE_DESC compute_command_queue_desc{};
//        compute_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//        compute_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
//
//        hr = (d3d12_ctx->device->CreateCommandQueue(&compute_command_queue_desc,
//                                                    IID_PPV_ARGS(&d3d12_ctx->compute_queue)));
//
//        if (FAILED(hr) || d3d12_ctx->compute_queue == nullptr) {
//            return gal_error_code::ERR;
//        }
//    } else {
//        d3d12_ctx->compute_queue = d3d12_ctx->graphics_queue;
//    }
//    if (gal_desc->b_async_transfer) {
//        D3D12_COMMAND_QUEUE_DESC transfer_command_queue_desc{};
//        transfer_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//        transfer_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
//
//        hr = (d3d12_ctx->device->CreateCommandQueue(&transfer_command_queue_desc,
//                                                    IID_PPV_ARGS(&d3d12_ctx->transfer_queue)));
//        if (FAILED(hr) || d3d12_ctx->transfer_queue == nullptr) {
//            return gal_error_code::ERR;
//        }
//    } else {
//
//        d3d12_ctx->transfer_queue = d3d12_ctx->graphics_queue;
//    }
//    if (result == gal_error_code::SUC && d3d12_ctx->active_gpu != nullptr && d3d12_ctx->device != nullptr) {
//        return gal_error_code::SUC;
//    } else {
//        return gal_error_code::ERR;
//    }
//}
//
//gal_error_code d3d12_destroy_device(gal_context *context) {
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
//    if (d3d12_ctx->device != nullptr) {
//        d3d12_ctx->device->Release();
//        d3d12_ctx->device = nullptr;
//    }
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_create_memory_allocator(gal_context *context) {
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
//    D3D12MA::ALLOCATOR_DESC allocatorDesc{};
//    allocatorDesc.pDevice = d3d12_ctx->device;
//    allocatorDesc.pAdapter = d3d12_ctx->active_gpu;
//
//    HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &d3d12_ctx->d3dma_allocator);
//
//    if (FAILED(hr) || d3d12_ctx->d3dma_allocator == nullptr) {
//        return gal_error_code::ERR;
//    }
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_destroy_memory_allocator(gal_context *context) {
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
//    if (d3d12_ctx->d3dma_allocator != nullptr) {
//        d3d12_ctx->d3dma_allocator->Release();
//        d3d12_ctx->d3dma_allocator = nullptr;
//    }
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_create_buffer(gal_context context, gal_buffer_desc *desc, gal_buffer *buffer) {
//    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
//    *buffer = reinterpret_cast<gal_handle>(new ante::gal::d3d12_buffer);
//    d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(*buffer);
//
//    CD3DX12_RESOURCE_DESC buffer_desc =
//        CD3DX12_RESOURCE_DESC::Buffer(desc->size, D3D12_RESOURCE_FLAG_NONE, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
//
//    buffer_desc.Flags = util_to_dx12_resource_flags(desc);
//
//    D3D12MA::ALLOCATION_DESC allocation_desc{};
//
//    D3D12_RESOURCE_STATES initial_state = util_to_dx12_resource_state(desc->initial_state);
//
//    if (desc->memory_flag == gal_memory_flag::GPU_DEDICATED) {
//        allocation_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
//    } else if (desc->memory_flag == gal_memory_flag::CPU_UPLOAD) {
//        allocation_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
//        initial_state = D3D12_RESOURCE_STATE_GENERIC_READ; // genric read is a required start state for upload heap
//    } else if (desc->memory_flag == gal_memory_flag::GPU_DOWNLOAD) {
//        allocation_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
//        initial_state = D3D12_RESOURCE_STATE_COPY_DEST;
//    } else {
//        return gal_error_code::invalid_parameter;
//    }
//    if (desc->flags & gal_buffer_flag::OWN_MEMORY) {
//        allocation_desc.Flags |= D3D12MA::ALLOCATION_FLAGS::ALLOCATION_FLAG_COMMITTED;
//    }
//
//    HRESULT hr = (d3d12_ctx->d3dma_allocator->CreateResource(&allocation_desc, &buffer_desc, initial_state, NULL,
//                                                             &d3d12_buf->allocation, IID_PPV_ARGS(&d3d12_buf->buffer)));
//
//    if (FAILED(hr) || d3d12_buf->allocation == nullptr) {
//        return gal_error_code::ERR;
//    }
//
//    // create descriptor
//    //if (!desc->flags & buffer_creation_flag::NO_DESCRIPTOR_VIEW_CREATION) {
//    //    //TODO(hyl5): create resource descriptor
//    //}
//
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_destroy_buffer([[maybe_unused]] gal_context context, gal_buffer buffer) {
//    if (buffer != nullptr) {
//        d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(buffer);
//        d3d12_buf->buffer->Release();
//        d3d12_buf->allocation->Release();
//        delete d3d12_buf;
//        buffer = nullptr;
//    }
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_create_texture(gal_context context, gal_texture_desc *desc, gal_texture *texture) {
//    // TODO(hyl5): finish this
//    if (context)
//        return gal_error_code::SUC;
//    else if (texture)
//        return gal_error_code::SUC;
//    else if (desc)
//        return gal_error_code::SUC;
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_destroy_texture(gal_context context, gal_texture texture) {
//    // TODO(hyl5): finish this
//    if (context)
//        return gal_error_code::SUC;
//    else if (texture)
//        return gal_error_code::SUC;
//    return gal_error_code::SUC;
//}
//
//gal_error_code d3d12_create_sampler(gal_context context, gal_sampler_desc* sampler_desc, gal_sampler* sampler){
//    if (context)
//        return gal_error_code::SUC;
//    else if (sampler)
//        return gal_error_code::SUC;
//    else if (sampler_desc)
//        return gal_error_code::SUC;
//    return gal_error_code::SUC;
//    //    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
//    //*sampler = reinterpret_cast<gal_handle>(new ante::gal::d3d12_sampler);
//    //d3d12_buffer *d3d12_spl = reinterpret_cast<d3d12_buffer *>(*sampler);
//    //// d3d12_context *d3d12_ctx;
//    //d3d12_ctx->device->CreateSampler();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_sampler() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_create_render_target() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_render_target() {
//    // d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//// surface
//gal_error_code d3d12_create_swap_chain() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_ctx->factory->CreateSwapChain();
////    
////	SwapChain* pSwapChain = (SwapChain*)tf_calloc(1, sizeof(SwapChain) + pDesc->mImageCount * sizeof(RenderTarget*));
////	ASSERT(pSwapChain);
////	pSwapChain->ppRenderTargets = (RenderTarget**)(pSwapChain + 1);
////	ASSERT(pSwapChain->ppRenderTargets);
////
////	pSwapChain->mD3D12.mDxSyncInterval = pDesc->mEnableVsync ? 1 : 0;
////
////	DXGI_SWAP_CHAIN_DESC1 desc = {};
////	desc.Width = pDesc->mWidth;
////	desc.Height = pDesc->mHeight;
////	desc.Format = util_to_dx12_swap_chain_format(pDesc->mColorFormat);
////	desc.Stereo = false;
////	desc.SampleDesc.Count = 1;    // If multisampling is needed, we'll resolve it later
////	desc.SampleDesc.Quality = 0;
////	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////	desc.BufferCount = pDesc->mImageCount;
////	desc.Scaling = DXGI_SCALING_STRETCH;
////	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
////	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
////	desc.Flags = 0;
////
////	BOOL allowTearing = FALSE;
////	pRenderer->mD3D12.pDXGIFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
////	desc.Flags |= allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
////
////	pSwapChain->mD3D12.mFlags |= (!pDesc->mEnableVsync && allowTearing) ? DXGI_PRESENT_ALLOW_TEARING : 0;
////
////	IDXGISwapChain1* swap_chain;
////
////	HWND hwnd = (HWND)pDesc->mWindowHandle.window;
////
////	(pRenderer->mD3D12.pDXGIFactory->CreateSwapChainForHwnd(
////		pDesc->ppPresentQueues[0]->mD3D12.pDxQueue, hwnd, &desc, NULL, NULL, &swap_chain));
////
////	CHECK_HRESULT(pRenderer->mD3D12.pDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
////
////	CHECK_HRESULT(swap_chain->QueryInterface(IID_ARGS(&pSwapChain->mD3D12.pDxSwapChain)));
////	swap_chain->Release();
////
////
////	ID3D12Resource** buffers = (ID3D12Resource**)alloca(pDesc->mImageCount * sizeof(ID3D12Resource*));
////
////	// Create rendertargets from swap_chain
////	for (uint32_t i = 0; i < pDesc->mImageCount; ++i)
////	{
////		(pSwapChain->mD3D12.pDxSwapChain->GetBuffer(i, IID_ARGS(&buffers[i])));
////	}
////
////
////	RenderTargetDesc descColor = {};
////	descColor.mWidth = pDesc->mWidth;
////	descColor.mHeight = pDesc->mHeight;
////	descColor.mDepth = 1;
////	descColor.mArraySize = 1;
////	descColor.mFormat = pDesc->mColorFormat;
////	descColor.mClearValue = pDesc->mColorClearValue;
////	descColor.mSampleCount = SAMPLE_COUNT_1;
////	descColor.mSampleQuality = 0;
////	descColor.pNativeHandle = NULL;
////	descColor.mFlags = TEXTURE_CREATION_FLAG_ALLOW_DISPLAY_TARGET;
////	descColor.mStartState = RESOURCE_STATE_PRESENT;
////#if defined(XBOX)
////	descColor.mFlags |= TEXTURE_CREATION_FLAG_OWN_MEMORY_BIT;
////	pSwapChain->mD3D12.pPresentQueue = pDesc->mPresentQueueCount ? pDesc->ppPresentQueues[0] : NULL;
////#endif
////
////	for (uint32_t i = 0; i < pDesc->mImageCount; ++i)
////	{
////#if !defined(XBOX)
////		descColor.pNativeHandle = (void*)buffers[i];
////#endif
////		::addRenderTarget(pRenderer, &descColor, &pSwapChain->ppRenderTargets[i]);
////	}
//
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_swap_chain() { return gal_error_code::SUC; }
//gal_error_code d3d12_create_surface() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_surface() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//// pipeline
//gal_error_code d3d12_create_shader() {
//    //d3d12_context *d3d12_ctx;
//
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_shader() { return gal_error_code::SUC; }
//gal_error_code d3d12_create_pipeline() {
//    // d3d12_context *d3d12_ctx;
//    //d3d12_ctx->device->CreateGraphicsPipelineState();
//    //d3d12_ctx->device->CreateComputePipelineState();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_pipeline() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_create_descriptor_pool() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_ctx->device->CreateDescriptorHeap();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_descriptor_pool() {
//    //d3d12_context *d3d12_ctx;
//
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_consume_descriptor_set() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_free_descriptor_set() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_create_root_signature() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_ctx->device->Createroot_signature();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_root_signature() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//// sync
//gal_error_code d3d12_create_fence() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_fence *ded12_fc;
//    //(d3d12_ctx->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&ded12_fc->fence)));
//    //ded12_fc->fence_value = 1;
//
//    //ded12_fc->wait_idle_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
//    //if (ded12_fc->wait_idle_fence_event == nullptr) {
//    //    (HRESULT_FROM_WIN32(GetLastError()));
//    //}
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_wait_fence() { return gal_error_code::SUC; }
//gal_error_code d3d12_destroy_fence() {
//    //    (gpu_fence->Release());
//    //CloseHandle(wait_idle_fence_event);
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_wait_gpu() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_create_semaphore() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_semaphore() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//// cmds
//gal_error_code d3d12_create_command_pool() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_ctx->device->CreateCommandAllocator();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_reset_command_pool() {
//    //d3d12_context *d3d12_ctx;
//
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_destroy_command_pool() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_allocate_command_list() {
//    //d3d12_context *d3d12_ctx;
//
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_free_command_list() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->Release();
//    //d3d12_cmd->command_list = nullptr;
//
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_begin() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->Reset();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_end() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->Close();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_resource_barrier() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->ResourceBarrier();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_bind_index_buffer() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->IASetIndexBuffer();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_bind_vertex_buffer() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->IASetVertexBuffers();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_bind_descriptor_set() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_bind_pipeline() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->SetPipelineState();
//    //d3d12_cmd->command_list->SetPipelineState1();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_begin_renderpass() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->BeginRenderPass();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_end_renderpass() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->EndRenderPass();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_dispatch() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //
//    //d3d12_cmd->command_list->Dispatch();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_dispatch_indirect() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->ExecuteIndirect();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_draw_instanced() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->DrawInstanced();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_draw_indexed_instanced() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->DrawIndexedInstanced();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_draw_indirect_instanced() {
//    // d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_draw_indirect_indexed_instanced() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_draw_mesh_task() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->DispatchMesh();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_draw_indirect_mesh_task() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_copy_texture() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->CopyBufferRegion();
//    //d3d12_cmd->command_list->CopyResource();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_copy_buffer() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_cmd->command_list->CopyTextureRegion();
//    //d3d12_cmd->command_list->CopyResource();
//    return gal_error_code::SUC;
//}
//// TODO(hyl5): deprecate the method, use compute shader instead
//gal_error_code d3d12_cmd_fill_buffer() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->ClearUnorderedAccessViewFloat();
//    //d3d12_cmd->command_list->ClearUnorderedAccessViewUint();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_fill_texture() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->ClearUnorderedAccessViewFloat();
//    //d3d12_cmd->command_list->ClearUnorderedAccessViewUint();
//    //d3d12_cmd->command_list->ClearRenderTargetView();
//    //d3d12_cmd->command_list->ClearDepthStencilView();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_upload_buffer() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_upload_texture() {
//    //d3d12_context *d3d12_ctx;
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_copy_buffer_to_texture() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->CopyBufferRegion();
//    return gal_error_code::SUC;
//}
//gal_error_code d3d12_cmd_copy_texture_to_buffer() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_command_list *d3d12_cmd;
//    //d3d12_cmd->command_list->CopyTextureRegion();
//    return gal_error_code::SUC;
//}
//#endif
//} // namespace ante::gal
