#pragma once

// standard libraries
#include <memory_resource>

// third party libraries
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>
// project headers
#include <framework/01core/utils/utils.h>
#include <framework/02gal/gal.h>

namespace ant::gal {

struct vk_gal_context {
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

gal_error_code vk_init_gal(gal_context *context);


#define VK_FUNCTION_DECLARE
#include "../helper/helper_macro.h"
#undef VK_FUNCTION_DECLARE

} // namespace ant::gal
