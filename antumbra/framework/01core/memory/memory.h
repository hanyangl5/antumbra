#pragma once

// standard libraries
#include <memory_resource>

// third party libraries

// project headers
#include "framework/01core/memory/container.h"
#include "framework/01core/utils/utils.h"

namespace ant {

inline std::pmr::monotonic_buffer_resource get_stack_memory_resource(u64 size = 1024) {
    return std::pmr::monotonic_buffer_resource(size);
}

} // namespace ant