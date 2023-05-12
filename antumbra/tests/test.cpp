#include <01core/logging/log.h>
#include <01core/math/math.h>
#include <01core/memory/memory.h>
#include <glm.hpp>

#include <catch2/catch_test_macros.hpp>

#include <cstdint>

uint32_t factorial(uint32_t number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_CASE("Factorials are computed", "[factorial]")
{
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);
    REQUIRE(factorial(3) == 6);
    REQUIRE(factorial(10) == 3'628'800);
}
//
//int main()
//{
//    LOG_ERROR("{}", "test");
//    LOG_INFO("{}", ant::math::_1DIVPI);
//    auto stack_resource = ant::GetStackMemoryResource(1024);
//    ant::vector<int> vec(&stack_resource);
//    for (int i = 0; i < 10; i++) { vec.push_back(i); }
//    for (auto &v : vec) { v = 5; }
//    glm::vec3 v(1, 2, 3);
//    ant::math::vec3();
//    return 1;
//}