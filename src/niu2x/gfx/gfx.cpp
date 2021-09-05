#include <niu2x/gfx.h>

#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <niu2x/gfx/internal.h>

namespace nx::gfx {

namespace {

mallocfree_memory default_memory;

void glfw_setup()
{
    auto r = glfwInit();
    NX_ASSERT(r, "glfw init failed");
}

void glfw_cleanup() { glfwTerminate(); }

auto create_glfw_window(const config& c)
{
    if (c.options & MSAA)
        glfwWindowHint(GLFW_SAMPLES, 8);

    auto glfw_window
        = glfwCreateWindow(c.width, c.height, c.title.c_str(), 0, nullptr);
    NX_ASSERT(glfw_window, "create glfw_window failed.");

    glfwMakeContextCurrent(glfw_window);
    // glfwSwapInterval(1);
    NX_ASSERT(glewInit() == GLEW_OK, "glew init failed");

    return glfw_window;
}

void destroy_glfw_window(GLFWwindow* glfw_window)
{
    glfwDestroyWindow(glfw_window);
}

} // namespace

memory_proxy default_allocator { &default_memory };

void set_allocator(memory_proxy allocator) { default_allocator = allocator; }

void run(const config& c)
{
    glfw_setup();

    auto glfw_window = create_glfw_window(c);

    auto last_now = std::chrono::steady_clock::now();
    auto now = last_now;
    float dt;

    auto update = c.update;

#define duration_cast(t)                                                       \
    std::chrono::duration_cast<std::chrono::microseconds>((t))

    while (!glfwWindowShouldClose(glfw_window)) {
        glfwPollEvents();
        glfwSwapBuffers(glfw_window);

        now = std::chrono::steady_clock::now();
        dt = duration_cast(now - last_now).count() / 1000000.0;

        last_now = now;

        update(dt);
    }

    destroy_glfw_window(glfw_window);

    glfw_cleanup();
}

} // namespace nx::gfx