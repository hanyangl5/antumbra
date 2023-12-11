#include "input.h"
namespace ante {
bool create_window(window_desc *desc, ante_window *window) {
    if (glfwInit() != GLFW_TRUE) {
        LOG_ERROR("failed to init glfw");
    };
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow *glfw_window =
        glfwCreateWindow(static_cast<i32>(desc->width), static_cast<i32>(desc->height), desc->name, nullptr, nullptr);

    if (glfw_window == nullptr) {
        glfwTerminate();
        LOG_ERROR("failed to init window");
    }
    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(desc->b_vsync ? 1 : 0);

    window->m_window = glfw_window;
    return true;
}
void destroy_window(ante_window *window) {
    glfwDestroyWindow(window->m_window);
    glfwTerminate();
    window = nullptr;
}
bool window_pool_events(ante_window *window) {
    if (glfwWindowShouldClose(window->m_window) != 0) {
        destroy_window(window);
        return false;
    }
    glfwPollEvents();
    return true;
}
} // namespace ante