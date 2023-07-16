/*
 * Code adapted from ConfettiFX The-Forge
 * 
 * Copyright (c) 2017-2022 The Forge Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

#pragma once

#include "../gal.h"
#include "gal_vulkan_enum.h"

namespace ant::gal {

// TODO(hyl5): static or inline?

constexpr void load_gal_vk_functions() {
#define VK_LOAD_FUNCTION_PTRS
#include "../helper/helper_macro.h"
#undef VK_LOAD_FUNCTION_PTRS
}

constexpr void offload_gal_vk_functions() {
#define VK_OFFLOAD_FUNCTION_PTRS
#include "../helper/helper_macro.h"
#undef VK_LOAD_FUNCTION_PTRS
}

constexpr VkDescriptorType utils_to_vk_descriptor_type(gal_descriptor_type type) {
    switch (type) {
    case gal_descriptor_type::CONSTANT_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case gal_descriptor_type::RW_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case gal_descriptor_type::TEXTURE:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case gal_descriptor_type::RW_TEXTURE:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case gal_descriptor_type::SAMPLER:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    default:
        return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

constexpr VkFilter utils_to_vk_filter(gal_sampler_filter_mode filter) {
    switch (filter) {
    case gal_sampler_filter_mode::POINT:
        return VK_FILTER_NEAREST;
    case gal_sampler_filter_mode::LINEAR:
        return VK_FILTER_LINEAR;
    default:
        return VK_FILTER_MAX_ENUM;
    }
}

constexpr VkBufferUsageFlags util_to_vk_buffer_usage(gal_descriptor_type types) {
    VkBufferUsageFlags flags = 0;
    if ((types & gal_descriptor_type::CONSTANT_BUFFER) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    if ((types & gal_descriptor_type::RW_BUFFER) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }

    if ((types & gal_descriptor_type::VERTEX_BUFFER) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    if ((types & gal_descriptor_type::INDEX_BUFFER) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    if ((types & gal_descriptor_type::INDIRECT_ARGUMENT) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    }

    // TODO(hyl5): rt;
    return flags;
}

constexpr VkImageType utils_to_vk_image_type(gal_texture_dimension dimension) {
    switch (dimension) {
    case ant::gal::gal_texture_dimension::_1D:
        return VK_IMAGE_TYPE_1D;
    case ant::gal::gal_texture_dimension::_2D:
        return VK_IMAGE_TYPE_2D;
    case ant::gal::gal_texture_dimension::_3D:
        return VK_IMAGE_TYPE_3D;
    default:
        return VK_IMAGE_TYPE_MAX_ENUM;
    }
}

constexpr VkImageViewType utils_to_vk_image_view_type(gal_texture_dimension dimension) {
    switch (dimension) {
    case ant::gal::gal_texture_dimension::_1D:
        return VK_IMAGE_VIEW_TYPE_1D;
    case ant::gal::gal_texture_dimension::_2D:
        return VK_IMAGE_VIEW_TYPE_2D;
    case ant::gal::gal_texture_dimension::_3D:
        return VK_IMAGE_VIEW_TYPE_3D;
    case ant::gal::gal_texture_dimension::CUBE:
        return VK_IMAGE_VIEW_TYPE_CUBE;
    case ant::gal::gal_texture_dimension::_1D_ARRAY:
        return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
    case ant::gal::gal_texture_dimension::_2D_ARRAY:
        return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    case ant::gal::gal_texture_dimension::CUBE_ARRAY:
        return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
    case ant::gal::gal_texture_dimension::_2D_MS_ARRAY:
    case ant::gal::gal_texture_dimension::_2D_MS:
        // TODO(hyl5): how ms map to vk?
        return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
    default:
        return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
    }
}

constexpr VkSampleCountFlagBits utils_to_vk_sample_count_flags(gal_texture_sample_count samples) {
    switch (samples) {
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_1:
        return VK_SAMPLE_COUNT_1_BIT;
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_2:
        return VK_SAMPLE_COUNT_2_BIT;
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_4:
        return VK_SAMPLE_COUNT_4_BIT;
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_8:
        return VK_SAMPLE_COUNT_8_BIT;
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_16:
        return VK_SAMPLE_COUNT_16_BIT;
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_32:
        return VK_SAMPLE_COUNT_32_BIT;
    case ant::gal::gal_texture_sample_count::SAMPLE_COUNT_64:
        return VK_SAMPLE_COUNT_64_BIT;
    default:
        return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
    }
}

//VkImageCreateFlags utils_to_vk_image_creation_flags(resource_types types) {
//    VkImageCreateFlags flags = 0;
//
//    return flags;
//}

constexpr VkImageUsageFlags utils_to_vk_image_usage(gal_descriptor_type types, gal_memory_flag memory_flag) {
    VkImageUsageFlags flags = 0;
    if ((memory_flag & gal_memory_flag::CPU_UPLOAD) != gal_memory_flag::UNDEFINED) {
        flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    } else if ((memory_flag & gal_memory_flag::GPU_DOWNLOAD) != gal_memory_flag::UNDEFINED) {
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if ((types & gal_descriptor_type::COLOR_RT) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if ((types & gal_descriptor_type::DEPTH_STENCIL_RT) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if ((types & gal_descriptor_type::TEXTURE) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if ((types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED) {
        flags |= VK_IMAGE_USAGE_STORAGE_BIT;
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    // add copy usage for all resources
    flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    return flags;
}

constexpr VkImageAspectFlags util_vk_determine_aspect_mask(VkFormat format, bool has_stencil) {
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
constexpr VkFormatFeatureFlags util_vk_image_usage_to_format_features(VkImageUsageFlags usage) {
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

constexpr VkQueueFlags util_to_vk_queue_flags(gal_queue_type queueType) {
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

constexpr gal_texture_format vkformat_to_galtextureformat(VkFormat fmt) {
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

constexpr VkFormat galtextureformat_to_vkformat(gal_texture_format fmt) {
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

constexpr VkSamplerAddressMode utils_to_vk_address_mode(gal_sampler_address_mode address_mode) {
    switch (address_mode) {
    case gal_sampler_address_mode::REPEAT:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case gal_sampler_address_mode::MIRROR:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case gal_sampler_address_mode::CLAMP:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case gal_sampler_address_mode::BORDER:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    default:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }
}

constexpr VkCompareOp utils_to_vk_compare_op(gal_compare_mode compare_mode) {
    switch (compare_mode) {
    case gal_compare_mode::NEVER:
        return VK_COMPARE_OP_NEVER;
    case gal_compare_mode::LESS:
        return VK_COMPARE_OP_LESS;
    case gal_compare_mode::EQUAL:
        return VK_COMPARE_OP_EQUAL;
    case gal_compare_mode::LESS_EQUAL:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case gal_compare_mode::GREATER:
        return VK_COMPARE_OP_GREATER;
    case gal_compare_mode::NOT_EQUAL:
        return VK_COMPARE_OP_NOT_EQUAL;
    case gal_compare_mode::GREATER_EQUAL:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case gal_compare_mode::ALWAYS:
        return VK_COMPARE_OP_ALWAYS;
    default:
        return VK_COMPARE_OP_NEVER;
    }
}

constexpr VkSamplerMipmapMode utils_to_vk_mip_map_mode(gal_sampler_mip_mode mip_mode) {
    switch (mip_mode) {
    case gal_sampler_mip_mode::POINT:
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    case gal_sampler_mip_mode::LINEAR:
        return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    default:
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }
}
// This function converts a gal_texture_sample_count to VkSampleCountFlagBits
constexpr VkSampleCountFlagBits util_to_vk_sample_count(gal_texture_sample_count sampleCount) {
    switch (sampleCount) {
    case gal_texture_sample_count::SAMPLE_COUNT_1:
        return VK_SAMPLE_COUNT_1_BIT;
    case gal_texture_sample_count::SAMPLE_COUNT_2:
        return VK_SAMPLE_COUNT_2_BIT;
    case gal_texture_sample_count::SAMPLE_COUNT_4:
        return VK_SAMPLE_COUNT_4_BIT;
    case gal_texture_sample_count::SAMPLE_COUNT_8:
        return VK_SAMPLE_COUNT_8_BIT;
    case gal_texture_sample_count::SAMPLE_COUNT_16:
        return VK_SAMPLE_COUNT_16_BIT;
    case gal_texture_sample_count::SAMPLE_COUNT_32:
        return VK_SAMPLE_COUNT_32_BIT;
    default:
        return VK_SAMPLE_COUNT_1_BIT;
    }
}
constexpr VkStencilOp utils_to_vk_stencil_op(gal_stencil_op op) {
    switch (op) {
    case ant::gal::gal_stencil_op::UNDEFINED:
        return VK_STENCIL_OP_KEEP;
    case ant::gal::gal_stencil_op::KEEP:
        return VK_STENCIL_OP_KEEP;
    case ant::gal::gal_stencil_op::SET_ZERO:
        return VK_STENCIL_OP_ZERO;
    case ant::gal::gal_stencil_op::REPLACE:
        return VK_STENCIL_OP_REPLACE;
    case ant::gal::gal_stencil_op::INVERT:
        return VK_STENCIL_OP_INVERT;
    case ant::gal::gal_stencil_op::INCR:
        return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case ant::gal::gal_stencil_op::DECR:
        return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    case ant::gal::gal_stencil_op::INCR_SAT:
        return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case ant::gal::gal_stencil_op::DECR_SAT:
        return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    default:
        return VK_STENCIL_OP_KEEP;
    }
}

constexpr VkPipelineDepthStencilStateCreateInfo util_to_depth_desc(const gal_depth_state_desc *desc) {

    VkPipelineDepthStencilStateCreateInfo ds = {};
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.pNext = NULL;
    ds.flags = 0;
    ds.depthTestEnable = desc->mDepthTest ? VK_TRUE : VK_FALSE;
    ds.depthWriteEnable = desc->mDepthWrite ? VK_TRUE : VK_FALSE;
    ds.depthCompareOp = utils_to_vk_compare_op(desc->mDepthFunc);

    ds.depthBoundsTestEnable = VK_FALSE;
    ds.stencilTestEnable = desc->mStencilTest ? VK_TRUE : VK_FALSE;

    ds.front.failOp = utils_to_vk_stencil_op(desc->mStencilFrontFail);
    ds.front.passOp = utils_to_vk_stencil_op(desc->mStencilFrontPass);
    ds.front.depthFailOp = utils_to_vk_stencil_op(desc->mDepthFrontFail);
    ds.front.compareOp = utils_to_vk_compare_op(desc->mStencilFrontFunc);
    ds.front.compareMask = desc->mStencilReadMask;
    ds.front.writeMask = desc->mStencilWriteMask;
    ds.front.reference = 0;

    ds.back.failOp = utils_to_vk_stencil_op(desc->mStencilBackFail);
    ds.back.passOp = utils_to_vk_stencil_op(desc->mStencilBackPass);
    ds.back.depthFailOp = utils_to_vk_stencil_op(desc->mDepthBackFail);
    ds.back.compareOp = utils_to_vk_compare_op(desc->mStencilBackFunc);
    ds.back.compareMask = desc->mStencilReadMask;
    ds.back.writeMask = desc->mStencilWriteMask; // devsh fixed
    ds.back.reference = 0;

    ds.minDepthBounds = 0;
    ds.maxDepthBounds = 1;

    return ds;
}
constexpr VkPolygonMode utils_to_vk_fill_mode(gal_polygon_fill_mode mode) {
    switch (mode) {
    case ant::gal::gal_polygon_fill_mode::SOLID:
        return VK_POLYGON_MODE_FILL;
    case ant::gal::gal_polygon_fill_mode::WIREFRAME:
        return VK_POLYGON_MODE_LINE;
    default:
        return VK_POLYGON_MODE_MAX_ENUM;
    }
}

constexpr VkCullModeFlags utils_to_vk_culll_mode_flags(gal_cull_mode mode) {
    switch (mode) {
    case ant::gal::gal_cull_mode::NONE:
        return VK_CULL_MODE_NONE;
    case ant::gal::gal_cull_mode::BACK:
        return VK_CULL_MODE_BACK_BIT;
    case ant::gal::gal_cull_mode::FRONT:
        return VK_CULL_MODE_FRONT_BIT;
    case ant::gal::gal_cull_mode::BOTH:
        return VK_CULL_MODE_FRONT_AND_BACK;
    default:
        return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
    }
}

constexpr VkShaderStageFlags util_to_vk_shader_stage_flags(gal_shader_stage stages) {
    VkShaderStageFlags res = 0;
    if ((stages & gal_shader_stage::ALL_GRAPHICS) != gal_shader_stage::UNDEFINED)
        return VK_SHADER_STAGE_ALL_GRAPHICS;

    if ((stages & gal_shader_stage::VERT) != gal_shader_stage::UNDEFINED)
        res |= VK_SHADER_STAGE_VERTEX_BIT;
    if ((stages & gal_shader_stage::GEOM) != gal_shader_stage::UNDEFINED)
        res |= VK_SHADER_STAGE_GEOMETRY_BIT;
    if ((stages & gal_shader_stage::TESE) != gal_shader_stage::UNDEFINED)
        res |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    if ((stages & gal_shader_stage::TESC) != gal_shader_stage::UNDEFINED)
        res |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    if ((stages & gal_shader_stage::COMP) != gal_shader_stage::UNDEFINED)
        res |= VK_SHADER_STAGE_COMPUTE_BIT;
#ifdef VK_RAYTRACING_AVAILABLE
    if (stages & SHADER_STAGE_RAYTRACING)
        res |= (VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR |
                VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_INTERSECTION_BIT_KHR | VK_SHADER_STAGE_CALLABLE_BIT_KHR);
#endif

    if (res == 0) {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
    return res;
}
constexpr VkBlendOp utils_to_vk_blend_op(gal_blend_mode bm) {
    switch (bm) {
    case ant::gal::gal_blend_mode::UNDEFINED:
        return VK_BLEND_OP_MAX_ENUM;
    case ant::gal::gal_blend_mode::ADD:
        return VK_BLEND_OP_ADD;
    case ant::gal::gal_blend_mode::SUBTRACT:
        return VK_BLEND_OP_SUBTRACT;
    case ant::gal::gal_blend_mode::REVERSE_SUBTRACT:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    case ant::gal::gal_blend_mode::MIN:
        return VK_BLEND_OP_MIN;
    case ant::gal::gal_blend_mode::MAX:
        return VK_BLEND_OP_MAX;
    default:
        return VK_BLEND_OP_MAX_ENUM;
    }
}
constexpr VkBlendFactor utils_to_vk_blend_factor(BlendConstant bc) {
    switch (bc) {
    case ant::gal::BlendConstant::ZERO:
        return VK_BLEND_FACTOR_ZERO;
    case ant::gal::BlendConstant::ONE:
        return VK_BLEND_FACTOR_ONE;
    case ant::gal::BlendConstant::SRC_COLOR:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case ant::gal::BlendConstant::ONE_MINUS_SRC_COLOR:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case ant::gal::BlendConstant::DST_COLOR:
        return VK_BLEND_FACTOR_DST_COLOR;
    case ant::gal::BlendConstant::ONE_MINUS_DST_COLOR:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case ant::gal::BlendConstant::SRC_ALPHA:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case ant::gal::BlendConstant::ONE_MINUS_SRC_ALPHA:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case ant::gal::BlendConstant::DST_ALPHA:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case ant::gal::BlendConstant::ONE_MINUS_DST_ALPHA:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case ant::gal::BlendConstant::SRC_ALPHA_SATURATE:
        return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    case ant::gal::BlendConstant::BLEND_FACTOR:
        return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case ant::gal::BlendConstant::ONE_MINUS_BLEND_FACTOR:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    default:
        return VK_BLEND_FACTOR_MAX_ENUM;
    }
}

constexpr VkPipelineColorBlendStateCreateInfo util_to_blend_desc(const gal_blend_state_desc *desc,
                                                                 VkPipelineColorBlendAttachmentState *pAttachments) {
    int blendDescIndex = 0;
    //#if defined(ENABLE_GRAPHICS_DEBUG)
    //
    //    for (uint32_t i = 0; i < MAX_RENDER_TARGET_ATTACHMENTS; ++i) {
    //        if (desc->mRenderTargetMask & (1 << i)) {
    //            ASSERT(desc->mSrcFactors[blendDescIndex] < BlendConstant::MAX_BLEND_CONSTANTS);
    //            ASSERT(desc->mDstFactors[blendDescIndex] < BlendConstant::MAX_BLEND_CONSTANTS);
    //            ASSERT(desc->mSrcAlphaFactors[blendDescIndex] < BlendConstant::MAX_BLEND_CONSTANTS);
    //            ASSERT(desc->mDstAlphaFactors[blendDescIndex] < BlendConstant::MAX_BLEND_CONSTANTS);
    //            ASSERT(desc->mBlendModes[blendDescIndex] < BlendMode::MAX_BLEND_MODES);
    //            ASSERT(desc->mBlendAlphaModes[blendDescIndex] < BlendMode::MAX_BLEND_MODES);
    //        }
    //
    //        if (desc->mIndependentBlend)
    //            ++blendDescIndex;
    //    }
    //
    //    blendDescIndex = 0;
    //#endif

    for (uint32_t i = 0; i < MAX_RENDER_TARGET_ATTACHMENTS; ++i) {
        if (desc->mRenderTargetMask & (1 << i)) {
            VkBool32 blendEnable =
                (utils_to_vk_blend_factor(desc->mSrcFactors[blendDescIndex]) != VK_BLEND_FACTOR_ONE ||
                 utils_to_vk_blend_factor(desc->mDstFactors[blendDescIndex]) != VK_BLEND_FACTOR_ZERO ||
                 utils_to_vk_blend_factor(desc->mSrcAlphaFactors[blendDescIndex]) != VK_BLEND_FACTOR_ONE ||
                 utils_to_vk_blend_factor(desc->mDstAlphaFactors[blendDescIndex]) != VK_BLEND_FACTOR_ZERO);

            pAttachments[i].blendEnable = blendEnable;
            pAttachments[i].colorWriteMask = desc->mMasks[blendDescIndex];
            pAttachments[i].srcColorBlendFactor = utils_to_vk_blend_factor(desc->mSrcFactors[blendDescIndex]);
            pAttachments[i].dstColorBlendFactor = utils_to_vk_blend_factor(desc->mDstFactors[blendDescIndex]);
            pAttachments[i].colorBlendOp = utils_to_vk_blend_op(desc->mBlendModes[blendDescIndex]);
            pAttachments[i].srcAlphaBlendFactor = utils_to_vk_blend_factor(desc->mSrcAlphaFactors[blendDescIndex]);
            pAttachments[i].dstAlphaBlendFactor = utils_to_vk_blend_factor(desc->mDstAlphaFactors[blendDescIndex]);
            pAttachments[i].alphaBlendOp = utils_to_vk_blend_op(desc->mBlendAlphaModes[blendDescIndex]);
        }

        if (desc->mIndependentBlend)
            ++blendDescIndex;
    }

    VkPipelineColorBlendStateCreateInfo cb = {};
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.pNext = NULL;
    cb.flags = 0;
    cb.logicOpEnable = VK_FALSE;
    cb.logicOp = VK_LOGIC_OP_CLEAR;
    cb.pAttachments = pAttachments;
    cb.blendConstants[0] = 0.0f;
    cb.blendConstants[1] = 0.0f;
    cb.blendConstants[2] = 0.0f;
    cb.blendConstants[3] = 0.0f;

    return cb;
}

constexpr VkFrontFace utils_to_vk_front_face(gal_front_face ff) {
    switch (ff) {
    case ant::gal::gal_front_face::CCW:
        return VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
    case ant::gal::gal_front_face::CW:
        return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
    default:
        return VkFrontFace::VK_FRONT_FACE_MAX_ENUM;
    }
}

constexpr VkPipelineRasterizationStateCreateInfo util_to_rasterizer_desc(const gal_rasterizer_state_desc *desc) {

    VkPipelineRasterizationStateCreateInfo rs = {};
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;
    rs.flags = 0;
    rs.depthClampEnable = desc->mDepthClampEnable ? VK_TRUE : VK_FALSE;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.polygonMode = utils_to_vk_fill_mode(desc->mFillMode);
    rs.cullMode = utils_to_vk_culll_mode_flags(desc->mCullMode);
    rs.frontFace = utils_to_vk_front_face(desc->mFrontFace);
    rs.depthBiasEnable = (desc->mDepthBias != 0) ? VK_TRUE : VK_FALSE;
    rs.depthBiasConstantFactor = float(desc->mDepthBias);
    rs.depthBiasClamp = 0.0f;
    rs.depthBiasSlopeFactor = desc->mSlopeScaledDepthBias;
    rs.lineWidth = 1;
    return rs;
}

constexpr VkPipelineBindPoint utils_to_vk_pipeline_bind_point(gal_pipeline_type type) {
    switch (type) {
    case ant::gal::gal_pipeline_type::COMPUTE:
        return VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;
    case ant::gal::gal_pipeline_type::GRAPHICS:
        return VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
    case ant::gal::gal_pipeline_type::RAYTRACING:
        VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
    default:
        return VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_MAX_ENUM;
    }
}

constexpr VkIndexType utils_to_vk_index_type(gal_index_type type) {
    switch (type) {
    case ant::gal::gal_index_type::u32:
        return VkIndexType::VK_INDEX_TYPE_UINT32;
    case ant::gal::gal_index_type::u16:
        return VkIndexType::VK_INDEX_TYPE_UINT16;
    default:
        return VkIndexType::VK_INDEX_TYPE_MAX_ENUM;
    }
}

constexpr VkImageAspectFlags utils_to_vk_aspect_mask(gal_texture_format format, [[maybe_unused]]bool b_has_stencil) {
    VkImageAspectFlags flags = 0;
    if (gal_tf_has_depth_or_stencil(format)) {
        if (gal_tf_has_stencil(format)) {
            flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        if (gal_tf_has_depth(format)) {
            flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
        }
        return flags;
    }
    flags |= VK_IMAGE_ASPECT_COLOR_BIT;
    return flags;
}
} // namespace ant::gal