#include <cassert>
#include <framework/02gal/gal.h>
#include <iostream>

using namespace ante;

int main() {
    ante::gal::gal_context context{};
    auto err = gal::init_gal(ante::gal::gal_api::VULKAN, &context);
    assert(err == gal::gal_error_code::SUC);

    err = gal::destroy_gal(context);
    assert(err == ante::gal::gal_error_code::SUC);
    context = gal::gal_null;
}