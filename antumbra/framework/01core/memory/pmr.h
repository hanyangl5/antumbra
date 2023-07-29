#pragma once

#if defined(__APPLE__)
#include <boost/container/pmr/memory_resource.hpp>
#else
#include <memory_resource>
#endif

namespace ante::memory {
#ifndef ANT_PMR
#define ANT_PMR
#if defined(__APPLE__)
namespace ante_pmr = boost::container::pmr;
#else
namespace ante_pmr = std::pmr;
#endif
#endif
} // namespace ante::memory