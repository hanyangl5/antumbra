//#pragma once
//
//#include <sstream>
//
//#include "framework/utils/memory/container.h"
//
//namespace ante {
//
//template<typename T> inline void Write(std::ostringstream &os, const T &value)
//{
//    os.write(reinterpret_cast<const char *>(&value), sizeof(T));
//}
//
//inline void Write(std::ostringstream &os, const ante::str &value)
//{
//    Write(os, value.size());
//    os.write(value.data(), value.size());
//}
//
//template<class T> inline void Write(std::ostringstream &os, const ante::set<T> &value)
//{
//    Write(os, value.size());
//    for (const T &item : value) { os.write(reinterpret_cast<const char *>(&item), sizeof(T)); }
//}
//
//template<class T> inline void Write(std::ostringstream &os, const ante::vector<T> &value)
//{
//    Write(os, value.size());
//    os.write(reinterpret_cast<const char *>(value.data()), value.size() * sizeof(T));
//}
//
//template<class T, class S> inline void Write(std::ostringstream &os, const ante::map<T, S> &value)
//{
//    Write(os, value.size());
//
//    for (const std::pair<T, S> &item : value) {
//        Write(os, item.first);
//        Write(os, item.second);
//    }
//}
//
//template<class T, uint32_t N> inline void Write(std::ostringstream &os, const ante::fixed_array<T, N> &value)
//{
//    os.write(reinterpret_cast<const char *>(value.data()), N * sizeof(T));
//}
//
//template<typename T, typename... Args>
//inline void Write(std::ostringstream &os, const T &first_arg, const Args &...args)
//{
//    Write(os, first_arg);
//
//    Write(os, args...);
//}
//}// namespace ante