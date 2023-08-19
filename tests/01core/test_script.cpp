//#include <catch2/catch_test_macros.hpp>
#include <doctest.h>

#include "framework/01core/script/lua_binding.h"

// TODO

TEST_CASE("lua test") {
    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x] { ++x; });
    lua.script("beep()");
    assert(x == 1);
}
