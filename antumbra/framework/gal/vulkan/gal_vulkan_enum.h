// this header can only included by gal_vulkan 
#pragma once

#include <atomic>
#include <mutex>
#include <variant>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#elif __linux__
#define VK_USE_PLATFORM_XLIB_KHR
#elif __APPLE__
#define VK_USE_PLATFORM_MACOS_MVK
#endif
#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>


#include "../enum.h"
#include "../gal.h"


namespace ante::gal {

#define DECLARE_VK_HANDLE(name) struct vk_##name : public gal_##name##_T

DECLARE_VK_HANDLE(queue) {
    bool used = false;
    u32 queue_family_index = 0;
    VkQueue queue = VK_NULL_HANDLE;
    //std::mutex submit_mutex; // prevent submit single queue in multithread
};

DECLARE_VK_HANDLE(context) {
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice active_gpu = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VmaAllocator vma_allocator = VK_NULL_HANDLE;

    //vk_queue graphics_queue;
    //vk_queue compute_queue;
    //vk_queue transfer_queue;
    ante::fixed_array<vk_queue, 3> queues;
    f32 default_queue_property = 0.0f;
};

DECLARE_VK_HANDLE(buffer) {
    VkBuffer m_buffer;
    VmaAllocation m_allocation;
};

DECLARE_VK_HANDLE(texture) {
    VkImage m_image;
    VmaAllocation m_allocation;
    //VkImageView pVkSRVDescriptor;
    ///// Opaque handle used by shaders for doing read/write operations on the texture
    //VkImageView pVkUAVDescriptors;
    ///// Opaque handle used by shaders for doing read/write operations on the texture
    //VkImageView pVkSRVStencilDescriptor;
    //VkImageView srv;
    /// Opaque handle used by shaders for doing read/write operations on the texture
    //VkImageView uav;
    /// Opaque handle used by shaders for doing read/write operations on the texture
    //VkImageView pVkSRVStencilDescriptor;
    VkImageView m_view;
};

DECLARE_VK_HANDLE(sampler) { VkSampler m_sampler; };

DECLARE_VK_HANDLE(render_target) {
    VkImageView pVkDescriptor;
    VkImageView *pVkSliceDescriptors;
};

//struct vk_buffer_desciptor_view {
//    VkDescriptorBufferInfo buffer_info;
//    VkBufferView view;
//};
//struct vk_image_desciptor_view {
//    VkDescriptorImageInfo image_info;
//    VkImageView view;
//};
//
//DECLARE_VK_HANDLE(srv_descriptor_view) {
//    constexpr void initialize(gal_src_descriptor_view_desc * desc) { m_desc = *desc; }
//    std::variant<vk_buffer_desciptor_view, vk_image_desciptor_view> m_view;
//};
//DECLARE_VK_HANDLE(uav_descriptor_view) {
//    constexpr void initialize(gal_uav_descriptor_view_desc * desc) { m_desc = *desc; }
//    std::variant<vk_buffer_desciptor_view, vk_image_desciptor_view> m_view;
//};

DECLARE_VK_HANDLE(fence) {
    VkFence fence;
    bool b_submitted;
};
DECLARE_VK_HANDLE(semaphore) {
    VkSemaphore semaphore;
    bool b_signaled;
};
DECLARE_VK_HANDLE(swap_chain) {
    ante::fixed_array<gal_render_target, MAX_SWAPCHAIN_IMAGES> &get_render_targets() { return m_render_targets; }
    u32 image_index;
    VkSwapchainKHR m_swap_chain = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSurfaceFormatKHR optimal_surface_format{};
    ante::fixed_array<VkImage, MAX_SWAPCHAIN_IMAGES> swap_chain_images{};
    ante::fixed_array<VkImageView, MAX_SWAPCHAIN_IMAGES> swap_chain_image_views{};
};

DECLARE_VK_HANDLE(command_pool) { VkCommandPool m_cmd_pool; };

DECLARE_VK_HANDLE(command_list) {
    gal_command_pool m_cmd_pool;
    VkCommandBuffer m_command;
    bool b_render_pass_bound;
};

DECLARE_VK_HANDLE(shader_program) {
    ante::fixed_array<VkShaderModule, gal_shader_stage_count> m_shader_modules;
    ante::fixed_array<const char *, gal_shader_stage_count> m_entrys;
    VkSpecializationInfo *m_specialization_info;
};

DECLARE_VK_HANDLE(root_signature) {
    VkPipelineLayout pipeline_layout;
    ante::fixed_array<VkDescriptorSetLayout, MAX_DESCRIPTOR_SET_COUNT> set_layouts;
    ante::fixed_array<VkDescriptorUpdateTemplate, MAX_DESCRIPTOR_SET_COUNT> descriptor_set_update_template;

    struct vk_pool_size_desc {
        VkDescriptorPoolSize *pool_sizes;
        u32 pool_size_count;
    };

    ante::fixed_array<vk_pool_size_desc, MAX_DESCRIPTOR_SET_COUNT> descriptor_pool_size;                      // an unordered_map
    ante::hash_map<ante::str, u32> resource_map; // 3bit set index(0-8), 6 bit binding index(0-64), 6 bit binding order(0-64), 17 bit empty
    // FIXME(hyl5): resource_map is not controlled by pmr
};

DECLARE_VK_HANDLE(pipeline) { VkPipeline pipeline; };
DECLARE_VK_HANDLE(pipeline_cache) { VkPipelineCache pipeline_cache; };

struct vk_descriptor_pool {
    VkDescriptorPool pool;
    //std::atomic<u32> ref_count; // TODO(hyl5): consider the thread safety
    u32 ref_count = 0;
    // precompute the descriptor set update template when create pipeline layout, require vulkan 1.1
    // https://github.com/SakuraEngine/SakuraEngine/blob/main/modules/runtime/src/cgpu/vulkan/cgpu_vulkan.c
    gal_root_signature root_signature;
};

struct vk_descriptor_pool_desc {
    u32 numDescriptorSets;
    VkDescriptorPoolCreateFlags flags;
    const VkDescriptorPoolSize *pPoolSizes;
    u32 numPoolSizes;
};

DECLARE_VK_HANDLE(descriptor_set) {
    VkDescriptorSet set;
    vk_descriptor_pool *pool;
};

// https://github.com/SakuraEngine/SakuraEngine/blob/main/modules/runtime/include/cgpu/backend/vulkan/cgpu_vulkan.h#L429
union VkDescriptorUpdateData {
    VkDescriptorImageInfo image_info;
    VkDescriptorBufferInfo buffer_info;
    VkBufferView buffer_view;
};

} // namespace ante::gal