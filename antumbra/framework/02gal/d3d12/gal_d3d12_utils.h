
#pragma once

#include "../gal.h"
#include "gal_d3d12_enum.h"

namespace ante::gal {

void load_gal_d3d12_functions() {
    #define D3D12_LOAD_FUNCTION_PTRS
    #include "../helper/helper_macro.h"
    #undef D3D12_LOAD_FUNCTION_PTRS
}

void offload_gal_d3d12_functions() {
    #define D3D12_OFFLOAD_FUNCTION_PTRS
    #include "../helper/helper_macro.h"
    #undef D3D12_OFFLOAD_FUNCTION_PTRS
}
constexpr D3D12_RESOURCE_STATES util_to_dx12_resource_state(gal_resource_state state) {
    D3D12_RESOURCE_STATES ret = D3D12_RESOURCE_STATE_COMMON;

    // These states cannot be combined with other states so we just do an == check
    if (state == gal_resource_state::COMMON)
        return D3D12_RESOURCE_STATE_COMMON;
    if (state == gal_resource_state::PRESENT)
        return D3D12_RESOURCE_STATE_PRESENT;

    if ((state & gal_resource_state::VERTEX_BUFFER) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if ((state & gal_resource_state::CONSTANT_BUFFER) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if ((state & gal_resource_state::INDEX_BUFFER) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
    if ((state & gal_resource_state::RENDER_TARGET) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_RENDER_TARGET;
    if ((state & gal_resource_state::RW_BUFFER) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if ((state & gal_resource_state::RW_TEXTURE) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if ((state & gal_resource_state::DEPTH_WRITE) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
    if ((state & gal_resource_state::DEPTH_READ) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_DEPTH_READ;
    if ((state & gal_resource_state::INDIRECT_BUFFER) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
    if ((state & gal_resource_state::COPY_DST) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_COPY_DEST;
    if ((state & gal_resource_state::COPY_SRC) != gal_resource_state::UNDEFINIED)
        ret |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if ((state & gal_resource_state::TEXTURE) != gal_resource_state::UNDEFINIED)
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

constexpr D3D12_RESOURCE_FLAGS util_to_dx12_resource_flags(gal_buffer_desc *desc) {
    D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

    if ((desc->descriptor_types & gal_descriptor_type::RW_BUFFER) != gal_descriptor_type::UNDEFINED) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if ((desc->descriptor_types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if ((desc->descriptor_types & gal_descriptor_type::COLOR_RT) != gal_descriptor_type::UNDEFINED) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if ((desc->descriptor_types & gal_descriptor_type::DEPTH_STENCIL_RT) != gal_descriptor_type::UNDEFINED) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    if ((desc->memory_flags & gal_memory_flag::GPU_DOWNLOAD) != gal_memory_flag::UNDEFINED) {
        flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }
    return flags;
}

constexpr gal_texture_format FromDXGI_FORMAT(DXGI_FORMAT fmt) {
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

constexpr DXGI_FORMAT galtextureformat_to_dxgiformat(gal_texture_format fmt) {
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

constexpr DXGI_FORMAT dxgiformat_to_typeless(DXGI_FORMAT fmt) {
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

}