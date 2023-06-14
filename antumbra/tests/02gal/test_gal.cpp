
#include "test_gal.h"
using namespace ant;
using namespace ant::gal;
TEST_CASE("test context initialization") {
    auto context_initialization = [](gal::gal_api api) {
        gal::gal_context context = initialize(api);
        destroy(context);
    };

    context_initialization(gal::gal_api::VULKAN);
    //context_initialization(gal::RenderApi::d3d12);
}

TEST_CASE("test gal buffer") {
    auto buffer_creation = [](gal::gal_api api) {
        gal::gal_context context = initialize(api);

        gal::gal_buffer_desc desc{};
        desc.size = 256;
        desc.initial_state = gal::gal_resource_state::RW_BUFFER;
        desc.descriptor_types = gal::gal_descriptor_type::CONSTANT_BUFFER | gal::gal_descriptor_type::RW_BUFFER |
                                gal::gal_descriptor_type::INDIRECT_ARGUMENT;
        desc.memory_flags =
            gal_memory_flag::GPU_DEDICATED | gal_memory_flag::GPU_DOWNLOAD | gal_memory_flag::CPU_UPLOAD;

        desc.flags = gal::gal_buffer_flag::OWN_MEMORY;
        gal::gal_buffer buffer{};
        gal::gal_error_code result = gal::create_buffer(context, &desc, &buffer);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        result = gal::destroy_buffer(context, buffer);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        destroy(context);
    };
    buffer_creation(gal::gal_api::VULKAN);
    //buffer_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal texture") {
    auto texture_creation = [](gal::gal_api api) {
        gal::gal_context context = initialize(api);

        gal_texture_desc texture_desc{};
        texture_desc.width = 256;
        texture_desc.height = 256;
        texture_desc.depth = 1;
        texture_desc.mip_level = 1;
        texture_desc.array_size = 1;
        texture_desc.format = gal_texture_format::R8G8B8A8_UNORM;
        texture_desc.dimension = gal_texture_dimension::_2D;
        texture_desc.descriptor_types = gal_descriptor_type::TEXTURE | gal_descriptor_type::RW_TEXTURE;
        texture_desc.initial_state = gal_resource_state::RW_TEXTURE;
        texture_desc.memory_flags = gal_memory_flag::GPU_DEDICATED;
        texture_desc.sample_count = gal_texture_sample_count::SAMPLE_COUNT_1;
        gal::gal_texture texture{};
        gal::gal_error_code result = gal::create_texture(context, &texture_desc, &texture);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        result = gal::destroy_texture(context, texture);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        destroy(context);
    };
    texture_creation(gal::gal_api::VULKAN);
    //buffer_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal sampler") {
    auto sampler_creation = [](gal::gal_api api) {
        gal::gal_context context = initialize(api);

        gal_sampler_desc sampler_desc{};
        sampler_desc.min_filter = gal_sampler_filter_mode::LINEAR;
        sampler_desc.mag_filter = gal_sampler_filter_mode::LINEAR;
        sampler_desc.mip_mode = gal_sampler_mip_mode::LINEAR;
        sampler_desc.address_mode_u = gal_sampler_address_mode::CLAMP;
        sampler_desc.address_mode_v = gal_sampler_address_mode::CLAMP;
        sampler_desc.address_mode_w = gal_sampler_address_mode::CLAMP;
        sampler_desc.min_lod = 0.0f;
        sampler_desc.max_lod = 1.0f;
        sampler_desc.max_anisotropy = 1.0f;
        sampler_desc.compare_mode = gal_compare_mode::NEVER;
        sampler_desc.mip_lod_bias = 0.0f;
        gal::gal_sampler sampler{};
        gal::gal_error_code result = gal::create_sampler(context, &sampler_desc, &sampler);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        result = gal::destroy_sampler(context, sampler);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        destroy(context);
    };
    sampler_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal render target") {
    auto render_target_creation = [](gal::gal_api api) {
        gal::gal_context context = initialize(api);

        gal_render_target_desc desc{};
        desc.width = 256;
        desc.height = 256;
        desc.depth = 1;
        desc.mip_level = 1;
        desc.format = gal_texture_format::R8G8B8A8_UNORM;
        desc.dimension = gal_texture_dimension::_2D;
        desc.descriptor_types = gal_descriptor_type::COLOR_RT;
        desc.initial_state = gal_resource_state::RENDER_TARGET;
        desc.sample_count = gal_texture_sample_count::SAMPLE_COUNT_1;
        desc.clear_value.value = gal_clear_value::rgb{0.0f, 0.0f, 0.0f, 1.0f};
        gal::gal_render_target rt;
        gal::gal_error_code result = gal::create_render_target(context, &desc, &rt);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        result = gal::destroy_render_target(context, rt);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        destroy(context);
    };
    render_target_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal swapchain") {
    auto swap_chain_creation = [](gal::gal_api api) {
        ant::ant_window window;
        ant::window_desc window_desc{};
        window_desc.b_vsync = true;
        window_desc.name = "test swapchain";
        window_desc.height = 256;
        window_desc.width = 256;
        ant::create_window(&window_desc, &window);
        gal::gal_context context = initialize(api);
        gal_swap_chain sc;
        gal_swap_chain_desc desc{};
        desc.b_present = true;
        desc.b_vsync = true;
        desc.width = 256;
        desc.height = 256;
        desc.format = gal_texture_format::R8G8B8A8_UNORM;
        desc.image_count = 3;
        desc.clear_value.value = gal_clear_value::rgb{0.0f, 0.0f, 0.0f, 1.0f};
        desc.hwnd_window = ant::get_hwnd_window(window);
        gal::gal_error_code result = gal::create_swap_chain(context, &desc, &sc);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        result = gal::destroy_swap_chain(context, sc);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        destroy(context);

        ant::destroy_window(window);
    };
    swap_chain_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal command pool") {
    auto command_test = [](gal::gal_api api) {
        ant::gal::gal_command_pool cmd_pool{};
        gal::gal_context context = initialize(api);
        gal::gal_command_pool_desc desc{};
        desc.b_transient = false;
        desc.queue_type = gal_queue_type::graphcis;
        gal_error_code result = gal::create_command_pool(context, &desc, &cmd_pool);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);

        gal_command_list cmd{};
        gal_command_list_desc cmd_desc{};
        cmd_desc.command_pool = cmd_pool;
        cmd_desc.b_secondary = false;
        result = gal::allocate_command_list(context, &cmd_desc, &cmd);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        gal::cmd_begin(cmd);
        gal::cmd_end(cmd);
        result = gal::reset_command_pool(context, cmd_pool);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        result = gal::destroy_command_pool(context, cmd_pool);
        REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
        destroy(context);
    };
    command_test(gal_api::VULKAN);
}

TEST_CASE("test compute pipeline creation vk") {
    ant::str test_cs = "\
#define UPDATE_FREQ_NONE space0\n\
#define UPDATE_FREQ_PER_FRAME space1\n\
#define UPDATE_FREQ_PER_BATCH space2\n\
#define UPDATE_FREQ_PER_DRAW space3\n\
#define UPDATE_FREQ_BINDLESS space4\n\
// #define CBUFFER(NAME, FREQ, REG, BINDING) [[vk::binding(BINDING, FREQ)]] cbuffer NAME : register(REG, FREQ)\n\
// #define RES(TYPE, NAME, FREQ, REG, BINDING) [[vk::binding(BINDING, FREQ)]] TYPE NAME : register(REG, FREQ)\n\
#define CBUFFER(NAME, FREQ) cbuffer NAME : register(FREQ)\n\
#define RES(TYPE, NAME, FREQ) TYPE NAME : register(FREQ)\n\
CBUFFER(cbuf1, UPDATE_FREQ_NONE)\n\
{\n\
    float4 a;\n\
    float b;\n\
};\n\
RES(StructuredBuffer<uint>, sbuffer, UPDATE_FREQ_NONE);\n\
RES(StructuredBuffer<uint3>, sbuffer_arr[4], UPDATE_FREQ_NONE);\n\
RES(Texture2D<float>, tex1, UPDATE_FREQ_PER_FRAME);\n\
RES(SamplerState, spl, UPDATE_FREQ_PER_FRAME);\n\
RES(RWTexture2D<float2>, tex2, UPDATE_FREQ_PER_FRAME);\n\
RES(RWTexture2D<float2>, tex_arr[4], UPDATE_FREQ_PER_FRAME);\n\
[numthreads(8, 8, 1)]\n\
void CS_MAIN(uint3 thread_id: SV_DispatchThreadID) \n\
{\n\
    float color = tex1.SampleLevel(spl, float2(thread_id.xy), 0);\n\
    tex2[thread_id.xy] = color;\n\
}";

    using namespace ant::gal;
    // compile shader from source
    gal::gal_context context = initialize(gal_api::VULKAN);
    shader_compiler sc;
    shader_source_blob source;
    source.set(test_cs.data(), test_cs.size());

    shader_compile_desc desc;
    desc.entry = "CS_MAIN";
    desc.optimization_level = shader_optimization_level::NONE;
    desc.target_api = shader_blob_type::SPIRV;
    desc.target_profile = shader_target_profile::CS_6_0;

    gal::gal_shader_program sp{};
    gal::compiled_shader_group sg{};
    gal::shader_gourp_source_desc sg_desc{};

    sg_desc.desc_comp = &desc;
    sg.set_from_source(&source, &sg_desc);

    // root signature
    gal::gal_rootsignature rs{};
    gal::gal_rootsignature_desc rs_desc{};
    rs_desc.shader = &sg;
    gal::create_rootsignature(context, &rs_desc, &rs);


    // create shader program
    gal::gal_shader_program_desc sp_desc{};
    sp_desc.shader_group = &sg;

    gal_error_code result = gal::create_shader_program(context, &sp_desc, &sp);
    REQUIRE(result == gal_error_code::GAL_ERRORCODE_SUCCESS);


    // create pso
    gal_compute_pipeline_desc comp_pipe_desc{};
    comp_pipe_desc.root_signature;
    comp_pipe_desc.shader = &sp;

    gal_pipeline_desc pipe_desc{};
    pipe_desc.desc = comp_pipe_desc;

    gal::gal_pipeline comp_pipe{};
    result = gal::create_compute_pipeline(context, &pipe_desc, &comp_pipe);
    REQUIRE(result == gal_error_code::GAL_ERRORCODE_SUCCESS);
    sg.release();
}

TEST_CASE("test graphics pipeline creation") {}
