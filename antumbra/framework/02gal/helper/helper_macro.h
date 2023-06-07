// clang-format off
#if defined(VK_FUNCTION_DECLARE)
// declare vulkan functions in gal_vulkan.h
#define __GAL_HELPER_MACRO__(ret, name, ...) ret vk_##name(__VA_ARGS__);

#elif defined(D3D12_FUNCTION_DECLARE)
// declare d3d12 functions in gal_d3d12.h
#define __GAL_HELPER_MACRO__(ret, name, ...) ret d3d12_##name(__VA_ARGS__);

#elif defined(GAL_FUNCTION_DEFINATION)
// define function pointer in gal.cpp
#define __GAL_HELPER_MACRO__(ret, name, ...) gal_pfn_##name name;

#elif defined(GAL_FUNCTION_DECLARE)
// declare gal functions in gal.h
#define __GAL_HELPER_MACRO__(ret, name, ...)                                                                           \
    using gal_pfn_##name = ret (*)(__VA_ARGS__);                                                                       \
    extern gal_pfn_##name name;
// load/offload function ptrs
#elif defined(VK_LOAD_FUNCTION_PTRS)
#define __GAL_HELPER_MACRO__(ret, name, ...) if(&vk_##name) name = &vk_##name;
#elif defined(VK_OFFLOAD_FUNCTION_PTRS)
#define __GAL_HELPER_MACRO__(ret, name, ...) name = nullptr;
#elif defined(D3D12_LOAD_FUNCTION_PTRS)
#define __GAL_HELPER_MACRO__(ret, name, ...) if (&d3d12_##name) name = &d3d12_##name;
#elif defined(D3D12_OFFLOAD_FUNCTION_PTRS)
#define __GAL_HELPER_MACRO__(ret, name, ...) name = nullptr;
#else
#define __GAL_HELPER_MACRO__(ret, name, ...)
#error invalid
#endif

// clang-format on
//
// destroy gal_context
__GAL_HELPER_MACRO__(gal_error_code, destroy_gal, gal_context context);
// TODO(hyl5): how to add [[nodiscard]] attribute to these function?

// create gal instance, vkinstance or dxgifactory
__GAL_HELPER_MACRO__(gal_error_code, create_instance, gal_desc *desc, gal_context *context);
// destroy gal instance
__GAL_HELPER_MACRO__(gal_error_code, destroy_instance, gal_context *context);
// select required gpu and create gal device
__GAL_HELPER_MACRO__(gal_error_code, create_device, gal_desc *_gal_desc, gal_context *context);
// destroy gal device
__GAL_HELPER_MACRO__(gal_error_code, destroy_device, gal_context *context);
// create memory allocator, vma or d3dma
__GAL_HELPER_MACRO__(gal_error_code, create_memory_allocator, gal_context *context);
// destroy memory allocator
__GAL_HELPER_MACRO__(gal_error_code, destroy_memory_allocator, gal_context *context);
// resources
// create a gpu buffer and allocate gpu memory
__GAL_HELPER_MACRO__(gal_error_code, create_buffer, gal_context context, gal_buffer_desc *desc, gal_buffer *buffer);
// destroy a gpu buffer
__GAL_HELPER_MACRO__(gal_error_code, destroy_buffer, gal_context context, gal_buffer buffer);
// create a gpu texture and allocate memory
__GAL_HELPER_MACRO__(gal_error_code, create_texture, gal_context context, gal_texture_desc *desc, gal_texture *texture);
// destroy gpu texture
__GAL_HELPER_MACRO__(gal_error_code, destroy_texture, gal_context context, gal_texture texture);

__GAL_HELPER_MACRO__(gal_error_code, create_sampler, gal_context context, gal_sampler_desc *sampler_desc,
                     gal_sampler *sampler);
__GAL_HELPER_MACRO__(gal_error_code, destroy_sampler, gal_context context, gal_sampler sampler);

__GAL_HELPER_MACRO__(gal_error_code, create_render_target, gal_context context, gal_render_target_desc *desc,
                     gal_render_target *render_target);

__GAL_HELPER_MACRO__(gal_error_code, destroy_render_target, gal_context context, gal_render_target render_target);
// surface
__GAL_HELPER_MACRO__(gal_error_code, create_swap_chain, gal_context context, gal_swap_chain_desc *desc,
                     gal_swap_chain *_swap_chain);
__GAL_HELPER_MACRO__(gal_error_code, destroy_swap_chain, gal_context context, gal_swap_chain _swap_chain);
// pipeline
__GAL_HELPER_MACRO__(gal_error_code, create_shader, gal_context context, gal_shader_desc *desc, gal_shader *_shader);
__GAL_HELPER_MACRO__(gal_error_code, destroy_shader, gal_context context, gal_shader _shader);

__GAL_HELPER_MACRO__(gal_error_code, create_pipeline_cache, gal_context context, gal_pipeline_cache_desc *desc,
                     gal_pipeline_cache *pipeline_cache);

__GAL_HELPER_MACRO__(gal_error_code, destroy_pipeline_cache, gal_context context, gal_pipeline_cache _pipeline_cache);

__GAL_HELPER_MACRO__(gal_error_code, get_pipeline_cache_data, gal_context context, gal_pipeline_cache _pipeline_cache, u64 *size, void *_data);
__GAL_HELPER_MACRO__(gal_error_code, create_compute_pipeline, gal_context context, gal_pipeline_desc *desc,
                     gal_pipeline *pipeline);
//__GAL_HELPER_MACRO__(gal_error_code, create_graphics_pipeline, gal_context context, gal_graphics_pipeline_desc *desc,
//                     gal_pipeline *pipeline);
// inline raytracing?
//__GAL_HELPER_MACRO__(gal_error_code, create_raytracing_pipeline, gal_context context,
//                     gal_raytracing_pipeline_desc *desc, gal_pipeline *pipeline);

__GAL_HELPER_MACRO__(gal_error_code, destroy_pipeline);
//__GAL_HELPER_MACRO__(gal_error_code, create_descriptorpool);
__GAL_HELPER_MACRO__(gal_error_code, destroy_descriptorpool);
// consume the descriptorset from pool
__GAL_HELPER_MACRO__(gal_error_code, consume_descriptorset);
__GAL_HELPER_MACRO__(gal_error_code, free_descriptorset);
__GAL_HELPER_MACRO__(gal_error_code, create_rootsignature);
__GAL_HELPER_MACRO__(gal_error_code, destroy_rootsignature);
// sync
__GAL_HELPER_MACRO__(gal_error_code, create_fence);
__GAL_HELPER_MACRO__(gal_error_code, wait_fence);
__GAL_HELPER_MACRO__(gal_error_code, destroy_fence);
__GAL_HELPER_MACRO__(gal_error_code, wait_gpu);
__GAL_HELPER_MACRO__(gal_error_code, create_semaphore);
__GAL_HELPER_MACRO__(gal_error_code, destroy_semaphore);
// cmds
__GAL_HELPER_MACRO__(gal_error_code, create_commandpool);
__GAL_HELPER_MACRO__(gal_error_code, reset_commandpool);
__GAL_HELPER_MACRO__(gal_error_code, destroy_commandpool);
__GAL_HELPER_MACRO__(gal_error_code, allocate_commandlist);
__GAL_HELPER_MACRO__(gal_error_code, free_commandlist);
__GAL_HELPER_MACRO__(gal_error_code, cmd_begin_recording);
__GAL_HELPER_MACRO__(gal_error_code, cmd_end_recording);
__GAL_HELPER_MACRO__(gal_error_code, cmd_resource_barrier);
__GAL_HELPER_MACRO__(gal_error_code, cmd_bind_index_buffer);
__GAL_HELPER_MACRO__(gal_error_code, cmd_bind_vertex_buffer);
__GAL_HELPER_MACRO__(gal_error_code, cmd_bind_pipeline);
__GAL_HELPER_MACRO__(gal_error_code, cmd_begin_renderpass);
__GAL_HELPER_MACRO__(gal_error_code, cmd_end_renderpass);
__GAL_HELPER_MACRO__(gal_error_code, cmd_dispatch);
__GAL_HELPER_MACRO__(gal_error_code, cmd_dispatch_indirect);
__GAL_HELPER_MACRO__(gal_error_code, cmd_draw_instanced);
__GAL_HELPER_MACRO__(gal_error_code, cmd_draw_indexed_instanced);
__GAL_HELPER_MACRO__(gal_error_code, cmd_draw_indirect_instanced);
__GAL_HELPER_MACRO__(gal_error_code, cmd_draw_indirect_indexed_instanced);
__GAL_HELPER_MACRO__(gal_error_code, cmd_draw_mesh_task);
__GAL_HELPER_MACRO__(gal_error_code, cmd_draw_indirect_mesh_task);
__GAL_HELPER_MACRO__(gal_error_code, cmd_copy_texture);
__GAL_HELPER_MACRO__(gal_error_code, cmd_copy_buffer);
__GAL_HELPER_MACRO__(gal_error_code, cmd_fill_buffer);
__GAL_HELPER_MACRO__(gal_error_code, cmd_fill_texture);
__GAL_HELPER_MACRO__(gal_error_code, cmd_upload_buffer);
__GAL_HELPER_MACRO__(gal_error_code, cmd_upload_texture);
__GAL_HELPER_MACRO__(gal_error_code, cmd_copy_buffer_to_texture);
__GAL_HELPER_MACRO__(gal_error_code, cmd_copy_texture_to_buffer);

#ifdef __GAL_HELPER_MACRO__
#undef __GAL_HELPER_MACRO__
#endif // __GAL_HELPER_MACRO__

#if defined(VK_FUNCTION_DECLARE)
#undef VK_FUNCTION_DECLARE
#elif defined(D3D12_FUNCTION_DECLARE)
#undef D3D12_FUNCTION_DECLARE
#elif defined(GAL_FUNCTION_DEFINATION)
#undef GAL_FUNCTION_DEFINATION
#elif defined(GAL_FUNCTION_DECLARE)
#undef GAL_FUNCTION_DECLARE
#elif defined(VK_LOAD_FUNCTION_PTRS)
#undef VK_LOAD_FUNCTION_PTRS
#elif defined(VK_OFFLOAD_FUNCTION_PTRS)
#undef VK_OFFLOAD_FUNCTION_PTRS
#elif defined(D3D12_LOAD_FUNCTION_PTRS)
#undef D3D12_LOAD_FUNCTION_PTRS
#elif defined(D3D12_OFFLOAD_FUNCTION_PTRS)
#undef D3D12_OFFLOAD_FUNCTION_PTRS
#else
#endif

// #ifdef WIN32
// #def ine  DECLARE_GAL_RESOURCE(name) \
//    struct vk_##name;              \
//    struct d3d12_##name;           \
//    struct name                    \
//    {                              \
//        vk_##name *vk;             \
//        d3d12_##name *d3d12;       \
//    };
// #else
// #def ine  DECLARE_GAL_RESOURCE(name) \
//    struct vk_##name;              \
//    struct name                    \
//    {                              \
//        vk_##name *vk;             \
//    };
// #endif// WIN32

// TODO(hyl5): support multiple api?
