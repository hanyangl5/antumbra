#pragma once

// standard libraries
#include <array>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory_resource>
// third party libraries

// project headers

namespace ant {

using str = std::pmr::string;
using wstr = std::pmr::wstring;
template<typename T, size_t size> using fixed_array = std::array<T, size>;
template<typename T> using vector = std::pmr::vector<T>;
template<typename T> using set = std::pmr::set<T>;
template<typename T> using hash_set = std::pmr::unordered_set<T>;
template<typename Key, typename Val> using map = std::pmr::map<Key, Val>;
template<typename Key, typename Val> using hash_map = std::pmr::unordered_map<Key, Val>;

}// namespace ant