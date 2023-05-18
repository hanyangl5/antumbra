#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "framework/01core/logging/log.h"
#include "framework/01core/memory/container.h"
#include "framework/01core/reflection/enum_reflection.h"

using namespace ant;

enum class Color { NONE, RED, YELLOW, GREEN, BLUE, WHITE, BLACK };
TEST_CASE("Enum value to string") {

    constexpr auto color_name = ant::enum_refl::enum_name(Color::RED);
    // color_name -> "RED"
    REQUIRE(std::strcmp(color_name.data(), "RED") == 0);
}

TEST_CASE("String to enum value") {

    ant::str color_name{"GREEN"};
    auto color = ant::enum_refl::enum_cast<Color>(color_name);
    REQUIRE(color.value() == Color::GREEN);
    ant::str value{"none"};
    auto color_or_default = ant::enum_refl::enum_cast<Color>(value).value_or(Color::NONE);
    REQUIRE(color_or_default == Color::NONE);
}

TEST_CASE("Indexed access to enum value") {
    u32 i = 0;
    Color color = ant::enum_refl::enum_value<Color>(i);
    REQUIRE(color == Color::NONE);
}
TEST_CASE("Enum value sequence") {

    constexpr auto colors = ant::enum_refl::enum_values<Color>();
    for (u32 i = 0; i < colors.size(); i++) {
        REQUIRE(colors[i] == static_cast<Color>(i));
    }
}
TEST_CASE("Number of enum elements") {
    constexpr u64 color_count = ant::enum_refl::enum_count<Color>();
    REQUIRE(color_count == 7);
    // color_count -> 3
}
TEST_CASE("Enum value to integer") {
    Color color = Color::RED;
    auto color_integer = ant::enum_refl::enum_integer(color);
    REQUIRE(color_integer == 1);
    // color -> 1
}

TEST_CASE("Enum names sequence") {
    constexpr auto color_names = ant::enum_refl::enum_names<Color>();
    for (u32 i = 0; i < color_names.size(); i++) {
        REQUIRE(std::strcmp(color_names[i].data(), enum_refl::enum_name(static_cast<Color>(i)).data()) == 0);
    }
}
TEST_CASE("Enum entries sequence") {
    constexpr auto color_entries = ant::enum_refl::enum_entries<Color>();
    // color_entries -> {{Color::RED, "RED"}, {Color::BLUE, "BLUE"}, {Color::GREEN, "GREEN"}}
    // color_entries[0].first -> Color::RED
    // color_entries[0].second -> "RED"
}

TEST_CASE("Enum iterate for each enum as constexpr constant") {
    //ant::enum_refl::enum_for_each<Color>([](auto val) {
    //    constexpr Color c_color = val;
    //    // ...
    //});
}
TEST_CASE("Ostream operator for enum") {
    using namespace ant::enum_refl::ostream_operators; // out-of-the-box ostream operators for enums.
    Color color = Color::BLUE;
    std::cout << color << std::endl; // "BLUE"
    LOG_INFO("color: {}", ant::enum_refl::enum_name(color));
}
