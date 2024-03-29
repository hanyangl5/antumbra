//#pragma once
//
//#include "framework/utils/memory/container.h"
//#include "framework/utils/types/types.h"
//#include <sstream>
//
//namespace ante {
//
//template<typename T> inline void Read(std::istringstream &is, T &value)
//{
//    is.read(reinterpret_cast<char *>(&value), sizeof(T));
//}
//
//inline void Read(std::istringstream &is, ante::str &value)
//{
//    u64 size;
//    Read(is, size);
//    value.resize(size);
//    is.read(const_cast<char *>(value.data()), size);
//}
//
//template<class T> inline void Read(std::istringstream &is, ante::set<T> &value)
//{
//    u64 size;
//    Read(is, size);
//    for (uint32_t i = 0; i < size; i++) {
//        T item;
//        is.read(reinterpret_cast<char *>(&item), sizeof(T));
//        value.insert(std::move(item));
//    }
//}
//
//template<class T> inline void Read(std::istringstream &is, ante::vector<T> &value)
//{
//    u64 size;
//    Read(is, size);
//    value.resize(size);
//    is.read(reinterpret_cast<char *>(value.data()), value.size() * sizeof(T));
//}
//
//template<class T, class S> inline void Read(std::istringstream &is, ante::vector<T, S> &value)
//{
//    u64 size;
//    Read(is, size);
//
//    for (uint32_t i = 0; i < size; i++) {
//        std::pair<T, S> item;
//        Read(is, item.first);
//        Read(is, item.second);
//
//        value.insert(std::move(item));
//    }
//}
//
//template<class T, uint32_t N> inline void Read(std::istringstream &is, ante::fixed_array<T, N> &value)
//{
//    is.read(reinterpret_cast<char *>(value.data()), N * sizeof(T));
//}
//
//template<typename T, typename... Args> inline void Read(std::istringstream &is, T &first_arg, Args &...args)
//{
//    Read(is, first_arg);
//
//    Read(is, args...);
//}
//
//}// namespace ante