#include <catch2/catch_test_macros.hpp>
#include <framework/01core/logging/log.h>
#include <framework/02gal/gal.h>

using namespace ant;

gal::gal_context initialize(ant::gal::RenderApi api)
{
    gal::gal_context context{};
    gal::gal_error_code result = ant::gal::init_gal(api, &context);
    REQUIRE(result == gal::gal_error_code::success);
    gal::GalDesc gal_desc{};
    gal_desc.b_debug_layer = true;
    gal_desc.b_swap_chain = true;
    gal_desc.b_surface = true;
    gal_desc.b_async_compute = true;
    gal_desc.b_async_transfer = true;
    result = ant::gal::create_instance(&gal_desc, &context);
    REQUIRE(result == gal::gal_error_code::success);
    result = ant::gal::create_device(&gal_desc, &context);
    REQUIRE(result == gal::gal_error_code::success);
    result = ant::gal::create_memory_allocator(&context);
    REQUIRE(result == gal::gal_error_code::success);
    return context;
}

void destroy(gal::gal_context &context)
{
    gal::gal_error_code result = ant::gal::destroy_memory_allocator(&context);
    REQUIRE(result == gal::gal_error_code::success);
    result = ant::gal::destroy_device(&context);
    REQUIRE(result == gal::gal_error_code::success);
    result = ant::gal::destroy_instance(&context);
    REQUIRE(result == gal::gal_error_code::success);
    result = ant::gal::destroy_gal(context);
    REQUIRE(result == gal::gal_error_code::success);
};
