#include "framework/01core/logging/log.h"
#include "framework/01core/input/input.h"
#include "framework/02gal/gal.h"
#include <catch2/catch_test_macros.hpp>
using namespace ant;

gal::gal_context initialize(ant::gal::gal_api api) {
    gal::gal_context context{};
    gal::gal_error_code result = ant::gal::init_gal(api, &context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    gal::gal_desc gal_desc{};
    gal_desc.b_debug_layer = true;
    gal_desc.b_swap_chain = true;
    gal_desc.b_surface = true;
    gal_desc.b_async_compute = true;
    gal_desc.b_async_transfer = true;
    // gal_desc.b_ray_tracing = true;
    //gal_desc.b_mesh_shader = true;
    //gal_desc.b_variable_rate_shading = true;
    result = ant::gal::create_instance(&gal_desc, &context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    result = ant::gal::create_device(&gal_desc, &context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    result = ant::gal::create_memory_allocator(&context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    return context;
}

void destroy(gal::gal_context &context) {
    gal::gal_error_code result = ant::gal::destroy_memory_allocator(&context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    result = ant::gal::destroy_device(&context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    result = ant::gal::destroy_instance(&context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
    result = ant::gal::destroy_gal(context);
    REQUIRE(result == gal::gal_error_code::GAL_ERRORCODE_SUCCESS);
};
