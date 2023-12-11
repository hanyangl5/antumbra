#pragma once

#if 0
#include <mimalloc-override.h> // redefines malloc etc.
#include <mimalloc.h>
//#define aligned_alloc(alignment, size) mi_aligned_alloc(alignment, size)
#define aligned_free(ptr) free(ptr)

#else
#include <cstdlib>

#ifdef _MSC_VER
#define aligned_alloc(alignment, size) _aligned_malloc(size, alignment)
#define aligned_free(ptr) _aligned_free(ptr)
#else
// Code for other compilers

inline void *aligned_alloc(size_t alignment, size_t size) {
    void *ptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        ptr = nullptr;
    }
    return ptr;
}

inline void aligned_free(void *ptr) { free(ptr); }
#endif
#endif
