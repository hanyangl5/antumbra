
#include "test_gal.h"

using namespace ante;
using namespace ante::gal;
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
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_buffer(context, buffer);
        REQUIRE(result == gal::gal_error_code::SUC);
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
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_texture(context, texture);
        REQUIRE(result == gal::gal_error_code::SUC);
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
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_sampler(context, sampler);
        REQUIRE(result == gal::gal_error_code::SUC);
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
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_render_target(context, rt);
        REQUIRE(result == gal::gal_error_code::SUC);
        destroy(context);
    };
    render_target_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal swapchain") {
    auto swap_chain_creation = [](gal::gal_api api) {
        ante::ant_window window;
        ante::window_desc window_desc{};
        window_desc.b_vsync = true;
        window_desc.name = "test swapchain";
        window_desc.height = 256;
        window_desc.width = 256;
        ante::create_window(&window_desc, &window);
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
#ifdef _WIN32
        desc.hwnd_window = ante::get_hwnd_window(window);
#endif
        gal::gal_error_code result = gal::create_swap_chain(context, &desc, &sc);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_swap_chain(context, sc);
        REQUIRE(result == gal::gal_error_code::SUC);
        destroy(context);

        ante::destroy_window(window);
    };
    swap_chain_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal command pool") {
    auto command_test = [](gal::gal_api api) {
        ante::gal::gal_command_pool cmd_pool{};
        gal::gal_context context = initialize(api);

        gal_queue gfx_queue{};
        gal_queue_desc q_desc{};
        q_desc.type = gal_queue_type::graphcis;
        gal::add_queue(context, &q_desc, &gfx_queue);

        gal::gal_command_pool_desc desc{};
        desc.b_transient = false;
        desc.queue_type = gal_queue_type::graphcis;
        gal_error_code result = gal::create_command_pool(context, &desc, &cmd_pool);
        REQUIRE(result == gal::gal_error_code::SUC);

        gal_command_list cmd{};
        gal_command_list_desc cmd_desc{};
        cmd_desc.queue_type = gal_queue_type::graphcis;
        cmd_desc.command_pool = cmd_pool;
        cmd_desc.b_secondary = false;
        result = gal::get_command_list(context, &cmd_desc, &cmd);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::reset_command_pool(context, cmd_pool);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_command_pool(context, cmd_pool);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::remove_queue(context, gfx_queue);
        REQUIRE(result == gal::gal_error_code::SUC);
        destroy(context);
    };
    command_test(gal_api::VULKAN);
}

TEST_CASE("test compute pipeline creation vk") {
    ante::str test_cs = "\
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
RES(RWTexture2D<float2>, tex_bindless1, UPDATE_FREQ_BINDLESS);\n\
RES(RWTexture2D<float2>, tex_bindless2[4], UPDATE_FREQ_BINDLESS);\n\
[numthreads(8, 8, 1)]\n\
void CS_MAIN(uint3 thread_id: SV_DispatchThreadID) \n\
{\n\
    float color = tex1.SampleLevel(spl, float2(thread_id.xy), 0);\n\
    tex2[thread_id.xy] = color;\n\
}";
    gal_error_code result;
    using namespace ante::gal;
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
    gal::gal_root_signature rs{};
    gal::gal_root_signature_desc rs_desc{};
    rs_desc.shader = &sg;
    result = gal::create_root_signature(context, &rs_desc, &rs);
    REQUIRE(result == gal_error_code::SUC);

    // create shader program
    gal::gal_shader_program_desc sp_desc{};
    sp_desc.shader_group = &sg;

    result = gal::create_shader_program(context, &sp_desc, &sp);
    REQUIRE(result == gal_error_code::SUC);

    // create pso
    gal_compute_pipeline_desc comp_pipe_desc{};
    comp_pipe_desc.root_signature = rs;
    comp_pipe_desc.shader = sp;

    gal_pipeline_desc pipe_desc{};
    pipe_desc.desc = comp_pipe_desc;

    gal::gal_pipeline comp_pipe{};
    result = gal::create_compute_pipeline(context, &pipe_desc, &comp_pipe);
    REQUIRE(result == gal_error_code::SUC);

    result = gal::destroy_pipeline(context, comp_pipe);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_root_signature(context, rs);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_shader_program(context, sp);
    REQUIRE(result == gal_error_code::SUC);
    sg.release();
    destroy(context);
}

TEST_CASE("test pso cache vk") {
    ante::str teststr = "\n\
        // shader from GPUOpen/Cauldron src/DX12/shaders/TAASharpenerCS.hlsl\n\
        Texture2D<float4> TAABuffer;\n\
RWTexture2D<float4> HDR;\n\
RWTexture2D<float4> History;\n\
float3 RGBToYCoCg(in float3 rgb)\n\
{\n\
    return float3(\n\
        0.25f * rgb.r + 0.5f * rgb.g + 0.25f * rgb.b,\n\
        0.5f * rgb.r - 0.5f * rgb.b,\n\
        -0.25f * rgb.r + 0.5f * rgb.g - 0.25f * rgb.b);\n\
}\n\
float3 YCoCgToRGB(in float3 yCoCg)\n\
{\n\
    return float3(\n\
        yCoCg.x + yCoCg.y - yCoCg.z,\n\
        yCoCg.x + yCoCg.z,\n\
        yCoCg.x - yCoCg.y - yCoCg.z);\n\
}\n\
float3 ApplySharpening(in float3 center, in float3 top, in float3 left, in float3 right, in float3 bottom)\n\
{\n\
    float3 result = RGBToYCoCg(center);\n\
    float unsharpenMask = 4.0f * result.x;\n\
    unsharpenMask -= RGBToYCoCg(top).x;\n\
    unsharpenMask -= RGBToYCoCg(bottom).x;\n\
    unsharpenMask -= RGBToYCoCg(left).x;\n\
    unsharpenMask -= RGBToYCoCg(right).x;\n\
    result.x = min(result.x + 0.25f * unsharpenMask, 1.1f * result.x);\n\
    return YCoCgToRGB(result);\n\
}\n\
float3 ReinhardInverse(in float3 sdr)\n\
{\n\
    return sdr / max(1.0f - sdr, 1e-2f);\n\
}\n\
[numthreads(8, 8, 1)]\n\
void CS_MAIN(uint3 globalID : SV_DispatchThreadID, uint3 localID : SV_GroupThreadID, uint localIndex : SV_GroupIndex, uint3 groupID : SV_GroupID)\n\
{\n\
    const float3 center = TAABuffer[globalID.xy].xyz;\n\
    const float3 top    = TAABuffer[globalID.xy + uint2( 0,  1)].xyz;\n\
    const float3 left   = TAABuffer[globalID.xy + uint2( 1,  0)].xyz;\n\
    const float3 right  = TAABuffer[globalID.xy + uint2(-1,  0)].xyz;\n\
    const float3 bottom = TAABuffer[globalID.xy + uint2( 0, -1)].xyz;\n\
    const float3 color = ApplySharpening(center, top, left, right, bottom);\n\
    HDR[globalID.xy] = float4(ReinhardInverse(color), 1.0f);\n\
    History[globalID.xy] = float4(center, 1.0f);\n\
}";

    using namespace ante::gal;
    // compile shader from source
    gal::gal_context context = initialize(gal_api::VULKAN);
    shader_compiler sc;
    shader_source_blob source;
    source.set(teststr.data(), teststr.size());

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
    gal::gal_root_signature rs{};
    gal::gal_root_signature_desc rs_desc{};
    rs_desc.shader = &sg;
    gal::create_root_signature(context, &rs_desc, &rs);

    // create shader program
    gal::gal_shader_program_desc sp_desc{};
    sp_desc.shader_group = &sg;

    gal_error_code result = gal::create_shader_program(context, &sp_desc, &sp);
    REQUIRE(result == gal_error_code::SUC);

    // create pso
    gal_compute_pipeline_desc comp_pipe_desc{};
    comp_pipe_desc.root_signature = rs;
    comp_pipe_desc.shader = sp;

    gal::gal_pipeline_cache pso_cache{};

    gal_pipeline_cache_desc pso_cache_desc{};
    gal::create_pipeline_cache(context, &pso_cache_desc, &pso_cache);

    gal_pipeline_desc pipe_desc{};
    pipe_desc.desc = comp_pipe_desc;
    pipe_desc.pipeline_cache = pso_cache;
    gal::gal_pipeline comp_pipe{};
    //BENCHMARK("create pipeline without cache") {
    result = gal::create_compute_pipeline(context, &pipe_desc, &comp_pipe);
    //};

    REQUIRE(result == gal_error_code::SUC);
    u64 size;
    result = gal::get_pipeline_cache_data(context, pso_cache, &size, nullptr);
    REQUIRE(result == gal_error_code::SUC);
    ante::vector<char> cache_data(size);
    result = gal::get_pipeline_cache_data(context, pso_cache, &size, cache_data.data());
    REQUIRE(result == gal_error_code::SUC);
    gal::gal_pipeline_cache pso_cache1{};
    gal_pipeline_cache_desc pso_cache_desc1{};
    pso_cache_desc1.data.set(cache_data.data(), cache_data.size());
    result = gal::create_pipeline_cache(context, &pso_cache_desc, &pso_cache1);
    REQUIRE(result == gal_error_code::SUC);

    // create pso
    gal_compute_pipeline_desc comp_pipe_desc1{};
    comp_pipe_desc1.root_signature = rs;
    comp_pipe_desc1.shader = sp;
    gal_pipeline_desc pipe_desc1{};
    pipe_desc1.desc = comp_pipe_desc1;
    pipe_desc1.pipeline_cache = pso_cache1;
    gal::gal_pipeline comp_pipe1{};
    //BENCHMARK("create pipeline with cache") {
    result = gal::create_compute_pipeline(context, &pipe_desc1, &comp_pipe1);
    //};
    REQUIRE(result == gal_error_code::SUC);

    result = gal::destroy_pipeline_cache(context, pso_cache);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_pipeline_cache(context, pso_cache1);

    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_pipeline(context, comp_pipe1);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_pipeline(context, comp_pipe);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_root_signature(context, rs);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_shader_program(context, sp);
    REQUIRE(result == gal_error_code::SUC);
    sg.release();
    destroy(context);
}

TEST_CASE("test dispatch") {}

TEST_CASE("test compute vk") {
    ante::str test_cs = " [numthreads(8, 8, 1)] void CS_MAIN(uint3 thread_id: SV_DispatchThreadID) {}";

    using namespace ante::gal;
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
    gal::gal_root_signature rs{};
    gal::gal_root_signature_desc rs_desc{};
    rs_desc.shader = &sg;
    gal::create_root_signature(context, &rs_desc, &rs);

    // create shader program
    gal::gal_shader_program_desc sp_desc{};
    sp_desc.shader_group = &sg;

    gal_error_code result = gal::create_shader_program(context, &sp_desc, &sp);
    REQUIRE(result == gal_error_code::SUC);

    // create pso
    gal_compute_pipeline_desc comp_pipe_desc{};
    comp_pipe_desc.root_signature = rs;
    comp_pipe_desc.shader = sp;

    gal_pipeline_desc pipe_desc{};
    pipe_desc.desc = comp_pipe_desc;

    gal::gal_pipeline comp_pipe{};
    result = gal::create_compute_pipeline(context, &pipe_desc, &comp_pipe);
    REQUIRE(result == gal_error_code::SUC);

    result = gal::destroy_pipeline(context, comp_pipe);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_root_signature(context, rs);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_shader_program(context, sp);
    REQUIRE(result == gal_error_code::SUC);
    sg.release();
    destroy(context);
}

TEST_CASE("buffer test") {
    auto command_test = [](gal::gal_api api) {
        ante::gal::gal_command_pool cmd_pool{};
        gal::gal_context context = initialize(api);

        gal_queue gfx_queue{};
        gal_queue_desc queue_desc{};
        queue_desc.type = gal_queue_type::graphcis;
        gal::add_queue(context, &queue_desc, &gfx_queue);

        gal::gal_command_pool_desc desc{};
        desc.b_transient = false;
        desc.queue_type = gal_queue_type::graphcis;
        gal_error_code result = gal::create_command_pool(context, &desc, &cmd_pool);
        REQUIRE(result == gal::gal_error_code::SUC);

        ante::fixed_array<u8, 64> arr;
        for (u8 i = 0; i < 64; i++) {
            arr[i] = i;
        }

        gal_buffer upload_buf{};
        gal_buffer_desc upload_buf_desc{};
        upload_buf_desc.descriptor_types = gal_descriptor_type::UNDEFINED;
        upload_buf_desc.memory_flags = gal_memory_flag::CPU_UPLOAD;
        upload_buf_desc.initial_state = gal_resource_state::COPY_SRC;
        //upload_buf_desc.flags = gal_buffer_flag::PERSISTENT_MAP;
        upload_buf_desc.size = arr.size();
        result = gal::create_buffer(context, &upload_buf_desc, &upload_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        read_range range{0, arr.size()};
        result = gal::mapbuffer(context, upload_buf, &range);
        REQUIRE(result == gal::gal_error_code::SUC);
        //
        std::memcpy(upload_buf->cpu_mapped_address, arr.data(), arr.size());
        result = gal::unmapbuffer(context, upload_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        gal_buffer gpu_buf{};
        gal_buffer_desc gpu_buf_desc{};
        gpu_buf_desc.descriptor_types = gal_descriptor_type::RW_BUFFER;
        gpu_buf_desc.memory_flags = gal_memory_flag::GPU_DEDICATED;
        gpu_buf_desc.initial_state = gal_resource_state::COPY_DST;
        gpu_buf_desc.flags = gal_buffer_flag::OWN_MEMORY;
        gpu_buf_desc.size = arr.size();
        result = gal::create_buffer(context, &gpu_buf_desc, &gpu_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        // download buf
        gal_buffer download_buf{};
        gal_buffer_desc download_buf_desc{};
        download_buf_desc.descriptor_types = gal_descriptor_type::UNDEFINED;
        download_buf_desc.memory_flags = gal_memory_flag::GPU_DOWNLOAD;
        download_buf_desc.initial_state = gal_resource_state::COPY_DST;
        //download_buf_desc.flags = gal_buffer_flag::PERSISTENT_MAP;
        download_buf_desc.size = arr.size();
        result = gal::create_buffer(context, &download_buf_desc, &download_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        ante::fixed_array<u8, 64> dumped_arr;

        gal_command_list cmd{};
        gal_command_list_desc cmd_desc{};
        cmd_desc.command_pool = cmd_pool;
        cmd_desc.b_secondary = false;
        cmd_desc.queue_type = gal_queue_type::graphcis;
        result = gal::get_command_list(context, &cmd_desc, &cmd);
        REQUIRE(result == gal::gal_error_code::SUC);

        REQUIRE(gal_error_code::SUC == gal::cmd_begin(cmd));
        REQUIRE(gal_error_code::SUC == gal::cmd_copy_buffer(cmd, upload_buf, gpu_buf, 0, 0, arr.size()));
        // barrier
        gal::gal_buffer_barrier bb{};
        bb.buffer = gpu_buf;
        bb.src_state = gal::gal_resource_state::COPY_DST;
        bb.dst_state = gal::gal_resource_state::COPY_SRC;
        //bb.mQueueType = gal_queue_type::graphcis;
        result = gal::cmd_resource_barrier(cmd, 1, &bb, 0, nullptr);
        REQUIRE(result == gal::gal_error_code::SUC);
        REQUIRE(gal_error_code::SUC == gal::cmd_copy_buffer(cmd, gpu_buf, download_buf, 0, 0, arr.size()));
        REQUIRE(gal_error_code::SUC == gal::cmd_end(cmd));

        gal_fence fence{};
        result = gal::create_fence(context, &fence);
        REQUIRE(result == gal::gal_error_code::SUC);
        // submmit
        gal_queue_submit_desc submit_desc{};
        submit_desc.cmds = &cmd;
        submit_desc.cmd_count = 1;
        submit_desc.mSignalSemaphoreCount = 0;
        submit_desc.mWaitSemaphoreCount = 0;
        submit_desc.pSignalFence = fence;
        result = gal::queue_submit(gfx_queue, &submit_desc);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::wait_fences(context, &fence, 1);
        REQUIRE(result == gal::gal_error_code::SUC);

        // download
        result = gal::mapbuffer(context, download_buf, &range);
        REQUIRE(result == gal::gal_error_code::SUC);
        std::memcpy(dumped_arr.data(), download_buf->cpu_mapped_address, arr.size());
        result = gal::unmapbuffer(context, download_buf);
        REQUIRE(result == gal::gal_error_code::SUC);

        for (u32 i = 0; i < arr.size(); i++) {
            REQUIRE(dumped_arr[i] == arr[i]);
        }

        result = gal::reset_command_pool(context, cmd_pool);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_command_pool(context, cmd_pool);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_buffer(context, upload_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_buffer(context, gpu_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_buffer(context, download_buf);
        REQUIRE(result == gal::gal_error_code::SUC);
        result = gal::destroy_fence(context, fence);
        REQUIRE(result == gal::gal_error_code::SUC);
        destroy(context);
    };
    command_test(gal_api::VULKAN);
}

TEST_CASE("descriptor set") {
    //    ante::str test_cs = "\
//#define UPDATE_FREQ_NONE space0\n\
//#define UPDATE_FREQ_PER_FRAME space1\n\
//#define UPDATE_FREQ_PER_BATCH space2\n\
//#define UPDATE_FREQ_PER_DRAW space3\n\
//#define UPDATE_FREQ_BINDLESS space4\n\
//#define CBUFFER(NAME, FREQ) cbuffer NAME : register(FREQ)\n\
//#define RES(TYPE, NAME, FREQ) TYPE NAME : register(FREQ)\n\
//CBUFFER(cbuf1, UPDATE_FREQ_NONE)\n\
//{\n\
//    float4 a;\n\
//    float b;\n\
//};\n\
//RES(StructuredBuffer<uint>, sbuffer, UPDATE_FREQ_NONE);\n\
//RES(SamplerState, spl, UPDATE_FREQ_PER_FRAME);\n\
//RES(Texture2D<float>, tex1, UPDATE_FREQ_PER_FRAME);\n\
//RES(RWTexture2D<float2>, tex2, UPDATE_FREQ_PER_FRAME);\n\
//[numthreads(8, 8, 1)]\n\
//void CS_MAIN(uint3 thread_id: SV_DispatchThreadID) \n\
//{\n\
//    float color = tex1.SampleLevel(spl, float2(thread_id.xy), 0);\n\
//    tex2[thread_id.xy] = color;\n\
//}";
    ante::str test_cs = "\
#define UPDATE_FREQ_NONE space0\n\
#define UPDATE_FREQ_PER_FRAME space1\n\
#define UPDATE_FREQ_PER_BATCH space2\n\
#define UPDATE_FREQ_PER_DRAW space3\n\
#define UPDATE_FREQ_BINDLESS space4\n\
#define CBUFFER(NAME, FREQ) cbuffer NAME : register(FREQ)\n\
#define RES(TYPE, NAME, FREQ) TYPE NAME : register(FREQ)\n\
RES(SamplerState, spl2, UPDATE_FREQ_BINDLESS);\n\
RES(SamplerState, spl3, UPDATE_FREQ_BINDLESS);\n\
RES(SamplerState, spl4, UPDATE_FREQ_BINDLESS);\n\
RES(SamplerState, spl, UPDATE_FREQ_PER_FRAME);\n\
RES(Texture2D<float>, tex1, UPDATE_FREQ_PER_FRAME);\n\
[numthreads(8, 8, 1)]\n\
void CS_MAIN(uint3 thread_id: SV_DispatchThreadID) \n\
{\n\
    float color = tex1.SampleLevel(spl, float2(thread_id.xy), 0);\n\
}";
    gal_error_code result;
    using namespace ante::gal;
    // compile shader from source
    gal::gal_context context = initialize(gal_api::VULKAN);
    shader_compiler sc;
    shader_source_blob source;
    source.set(test_cs.data(), test_cs.size());

    shader_compile_desc desc;
    desc.entry = "CS_MAIN";
    desc.optimization_level = shader_optimization_level::O0;
    desc.target_api = shader_blob_type::SPIRV;
    desc.target_profile = shader_target_profile::CS_6_0;

    gal::gal_shader_program sp{};
    gal::compiled_shader_group sg{};
    gal::shader_gourp_source_desc sg_desc{};

    sg_desc.desc_comp = &desc;
    sg.set_from_source(&source, &sg_desc);

    // root signature
    gal::gal_root_signature rs{};
    gal::gal_root_signature_desc rs_desc{};
    rs_desc.shader = &sg;
    rs_desc.type = gal_pipeline_type::COMPUTE;
    result = gal::create_root_signature(context, &rs_desc, &rs);
    REQUIRE(result == gal_error_code::SUC);

    // create shader program
    gal::gal_shader_program_desc sp_desc{};
    sp_desc.shader_group = &sg;

    result = gal::create_shader_program(context, &sp_desc, &sp);
    REQUIRE(result == gal_error_code::SUC);

    // create pso
    gal_compute_pipeline_desc comp_pipe_desc{};
    comp_pipe_desc.root_signature = rs;
    comp_pipe_desc.shader = sp;

    gal_pipeline_desc pipe_desc{};
    pipe_desc.desc = comp_pipe_desc;

    gal::gal_pipeline comp_pipe{};
    result = gal::create_compute_pipeline(context, &pipe_desc, &comp_pipe);
    REQUIRE(result == gal_error_code::SUC);

    gal::gal_sampler spl{};
    gal::gal_texture tex{};

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
    result = gal::create_texture(context, &texture_desc, &tex);

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
    result = gal::create_sampler(context, &sampler_desc, &spl);

    gal_descriptor_set ds{};
    gal_descriptor_set ds2{};
    gal_descriptor_set_desc ds_desc{};
    ds_desc.root_signature = rs;
    ds_desc.set.freq = gal_descriptor_set_update_freq::PER_FRAME;

    result = gal::get_descriptor_set(context, &ds_desc, 1, &ds);
    REQUIRE(result == gal_error_code::SUC);

    ds_desc.set.freq = gal_descriptor_set_update_freq::BINDLESS;
    result = gal::get_descriptor_set(context, &ds_desc, 1, &ds2);
    REQUIRE(result == gal_error_code::SUC);
    {
        gal_descriptor_upate_desc buf_descriptor_update_desc[3];
        buf_descriptor_update_desc[0].desc.s.sampler = spl;
        buf_descriptor_update_desc[0].type = gal_descriptor_type::SAMPLER;
        buf_descriptor_update_desc[0].name = "spl";
        buf_descriptor_update_desc[1].desc.t.texture = tex;
        buf_descriptor_update_desc[1].type = gal_descriptor_type::TEXTURE;
        buf_descriptor_update_desc[1].name = "tex1";

        gal_descriptor_set_update_desc ds_update_desc{};
        ds_update_desc.updates = &buf_descriptor_update_desc[0];
        ds_update_desc.count = 2;
        result = gal::update_descriptor_set(context, &ds_update_desc, ds);
        REQUIRE(result == gal_error_code::SUC);
    }
    {
        gal_descriptor_upate_desc buf_descriptor_update_desc[3];
        buf_descriptor_update_desc[0].desc.s.sampler = spl;
        buf_descriptor_update_desc[0].type = gal_descriptor_type::SAMPLER;
        buf_descriptor_update_desc[0].name = "spl2";
        buf_descriptor_update_desc[1].desc.s.sampler = spl;
        buf_descriptor_update_desc[1].type = gal_descriptor_type::SAMPLER;
        buf_descriptor_update_desc[1].name = "spl3";
        buf_descriptor_update_desc[2].desc.s.sampler = spl;
        buf_descriptor_update_desc[2].type = gal_descriptor_type::SAMPLER;
        buf_descriptor_update_desc[2].name = "spl4";

        gal_descriptor_set_update_desc ds_update_desc{};
        ds_update_desc.updates = &buf_descriptor_update_desc[0];
        ds_update_desc.count = 3;
        result = gal::update_descriptor_set(context, &ds_update_desc, ds2);
    }
    result = gal::destroy_texture(context, tex);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_sampler(context, spl);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::free_descriptor_set(context, ds, true);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::free_descriptor_set(context, ds2, true);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_pipeline(context, comp_pipe);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_root_signature(context, rs);
    REQUIRE(result == gal_error_code::SUC);
    result = gal::destroy_shader_program(context, sp);
    REQUIRE(result == gal_error_code::SUC);
    sg.release();
    destroy(context);
}