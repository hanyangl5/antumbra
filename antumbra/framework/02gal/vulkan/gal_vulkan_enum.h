#pragma once

#include <variant>

#include "../enum.h"
#include "../gal.h"

namespace ant::gal {

#define DECLARE_VK_HANDLE(name) struct vk_##name : protected gal_##name##_T

DECLARE_VK_HANDLE(context) {
    void initialize(gal_desc * _desc) { m_gal_desc = *_desc; }
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
    void initialize(gal_buffer_desc * _desc) { m_gal_buffer_desc = *_desc; }
    VkBuffer m_buffer;
    VmaAllocation m_allocation;
};

DECLARE_VK_HANDLE(texture) {
    void initialize(gal_texture_desc * _desc) { m_gal_texture_desc = *_desc; }
    gal_texture_flag get_flags() { return m_gal_texture_desc.texture_flags; }
    VkImage m_image;
    VmaAllocation m_allocation;
    VkImageView pVkSRVDescriptor;
    /// Opaque handle used by shaders for doing read/write operations on the texture
    VkImageView *pVkUAVDescriptors;
    /// Opaque handle used by shaders for doing read/write operations on the texture
    VkImageView pVkSRVStencilDescriptor;
};

DECLARE_VK_HANDLE(sampler) {
    void initialize(gal_sampler_desc * _desc) { m_gal_sampler_desc = *_desc; }
    VkSampler m_sampler;
};

DECLARE_VK_HANDLE(render_target) {
    void initialize(gal_render_target_desc * _desc) { m_desc = *_desc; }
    gal_texture *get_texture() { return &m_texture; }
    gal_render_target_desc *get_render_target_desc() { return &m_desc; }
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
//    constexpr void initialize(gal_src_descriptor_view_desc * _desc) { m_desc = *_desc; }
//    std::variant<vk_buffer_desciptor_view, vk_image_desciptor_view> m_view;
//};
//DECLARE_VK_HANDLE(uav_descriptor_view) {
//    constexpr void initialize(gal_uav_descriptor_view_desc * _desc) { m_desc = *_desc; }
//    std::variant<vk_buffer_desciptor_view, vk_image_desciptor_view> m_view;
//};

struct vk_fence {};
struct vk_semaphore {};
DECLARE_VK_HANDLE(swap_chain) {
    void initialize(gal_swap_chain_desc * _desc) { m_desc = *_desc; }
    ant::fixed_array<gal_render_target, MAX_SWAPCHAIN_IMAGES> &get_render_targets() { return m_render_targets; }
    VkSwapchainKHR m_swap_chain = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSurfaceFormatKHR optimal_surface_format{};
    ant::fixed_array<VkImage, MAX_SWAPCHAIN_IMAGES> swap_chain_images{};
    ant::fixed_array<VkImageView, MAX_SWAPCHAIN_IMAGES> swap_chain_image_views{};
};

struct vk_commandlist {};

DECLARE_VK_HANDLE(shader_program) {
    void initialize(gal_shader_program_desc * _desc, u32 _stage_count) {
        m_desc = *_desc;
        m_stage_count = _stage_count;
    }
    gal_shader_program_desc *get_desc() { return &m_desc;
    }

    ant::vector<VkShaderModule> m_shader_modules;
    ant::vector<ant::str> m_entry_names;
    VkSpecializationInfo *m_specialization_info;
};

struct vk_rootsignature {
    VkPipelineLayout pipeline_layout;
};



DECLARE_VK_HANDLE(pipeline) {
    void initialize(gal_pipeline_desc * _desc) {
        if (std::holds_alternative<gal_compute_pipeline_desc>(_desc->desc)) {
            m_type = gal_pipeline_type::PIPELINE_TYPE_COMPUTE;
        } else if (std::holds_alternative<gal_graphics_pipeline_desc>(_desc->desc)) {
            m_type = gal_pipeline_type::PIPELINE_TYPE_GRAPHICS;
        } else if (std::holds_alternative<gal_raytracing_pipeline_desc>(_desc->desc)) {
            m_type = gal_pipeline_type::PIPELINE_TYPE_RAYTRACING;
        } else {
            LOG_ERROR("invalid pipeline desc");
            return;
        }
        m_desc = *_desc;
    }
    VkPipeline pipeline;
};
struct vk_pipelinecache {
    VkPipelineCache pipeline_cache;
};

} // namespace ant::gal