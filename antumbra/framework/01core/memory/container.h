#pragma once

// standard libraries
#include <array>
#if defined(__APPLE__)
#include <boost/container/pmr/flat_map.hpp>
#include <boost/container/pmr/flat_set.hpp>
#include <boost/container/pmr/map.hpp>
#include <boost/container/pmr/memory_resource.hpp>
#include <boost/container/pmr/set.hpp>
#include <boost/container/pmr/string.hpp>
#include <boost/container/pmr/vector.hpp>
#else
#include <map>
#include <memory_resource>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#endif

#include "pmr.h"

// third party libraries

// project headers

namespace ante {

// FIXME(hyl5): dupe with allocator2.h

using str = memory::ante_pmr::string;
using wstr = memory::ante_pmr::wstring;
template <typename T, size_t size> using fixed_array = std::array<T, size>;
template <typename T> using vector = memory::ante_pmr::vector<T>;
template <typename T> using set = memory::ante_pmr::set<T>;
template <typename Key, typename Val> using map = memory::ante_pmr::map<Key, Val>;

#if defined(__APPLE__)
template <typename T> using hash_set = memory::ante_pmr::flat_set<T>;
template <typename Key, typename Val> using hash_map = memory::ante_pmr::flat_map<Key, Val>;
#else
template <typename T> using hash_set = memory::ante_pmr::unorderd_set<T>;
template <typename Key, typename Val> using hash_map = memory::ante_pmr::unorderd_map<Key, Val>;
#endif
} // namespace ante