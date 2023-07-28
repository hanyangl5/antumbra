#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "framework/01core/logging/log.h"
#include "framework/01core/input/input.h"
#include "framework/01core/memory/memory.h"
#include "framework/02gal/gal.h"
#include "framework/02gal/shader/shader_compiler.h"

using namespace ante;

gal::gal_context initialize(ante::gal::gal_api api) {
    //ante::memory::initialize_memory_system();
    gal::gal_context context{};
    gal::gal_error_code result = ante::gal::init_gal(api, &context);
    REQUIRE(result == gal::gal_error_code::SUC);
    gal::gal_desc gal_desc{};
    gal_desc.b_debug_layer = true;
    gal_desc.b_swap_chain = true;
    gal_desc.b_surface = true;
    gal_desc.b_async_compute = true;
    gal_desc.b_async_transfer = true;
    // gal_desc.b_ray_tracing = true;
    //gal_desc.b_mesh_shader = true;
    //gal_desc.b_variable_rate_shading = true;
    result = ante::gal::create_instance(&gal_desc, &context);
    REQUIRE(result == gal::gal_error_code::SUC);
    result = ante::gal::create_device(&gal_desc, &context);
    REQUIRE(result == gal::gal_error_code::SUC);
    result = ante::gal::create_memory_allocator(&context);
    REQUIRE(result == gal::gal_error_code::SUC);
    return context;
}

void destroy(gal::gal_context &context) {
    gal::gal_error_code result = ante::gal::destroy_memory_allocator(&context);
    REQUIRE(result == gal::gal_error_code::SUC);
    result = ante::gal::destroy_device(&context);
    REQUIRE(result == gal::gal_error_code::SUC);
    result = ante::gal::destroy_instance(&context);
    REQUIRE(result == gal::gal_error_code::SUC);
    result = ante::gal::destroy_gal(context);
    REQUIRE(result == gal::gal_error_code::SUC);
   //ante::memory::destroy_memory_system();
};
