#pragma once

// standard libraries
#include <chrono>
#include <optional>
#include <type_traits>

// third party libraries

// project headers
#include "framework/01core/utils/utils.h"

namespace ant {

using namespace std::chrono_literals;

template <typename Interval, typename Func, typename... Args>
[[nodiscard]] std::optional<typename std::result_of<Func(Args...)>::type>
call_func_with_interval(Interval interval, Func func, Args... args) {
    using namespace std::chrono_literals;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<f64> time_point = std::chrono::time_point_cast<Interval>(now).time_since_epoch();
    static std::chrono::duration<f64> last_time_point;
    if (time_point - last_time_point > std::chrono::duration<f64>(interval)) {
        last_time_point = time_point;
        return func(args...);
    } else {
        return {};
    }
}

template <typename Func, typename... Args> auto callFuncPerSecond(Func func, Args... args) {
    call_func_with_interval(1s, func, args...);
}
template <typename Func, typename... Args> auto callFuncPerMinutes(Func func, Args... args) {
    call_func_with_interval(1m, func, args...);
}
template <typename Func, typename... Args> auto callFuncPerHour(Func func, Args... args) {
    call_func_with_interval(1h, func, args...);
}
} // namespace ant