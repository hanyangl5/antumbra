
#include "test_gal.h"
#include <tracy/Tracy.hpp>
using namespace ant;

TEST_CASE("test context initialization") {
    auto context_initialization = [](gal::RenderApi api) {
        gal::gal_context context = initialize(api);
        destroy(context);
    };

    context_initialization(gal::RenderApi::vulkan);
    context_initialization(gal::RenderApi::d3d12);
}

TEST_CASE("test gal buffer") {
    auto buffer_creation = [](gal::RenderApi api) {
        gal::gal_context context = initialize(api);

        gal::gal_buffer_desc desc{};
        desc.size = 256;
        desc.initial_state = gal::gal_resource_state::rs_rw_buffer;
        desc.resource_types = gal::gal_resource_type::rt_constant_buffer | gal::gal_resource_type::rt_rw_buffer |
                              gal::gal_resource_type::rt_indirect_buffer;
        desc.memory_flags = gal::memory_flag::gpu_dedicated;
        desc.flags = gal::gal_buffer_flag::bcf_own_memory;
        gal::gal_buffer buffer{};
        gal::gal_error_code result = gal::create_buffer(context, &desc, &buffer);
        REQUIRE(result == gal::gal_error_code::success);
        result = gal::destroy_buffer(context, buffer);
        REQUIRE(result == gal::gal_error_code::success);
        destroy(context);
    };
    buffer_creation(gal::RenderApi::vulkan);
    buffer_creation(gal::RenderApi::d3d12);
}
