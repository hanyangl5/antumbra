#include <catch2/catch_test_macros.hpp>
#include <framework/01core/logging/log.h>

TEST_CASE("log")
{
    LOG_DEBUG("{}", "log test: debug");
    LOG_INFO("{}", "log test: info");
    LOG_WARN("{}", "log test: warn");
    LOG_ERROR("{}", "log test: error");
}