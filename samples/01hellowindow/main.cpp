#include <framework/utils/input/input.h>

using namespace ante;

int main() {
    ante_window window{};
    window_desc desc{};
    desc.width = 1024;
    desc.height = 720;
    desc.name = "01hellowindow";
    desc.b_vsync = false;
    create_window(&desc, &window);

    while (window_pool_events(&window)) {
        ;
    }

    return 0;
}