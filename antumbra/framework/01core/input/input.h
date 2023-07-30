#pragma once

#include "framework/01core/logging/log.h"
#include "framework/01core/utils/utils.h"
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
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

bool create_window(window_desc *desc, ante_window *window) {
    if (glfwInit() != GLFW_TRUE) {
        LOG_ERROR("failed to init glfw");
    };
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow* glfw_window = glfwCreateWindow(static_cast<i32>(desc->width), static_cast<i32>(desc->height), desc->name, nullptr, nullptr);

    if (glfw_window == nullptr) {
        glfwTerminate();
        LOG_ERROR("failed to init window");
    }
    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(desc->b_vsync ? 1 : 0);

    window->m_window = glfw_window;
    return true;
}

void destroy_window(ante_window* window) {
    glfwDestroyWindow(window->m_window);
    glfwTerminate();
    window = nullptr;
}

bool window_pool_events(ante_window *window) {
    if (glfwWindowShouldClose(window->m_window) !=0) {
        destroy_window(window);
        return false;
    }
    glfwPollEvents();
    return true;
}

#ifdef _WIN32
HWND get_hwnd_window(ante_window window) { return glfwGetWin32Window(window.m_window); }
#endif // WIN32

//enum class input_mode { mouse_keyboard, controller };
//
//bool add_device(input_mode mode);
//
//bool get_key();
//bool get_key_down();
//};

} // namespace ante
