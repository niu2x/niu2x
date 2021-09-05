#include <niu2x/gfx.h>

#include <string.h>

#include <limits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <niu2x/gfx/internal.h>

namespace nx::gfx {

namespace {

struct window_item {
    list_head list;
    GLFWwindow* glfw_window;
    uint32_t bgcolor;
    struct {
        uint8_t bgcolor_dirty : 1;
    } flags;
};

NX_LIST_HEAD(windows);

void glfw_setup()
{
    auto r = glfwInit();
    NX_ASSERT(r, "glfw init failed");
}
void glfw_cleanup() { glfwTerminate(); }

bool glew_inited = false;

} // namespace

rid create_window(uint64_t options)
{
    if (list_empty(&windows)) {
        glfw_setup();
    }

    auto window = reinterpret_cast<window_item*>(
        default_allocator.allocate(sizeof(window_item)));
    memset(window, 0, sizeof(window_item));

    list_add(&(window->list), &windows);

    if (options & MSAA)
        glfwWindowHint(GLFW_SAMPLES, 8);

    auto glfw_window = glfwCreateWindow(640, 480, "NX GFX", 0, nullptr);
    NX_ASSERT(glfw_window, "create glfw_window failed.");

    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);

    if (!glew_inited) {
        NX_ASSERT(glewInit() == GLEW_OK, "glew init failed");
        glew_inited = true;
    }

    window->glfw_window = glfw_window;

    return window;
}

void destroy_window(rid window_id)
{
    auto window = reinterpret_cast<window_item*>(window_id);
    list_del(&(window->list));

    glfwDestroyWindow(window->glfw_window);

    default_allocator.free(window);

    if (list_empty(&windows)) {
        glfw_cleanup();
    }
}

void window_set_bgcolor(rid window_id, uint32_t color)
{
    auto window = reinterpret_cast<window_item*>(window_id);
    window->bgcolor = color;
    window->flags.bgcolor_dirty = 1;
}

void frame()
{
    window_item* window;
    NX_LIST_FOR_EACH(ptr, &windows)
    {
        window = NX_LIST_ENTRY(ptr, window_item, list);
        auto* glfw_window = window->glfw_window;
        glfwMakeContextCurrent(glfw_window);
        glfwPollEvents();

        auto flags = window->flags;
        if (flags.bgcolor_dirty) {
            window->flags.bgcolor_dirty = 0;
            auto bgcolor = window->bgcolor;
            glClearColor((float)((bgcolor >> 0) & 8),
                (float)((bgcolor >> 8) & 8), (float)((bgcolor >> 16) & 8),
                (float)((bgcolor >> 24) & 8));
        }

        glClear(
            GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(glfw_window);
    }
}

} // namespace nx::gfx