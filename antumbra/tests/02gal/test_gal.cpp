
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
        desc.resource_types = gal::gal_resource_type::CONSTANT_BUFFER | gal::gal_resource_type::RW_BUFFER |
                              gal::gal_resource_type::INDIRECT_ARGUMENT;
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
        texture_desc.dimension = gal_texture_dimension::td_2D;
        texture_desc.resource_types = gal_resource_type::TEXTURE | gal_resource_type::RW_TEXTURE;
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
    auto sampler_creation = [](gal::gal_api api) {
        gal::gal_context context = initialize(api);

        gal_render_target_desc desc{};
        desc.width = 256;
        desc.height = 256;
        desc.depth = 1;
        desc.mip_level = 1;
        desc.format = gal_texture_format::R8G8B8A8_UNORM;
        desc.dimension = gal_texture_dimension::td_2D;
        desc.resource_types = gal_resource_type::COLOR_RT;
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
    sampler_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal swapchain") {
    auto sampler_creation = [](gal::gal_api api) {
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
    sampler_creation(gal::gal_api::VULKAN);
    //sampler_creation(gal::RenderApi::d3d12);
}
