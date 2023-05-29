#pragma once

#include "framework/01core/logging/log.h"
#include "framework/01core/utils/utils.h"
#include <GLFW/glfw3.h>
#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
namespace ant {
struct ant_window {
    GLFWwindow *m_window;
};

struct window_desc {
    const char *name;
    u32 width;
    u32 height;
    bool b_vsync;
};

bool create_window(window_desc *_desc, ant_window *_window) {
    if (glfwInit() != GLFW_TRUE) {
        glfwTerminate();
        LOG_ERROR("failed to init glfw");
    };
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window->m_window = glfwCreateWindow(_desc->width, _desc->height, _desc->name, nullptr, nullptr);

    if (!_window) {
        glfwTerminate();
        glfwTerminate();
        LOG_ERROR("failed to init window");
    }
    glfwMakeContextCurrent(_window->m_window);
    glfwSwapInterval(_desc->b_vsync ? 1 : 0);
    return true;
}

void destroy_window(ant_window window) {
    glfwDestroyWindow(window.m_window);
    glfwTerminate();
}

#ifdef WIN32
HWND get_hwnd_window(ant_window window) { return glfwGetWin32Window(window.m_window); }
#endif // WIN32

//enum class input_mode { mouse_keyboard, controller };
//
//bool add_device(input_mode mode);
//
//bool get_key();
//bool get_key_down();
//};

} // namespace ant
