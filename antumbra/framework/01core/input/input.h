#pragma once

#include <GLFW/glfw3.h>

#include "framework/01core/utils/utils.h"
namespace ant {
struct ant_window {
    GLFWwindow *window;
};

struct window_desc {
    const char *name;
    u32 width;
    u32 height;
};

//bool create_window(window_desc *desc, ant_window *w) {
//    //glfwCreateWindow();
//    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//    //m_window = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
//
//    //if (!m_window) {
//    //    glfwTerminate();
//    //    LOG_ERROR("failed to init window");
//    //}
//    //glfwMakeContextCurrent(m_window);
//    //glfwSwapInterval(m_vsync_enabled ? 1 : 0);
//}
//
//void destroy_window(ant_window *w) {
//    //glfwDestroyWindow(m_window);
//    //glfwTerminate();
//}

enum class input_mode { mouse_keyboard, controller };

bool add_device(input_mode mode);

bool get_key();
bool get_key_down();
};

} // namespace ant
