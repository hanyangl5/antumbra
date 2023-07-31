#pragma once

#include <GLFW/glfw3.h>

#include "framework/01core/logging/log.h"
#include "framework/01core/utils/utils.h"

namespace ante {

struct ante_window {
    GLFWwindow *m_window;
};

struct window_desc {
    const char *name;
    u32 width;
    u32 height;
    bool b_vsync;
};

bool create_window(window_desc *desc, ante_window *window);

void destroy_window(ante_window *window);

bool window_pool_events(ante_window *window);

//#ifdef _WIN32
//HWND get_hwnd_window(ante_window window) { return glfwGetWin32Window(window.m_window); }
//#endif // WIN32

//enum class input_mode { mouse_keyboard, controller };
//
//bool add_device(input_mode mode);
//
//bool get_key();
//bool get_key_down();
//};

} // namespace ante
