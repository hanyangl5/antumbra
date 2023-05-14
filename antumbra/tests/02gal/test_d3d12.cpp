#include <catch2/catch_test_macros.hpp>
#include <framework/02gal/gal.h>

using namespace ant;

TEST_CASE("test d3d12")
{
    gal::gal_context context;
    gal::gal_error_code result = ant::gal::init_gal(ant::gal::RenderApi::d3d12, &context);
    REQUIRE(result == gal::gal_error_code::success);
    gal::GalDesc gal_desc{};
    gal_desc.b_debug_layer = true;
    gal_desc.b_swap_chain = true;
    ant::gal::create_instance(&gal_desc,&context);
    //ant::gal::create_device(&gal_desc, &context);
}
