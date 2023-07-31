#include <cassert>
#include <framework/01core/input/input.h>
#include <framework/01core/math/math.h>
#include <framework/01core/memory/smart_ptr.h>
#include <framework/02gal/gal.h>
#include <framework/02gal/shader/shader_compiler.h>
using namespace ante;

constexpr void CHECK_ERR_CODE(gal::gal_error_code err) {
    if (err != gal::gal_error_code::SUC) {
        std::terminate();
    }
}

class HelloTriangle {
  public:
    HelloTriangle() {
        window = ante::memory::make_unique<ante_window>(nullptr);
        window_desc desc{};
        desc.width = width;
        desc.height = height;
        desc.name = "02hellotriangle";
        desc.b_vsync = false;
        create_window(&desc, window.get());

        CHECK_ERR_CODE(gal::init_gal(gal::gal_api::VULKAN, &context));

        gal::gal_desc gal_desc{};
        gal_desc.b_debug_layer = true;
        gal_desc.b_swap_chain = true;
        gal_desc.b_surface = true;
        gal_desc.b_async_compute = false;
        gal_desc.b_async_transfer = false;
        // gal_desc.b_ray_tracing = true;
        //gal_desc.b_mesh_shader = true;
        //gal_desc.b_variable_rate_shading = true;
        CHECK_ERR_CODE(ante::gal::create_instance(&gal_desc, &context));
        CHECK_ERR_CODE(ante::gal::create_device(&gal_desc, &context));
        CHECK_ERR_CODE(ante::gal::create_memory_allocator(&context));

        gal::gal_queue_desc queue_desc{};
        queue_desc.type = gal::gal_queue_type::graphcis;
        CHECK_ERR_CODE(gal::add_queue(context, &queue_desc, &queue));

        gal::gal_swap_chain_desc swap_chain_desc{};
        swap_chain_desc.window = window.get();
        swap_chain_desc.b_present = true;
        swap_chain_desc.b_vsync = true;
        swap_chain_desc.width = width;
        swap_chain_desc.height = height;
        swap_chain_desc.format = gal::gal_texture_format::R8G8B8A8_UNORM;
        swap_chain_desc.image_count = 3;
        swap_chain_desc.clear_value.value = gal::gal_clear_value::rgb{0.0f, 0.0f, 0.0f, 1.0f};
        CHECK_ERR_CODE(gal::create_swap_chain(context, &swap_chain_desc, &swap_chain));
    }
    ~HelloTriangle() {
        CHECK_ERR_CODE(ante::gal::remove_queue(context, queue));
        CHECK_ERR_CODE(ante::gal::destroy_swap_chain(context, swap_chain));

        CHECK_ERR_CODE(ante::gal::destroy_memory_allocator(&context));
        CHECK_ERR_CODE(ante::gal::destroy_device(&context));
        CHECK_ERR_CODE(ante::gal::destroy_instance(&context));

        CHECK_ERR_CODE(ante::gal::destroy_gal(context));

        destroy_window(window.get());
    }
    void UpdateResources() {

    }
    void InitResources() {


        
        gal::gal_command_pool_desc command_pool_desc{};
        command_pool_desc.b_transient = false;
        command_pool_desc.queue_type = gal::gal_queue_type::graphcis;
        gal::create_command_pool(context, &command_pool_desc, &command_pool);
        gal::gal_command_list_desc command_list_desc{};
        command_list_desc.command_pool = command_pool;
        command_list_desc.queue_type = gal::gal_queue_type::graphcis;
        gal::get_command_list(context, &command_list_desc, &command_list);

        using namespace ante::gal;
        // compile shader from source
        shader_compiler sc;
        shader_source_blob source;
        source.set(test_cs.data(), test_cs.size());

        shader_compile_desc vs_desc;
        vs_desc.entry = "VS_MAIN";
        vs_desc.optimization_level = shader_optimization_level::NONE;
        vs_desc.target_api = shader_blob_type::SPIRV;
        vs_desc.target_profile = shader_target_profile::VS_6_0;

        shader_compile_desc ps_desc;
        ps_desc.entry = "PS_MAIN";
        ps_desc.optimization_level = shader_optimization_level::NONE;
        ps_desc.target_api = shader_blob_type::SPIRV;
        ps_desc.target_profile = shader_target_profile::PS_6_0;

        gal::gal_shader_program sp{};
        gal::compiled_shader_group sg{};
        gal::shader_gourp_source_desc sg_desc{};

        sg_desc.desc_vert = &vs_desc;
        sg_desc.desc_frag = &ps_desc;
        sg.set_from_source(&source, &sg_desc);

        // root signature
        gal::gal_root_signature rs{};
        gal::gal_root_signature_desc rs_desc{};
        rs_desc.shader = &sg;
        gal::create_root_signature(context, &rs_desc, &rs);

        // create shader program
        gal::gal_shader_program_desc sp_desc{};
        sp_desc.shader_group = &sg;

        gal::create_shader_program(context, &sp_desc, &sp);

        // create pso
        gal_compute_pipeline_desc comp_pipe_desc{};
        comp_pipe_desc.root_signature = rs;
        comp_pipe_desc.shader = sp;

        gal_pipeline_desc pipe_desc{};
        pipe_desc.desc = comp_pipe_desc;

        gal::gal_pipeline comp_pipe{};
        gal::create_compute_pipeline(context, &pipe_desc, &comp_pipe);

        gal::destroy_pipeline(context, comp_pipe);
        gal::destroy_root_signature(context, rs);
        gal::destroy_shader_program(context, sp);
        sg.release();
    }
    void Run() {
        while (window_pool_events(window.get())) {
            gal::free_command_list(context, command_list);
            gal::cmd_begin(command_list);
            u32 stride = sizeof(vertex);
            u64 offset = 0;
            gal::cmd_bind_vertex_buffer(command_list, 1, &vertex_buffer, &stride, &offset);
            gal::cmd_draw_instanced(command_list, 3, 0, 1, 0);
            gal::cmd_end(command_list);

            gal::gal_queue_submit_desc submit_desc{};
            submit_desc.cmds = &command_list;
            gal::queue_submit(queue, &submit_desc);
        }
    }

  private:
    gal::gal_error_code error_code = gal::gal_error_code::SUC;
    static constexpr u32 width = 1280;
    static constexpr u32 height = 720;
    ante::memory::unique_ptr<ante_window> window;
    gal::gal_context context;
    gal::gal_queue queue;
    gal::gal_swap_chain swap_chain;
    gal::gal_command_pool command_pool;
    gal::gal_command_list command_list;
    gal::gal_buffer vertex_buffer;
    gal::gal_shader_program shader_program;
    gal::gal_root_signature root_signature;
    gal::gal_pipeline pipeline;
    gal::gal_fence fence;

    struct vertex {
        ante::math::vec3 position;
        ante::math::vec3 color;
    };

    ante::fixed_array<vertex, 3> triangle = {vertex{math::vec3{0.5, 0.5, 0}, math::vec3{1.0, 0.0, 0.0}},
                                             vertex{math::vec3{-.5, 0.5, 0}, math::vec3{0.0, 1.0, 0.0}},
                                             vertex{math::vec3{0, 0.5, 0}, math::vec3{0.0, 0.0, 1.0}}};
};

int main() {
    auto app = ante::memory::make_unique<HelloTriangle>(nullptr);
    app->InitResources();
    app->Run();
    return 0;
}
