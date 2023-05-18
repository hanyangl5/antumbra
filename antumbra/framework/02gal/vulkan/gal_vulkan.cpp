#include "gal_vulkan.h"

#include <optional>

#define VMA_RECORDING_ENABLED 1
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#define VMA_IMPLEMENTATION 1
#include <vk_mem_alloc.h>

#include "framework/01core/logging/log.h"

#define ANT_VK_API_VERSION VK_API_VERSION_1_3

namespace ant::gal {

// gal_error_code vk_result_to_gal_error_code(VkResult res) {

//}
// TODO(hyl5): chain macro to enable physical device features
// #define ADD_TO_NEXT_CHAIN(condition, next)         \
//    if ((condition)) {                             \
//        base->pNext = (VkBaseOutStructure *)&next; \
//        base = (VkBaseOutStructure *)base->pNext;  \
//    }

struct vk_context {
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice active_gpu = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VmaAllocator vma_allocator = nullptr;

    // queues
    u32 graphcis_queue_family_index = 0;
    u32 compute_queue_family_index = 0;
    u32 transfer_queue_family_index = 0;
    f32 default_queue_property = 0.0f;
    VkQueue graphics_queue = VK_NULL_HANDLE;
    VkQueue compute_queue = VK_NULL_HANDLE;
    VkQueue transfer_queue = VK_NULL_HANDLE;
};

struct vk_buffer {
    VkBuffer buffer;
    VmaAllocation allocation;
};

struct vk_texture {
    VkImage image;
    VkImageView image_view;
    VmaAllocation allocation;
};

VkBufferUsageFlags util_to_vk_buffer_usage(gal_resource_types types) {
    VkBufferUsageFlags flags = 0;

    if (types & gal_resource_type::rt_constant_buffer) {
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    if (types & gal_resource_type::rt_rw_buffer) {
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }

    if (types & gal_resource_type::rt_vertex_buffer) {
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    if (types & gal_resource_type::rt_index_buffer) {
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    if (types & gal_resource_type::rt_indirect_buffer) {
        flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    }

    // TODO(hyl5): rt;
    return flags;
}

VkImageType utils_to_vk_image_type(gal_texture_dimension dimension) {
    switch (dimension) {
    case ant::gal::gal_texture_dimension::td_1D:
        return VK_IMAGE_TYPE_1D;
    case ant::gal::gal_texture_dimension::td_2D:
        return VK_IMAGE_TYPE_2D;
    case ant::gal::gal_texture_dimension::td_3D:
        return VK_IMAGE_TYPE_3D;
    default:
        return VK_IMAGE_TYPE_MAX_ENUM;
    }
}

VkFormat utils_to_vk_image_format(gal_texture_format format) {
    switch (format) {
    case ant::gal::gal_texture_format::UNDEFINED:
        return VK_FORMAT_MAX_ENUM;
        break;
    case ant::gal::gal_texture_format::R1_UNORM:
        break;
    case ant::gal::gal_texture_format::R2_UNORM:
        break;
    case ant::gal::gal_texture_format::R4_UNORM:
        break;
    case ant::gal::gal_texture_format::R4G4_UNORM:
        break;
    case ant::gal::gal_texture_format::G4R4_UNORM:
        break;
    case ant::gal::gal_texture_format::A8_UNORM:
        break;
    case ant::gal::gal_texture_format::R8_UNORM:
        break;
    case ant::gal::gal_texture_format::R8_SNORM:
        break;
    case ant::gal::gal_texture_format::R8_UINT:
        break;
    case ant::gal::gal_texture_format::R8_SINT:
        break;
    case ant::gal::gal_texture_format::R8_SRGB:
        break;
    case ant::gal::gal_texture_format::B2G3R3_UNORM:
        break;
    case ant::gal::gal_texture_format::R4G4B4A4_UNORM:
        break;
    case ant::gal::gal_texture_format::R4G4B4X4_UNORM:
        break;
    case ant::gal::gal_texture_format::B4G4R4A4_UNORM:
        break;
    case ant::gal::gal_texture_format::B4G4R4X4_UNORM:
        break;
    case ant::gal::gal_texture_format::A4R4G4B4_UNORM:
        break;
    case ant::gal::gal_texture_format::X4R4G4B4_UNORM:
        break;
    case ant::gal::gal_texture_format::A4B4G4R4_UNORM:
        break;
    case ant::gal::gal_texture_format::X4B4G4R4_UNORM:
        break;
    case ant::gal::gal_texture_format::R5G6B5_UNORM:
        break;
    case ant::gal::gal_texture_format::B5G6R5_UNORM:
        break;
    case ant::gal::gal_texture_format::R5G5B5A1_UNORM:
        break;
    case ant::gal::gal_texture_format::B5G5R5A1_UNORM:
        break;
    case ant::gal::gal_texture_format::A1B5G5R5_UNORM:
        break;
    case ant::gal::gal_texture_format::A1R5G5B5_UNORM:
        break;
    case ant::gal::gal_texture_format::R5G5B5X1_UNORM:
        break;
    case ant::gal::gal_texture_format::B5G5R5X1_UNORM:
        break;
    case ant::gal::gal_texture_format::X1R5G5B5_UNORM:
        break;
    case ant::gal::gal_texture_format::X1B5G5R5_UNORM:
        break;
    case ant::gal::gal_texture_format::B2G3R3A8_UNORM:
        break;
    case ant::gal::gal_texture_format::R8G8_UNORM:
        break;
    case ant::gal::gal_texture_format::R8G8_SNORM:
        break;
    case ant::gal::gal_texture_format::G8R8_UNORM:
        break;
    case ant::gal::gal_texture_format::G8R8_SNORM:
        break;
    case ant::gal::gal_texture_format::R8G8_UINT:
        break;
    case ant::gal::gal_texture_format::R8G8_SINT:
        break;
    case ant::gal::gal_texture_format::R8G8_SRGB:
        break;
    case ant::gal::gal_texture_format::R16_UNORM:
        break;
    case ant::gal::gal_texture_format::R16_SNORM:
        break;
    case ant::gal::gal_texture_format::R16_UINT:
        break;
    case ant::gal::gal_texture_format::R16_SINT:
        break;
    case ant::gal::gal_texture_format::R16_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R16_SBFLOAT:
        break;
    case ant::gal::gal_texture_format::R8G8B8_UNORM:
        break;
    case ant::gal::gal_texture_format::R8G8B8_SNORM:
        break;
    case ant::gal::gal_texture_format::R8G8B8_UINT:
        break;
    case ant::gal::gal_texture_format::R8G8B8_SINT:
        break;
    case ant::gal::gal_texture_format::R8G8B8_SRGB:
        break;
    case ant::gal::gal_texture_format::B8G8R8_UNORM:
        break;
    case ant::gal::gal_texture_format::B8G8R8_SNORM:
        break;
    case ant::gal::gal_texture_format::B8G8R8_UINT:
        break;
    case ant::gal::gal_texture_format::B8G8R8_SINT:
        break;
    case ant::gal::gal_texture_format::B8G8R8_SRGB:
        break;
    case ant::gal::gal_texture_format::R8G8B8A8_UNORM:
        break;
    case ant::gal::gal_texture_format::R8G8B8A8_SNORM:
        break;
    case ant::gal::gal_texture_format::R8G8B8A8_UINT:
        break;
    case ant::gal::gal_texture_format::R8G8B8A8_SINT:
        break;
    case ant::gal::gal_texture_format::R8G8B8A8_SRGB:
        break;
    case ant::gal::gal_texture_format::B8G8R8A8_UNORM:
        break;
    case ant::gal::gal_texture_format::B8G8R8A8_SNORM:
        break;
    case ant::gal::gal_texture_format::B8G8R8A8_UINT:
        break;
    case ant::gal::gal_texture_format::B8G8R8A8_SINT:
        break;
    case ant::gal::gal_texture_format::B8G8R8A8_SRGB:
        break;
    case ant::gal::gal_texture_format::R8G8B8X8_UNORM:
        break;
    case ant::gal::gal_texture_format::B8G8R8X8_UNORM:
        break;
    case ant::gal::gal_texture_format::R16G16_UNORM:
        break;
    case ant::gal::gal_texture_format::G16R16_UNORM:
        break;
    case ant::gal::gal_texture_format::R16G16_SNORM:
        break;
    case ant::gal::gal_texture_format::G16R16_SNORM:
        break;
    case ant::gal::gal_texture_format::R16G16_UINT:
        break;
    case ant::gal::gal_texture_format::R16G16_SINT:
        break;
    case ant::gal::gal_texture_format::R16G16_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R16G16_SBFLOAT:
        break;
    case ant::gal::gal_texture_format::R32_UINT:
        break;
    case ant::gal::gal_texture_format::R32_SINT:
        break;
    case ant::gal::gal_texture_format::R32_SFLOAT:
        break;
    case ant::gal::gal_texture_format::A2R10G10B10_UNORM:
        break;
    case ant::gal::gal_texture_format::A2R10G10B10_UINT:
        break;
    case ant::gal::gal_texture_format::A2R10G10B10_SNORM:
        break;
    case ant::gal::gal_texture_format::A2R10G10B10_SINT:
        break;
    case ant::gal::gal_texture_format::A2B10G10R10_UNORM:
        break;
    case ant::gal::gal_texture_format::A2B10G10R10_UINT:
        break;
    case ant::gal::gal_texture_format::A2B10G10R10_SNORM:
        break;
    case ant::gal::gal_texture_format::A2B10G10R10_SINT:
        break;
    case ant::gal::gal_texture_format::R10G10B10A2_UNORM:
        break;
    case ant::gal::gal_texture_format::R10G10B10A2_UINT:
        break;
    case ant::gal::gal_texture_format::R10G10B10A2_SNORM:
        break;
    case ant::gal::gal_texture_format::R10G10B10A2_SINT:
        break;
    case ant::gal::gal_texture_format::B10G10R10A2_UNORM:
        break;
    case ant::gal::gal_texture_format::B10G10R10A2_UINT:
        break;
    case ant::gal::gal_texture_format::B10G10R10A2_SNORM:
        break;
    case ant::gal::gal_texture_format::B10G10R10A2_SINT:
        break;
    case ant::gal::gal_texture_format::B10G11R11_UFLOAT:
        break;
    case ant::gal::gal_texture_format::E5B9G9R9_UFLOAT:
        break;
    case ant::gal::gal_texture_format::R16G16B16_UNORM:
        break;
    case ant::gal::gal_texture_format::R16G16B16_SNORM:
        break;
    case ant::gal::gal_texture_format::R16G16B16_UINT:
        break;
    case ant::gal::gal_texture_format::R16G16B16_SINT:
        break;
    case ant::gal::gal_texture_format::R16G16B16_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R16G16B16_SBFLOAT:
        break;
    case ant::gal::gal_texture_format::R16G16B16A16_UNORM:
        break;
    case ant::gal::gal_texture_format::R16G16B16A16_SNORM:
        break;
    case ant::gal::gal_texture_format::R16G16B16A16_UINT:
        break;
    case ant::gal::gal_texture_format::R16G16B16A16_SINT:
        break;
    case ant::gal::gal_texture_format::R16G16B16A16_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R16G16B16A16_SBFLOAT:
        break;
    case ant::gal::gal_texture_format::R32G32_UINT:
        break;
    case ant::gal::gal_texture_format::R32G32_SINT:
        break;
    case ant::gal::gal_texture_format::R32G32_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R32G32B32_UINT:
        break;
    case ant::gal::gal_texture_format::R32G32B32_SINT:
        break;
    case ant::gal::gal_texture_format::R32G32B32_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R32G32B32A32_UINT:
        break;
    case ant::gal::gal_texture_format::R32G32B32A32_SINT:
        break;
    case ant::gal::gal_texture_format::R32G32B32A32_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R64_UINT:
        break;
    case ant::gal::gal_texture_format::R64_SINT:
        break;
    case ant::gal::gal_texture_format::R64_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R64G64_UINT:
        break;
    case ant::gal::gal_texture_format::R64G64_SINT:
        break;
    case ant::gal::gal_texture_format::R64G64_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R64G64B64_UINT:
        break;
    case ant::gal::gal_texture_format::R64G64B64_SINT:
        break;
    case ant::gal::gal_texture_format::R64G64B64_SFLOAT:
        break;
    case ant::gal::gal_texture_format::R64G64B64A64_UINT:
        break;
    case ant::gal::gal_texture_format::R64G64B64A64_SINT:
        break;
    case ant::gal::gal_texture_format::R64G64B64A64_SFLOAT:
        break;
    case ant::gal::gal_texture_format::D16_UNORM:
        break;
    case ant::gal::gal_texture_format::X8_D24_UNORM:
        break;
    case ant::gal::gal_texture_format::D32_SFLOAT:
        break;
    case ant::gal::gal_texture_format::S8_UINT:
        break;
    case ant::gal::gal_texture_format::D16_UNORM_S8_UINT:
        break;
    case ant::gal::gal_texture_format::D24_UNORM_S8_UINT:
        break;
    case ant::gal::gal_texture_format::D32_SFLOAT_S8_UINT:
        break;
    case ant::gal::gal_texture_format::DXBC1_RGB_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC1_RGB_SRGB:
        break;
    case ant::gal::gal_texture_format::DXBC1_RGBA_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC1_RGBA_SRGB:
        break;
    case ant::gal::gal_texture_format::DXBC2_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC2_SRGB:
        break;
    case ant::gal::gal_texture_format::DXBC3_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC3_SRGB:
        break;
    case ant::gal::gal_texture_format::DXBC4_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC4_SNORM:
        break;
    case ant::gal::gal_texture_format::DXBC5_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC5_SNORM:
        break;
    case ant::gal::gal_texture_format::DXBC6H_UFLOAT:
        break;
    case ant::gal::gal_texture_format::DXBC6H_SFLOAT:
        break;
    case ant::gal::gal_texture_format::DXBC7_UNORM:
        break;
    case ant::gal::gal_texture_format::DXBC7_SRGB:
        break;
    case ant::gal::gal_texture_format::PVRTC1_2BPP_UNORM:
        break;
    case ant::gal::gal_texture_format::PVRTC1_4BPP_UNORM:
        break;
    case ant::gal::gal_texture_format::PVRTC2_2BPP_UNORM:
        break;
    case ant::gal::gal_texture_format::PVRTC2_4BPP_UNORM:
        break;
    case ant::gal::gal_texture_format::PVRTC1_2BPP_SRGB:
        break;
    case ant::gal::gal_texture_format::PVRTC1_4BPP_SRGB:
        break;
    case ant::gal::gal_texture_format::PVRTC2_2BPP_SRGB:
        break;
    case ant::gal::gal_texture_format::PVRTC2_4BPP_SRGB:
        break;
    case ant::gal::gal_texture_format::ETC2_R8G8B8_UNORM:
        break;
    case ant::gal::gal_texture_format::ETC2_R8G8B8_SRGB:
        break;
    case ant::gal::gal_texture_format::ETC2_R8G8B8A1_UNORM:
        break;
    case ant::gal::gal_texture_format::ETC2_R8G8B8A1_SRGB:
        break;
    case ant::gal::gal_texture_format::ETC2_R8G8B8A8_UNORM:
        break;
    case ant::gal::gal_texture_format::ETC2_R8G8B8A8_SRGB:
        break;
    case ant::gal::gal_texture_format::ETC2_EAC_R11_UNORM:
        break;
    case ant::gal::gal_texture_format::ETC2_EAC_R11_SNORM:
        break;
    case ant::gal::gal_texture_format::ETC2_EAC_R11G11_UNORM:
        break;
    case ant::gal::gal_texture_format::ETC2_EAC_R11G11_SNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_4x4_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_4x4_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_5x4_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_5x4_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_5x5_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_5x5_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_6x5_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_6x5_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_6x6_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_6x6_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_8x5_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_8x5_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_8x6_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_8x6_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_8x8_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_8x8_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_10x5_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_10x5_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_10x6_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_10x6_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_10x8_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_10x8_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_10x10_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_10x10_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_12x10_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_12x10_SRGB:
        break;
    case ant::gal::gal_texture_format::ASTC_12x12_UNORM:
        break;
    case ant::gal::gal_texture_format::ASTC_12x12_SRGB:
        break;
    case ant::gal::gal_texture_format::CLUT_P4:
        break;
    case ant::gal::gal_texture_format::CLUT_P4A4:
        break;
    case ant::gal::gal_texture_format::CLUT_P8:
        break;
    case ant::gal::gal_texture_format::CLUT_P8A8:
        break;
    default:
        break;
    }
    return VK_FORMAT_MAX_ENUM;
}

VkSampleCountFlagBits utils_to_vk_sample_count_flags(gal_texture_sample_count samples) {
    switch (samples) {
    case ant::gal::gal_texture_sample_count::tsc_1:
        return VK_SAMPLE_COUNT_1_BIT;
        break;
    case ant::gal::gal_texture_sample_count::tsc_2:
        return VK_SAMPLE_COUNT_2_BIT;
        break;
    case ant::gal::gal_texture_sample_count::tsc_4:
        return VK_SAMPLE_COUNT_4_BIT;
        break;
    case ant::gal::gal_texture_sample_count::tsc_8:
        return VK_SAMPLE_COUNT_8_BIT;
        break;
    default:
        return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
        break;
    }
}

//VkImageCreateFlags utils_to_vk_image_creation_flags(resource_types types) {
//    VkImageCreateFlags flags = 0;
//
//    return flags;
//}

VkImageUsageFlags utils_to_vk_image_usage(gal_resource_types types, gal_memory_flag memory_flag) {
    VkImageUsageFlags flags = 0;
    if (memory_flag == gal_memory_flag::cpu_upload) {
        flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    } else if (memory_flag == gal_memory_flag::gpu_download) {
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if (types & gal_resource_type::rt_color_rt) {
        flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if (types & gal_resource_type::rt_depth_stencil_rt) {
        flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (types & gal_resource_type::rt_texture) {
        flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (types & gal_resource_type::rt_rw_texture) {
        flags |= VK_IMAGE_USAGE_STORAGE_BIT;
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    // add copy usage for all resources
    flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    return flags;
}

VkImageAspectFlags util_vk_determine_aspect_mask(VkFormat format, bool has_stencil) {
    VkImageAspectFlags result = 0;
    switch (format) {
        // Depth
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D32_SFLOAT:
        result = VK_IMAGE_ASPECT_DEPTH_BIT;
        break;
        // Stencil
    case VK_FORMAT_S8_UINT:
        result = VK_IMAGE_ASPECT_STENCIL_BIT;
        break;
        // Depth/stencil
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        result = VK_IMAGE_ASPECT_DEPTH_BIT;
        if (has_stencil)
            result |= VK_IMAGE_ASPECT_STENCIL_BIT;
        break;
        // Assume everything else is Color
    default:
        result = VK_IMAGE_ASPECT_COLOR_BIT;
        break;
    }
    return result;
}
VkFormatFeatureFlags util_vk_image_usage_to_format_features(VkImageUsageFlags usage) {
    VkFormatFeatureFlags result = (VkFormatFeatureFlags)0;
    if (VK_IMAGE_USAGE_SAMPLED_BIT == (usage & VK_IMAGE_USAGE_SAMPLED_BIT)) {
        result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    }
    if (VK_IMAGE_USAGE_STORAGE_BIT == (usage & VK_IMAGE_USAGE_STORAGE_BIT)) {
        result |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    }
    if (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT == (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)) {
        result |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    }
    if (VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT == (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)) {
        result |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    return result;
}

VkQueueFlags util_to_vk_queue_flags(gal_queue_type queueType) {
    switch (queueType) {
    case gal_queue_type::graphcis:
        return VK_QUEUE_GRAPHICS_BIT;
    case gal_queue_type::transfer:
        return VK_QUEUE_TRANSFER_BIT;
    case gal_queue_type::compute:
        return VK_QUEUE_COMPUTE_BIT;
    default:
        return VK_QUEUE_FLAG_BITS_MAX_ENUM;
    }
}

inline constexpr gal_texture_format vkformat_to_galtextureformat(VkFormat fmt) {
    switch (fmt) {

    case VK_FORMAT_UNDEFINED:
        return gal_texture_format::UNDEFINED;
    case VK_FORMAT_R4G4_UNORM_PACK8:
        return gal_texture_format::G4R4_UNORM;
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
        return gal_texture_format::A4B4G4R4_UNORM;
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
        return gal_texture_format::A4R4G4B4_UNORM;
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
        return gal_texture_format::B5G6R5_UNORM;
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
        return gal_texture_format::R5G6B5_UNORM;
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
        return gal_texture_format::A1B5G5R5_UNORM;
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
        return gal_texture_format::A1R5G5B5_UNORM;
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
        return gal_texture_format::B5G5R5A1_UNORM;
    case VK_FORMAT_R8_UNORM:
        return gal_texture_format::R8_UNORM;
    case VK_FORMAT_R8_SNORM:
        return gal_texture_format::R8_SNORM;
    case VK_FORMAT_R8_UINT:
        return gal_texture_format::R8_UINT;
    case VK_FORMAT_R8_SINT:
        return gal_texture_format::R8_SINT;
    case VK_FORMAT_R8_SRGB:
        return gal_texture_format::R8_SRGB;
    case VK_FORMAT_R8G8_UNORM:
        return gal_texture_format::R8G8_UNORM;
    case VK_FORMAT_R8G8_SNORM:
        return gal_texture_format::R8G8_SNORM;
    case VK_FORMAT_R8G8_UINT:
        return gal_texture_format::R8G8_UINT;
    case VK_FORMAT_R8G8_SINT:
        return gal_texture_format::R8G8_SINT;
    case VK_FORMAT_R8G8_SRGB:
        return gal_texture_format::R8G8_SRGB;
    case VK_FORMAT_R8G8B8_UNORM:
        return gal_texture_format::R8G8B8_UNORM;
    case VK_FORMAT_R8G8B8_SNORM:
        return gal_texture_format::R8G8B8_SNORM;
    case VK_FORMAT_R8G8B8_UINT:
        return gal_texture_format::R8G8B8_UINT;
    case VK_FORMAT_R8G8B8_SINT:
        return gal_texture_format::R8G8B8_SINT;
    case VK_FORMAT_R8G8B8_SRGB:
        return gal_texture_format::R8G8B8_SRGB;
    case VK_FORMAT_B8G8R8_UNORM:
        return gal_texture_format::B8G8R8_UNORM;
    case VK_FORMAT_B8G8R8_SNORM:
        return gal_texture_format::B8G8R8_SNORM;
    case VK_FORMAT_B8G8R8_UINT:
        return gal_texture_format::B8G8R8_UINT;
    case VK_FORMAT_B8G8R8_SINT:
        return gal_texture_format::B8G8R8_SINT;
    case VK_FORMAT_B8G8R8_SRGB:
        return gal_texture_format::B8G8R8_SRGB;
    case VK_FORMAT_R8G8B8A8_UNORM:
        return gal_texture_format::R8G8B8A8_UNORM;
    case VK_FORMAT_R8G8B8A8_SNORM:
        return gal_texture_format::R8G8B8A8_SNORM;
    case VK_FORMAT_R8G8B8A8_UINT:
        return gal_texture_format::R8G8B8A8_UINT;
    case VK_FORMAT_R8G8B8A8_SINT:
        return gal_texture_format::R8G8B8A8_SINT;
    case VK_FORMAT_R8G8B8A8_SRGB:
        return gal_texture_format::R8G8B8A8_SRGB;
    case VK_FORMAT_B8G8R8A8_UNORM:
        return gal_texture_format::B8G8R8A8_UNORM;
    case VK_FORMAT_B8G8R8A8_SNORM:
        return gal_texture_format::B8G8R8A8_SNORM;
    case VK_FORMAT_B8G8R8A8_UINT:
        return gal_texture_format::B8G8R8A8_UINT;
    case VK_FORMAT_B8G8R8A8_SINT:
        return gal_texture_format::B8G8R8A8_SINT;
    case VK_FORMAT_B8G8R8A8_SRGB:
        return gal_texture_format::B8G8R8A8_SRGB;
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        return gal_texture_format::A2R10G10B10_UNORM;
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
        return gal_texture_format::A2R10G10B10_UINT;
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        return gal_texture_format::A2B10G10R10_UNORM;
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
        return gal_texture_format::A2B10G10R10_UINT;
    case VK_FORMAT_R16_UNORM:
        return gal_texture_format::R16_UNORM;
    case VK_FORMAT_R16_SNORM:
        return gal_texture_format::R16_SNORM;
    case VK_FORMAT_R16_UINT:
        return gal_texture_format::R16_UINT;
    case VK_FORMAT_R16_SINT:
        return gal_texture_format::R16_SINT;
    case VK_FORMAT_R16_SFLOAT:
        return gal_texture_format::R16_SFLOAT;
    case VK_FORMAT_R16G16_UNORM:
        return gal_texture_format::R16G16_UNORM;
    case VK_FORMAT_R16G16_SNORM:
        return gal_texture_format::R16G16_SNORM;
    case VK_FORMAT_R16G16_UINT:
        return gal_texture_format::R16G16_UINT;
    case VK_FORMAT_R16G16_SINT:
        return gal_texture_format::R16G16_SINT;
    case VK_FORMAT_R16G16_SFLOAT:
        return gal_texture_format::R16G16_SFLOAT;
    case VK_FORMAT_R16G16B16_UNORM:
        return gal_texture_format::R16G16B16_UNORM;
    case VK_FORMAT_R16G16B16_SNORM:
        return gal_texture_format::R16G16B16_SNORM;
    case VK_FORMAT_R16G16B16_UINT:
        return gal_texture_format::R16G16B16_UINT;
    case VK_FORMAT_R16G16B16_SINT:
        return gal_texture_format::R16G16B16_SINT;
    case VK_FORMAT_R16G16B16_SFLOAT:
        return gal_texture_format::R16G16B16_SFLOAT;
    case VK_FORMAT_R16G16B16A16_UNORM:
        return gal_texture_format::R16G16B16A16_UNORM;
    case VK_FORMAT_R16G16B16A16_SNORM:
        return gal_texture_format::R16G16B16A16_SNORM;
    case VK_FORMAT_R16G16B16A16_UINT:
        return gal_texture_format::R16G16B16A16_UINT;
    case VK_FORMAT_R16G16B16A16_SINT:
        return gal_texture_format::R16G16B16A16_SINT;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return gal_texture_format::R16G16B16A16_SFLOAT;
    case VK_FORMAT_R32_UINT:
        return gal_texture_format::R32_UINT;
    case VK_FORMAT_R32_SINT:
        return gal_texture_format::R32_SINT;
    case VK_FORMAT_R32_SFLOAT:
        return gal_texture_format::R32_SFLOAT;
    case VK_FORMAT_R32G32_UINT:
        return gal_texture_format::R32G32_UINT;
    case VK_FORMAT_R32G32_SINT:
        return gal_texture_format::R32G32_SINT;
    case VK_FORMAT_R32G32_SFLOAT:
        return gal_texture_format::R32G32_SFLOAT;
    case VK_FORMAT_R32G32B32_UINT:
        return gal_texture_format::R32G32B32_UINT;
    case VK_FORMAT_R32G32B32_SINT:
        return gal_texture_format::R32G32B32_SINT;
    case VK_FORMAT_R32G32B32_SFLOAT:
        return gal_texture_format::R32G32B32_SFLOAT;
    case VK_FORMAT_R32G32B32A32_UINT:
        return gal_texture_format::R32G32B32A32_UINT;
    case VK_FORMAT_R32G32B32A32_SINT:
        return gal_texture_format::R32G32B32A32_SINT;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return gal_texture_format::R32G32B32A32_SFLOAT;
    case VK_FORMAT_R64_UINT:
        return gal_texture_format::R64_UINT;
    case VK_FORMAT_R64_SINT:
        return gal_texture_format::R64_SINT;
    case VK_FORMAT_R64_SFLOAT:
        return gal_texture_format::R64_SFLOAT;
    case VK_FORMAT_R64G64_UINT:
        return gal_texture_format::R64G64_UINT;
    case VK_FORMAT_R64G64_SINT:
        return gal_texture_format::R64G64_SINT;
    case VK_FORMAT_R64G64_SFLOAT:
        return gal_texture_format::R64G64_SFLOAT;
    case VK_FORMAT_R64G64B64_UINT:
        return gal_texture_format::R64G64B64_UINT;
    case VK_FORMAT_R64G64B64_SINT:
        return gal_texture_format::R64G64B64_SINT;
    case VK_FORMAT_R64G64B64_SFLOAT:
        return gal_texture_format::R64G64B64_SFLOAT;
    case VK_FORMAT_R64G64B64A64_UINT:
        return gal_texture_format::R64G64B64A64_UINT;
    case VK_FORMAT_R64G64B64A64_SINT:
        return gal_texture_format::R64G64B64A64_SINT;
    case VK_FORMAT_R64G64B64A64_SFLOAT:
        return gal_texture_format::R64G64B64A64_SFLOAT;
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
        return gal_texture_format::B10G11R11_UFLOAT;
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
        return gal_texture_format::E5B9G9R9_UFLOAT;
    case VK_FORMAT_D16_UNORM:
        return gal_texture_format::D16_UNORM;
    case VK_FORMAT_X8_D24_UNORM_PACK32:
        return gal_texture_format::X8_D24_UNORM;
    case VK_FORMAT_D32_SFLOAT:
        return gal_texture_format::D32_SFLOAT;
    case VK_FORMAT_S8_UINT:
        return gal_texture_format::S8_UINT;
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return gal_texture_format::D16_UNORM_S8_UINT;
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return gal_texture_format::D24_UNORM_S8_UINT;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return gal_texture_format::D32_SFLOAT_S8_UINT;
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
        return gal_texture_format::DXBC1_RGB_UNORM;
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
        return gal_texture_format::DXBC1_RGB_SRGB;
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
        return gal_texture_format::DXBC1_RGBA_UNORM;
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        return gal_texture_format::DXBC1_RGBA_SRGB;
    case VK_FORMAT_BC2_UNORM_BLOCK:
        return gal_texture_format::DXBC2_UNORM;
    case VK_FORMAT_BC2_SRGB_BLOCK:
        return gal_texture_format::DXBC2_SRGB;
    case VK_FORMAT_BC3_UNORM_BLOCK:
        return gal_texture_format::DXBC3_UNORM;
    case VK_FORMAT_BC3_SRGB_BLOCK:
        return gal_texture_format::DXBC3_SRGB;
    case VK_FORMAT_BC4_UNORM_BLOCK:
        return gal_texture_format::DXBC4_UNORM;
    case VK_FORMAT_BC4_SNORM_BLOCK:
        return gal_texture_format::DXBC4_SNORM;
    case VK_FORMAT_BC5_UNORM_BLOCK:
        return gal_texture_format::DXBC5_UNORM;
    case VK_FORMAT_BC5_SNORM_BLOCK:
        return gal_texture_format::DXBC5_SNORM;
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
        return gal_texture_format::DXBC6H_UFLOAT;
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
        return gal_texture_format::DXBC6H_SFLOAT;
    case VK_FORMAT_BC7_UNORM_BLOCK:
        return gal_texture_format::DXBC7_UNORM;
    case VK_FORMAT_BC7_SRGB_BLOCK:
        return gal_texture_format::DXBC7_SRGB;
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
        return gal_texture_format::ETC2_R8G8B8_UNORM;
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
        return gal_texture_format::ETC2_R8G8B8_SRGB;
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
        return gal_texture_format::ETC2_R8G8B8A1_UNORM;
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
        return gal_texture_format::ETC2_R8G8B8A1_SRGB;
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
        return gal_texture_format::ETC2_R8G8B8A8_UNORM;
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        return gal_texture_format::ETC2_R8G8B8A8_SRGB;
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
        return gal_texture_format::ETC2_EAC_R11_UNORM;
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
        return gal_texture_format::ETC2_EAC_R11_SNORM;
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
        return gal_texture_format::ETC2_EAC_R11G11_UNORM;
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        return gal_texture_format::ETC2_EAC_R11G11_SNORM;
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
        return gal_texture_format::ASTC_4x4_UNORM;
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
        return gal_texture_format::ASTC_4x4_SRGB;
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
        return gal_texture_format::ASTC_5x4_UNORM;
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
        return gal_texture_format::ASTC_5x4_SRGB;
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
        return gal_texture_format::ASTC_5x5_UNORM;
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
        return gal_texture_format::ASTC_5x5_SRGB;
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
        return gal_texture_format::ASTC_6x5_UNORM;
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
        return gal_texture_format::ASTC_6x5_SRGB;
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
        return gal_texture_format::ASTC_6x6_UNORM;
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
        return gal_texture_format::ASTC_6x6_SRGB;
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
        return gal_texture_format::ASTC_8x5_UNORM;
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
        return gal_texture_format::ASTC_8x5_SRGB;
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
        return gal_texture_format::ASTC_8x6_UNORM;
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
        return gal_texture_format::ASTC_8x6_SRGB;
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
        return gal_texture_format::ASTC_8x8_UNORM;
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
        return gal_texture_format::ASTC_8x8_SRGB;
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
        return gal_texture_format::ASTC_10x5_UNORM;
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
        return gal_texture_format::ASTC_10x5_SRGB;
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
        return gal_texture_format::ASTC_10x6_UNORM;
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
        return gal_texture_format::ASTC_10x6_SRGB;
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
        return gal_texture_format::ASTC_10x8_UNORM;
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
        return gal_texture_format::ASTC_10x8_SRGB;
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
        return gal_texture_format::ASTC_10x10_UNORM;
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
        return gal_texture_format::ASTC_10x10_SRGB;
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
        return gal_texture_format::ASTC_12x10_UNORM;
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
        return gal_texture_format::ASTC_12x10_SRGB;
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
        return gal_texture_format::ASTC_12x12_UNORM;
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
        return gal_texture_format::ASTC_12x12_SRGB;

    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
        return gal_texture_format::PVRTC1_2BPP_UNORM;
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
        return gal_texture_format::PVRTC1_4BPP_UNORM;
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
        return gal_texture_format::PVRTC1_2BPP_SRGB;
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
        return gal_texture_format::PVRTC1_4BPP_SRGB;

    case VK_FORMAT_G16B16G16R16_422_UNORM:
        return gal_texture_format::G16B16G16R16_422_UNORM;
    case VK_FORMAT_B16G16R16G16_422_UNORM:
        return gal_texture_format::B16G16R16G16_422_UNORM;
    case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
        return gal_texture_format::R12X4G12X4B12X4A12X4_UNORM_4PACK16;
    case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
        return gal_texture_format::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
    case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
        return gal_texture_format::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
    case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
        return gal_texture_format::R10X6G10X6B10X6A10X6_UNORM_4PACK16;
    case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
        return gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
    case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
        return gal_texture_format::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
    case VK_FORMAT_G8B8G8R8_422_UNORM:
        return gal_texture_format::G8B8G8R8_422_UNORM;
    case VK_FORMAT_B8G8R8G8_422_UNORM:
        return gal_texture_format::B8G8R8G8_422_UNORM;
    case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
        return gal_texture_format::G8_B8_R8_3PLANE_420_UNORM;
    case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
        return gal_texture_format::G8_B8R8_2PLANE_420_UNORM;
    case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
        return gal_texture_format::G8_B8_R8_3PLANE_422_UNORM;
    case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
        return gal_texture_format::G8_B8R8_2PLANE_422_UNORM;
    case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
        return gal_texture_format::G8_B8_R8_3PLANE_444_UNORM;
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
        return gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
        return gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
        return gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
        return gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
        return gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
        return gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
        return gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
        return gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
        return gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
        return gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
    case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
        return gal_texture_format::G16_B16_R16_3PLANE_420_UNORM;
    case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
        return gal_texture_format::G16_B16_R16_3PLANE_422_UNORM;
    case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
        return gal_texture_format::G16_B16_R16_3PLANE_444_UNORM;
    case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
        return gal_texture_format::G16_B16R16_2PLANE_420_UNORM;
    case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
        return gal_texture_format::G16_B16R16_2PLANE_422_UNORM;

    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_R8_USCALED:
    case VK_FORMAT_R8_SSCALED:
    case VK_FORMAT_R8G8_USCALED:
    case VK_FORMAT_R8G8_SSCALED:
    case VK_FORMAT_R8G8B8_USCALED:
    case VK_FORMAT_R8G8B8_SSCALED:
    case VK_FORMAT_B8G8R8_USCALED:
    case VK_FORMAT_B8G8R8_SSCALED:
    case VK_FORMAT_R8G8B8A8_USCALED:
    case VK_FORMAT_R8G8B8A8_SSCALED:
    case VK_FORMAT_B8G8R8A8_USCALED:
    case VK_FORMAT_B8G8R8A8_SSCALED:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
    case VK_FORMAT_R16_USCALED:
    case VK_FORMAT_R16_SSCALED:
    case VK_FORMAT_R16G16_USCALED:
    case VK_FORMAT_R16G16_SSCALED:
    case VK_FORMAT_R16G16B16_USCALED:
    case VK_FORMAT_R16G16B16_SSCALED:
    case VK_FORMAT_R16G16B16A16_USCALED:
    case VK_FORMAT_R16G16B16A16_SSCALED:
    case VK_FORMAT_R10X6_UNORM_PACK16:
    case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
    case VK_FORMAT_R12X4_UNORM_PACK16:
    case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return gal_texture_format::UNDEFINED;
    }
    return gal_texture_format::UNDEFINED;
}

inline constexpr VkFormat galtextureformat_to_vkformat(gal_texture_format fmt) {
    switch (fmt) {

    case gal_texture_format::UNDEFINED:
        return VK_FORMAT_UNDEFINED;
    case gal_texture_format::G4R4_UNORM:
        return VK_FORMAT_R4G4_UNORM_PACK8;
    case gal_texture_format::A4B4G4R4_UNORM:
        return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
    case gal_texture_format::A4R4G4B4_UNORM:
        return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
    case gal_texture_format::R5G6B5_UNORM:
        return VK_FORMAT_B5G6R5_UNORM_PACK16;
    case gal_texture_format::B5G6R5_UNORM:
        return VK_FORMAT_R5G6B5_UNORM_PACK16;
    case gal_texture_format::A1B5G5R5_UNORM:
        return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
    case gal_texture_format::A1R5G5B5_UNORM:
        return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
    case gal_texture_format::B5G5R5A1_UNORM:
        return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
    case gal_texture_format::A2B10G10R10_UNORM:
        return VK_FORMAT_A2B10G10R10_UNORM_PACK32;

    case gal_texture_format::R8_UNORM:
        return VK_FORMAT_R8_UNORM;
    case gal_texture_format::R8_SNORM:
        return VK_FORMAT_R8_SNORM;
    case gal_texture_format::R8_UINT:
        return VK_FORMAT_R8_UINT;
    case gal_texture_format::R8_SINT:
        return VK_FORMAT_R8_SINT;
    case gal_texture_format::R8_SRGB:
        return VK_FORMAT_R8_SRGB;
    case gal_texture_format::R8G8_UNORM:
        return VK_FORMAT_R8G8_UNORM;
    case gal_texture_format::R8G8_SNORM:
        return VK_FORMAT_R8G8_SNORM;
    case gal_texture_format::R8G8_UINT:
        return VK_FORMAT_R8G8_UINT;
    case gal_texture_format::R8G8_SINT:
        return VK_FORMAT_R8G8_SINT;
    case gal_texture_format::R8G8_SRGB:
        return VK_FORMAT_R8G8_SRGB;
    case gal_texture_format::R8G8B8_UNORM:
        return VK_FORMAT_R8G8B8_UNORM;
    case gal_texture_format::R8G8B8_SNORM:
        return VK_FORMAT_R8G8B8_SNORM;
    case gal_texture_format::R8G8B8_UINT:
        return VK_FORMAT_R8G8B8_UINT;
    case gal_texture_format::R8G8B8_SINT:
        return VK_FORMAT_R8G8B8_SINT;
    case gal_texture_format::R8G8B8_SRGB:
        return VK_FORMAT_R8G8B8_SRGB;
    case gal_texture_format::B8G8R8_UNORM:
        return VK_FORMAT_B8G8R8_UNORM;
    case gal_texture_format::B8G8R8_SNORM:
        return VK_FORMAT_B8G8R8_SNORM;
    case gal_texture_format::B8G8R8_UINT:
        return VK_FORMAT_B8G8R8_UINT;
    case gal_texture_format::B8G8R8_SINT:
        return VK_FORMAT_B8G8R8_SINT;
    case gal_texture_format::B8G8R8_SRGB:
        return VK_FORMAT_B8G8R8_SRGB;
    case gal_texture_format::R8G8B8A8_UNORM:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case gal_texture_format::R8G8B8A8_SNORM:
        return VK_FORMAT_R8G8B8A8_SNORM;
    case gal_texture_format::R8G8B8A8_UINT:
        return VK_FORMAT_R8G8B8A8_UINT;
    case gal_texture_format::R8G8B8A8_SINT:
        return VK_FORMAT_R8G8B8A8_SINT;
    case gal_texture_format::R8G8B8A8_SRGB:
        return VK_FORMAT_R8G8B8A8_SRGB;
    case gal_texture_format::B8G8R8A8_UNORM:
        return VK_FORMAT_B8G8R8A8_UNORM;
    case gal_texture_format::B8G8R8A8_SNORM:
        return VK_FORMAT_B8G8R8A8_SNORM;
    case gal_texture_format::B8G8R8A8_UINT:
        return VK_FORMAT_B8G8R8A8_UINT;
    case gal_texture_format::B8G8R8A8_SINT:
        return VK_FORMAT_B8G8R8A8_SINT;
    case gal_texture_format::B8G8R8A8_SRGB:
        return VK_FORMAT_B8G8R8A8_SRGB;
    case gal_texture_format::R16_UNORM:
        return VK_FORMAT_R16_UNORM;
    case gal_texture_format::R16_SNORM:
        return VK_FORMAT_R16_SNORM;
    case gal_texture_format::R16_UINT:
        return VK_FORMAT_R16_UINT;
    case gal_texture_format::R16_SINT:
        return VK_FORMAT_R16_SINT;
    case gal_texture_format::R16_SFLOAT:
        return VK_FORMAT_R16_SFLOAT;
    case gal_texture_format::R16G16_UNORM:
        return VK_FORMAT_R16G16_UNORM;
    case gal_texture_format::R16G16_SNORM:
        return VK_FORMAT_R16G16_SNORM;
    case gal_texture_format::R16G16_UINT:
        return VK_FORMAT_R16G16_UINT;
    case gal_texture_format::R16G16_SINT:
        return VK_FORMAT_R16G16_SINT;
    case gal_texture_format::R16G16_SFLOAT:
        return VK_FORMAT_R16G16_SFLOAT;
    case gal_texture_format::R16G16B16_UNORM:
        return VK_FORMAT_R16G16B16_UNORM;
    case gal_texture_format::R16G16B16_SNORM:
        return VK_FORMAT_R16G16B16_SNORM;
    case gal_texture_format::R16G16B16_UINT:
        return VK_FORMAT_R16G16B16_UINT;
    case gal_texture_format::R16G16B16_SINT:
        return VK_FORMAT_R16G16B16_SINT;
    case gal_texture_format::R16G16B16_SFLOAT:
        return VK_FORMAT_R16G16B16_SFLOAT;
    case gal_texture_format::R16G16B16A16_UNORM:
        return VK_FORMAT_R16G16B16A16_UNORM;
    case gal_texture_format::R16G16B16A16_SNORM:
        return VK_FORMAT_R16G16B16A16_SNORM;
    case gal_texture_format::R16G16B16A16_UINT:
        return VK_FORMAT_R16G16B16A16_UINT;
    case gal_texture_format::R16G16B16A16_SINT:
        return VK_FORMAT_R16G16B16A16_SINT;
    case gal_texture_format::R16G16B16A16_SFLOAT:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    case gal_texture_format::R32_UINT:
        return VK_FORMAT_R32_UINT;
    case gal_texture_format::R32_SINT:
        return VK_FORMAT_R32_SINT;
    case gal_texture_format::R32_SFLOAT:
        return VK_FORMAT_R32_SFLOAT;
    case gal_texture_format::R32G32_UINT:
        return VK_FORMAT_R32G32_UINT;
    case gal_texture_format::R32G32_SINT:
        return VK_FORMAT_R32G32_SINT;
    case gal_texture_format::R32G32_SFLOAT:
        return VK_FORMAT_R32G32_SFLOAT;
    case gal_texture_format::R32G32B32_UINT:
        return VK_FORMAT_R32G32B32_UINT;
    case gal_texture_format::R32G32B32_SINT:
        return VK_FORMAT_R32G32B32_SINT;
    case gal_texture_format::R32G32B32_SFLOAT:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case gal_texture_format::R32G32B32A32_UINT:
        return VK_FORMAT_R32G32B32A32_UINT;
    case gal_texture_format::R32G32B32A32_SINT:
        return VK_FORMAT_R32G32B32A32_SINT;
    case gal_texture_format::R32G32B32A32_SFLOAT:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case gal_texture_format::R64_UINT:
        return VK_FORMAT_R64_UINT;
    case gal_texture_format::R64_SINT:
        return VK_FORMAT_R64_SINT;
    case gal_texture_format::R64_SFLOAT:
        return VK_FORMAT_R64_SFLOAT;
    case gal_texture_format::R64G64_UINT:
        return VK_FORMAT_R64G64_UINT;
    case gal_texture_format::R64G64_SINT:
        return VK_FORMAT_R64G64_SINT;
    case gal_texture_format::R64G64_SFLOAT:
        return VK_FORMAT_R64G64_SFLOAT;
    case gal_texture_format::R64G64B64_UINT:
        return VK_FORMAT_R64G64B64_UINT;
    case gal_texture_format::R64G64B64_SINT:
        return VK_FORMAT_R64G64B64_SINT;
    case gal_texture_format::R64G64B64_SFLOAT:
        return VK_FORMAT_R64G64B64_SFLOAT;
    case gal_texture_format::R64G64B64A64_UINT:
        return VK_FORMAT_R64G64B64A64_UINT;
    case gal_texture_format::R64G64B64A64_SINT:
        return VK_FORMAT_R64G64B64A64_SINT;
    case gal_texture_format::R64G64B64A64_SFLOAT:
        return VK_FORMAT_R64G64B64A64_SFLOAT;

    case gal_texture_format::B10G10R10A2_UNORM:
        return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
    case gal_texture_format::B10G10R10A2_UINT:
        return VK_FORMAT_A2R10G10B10_UINT_PACK32;
    case gal_texture_format::R10G10B10A2_UNORM:
        return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    case gal_texture_format::R10G10B10A2_UINT:
        return VK_FORMAT_A2B10G10R10_UINT_PACK32;

    case gal_texture_format::B10G11R11_UFLOAT:
        return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
    case gal_texture_format::E5B9G9R9_UFLOAT:
        return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;

    case gal_texture_format::G16B16G16R16_422_UNORM:
        return VK_FORMAT_G16B16G16R16_422_UNORM;
    case gal_texture_format::B16G16R16G16_422_UNORM:
        return VK_FORMAT_B16G16R16G16_422_UNORM;
    case gal_texture_format::R12X4G12X4B12X4A12X4_UNORM_4PACK16:
        return VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16;
    case gal_texture_format::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
        return VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
    case gal_texture_format::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
        return VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
    case gal_texture_format::R10X6G10X6B10X6A10X6_UNORM_4PACK16:
        return VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16;
    case gal_texture_format::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
        return VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
    case gal_texture_format::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
        return VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
    case gal_texture_format::G8B8G8R8_422_UNORM:
        return VK_FORMAT_G8B8G8R8_422_UNORM;
    case gal_texture_format::B8G8R8G8_422_UNORM:
        return VK_FORMAT_B8G8R8G8_422_UNORM;
    case gal_texture_format::G8_B8_R8_3PLANE_420_UNORM:
        return VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM;
    case gal_texture_format::G8_B8R8_2PLANE_420_UNORM:
        return VK_FORMAT_G8_B8R8_2PLANE_420_UNORM;
    case gal_texture_format::G8_B8_R8_3PLANE_422_UNORM:
        return VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM;
    case gal_texture_format::G8_B8R8_2PLANE_422_UNORM:
        return VK_FORMAT_G8_B8R8_2PLANE_422_UNORM;
    case gal_texture_format::G8_B8_R8_3PLANE_444_UNORM:
        return VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM;
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
        return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
        return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
    case gal_texture_format::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
        return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
        return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
    case gal_texture_format::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
        return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
        return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
        return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
    case gal_texture_format::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
        return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
        return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
    case gal_texture_format::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
        return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
    case gal_texture_format::G16_B16_R16_3PLANE_420_UNORM:
        return VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM;
    case gal_texture_format::G16_B16_R16_3PLANE_422_UNORM:
        return VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM;
    case gal_texture_format::G16_B16_R16_3PLANE_444_UNORM:
        return VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM;
    case gal_texture_format::G16_B16R16_2PLANE_420_UNORM:
        return VK_FORMAT_G16_B16R16_2PLANE_420_UNORM;
    case gal_texture_format::G16_B16R16_2PLANE_422_UNORM:
        return VK_FORMAT_G16_B16R16_2PLANE_422_UNORM;

    case gal_texture_format::D16_UNORM:
        return VK_FORMAT_D16_UNORM;
    case gal_texture_format::X8_D24_UNORM:
        return VK_FORMAT_X8_D24_UNORM_PACK32;
    case gal_texture_format::D32_SFLOAT:
        return VK_FORMAT_D32_SFLOAT;
    case gal_texture_format::S8_UINT:
        return VK_FORMAT_S8_UINT;
    case gal_texture_format::D16_UNORM_S8_UINT:
        return VK_FORMAT_D16_UNORM_S8_UINT;
    case gal_texture_format::D24_UNORM_S8_UINT:
        return VK_FORMAT_D24_UNORM_S8_UINT;
    case gal_texture_format::D32_SFLOAT_S8_UINT:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    case gal_texture_format::DXBC1_RGB_UNORM:
        return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
    case gal_texture_format::DXBC1_RGB_SRGB:
        return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
    case gal_texture_format::DXBC1_RGBA_UNORM:
        return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case gal_texture_format::DXBC1_RGBA_SRGB:
        return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
    case gal_texture_format::DXBC2_UNORM:
        return VK_FORMAT_BC2_UNORM_BLOCK;
    case gal_texture_format::DXBC2_SRGB:
        return VK_FORMAT_BC2_SRGB_BLOCK;
    case gal_texture_format::DXBC3_UNORM:
        return VK_FORMAT_BC3_UNORM_BLOCK;
    case gal_texture_format::DXBC3_SRGB:
        return VK_FORMAT_BC3_SRGB_BLOCK;
    case gal_texture_format::DXBC4_UNORM:
        return VK_FORMAT_BC4_UNORM_BLOCK;
    case gal_texture_format::DXBC4_SNORM:
        return VK_FORMAT_BC4_SNORM_BLOCK;
    case gal_texture_format::DXBC5_UNORM:
        return VK_FORMAT_BC5_UNORM_BLOCK;
    case gal_texture_format::DXBC5_SNORM:
        return VK_FORMAT_BC5_SNORM_BLOCK;
    case gal_texture_format::DXBC6H_UFLOAT:
        return VK_FORMAT_BC6H_UFLOAT_BLOCK;
    case gal_texture_format::DXBC6H_SFLOAT:
        return VK_FORMAT_BC6H_SFLOAT_BLOCK;
    case gal_texture_format::DXBC7_UNORM:
        return VK_FORMAT_BC7_UNORM_BLOCK;
    case gal_texture_format::DXBC7_SRGB:
        return VK_FORMAT_BC7_SRGB_BLOCK;
    case gal_texture_format::PVRTC1_2BPP_UNORM:
        return VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
    case gal_texture_format::PVRTC1_4BPP_UNORM:
        return VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
    case gal_texture_format::PVRTC1_2BPP_SRGB:
        return VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
    case gal_texture_format::PVRTC1_4BPP_SRGB:
        return VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
    case gal_texture_format::ETC2_R8G8B8_UNORM:
        return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
    case gal_texture_format::ETC2_R8G8B8A1_UNORM:
        return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
    case gal_texture_format::ETC2_R8G8B8A8_UNORM:
        return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
    case gal_texture_format::ETC2_R8G8B8_SRGB:
        return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
    case gal_texture_format::ETC2_R8G8B8A1_SRGB:
        return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
    case gal_texture_format::ETC2_R8G8B8A8_SRGB:
        return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
    case gal_texture_format::ETC2_EAC_R11_UNORM:
        return VK_FORMAT_EAC_R11_UNORM_BLOCK;
    case gal_texture_format::ETC2_EAC_R11G11_UNORM:
        return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
    case gal_texture_format::ETC2_EAC_R11_SNORM:
        return VK_FORMAT_EAC_R11_SNORM_BLOCK;
    case gal_texture_format::ETC2_EAC_R11G11_SNORM:
        return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
    case gal_texture_format::ASTC_4x4_UNORM:
        return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
    case gal_texture_format::ASTC_4x4_SRGB:
        return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
    case gal_texture_format::ASTC_5x4_UNORM:
        return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
    case gal_texture_format::ASTC_5x4_SRGB:
        return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
    case gal_texture_format::ASTC_5x5_UNORM:
        return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
    case gal_texture_format::ASTC_5x5_SRGB:
        return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
    case gal_texture_format::ASTC_6x5_UNORM:
        return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
    case gal_texture_format::ASTC_6x5_SRGB:
        return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
    case gal_texture_format::ASTC_6x6_UNORM:
        return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
    case gal_texture_format::ASTC_6x6_SRGB:
        return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
    case gal_texture_format::ASTC_8x5_UNORM:
        return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
    case gal_texture_format::ASTC_8x5_SRGB:
        return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
    case gal_texture_format::ASTC_8x6_UNORM:
        return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
    case gal_texture_format::ASTC_8x6_SRGB:
        return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
    case gal_texture_format::ASTC_8x8_UNORM:
        return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
    case gal_texture_format::ASTC_8x8_SRGB:
        return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
    case gal_texture_format::ASTC_10x5_UNORM:
        return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
    case gal_texture_format::ASTC_10x5_SRGB:
        return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
    case gal_texture_format::ASTC_10x6_UNORM:
        return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
    case gal_texture_format::ASTC_10x6_SRGB:
        return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
    case gal_texture_format::ASTC_10x8_UNORM:
        return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
    case gal_texture_format::ASTC_10x8_SRGB:
        return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
    case gal_texture_format::ASTC_10x10_UNORM:
        return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
    case gal_texture_format::ASTC_10x10_SRGB:
        return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
    case gal_texture_format::ASTC_12x10_UNORM:
        return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
    case gal_texture_format::ASTC_12x10_SRGB:
        return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
    case gal_texture_format::ASTC_12x12_UNORM:
        return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
    case gal_texture_format::ASTC_12x12_SRGB:
        return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
    case gal_texture_format::PVRTC2_2BPP_UNORM:
        return VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;
    case gal_texture_format::PVRTC2_4BPP_UNORM:
        return VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
    case gal_texture_format::PVRTC2_2BPP_SRGB:
        return VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
    case gal_texture_format::PVRTC2_4BPP_SRGB:
        return VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;

    default:
        return VK_FORMAT_UNDEFINED;
    }
}

void load_gal_vk_functions() {
#define VK_LOAD_FUNCTION_PTRS
#include "../helper/helper_macro.h"
#undef VK_LOAD_FUNCTION_PTRS
}

void offload_gal_vk_functions(){
#define VK_OFFLOAD_FUNCTION_PTRS
#include "../helper/helper_macro.h"
#undef VK_LOAD_FUNCTION_PTRS
}

gal_error_code vk_init_gal(gal_context *context) {
    *context = reinterpret_cast<gal_handle>(new vk_context);
    load_gal_vk_functions();
    if (*context == gal_null) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code vk_destroy_gal(gal_context context) {
    if (context != gal_null) {
        vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
        delete vk_ctx;
        context = gal_null;
        offload_gal_vk_functions();
    }
    return gal_error_code::success;
}

gal_error_code vk_create_instance(GalDesc *gal_desc, gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    auto stack_memory = ant::get_stack_memory_resource(2048);
    ant::vector<const char *> required_instance_layers(&stack_memory);
    ant::vector<const char *> required_instance_extensions(&stack_memory);

    if (gal_desc->b_debug_layer) {
        required_instance_layers.emplace_back("VK_LAYER_KHRONOS_validation");
        required_instance_extensions.emplace_back("VK_EXT_debug_utils");
    }

    // to query device features
    required_instance_extensions.emplace_back("VK_KHR_get_physical_device_properties2");

    if (gal_desc->b_surface) {
        required_instance_extensions.emplace_back("VK_KHR_surface");
#ifdef WIN32
        required_instance_extensions.emplace_back("VK_KHR_win32_surface");
#endif // WIN32
    }

    u32 available_layer_count = 0;

    u32 available_extension_count = 0;
    vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr);
    vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, nullptr);

    ant::vector<VkLayerProperties> available_layers(available_layer_count, &stack_memory);
    ant::vector<VkExtensionProperties> available_extensions(available_extension_count, &stack_memory);

    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data());
    vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, available_extensions.data());
    u32 required_layer_count = static_cast<u32>(required_instance_layers.size());
    u32 required_extension_count = static_cast<u32>(required_instance_extensions.size());

    for (u32 i = 0; i < available_layer_count; i++) {
        for (u32 j = 0; j < static_cast<u32>(required_instance_layers.size()); j++) {
            if (std::strcmp(required_instance_layers[j], available_layers[i].layerName) == 0) {
                required_layer_count--;
            }
        }
    }

    if (required_layer_count != 0) {
        return gal_error_code::error;
    }
    for (u32 i = 0; i < available_extension_count; i++) {
        for (u32 j = 0; j < static_cast<u32>(required_instance_extensions.size()); j++) {
            if (std::strcmp(required_instance_extensions[j], available_extensions[i].extensionName) == 0) {
                required_extension_count--;
            }
        }
    }
    if (required_extension_count != 0) {
        return gal_error_code::error;
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = nullptr;
    app_info.applicationVersion = 0u;
    app_info.pEngineName = nullptr;
    app_info.engineVersion = 0u;
    app_info.apiVersion = ANT_VK_API_VERSION;

    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.flags = 0;
    instance_create_info.enabledExtensionCount = static_cast<u32>(required_instance_extensions.size());
    instance_create_info.ppEnabledExtensionNames = required_instance_extensions.data();
    instance_create_info.enabledLayerCount = static_cast<u32>(required_instance_layers.size());
    instance_create_info.ppEnabledLayerNames = required_instance_layers.data();

    VkResult res = vkCreateInstance(&instance_create_info, nullptr, &(vk_ctx->instance));
    if (res == VK_SUCCESS && vk_ctx->instance != VK_NULL_HANDLE) {
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}
gal_error_code vk_destroy_instance(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    vkDestroyInstance(vk_ctx->instance, nullptr);
    return gal_error_code::success;
}
gal_error_code vk_create_device(GalDesc *gal_desc, gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    auto stack_memory = ant::get_stack_memory_resource(1024u);
    ant::vector<const char *> required_device_extensions(&stack_memory);
    ant::vector<const char *> required_device_layers(&stack_memory);
    if (gal_desc->b_swap_chain) {
        required_device_extensions.emplace_back("VK_KHR_swapchain");
    }
    required_device_extensions.emplace_back("VK_KHR_maintenance1");
    required_device_extensions.emplace_back("VK_KHR_dynamic_rendering");

    u32 physical_device_count = 0;
    // get physical device count
    vkEnumeratePhysicalDevices(vk_ctx->instance, &physical_device_count, nullptr);

    ant::vector<VkPhysicalDevice> physical_devices(physical_device_count, &stack_memory);
    if (physical_device_count == 0) {
        LOG_ERROR("no available device");
        return gal_error_code::error;
    }
    vkEnumeratePhysicalDevices(vk_ctx->instance, &physical_device_count, physical_devices.data());

    u32 required_queue_count = 1u;
    if (gal_desc->b_async_compute) {
        required_queue_count++;
    }
    if (gal_desc->b_async_transfer) {
        required_queue_count++;
    }

    // pick gpu
    auto pick_gpu = [&](ant::fixed_array<u32, 3> &queue_family_indices) -> gal_error_code {
        for (const auto &physical_device : physical_devices) {
            u32 queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                                     nullptr); // Get queue family properties

            // early exist

            if (queue_family_count < required_queue_count) {
                // unsupported device
                continue;
            }
            ant::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count, &stack_memory);

            queue_family_properties.resize(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                                     queue_family_properties.data()); // Get queue family properties

            auto getQueueFamilyIndex = [&](gal_queue_type queue_type) -> std::optional<u32> {
                // For other queue types or if no separate compute queue is present, return the first one to support
                // the requested flags

                if (queue_type == gal_queue_type::graphcis) {
                    return 0;
                }

                // Dedicated queue for compute
                // Try to find a queue family index that supports compute but not graphics
                if (queue_type == gal_queue_type::compute) {
                    for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++) {
                        if ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
                            return i;
                        }
                    }
                }

                // Dedicated queue for transfer
                // Try to find a queue family index that supports transfer but not graphics and compute
                if (queue_type == gal_queue_type::transfer) {
                    for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++) {
                        if ((queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
                            return i;
                        }
                    }
                }
                LOG_WARN("request queue not found, fallback to graphcis queue");
                return {};
            };

            auto graphics_queue_family_index = getQueueFamilyIndex(gal_queue_type::graphcis);
            auto compute_queue_family_index = graphics_queue_family_index;
            auto transfer_queue_family_index = graphics_queue_family_index;
            // TODO(hyl5): code is not clear
            if (gal_desc->b_async_compute) {
                auto t = getQueueFamilyIndex(gal_queue_type::compute);
                if (!t.has_value()) {
                    continue;
                }
                compute_queue_family_index = t.value();
            }
            if (gal_desc->b_async_transfer) {
                auto t = getQueueFamilyIndex(gal_queue_type::transfer);
                if (!t.has_value()) {
                    continue;
                }
                transfer_queue_family_index = t.value();
            }

            // check layers and extensions
            u32 available_layer_count = 0;
            u32 available_extension_count = 0;
            const char *layer_name = nullptr;
            vkEnumerateDeviceLayerProperties(physical_device, &available_layer_count, nullptr);
            ant::vector<VkLayerProperties> available_device_layers(available_layer_count, &stack_memory);
            vkEnumerateDeviceLayerProperties(physical_device, &available_layer_count, available_device_layers.data());

            vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &available_extension_count, nullptr);
            ant::vector<VkExtensionProperties> available_device_extensions(available_extension_count, &stack_memory);
            vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &available_extension_count,
                                                 available_device_extensions.data());

            u32 required_layer_count = static_cast<u32>(required_device_layers.size());
            u32 required_extension_count = static_cast<u32>(required_device_extensions.size());
            for (u32 i = 0; i < available_layer_count; i++) {
                for (u32 j = 0; j < static_cast<u32>(required_device_layers.size()); j++) {
                    if (std::strcmp(required_device_layers[j], available_device_layers[i].layerName) == 0) {
                        required_layer_count--;
                    }
                }
            }

            if (required_layer_count != 0) {
                continue;
            }
            for (u32 i = 0; i < available_extension_count; i++) {
                for (u32 j = 0; j < static_cast<u32>(required_device_extensions.size()); j++) {
                    if (std::strcmp(required_device_extensions[j], available_device_extensions[i].extensionName) == 0) {
                        required_extension_count--;
                    }
                }
            }
            if (required_extension_count != 0) {
                continue;
            }

            vk_ctx->active_gpu = physical_device;

            queue_family_indices = {graphics_queue_family_index.value(), compute_queue_family_index.value(),
                                    transfer_queue_family_index.value()};

            return gal_error_code::success;
        }
        return gal_error_code::error;
    };

    ant::fixed_array<u32, 3> queue_family_indices;

    gal_error_code result = pick_gpu(queue_family_indices);
    if (result != gal_error_code::success) {
        return gal_error_code::error;
    }
    if (vk_ctx->active_gpu == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }
    // TODO(hyl5): provide chain method to add features
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_features{};
    shader_draw_parameters_features.shaderDrawParameters = true;
    shader_draw_parameters_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;

    VkPhysicalDeviceDynamicRenderingFeatures dyanmic_rendering_features{};

    dyanmic_rendering_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    dyanmic_rendering_features.dynamicRendering = true;
    dyanmic_rendering_features.pNext = &shader_draw_parameters_features;

    VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_features{};
    descriptor_indexing_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
    descriptor_indexing_features.pNext = &dyanmic_rendering_features;
    descriptor_indexing_features.runtimeDescriptorArray = VK_TRUE;
    descriptor_indexing_features.descriptorBindingVariableDescriptorCount = VK_TRUE;
    descriptor_indexing_features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    descriptor_indexing_features.descriptorBindingPartiallyBound = VK_TRUE;

    VkPhysicalDeviceFeatures2 device_features{};
    device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    device_features.pNext = &descriptor_indexing_features;
    vkGetPhysicalDeviceFeatures2(vk_ctx->active_gpu, &device_features);
    ant::set<u32> queue_family_indices_set(queue_family_indices.begin(), queue_family_indices.end(), &stack_memory);
    ant::vector<VkDeviceQueueCreateInfo> device_queue_create_info(queue_family_indices_set.size(), &stack_memory);
    u32 idx = 0;
    ant::fixed_array<u32, 3> required_queue_family_indices;
    for (auto &index : queue_family_indices_set) {
        required_queue_family_indices[idx++] = index;
    }

    for (u32 i = 0; i < static_cast<u32>(queue_family_indices_set.size()); i++) {
        device_queue_create_info[i] = {};
        device_queue_create_info[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_info[i].pNext = NULL;
        device_queue_create_info[i].flags = 0;
        device_queue_create_info[i].queueFamilyIndex = required_queue_family_indices[i];
        device_queue_create_info[i].queueCount = 1;
        device_queue_create_info[i].pQueuePriorities = &vk_ctx->default_queue_property;
    }
    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = device_queue_create_info.data();
    device_create_info.queueCreateInfoCount = static_cast<u32>(device_queue_create_info.size());
    device_create_info.enabledExtensionCount = static_cast<u32>(required_device_extensions.size());
    device_create_info.ppEnabledExtensionNames = required_device_extensions.data();
    device_create_info.enabledLayerCount = static_cast<u32>(required_device_layers.size());
    device_create_info.ppEnabledLayerNames = required_device_layers.data();
    device_create_info.pNext = &device_features;

    VkResult res = vkCreateDevice(vk_ctx->active_gpu, &device_create_info, nullptr, &vk_ctx->device);

    // create device queue
    vkGetDeviceQueue(vk_ctx->device, queue_family_indices[0], 0, &vk_ctx->graphics_queue);

    if (gal_desc->b_async_compute) {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[1], 0, &vk_ctx->compute_queue);
    } else {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[0], 0, &vk_ctx->compute_queue);
    }

    if (gal_desc->b_async_transfer) {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[2], 0, &vk_ctx->transfer_queue);
    } else {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[0], 0, &vk_ctx->transfer_queue);
    }

    if (res != VK_SUCCESS || vk_ctx->device == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    // TODO(hyl5): query supported memory type of gpu, platform differences
    return gal_error_code::success;
}

gal_error_code vk_destroy_device(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    vkDestroyDevice(vk_ctx->device, nullptr);
    return gal_error_code::success;
}

gal_error_code vk_create_memory_allocator(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    VmaVulkanFunctions vulkan_functions{};
    vulkan_functions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkan_functions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo vma_create_info = {};
    vma_create_info.device = vk_ctx->device;
    vma_create_info.physicalDevice = vk_ctx->active_gpu;
    vma_create_info.instance = vk_ctx->instance;
    vma_create_info.pVulkanFunctions = &vulkan_functions;
    vma_create_info.vulkanApiVersion = ANT_VK_API_VERSION;
    VkResult result = vmaCreateAllocator(&vma_create_info, &vk_ctx->vma_allocator);

    if (result == VK_SUCCESS && vk_ctx->vma_allocator != nullptr) {
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}
gal_error_code vk_destroy_memory_allocator(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    vmaDestroyAllocator(vk_ctx->vma_allocator);
    return gal_error_code::success;
}

gal_error_code vk_create_buffer(gal_context context, gal_buffer_desc *desc, gal_buffer *buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    *buffer = reinterpret_cast<gal_handle>(new ant::gal::vk_buffer);
    vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(*buffer);

    reinterpret_cast<vk_buffer *>(buffer);

    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = desc->size;
    buffer_create_info.usage = util_to_vk_buffer_usage(desc->resource_types);

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

    if (desc->memory_flag == gal_memory_flag::gpu_dedicated) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    } else if (desc->memory_flag == gal_memory_flag::cpu_upload) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    } else if (desc->memory_flag == gal_memory_flag::gpu_download) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    } else {
        return gal_error_code::invalid_parameter;
    }

    if (desc->flags & gal_buffer_flag::bcf_own_memory) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    } else if (desc->flags & gal_buffer_flag::bcf_persistent_map) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }

    VkResult result = vmaCreateBuffer(vk_ctx->vma_allocator, &buffer_create_info, &alloc_info, &vk_buf->buffer,
                                      &vk_buf->allocation, nullptr);

    if (result != VK_SUCCESS || vk_buf->buffer == VK_NULL_HANDLE || vk_buf->allocation == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    return gal_error_code::success;
}

gal_error_code vk_destroy_buffer(gal_context context, gal_buffer buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);

    if (buffer != gal_null) {
        vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(buffer);

        vmaDestroyBuffer(vk_ctx->vma_allocator, vk_buf->buffer, vk_buf->allocation);
        delete vk_buf;
        buffer = gal_null;
    }
    return gal_error_code::success;
}

gal_error_code vk_create_texture(gal_context context, gal_texture_desc *desc, gal_texture *texture) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    *texture = reinterpret_cast<gal_handle>(new ant::gal::vk_texture);
    vk_texture *vk_tex = reinterpret_cast<vk_texture *>(*texture);

    u32 array_size = desc->array_size > 1u ? desc->array_size : 1u;
    u32 depth = desc->depth > 1u ? desc->depth : 1u;
    bool b_is_cubemap = (desc->resource_types & gal_resource_type::rt_texture_cube) && desc->array_size == 6;
    VkImageCreateInfo image_create_info{};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = utils_to_vk_image_type(desc->dimension);
    image_create_info.extent.width = desc->width;
    image_create_info.extent.height = desc->height;
    image_create_info.extent.depth = depth;
    image_create_info.mipLevels = desc->mip_level;
    image_create_info.arrayLayers = array_size;
    image_create_info.format = utils_to_vk_image_format(desc->format);
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.flags = 0; //utils_to_vk_image_creation_flags(desc->resource_types);
    image_create_info.usage = utils_to_vk_image_usage(desc->resource_types, desc->memory_flag);
    image_create_info.samples = utils_to_vk_sample_count_flags(desc->sample_count);

    if (desc->dimension == gal_texture_dimension::td_2D) {
        image_create_info.flags |= VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT;
    }

    // cubemap flags
    if (b_is_cubemap) {
        // TODO(hyl5): should gal layer validate parameter?
        //if (array_size != 6) {
        //    return gal_error_code::invalid_parameter;
        //}
        image_create_info.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }
    if (array_size > 1) {
        image_create_info.flags |= VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
    }
    VkFormatProperties format_props{};
    vkGetPhysicalDeviceFormatProperties(vk_ctx->active_gpu, image_create_info.format, &format_props);

    // Verify that GPU supports this format
    VkFormatFeatureFlags format_features = util_vk_image_usage_to_format_features(image_create_info.usage);

    if (format_props.optimalTilingFeatures & format_features) {
        return gal_error_code::error;
    }
    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_AUTO;

    if (desc->memory_flag == gal_memory_flag::gpu_dedicated) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    } else if (desc->memory_flag == gal_memory_flag::cpu_upload) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    } else if (desc->memory_flag == gal_memory_flag::gpu_download) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    } else {
        return gal_error_code::invalid_parameter;
    }

    VkResult result = vmaCreateImage(vk_ctx->vma_allocator, &image_create_info, &allocation_create_info, &vk_tex->image,
                                     &vk_tex->allocation, nullptr);
    if (result != VK_SUCCESS || vk_tex->image == VK_NULL_HANDLE || vk_tex->allocation == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    // create view form shader resources
    if (desc->resource_types & gal_resource_type::rt_texture ||
        desc->resource_types & gal_resource_type::rt_rw_texture) {

        VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;

        if (array_size > 1) {
            if (desc->dimension == gal_texture_dimension::td_1D) {
                view_type = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
            }
            if (desc->dimension == gal_texture_dimension::td_2D) {

                view_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            }
            if (b_is_cubemap) {
                view_type = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
            }
        } else {
            if (desc->dimension == gal_texture_dimension::td_1D) {
                view_type = VK_IMAGE_VIEW_TYPE_1D;
            }
            if (desc->dimension == gal_texture_dimension::td_2D) {

                view_type = VK_IMAGE_VIEW_TYPE_2D;
            }
            if (desc->dimension == gal_texture_dimension::td_3D) {
                view_type = VK_IMAGE_VIEW_TYPE_3D;
            }
            if (b_is_cubemap) {
                view_type = VK_IMAGE_VIEW_TYPE_CUBE;
            }
        }

        VkImageViewCreateInfo srvDesc = {};
        // SRV
        srvDesc.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        srvDesc.pNext = nullptr;
        srvDesc.flags = 0;
        srvDesc.image = vk_tex->image;
        srvDesc.viewType = view_type;
        srvDesc.format = galtextureformat_to_vkformat(desc->format);
        srvDesc.components.r = VK_COMPONENT_SWIZZLE_R;
        srvDesc.components.g = VK_COMPONENT_SWIZZLE_G;
        srvDesc.components.b = VK_COMPONENT_SWIZZLE_B;
        srvDesc.components.a = VK_COMPONENT_SWIZZLE_A;
        srvDesc.subresourceRange.aspectMask =
            util_vk_determine_aspect_mask(srvDesc.format, gal_tf_has_stencil(desc->format));
        srvDesc.subresourceRange.baseMipLevel = 0;
        srvDesc.subresourceRange.levelCount = desc->mip_level;
        srvDesc.subresourceRange.baseArrayLayer = 0;
        srvDesc.subresourceRange.layerCount = array_size;

        result = (vkCreateImageView(vk_ctx->device, &srvDesc, nullptr, &vk_tex->image_view));
        if (result != VK_SUCCESS || vk_tex->image == VK_NULL_HANDLE || vk_tex->allocation == VK_NULL_HANDLE) {
            return gal_error_code::error;
        }
    }

    return gal_error_code::success;
}

gal_error_code vk_destroy_texture(gal_context context, gal_texture texture) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (texture != gal_null) {
        vk_texture *vk_tex = reinterpret_cast<vk_texture *>(texture);
        vmaDestroyImage(vk_ctx->vma_allocator, vk_tex->image, vk_tex->allocation);
        vkDestroyImageView(vk_ctx->device, vk_tex->image_view, nullptr);
        delete vk_tex;
        texture = gal_null;
    }
    return gal_error_code::success;
}

gal_error_code vk_create_sampler() { return gal_error_code::success; }
gal_error_code vk_destroy_sampler() { return gal_error_code::success; }
gal_error_code vk_create_rendertarget() { return gal_error_code::success; }
gal_error_code vk_destroy_rendertarget() { return gal_error_code::success; }
// surface
gal_error_code vk_create_swapchain() { return gal_error_code::success; }
gal_error_code vk_destroy_swapchain() { return gal_error_code::success; }
gal_error_code vk_create_surface() { return gal_error_code::success; }
gal_error_code vk_destroy_surface() { return gal_error_code::success; }
// pipeline
gal_error_code vk_create_shader() { return gal_error_code::success; }
gal_error_code vk_destroy_shader() { return gal_error_code::success; }
gal_error_code vk_create_pipeline() { return gal_error_code::success; }
gal_error_code vk_destroy_pipeline() { return gal_error_code::success; }
gal_error_code vk_create_descriptorpool() { return gal_error_code::success; }
gal_error_code vk_destroy_descriptorpool() { return gal_error_code::success; }
gal_error_code vk_create_descriptorset() { return gal_error_code::success; }
gal_error_code vk_destroy_descriptorset() { return gal_error_code::success; }
gal_error_code vk_create_rootsignature() { return gal_error_code::success; }
gal_error_code vk_destroy_rootsignature() { return gal_error_code::success; }
// sync
gal_error_code vk_create_fence() { return gal_error_code::success; }
gal_error_code vk_wait_fence() { return gal_error_code::success; }
gal_error_code vk_destroy_fence() { return gal_error_code::success; }
gal_error_code vk_wait_gpu() { return gal_error_code::success; }
gal_error_code vk_create_semaphore() { return gal_error_code::success; }
gal_error_code vk_destroy_semaphore() { return gal_error_code::success; }
// cmds
gal_error_code vk_create_commandpool() { return gal_error_code::success; }
gal_error_code vk_reset_commandpool() { return gal_error_code::success; }
gal_error_code vk_destroy_commandpool() { return gal_error_code::success; }
gal_error_code vk_allocate_commandlist() { return gal_error_code::success; }
gal_error_code vk_free_commandlist() { return gal_error_code::success; }
gal_error_code vk_cmd_begin_recording() { return gal_error_code::success; }
gal_error_code vk_cmd_end_recording() { return gal_error_code::success; }
gal_error_code vk_cmd_resource_barrier() { return gal_error_code::success; }
gal_error_code vk_cmd_bind_descriptor_set() { return gal_error_code::success; }
gal_error_code vk_cmd_bind_index_buffer() { return gal_error_code::success; }
gal_error_code vk_cmd_bind_vertex_buffer() { return gal_error_code::success; }
gal_error_code vk_cmd_bind_descriptorset() { return gal_error_code::success; }
gal_error_code vk_cmd_bind_pipeline() { return gal_error_code::success; }
gal_error_code vk_cmd_begin_renderpass() { return gal_error_code::success; }
gal_error_code vk_cmd_end_renderpass() { return gal_error_code::success; }
gal_error_code vk_cmd_dispatch() { return gal_error_code::success; }
gal_error_code vk_cmd_dispatch_indirect() { return gal_error_code::success; }
gal_error_code vk_cmd_draw_instanced() { return gal_error_code::success; }
gal_error_code vk_cmd_draw_indexed_instanced() { return gal_error_code::success; }
gal_error_code vk_cmd_draw_indirect_instanced() { return gal_error_code::success; }
gal_error_code vk_cmd_draw_indirect_indexed_instanced() { return gal_error_code::success; }
gal_error_code vk_cmd_draw_mesh_task() { return gal_error_code::success; }
gal_error_code vk_cmd_draw_indirect_mesh_task() { return gal_error_code::success; }
gal_error_code vk_cmd_copy_texture() { return gal_error_code::success; }
gal_error_code vk_cmd_copy_buffer() { return gal_error_code::success; }
gal_error_code vk_cmd_fill_buffer() { return gal_error_code::success; }
gal_error_code vk_cmd_fill_texture() { return gal_error_code::success; }
gal_error_code vk_cmd_upload_buffer() { return gal_error_code::success; }
gal_error_code vk_cmd_upload_texture() { return gal_error_code::success; }
gal_error_code vk_cmd_copy_buffer_to_texture() { return gal_error_code::success; }
gal_error_code vk_cmd_copy_texture_to_buffer() { return gal_error_code::success; }

} // namespace ant::gal
