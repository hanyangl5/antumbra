
#include "test_gal.h"
#include <tracy/Tracy.hpp>
using namespace ant;
using namespace ant::gal;
TEST_CASE("test context initialization") {
    auto context_initialization = [](gal::RenderApi api) {
        gal::gal_context context = initialize(api);
        destroy(context);
    };

    context_initialization(gal::RenderApi::vulkan);
    //context_initialization(gal::RenderApi::d3d12);
}

TEST_CASE("test gal buffer") {
    auto buffer_creation = [](gal::RenderApi api) {
        gal::gal_context context = initialize(api);

        gal::gal_buffer_desc desc{};
        desc.size = 256;
        desc.initial_state = gal::gal_resource_state::rs_rw_buffer;
        desc.resource_types = gal::gal_resource_type::rt_constant_buffer | gal::gal_resource_type::rt_rw_buffer |
                              gal::gal_resource_type::rt_indirect_buffer;
        desc.memory_flags =
            gal::gal_memory_flag::gpu_dedicated | gal_memory_flag::gpu_download | gal_memory_flag::cpu_upload;
        ;
        desc.flags = gal::gal_buffer_flag::bcf_own_memory;
        gal::gal_buffer buffer{};
        gal::gal_error_code result = gal::create_buffer(context, &desc, &buffer);
        REQUIRE(result == gal::gal_error_code::success);
        result = gal::destroy_buffer(context, buffer);
        REQUIRE(result == gal::gal_error_code::success);
        destroy(context);
    };
    buffer_creation(gal::RenderApi::vulkan);
    //buffer_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal texture") {
    auto texture_creation = [](gal::RenderApi api) {
        gal::gal_context context = initialize(api);

        gal_texture_desc texture_desc{};
        texture_desc.width = 256;
        texture_desc.height = 256;
        texture_desc.depth = 1;
        texture_desc.mip_level = 1;
        texture_desc.array_size = 1;
        texture_desc.format = gal_texture_format::R8G8B8A8_UNORM;
        texture_desc.dimension = gal_texture_dimension::td_2D;
        texture_desc.resource_types = gal_resource_type::rt_texture | gal_resource_type::rt_rw_texture;
        texture_desc.initial_state = gal_resource_state::rs_rw_texture;
        texture_desc.memory_flags =
            gal_memory_flag::gpu_dedicated | gal_memory_flag::gpu_download | gal_memory_flag::cpu_upload;
        gal::gal_texture texture{};
        gal::gal_error_code result = gal::create_texture(context, &texture_desc, &texture);
        REQUIRE(result == gal::gal_error_code::success);
        result = gal::destroy_texture(context, texture);
        REQUIRE(result == gal::gal_error_code::success);
        destroy(context);
    };
    texture_creation(gal::RenderApi::vulkan);
    //buffer_creation(gal::RenderApi::d3d12);
}

TEST_CASE("test gal sampler") {
    auto sampler_creation = [](gal::RenderApi api) {
        gal::gal_context context = initialize(api);

        gal_sampler_desc sampler_desc{};
        sampler_desc.min_filter = gal_sampler_filter_mode::linear;
        sampler_desc.mag_filter = gal_sampler_filter_mode::linear;
        sampler_desc.mip_mode = gal_sampler_mip_mode::linear;
        sampler_desc.address_mode_u = gal_sampler_address_mode::clamp;
        sampler_desc.address_mode_v = gal_sampler_address_mode::clamp;
        sampler_desc.address_mode_w = gal_sampler_address_mode::clamp;
        sampler_desc.min_lod = 0.0f;
        sampler_desc.max_lod = 1.0f;
        sampler_desc.max_anisotropy = 1.0f;
        sampler_desc.compare_mode = gal_compare_mode::never;
        sampler_desc.mip_lod_bias = 0.0f;
        gal::gal_sampler sampler{};
        gal::gal_error_code result = gal::create_sampler(context, &sampler_desc, &sampler);
        REQUIRE(result == gal::gal_error_code::success);
        result = gal::destroy_sampler(context, sampler);
        REQUIRE(result == gal::gal_error_code::success);
        destroy(context);
    };
    sampler_creation(gal::RenderApi::vulkan);
    //sampler_creation(gal::RenderApi::d3d12);
}
