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

#define DECLARE_VK_FUNCTION(ret, name, ...) ret vk_##name(__VA_ARGS__);

struct vk_gal_context
{
    VkInstance instance;
    VkPhysicalDevice active_gpu;
    VkDevice device;
    VmaAllocator *vma_allocator;
};

gal_error_code vk_init_gal(gal_context *context);
gal_error_code vk_destroy_gal(gal_context *context);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_instance, GalDesc *gal_desc, gal_context *context);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_instane, gal_context *context);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, select_gpu, GalDesc *desc, gal_context *context);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_device, GalDesc* gal_desc, gal_context *context);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_device, gal_context *context);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, init_memory_allocator, gal_context *context);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_buffer);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_buffer);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_texture);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_texture);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_sampler);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_sampler);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_swap_chain);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_swap_chain);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_shader);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_shader);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_pipeline);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_pipeline);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, create_surface);

DECLARE_VK_FUNCTION(ant::gal::gal_error_code, destroy_surface);

// cmds
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, cmd_begin_recording);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, cmd_end_recording);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, bind_descriptor_set);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, bind_index_buffer);
DECLARE_VK_FUNCTION(ant::gal::gal_error_code, bind_index_buffer);

}// namespace ant::gal
