#include <framework/01core/timer/timer.h>

#include <catch2/catch_test_macros.hpp>

#include <iostream>

using namespace ant;

TEST_CASE("call function with interval test") {
    auto test_function = [](int x, int y) -> int { return x + y; };
    auto res = ant::callFuncWithInterval(5ms, test_function, 4, 5);
    if (res.has_value()) { REQUIRE(res.value() == 9); }
}
