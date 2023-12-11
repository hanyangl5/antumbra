#pragma once

#include "framework/utils/types/types.h"

namespace ante {

struct blob {
  public:
    inline void set(void *data, u64 size) {
        m_data = data;
        m_size = size;
    }
    inline void *data() const { return m_data; }
    inline u64 size() const { return m_size; }
    inline void release();
    inline void reset() {
        m_data = nullptr;
        m_size = 0;
    }

  private:
    void *m_data = nullptr;
    u64 m_size = 0;
};
} // namespace ante