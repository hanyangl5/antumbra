#pragma once

// standard libraries
#include <array>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// third party libraries

// project headers
#include "allocator2.h"
namespace ante {

// FIXME(hyl5): dupe with allocator2.h

using str = std::string;
using wstr = std::wstring;
template <typename T, size_t size> using fixed_array = std::array<T, size>;
template <typename T> using vector = std::vector<T>;
template <typename T> using set = std::set<T>;
template <typename Key, typename Val> using map = std::map<Key, Val>;

#if defined(__APPLE__)
template <typename T> using hash_set = std::flat_set<T>;
template <typename Key, typename Val> using hash_map = std::flat_map<Key, Val>;
#else
template <typename T> using hash_set = std::unordered_set<T>;
template <typename Key, typename Val> using hash_map = std::unordered_map<Key, Val>;
#endif
} // namespace ante