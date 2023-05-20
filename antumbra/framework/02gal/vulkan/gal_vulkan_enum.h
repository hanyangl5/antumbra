#pragma once

#include "../enum.h"
#include "../gal.h"

namespace ant::gal {

#define DECLARE_VK_HANDLE(name) struct vk_##name : protected gal_##name##_T

DECLARE_VK_HANDLE(context) {
    constexpr void initialize(gal_desc * _desc) { m_gal_desc = *_desc; }
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice active_gpu = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VmaAllocator vma_allocator = VK_NULL_HANDLE;

    // queues
    u32 graphcis_queue_family_index = 0;
    u32 compute_queue_family_index = 0;
    u32 transfer_queue_family_index = 0;
    f32 default_queue_property = 0.0f;
    VkQueue graphics_queue = VK_NULL_HANDLE;
    VkQueue compute_queue = VK_NULL_HANDLE;
    VkQueue transfer_queue = VK_NULL_HANDLE;
};

DECLARE_VK_HANDLE(buffer) {
    constexpr void initialize(u64 _size, gal_memory_flags _memory_flags, gal_resource_states _resource_states) {
        m_size = _size;
        m_memory_flags = _memory_flags;
        m_resource_states = _resource_states;
    }
    VkBuffer buffer;
    VmaAllocation allocation;
};

DECLARE_VK_HANDLE(texture) {
    VkImage image;
    VmaAllocation allocation;
};

struct vk_sampler {
    VkSampler sampler;
};
struct vk_rendertarget {};
struct vk_fence {};
struct vk_semaphore {};
struct vk_swapchain {};
struct vk_commandlist {};
struct vk_shader {
    ant::str entry;
    VkShaderModule shader = VK_NULL_HANDLE;
};

struct vk_rootsignature {
    VkPipelineLayout pipeline_layout;
};
struct vk_pipeline {
    VkPipeline pipeline;
};
struct vk_pipelinecache {
    VkPipelineCache pipeline_cache;
};

} // namespace ant::gal