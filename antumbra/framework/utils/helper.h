#pragma once

#define DELETE_COPY_MOVE(class_name)                                                                                   \
    class_name(const class_name &) = delete;                                                                           \
    class_name(class_name &&) = delete;                                                                                \
    class_name &operator=(const class_name &) = delete;                                                                \
    class_name &operator=(class_name &&) = delete;

#define DEFAULT_COPY_MOVE(class_name)                                                                                  \
    class_name(const class_name &) = default;                                                                          \
    class_name(class_name &&) = default;                                                                               \
    class_name &operator=(const class_name &) = default;                                                               \
    class_name &operator=(class_name &&) = default;

namespace ante {

template <typename T> T align_up(T a, T size) { return ((a + size - 1) & (~(size - 1))); }
} // namespace ante