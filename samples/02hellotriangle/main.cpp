// #include <cassert>
// #include <framework/01core/input/input.h>
// #include <framework/01core/math/math.h>
// #include <framework/01core/memory/smart_ptr.h>
// #include <framework/02gal/gal.h>
// #include <framework/02gal/shader/shader_compiler.h>
// using namespace ante;

// constexpr void CHECK_ERR_CODE(gal::gal_error_code err) {
//     if (err != gal::gal_error_code::SUC) {
//         std::terminate();
//     }
// }

// class HelloTriangle {
//   public:
//     DELETE_COPY_MOVE(HelloTriangle)
//     HelloTriangle() {
//         window = ante::memory::make_unique<ante_window>(nullptr);
//         window_desc desc{};
//         desc.width = width;
//         desc.height = height;
//         desc.name = "02hellotriangle";
//         desc.b_vsync = false;
//         // create window
//         create_window(&desc, window.get());

//         // load gal functions
//         CHECK_ERR_CODE(gal::init_gal(gal::gal_api::VULKAN, &context));

//         gal::gal_desc gal_desc{};
//         gal_desc.b_debug_layer = true;
//         gal_desc.b_swap_chain = true;
//         gal_desc.b_surface = true;
//         gal_desc.b_async_compute = false;
//         gal_desc.b_async_transfer = false;
//         // gal_desc.b_ray_tracing = true;
//         //gal_desc.b_mesh_shader = true;
//         //gal_desc.b_variable_rate_shading = true;
//         CHECK_ERR_CODE(ante::gal::create_instance(&gal_desc, &context));
//         CHECK_ERR_CODE(ante::gal::create_device(&gal_desc, &context));
//         CHECK_ERR_CODE(ante::gal::create_memory_allocator(&context));

//         gal::gal_queue_desc queue_desc{};
//         queue_desc.type = gal::gal_queue_type::graphcis;
//         CHECK_ERR_CODE(gal::add_queue(context, &queue_desc, &queue));

//         gal::gal_swap_chain_desc swap_chain_desc{};
//         swap_chain_desc.window = window.get();
//         swap_chain_desc.b_present = true;
//         swap_chain_desc.b_vsync = true;
//         swap_chain_desc.width = width;
//         swap_chain_desc.height = height;
//         swap_chain_desc.format = gal::gal_texture_format::R8G8B8A8_UNORM;
//         swap_chain_desc.image_count = 3;
//         swap_chain_desc.clear_value.value = gal::gal_clear_value::rgb{0.0f, 0.0f, 0.0f, 1.0f};
//         CHECK_ERR_CODE(gal::create_swap_chain(context, &swap_chain_desc, &swap_chain));

//         gal::create_fence(context, &fence);

//         gal::gal_command_pool_desc command_pool_desc{};
//         command_pool_desc.b_transient = false;
//         command_pool_desc.queue_type = gal::gal_queue_type::graphcis;
//         gal::create_command_pool(context, &command_pool_desc, &command_pool);
//         gal::gal_command_list_desc command_list_desc{};
//         command_list_desc.command_pool = command_pool;
//         command_list_desc.queue_type = gal::gal_queue_type::graphcis;
//         gal::get_command_list(context, &command_list_desc, &command_list);
//         // pipeline resource
//         {
//         ante::str test_shader = "\
//             struct PSInput {\
//             float4 position : SV_POSITION;\
//             float4 color : COLOR;\
//         };\
//         PSInput VSMain(float4 position : POSITION, float4 color : COLOR) {\
//             PSInput result;\
//             result.position = position;\
//             result.color = color;\
//             return result;\
//         }\
//         float4 PSMain(PSInput input) : SV_TARGET { return input.color; }\
//         ";
//         // compile shader from source
//         gal::shader_compiler sc;
//         gal::shader_source_blob source;
//         source.set(test_shader.data(), test_shader.size());

//         gal::shader_compile_desc vs_desc;
//         vs_desc.entry = "VS_MAIN";
//         vs_desc.optimization_level = gal::shader_optimization_level::NONE;
//         vs_desc.target_api = gal::shader_blob_type::SPIRV;
//         vs_desc.target_profile = gal::shader_target_profile::VS_6_0;

//         gal::shader_compile_desc ps_desc;
//         ps_desc.entry = "PS_MAIN";
//         ps_desc.optimization_level = gal::shader_optimization_level::NONE;
//         ps_desc.target_api = gal::shader_blob_type::SPIRV;
//         ps_desc.target_profile = gal::shader_target_profile::PS_6_0;

//         gal::gal_shader_program sp{};
//         gal::compiled_shader_group sg{};
//         gal::shader_gourp_source_desc sg_desc{};

//         sg_desc.desc_vert = &vs_desc;
//         sg_desc.desc_frag = &ps_desc;
//         sg.set_from_source(&source, &sg_desc);

//         // root signature
//         gal::gal_root_signature_desc rs_desc{};
//         rs_desc.shader = &sg;
//         gal::create_root_signature(context, &rs_desc, &root_signature);

//         // create shader program
//         gal::gal_shader_program_desc sp_desc{};
//         sp_desc.shader_group = &sg;

//         gal::create_shader_program(context, &sp_desc, &sp);

//         // create pso
//         gal::gal_graphics_pipeline_desc gfx_pipe_desc{};
//         gfx_pipe_desc.root_signature = root_signature;
//         gfx_pipe_desc.shader = sp;

//         gal::gal_pipeline_desc pipe_desc{};
//         pipe_desc.desc = gfx_pipe_desc;

//         gal::create_graphics_pipeline(context, &pipe_desc, &pipeline);
//         sg.release();
//         }

//         // upload buffer(aka staging buffer)
//         gal::gal_buffer_desc upload_buf_desc{};
//         upload_buf_desc.descriptor_types = gal::gal_descriptor_type::UNDEFINED;
//         upload_buf_desc.memory_flags = gal::gal_memory_flag::CPU_UPLOAD;
//         upload_buf_desc.initial_state = gal::gal_resource_state::COPY_SRC;
//         //upload_buf_desc.flags = gal_buffer_flag::PERSISTENT_MAP;
//         upload_buf_desc.size = sizeof(triangle_vertices);
//         gal::create_buffer(context, &upload_buf_desc, &upload_buf);

//         gal::read_range range{0, sizeof(triangle_vertices)};
//         gal::mapbuffer(context, upload_buf, &range);
//         std::memcpy(upload_buf->cpu_mapped_address, triangle_vertices.data(), sizeof(triangle_vertices));
//         gal::unmapbuffer(context, upload_buf);

//         // vertex buffer
//         gal::gal_buffer_desc gpu_buf_desc{};
//         gpu_buf_desc.descriptor_types = gal::gal_descriptor_type::RW_BUFFER;
//         gpu_buf_desc.memory_flags = gal::gal_memory_flag::GPU_DEDICATED;
//         gpu_buf_desc.initial_state = gal::gal_resource_state::COPY_DST;
//         gpu_buf_desc.flags = gal::gal_buffer_flag::OWN_MEMORY;
//         gpu_buf_desc.size = sizeof(triangle_vertices);
//         gal::create_buffer(context, &gpu_buf_desc, &vertex_buffer);

//     }
//     ~HelloTriangle() {

//         gal::destroy_pipeline(context, pipeline);
//         gal::destroy_root_signature(context, root_signature);
//         gal::destroy_shader_program(context, shader_program);

//         CHECK_ERR_CODE(ante::gal::remove_queue(context, queue));
//         CHECK_ERR_CODE(ante::gal::destroy_swap_chain(context, swap_chain));

//         CHECK_ERR_CODE(ante::gal::destroy_memory_allocator(&context));
//         CHECK_ERR_CODE(ante::gal::destroy_device(&context));
//         CHECK_ERR_CODE(ante::gal::destroy_instance(&context));

//         CHECK_ERR_CODE(ante::gal::destroy_gal(context));

//         destroy_window(window.get());
//     }

//     void Run() {


//         while (window_pool_events(window.get())) {
//             gal::acquire_next_image(swap_chain, );
//             gal::free_command_list(context, command_list);

//             gal::cmd_begin(command_list);
//             if (b_first_frame) {
//                 gal::cmd_copy_buffer(command_list, upload_buf, vertex_buffer, 0, 0, sizeof(triangle_vertices));
//                 // barrier
//                 gal::gal_buffer_barrier bb{};
//                 bb.buffer = vertex_buffer;
//                 bb.src_state = gal::gal_resource_state::COPY_DST;
//                 bb.dst_state = gal::gal_resource_state::VERTEX_BUFFER;
//                 gal::cmd_resource_barrier(command_list, 1, &bb, 0, nullptr);
//                 b_first_frame = false;
//             }

//             u32 stride = sizeof(vertex);
//             u64 offset = 0;
//             gal::gal_renderpass_begin_desc renderpass_begin_desc{};

//             gal::cmd_begin_renderpass(command_list, &renderpass_begin_desc);
//             gal::cmd_bind_pipeline(command_list, pipeline);
//             gal::cmd_set_scissor();
//             gal::cmd_set_viewport()
//             gal::cmd_bind_vertex_buffer(command_list, 1, &vertex_buffer, &stride, &offset);


//             gal::cmd_draw_instanced(command_list, 3, 0, 1, 0);

//             gal::cmd_end_renderpass(command_list);
//             gal::cmd_end(command_list);

//             gal::gal_queue_submit_desc submit_desc{};
//             submit_desc.cmds = &command_list;
//             submit_desc.pSignalFence = fence;
//             gal::queue_submit(queue, &submit_desc);

//             gal::gal_queue_present_desc present_desc{};
//             present_desc.pSwapChain = swap_chain;
//             gal::queue_present(queue, &present_desc);
//             gal::wait_fences(context, &fence, 1);
//         }
//     }

//   private:
//     bool b_first_frame = true;
//     static constexpr u32 width = 1280;
//     static constexpr u32 height = 720;
//     ante::memory::unique_ptr<ante_window> window;
//     gal::gal_context context;
//     gal::gal_queue queue;
//     gal::gal_swap_chain swap_chain;
//     gal::gal_command_pool command_pool;
//     gal::gal_command_list command_list;
//     gal::gal_buffer upload_buf;
//     gal::gal_buffer vertex_buffer;
//     gal::gal_shader_program shader_program;
//     gal::gal_root_signature root_signature;
//     gal::gal_pipeline pipeline;
//     gal::gal_fence fence;

//     struct vertex {
//         ante::math::vec3 position;
//         ante::math::vec3 color;
//     };

//     ante::fixed_array<vertex, 3> triangle_vertices = {vertex{math::vec3{0.5, 0.5, 0}, math::vec3{1.0, 0.0, 0.0}},
//                                              vertex{math::vec3{-.5, 0.5, 0}, math::vec3{0.0, 1.0, 0.0}},
//                                              vertex{math::vec3{0, 0.5, 0}, math::vec3{0.0, 0.0, 1.0}}};
// };

int main() {
    //auto app = ante::memory::make_unique<HelloTriangle>(nullptr);
    // app->InitResources();
    // app->Run();
    return 0;
}
