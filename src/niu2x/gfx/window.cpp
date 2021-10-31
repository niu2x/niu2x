#include <niu2x/gfx.h>

#include <chrono>

#include <niu2x/log.h>

#include "niu2x/assert.h"
#include "niu2x/utils.h"

namespace nx::gfx {

namespace {

void glfw_setup();
void glfw_cleanup();

GLFWwindow* create_glfw_window(const window_config& c);
void destroy_glfw_window(GLFWwindow* window);

void key_callback(GLFWwindow*, int key, int scancode, int action, int mods);
void glfw_error_callback(int error, const char* description);
} // namespace

void run(const window_config& c)
{
    glfw_setup();

    auto glfw_window = create_glfw_window(c);

    glfwSetKeyCallback(glfw_window, key_callback);

    auto last_now = std::chrono::steady_clock::now();
    auto now = last_now;
    float dt;

    auto update = c.update;

    if (c.setup)
        c.setup();

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

    if (c.cleanup)
        c.cleanup();

    destroy_glfw_window(glfw_window);

    glfw_cleanup();
}

namespace {

void glfw_setup()
{
    glfwSetErrorCallback(glfw_error_callback);
    auto r = glfwInit();
    NX_ASSERT(r, "glfw init failed");
}

void glfw_cleanup() { glfwTerminate(); }

GLFWwindow* create_glfw_window(const window_config& c)
{
    if (c.options & MSAA)
        glfwWindowHint(GLFW_SAMPLES, 8);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    auto glfw_window
        = glfwCreateWindow(c.width, c.height, c.title.c_str(), 0, nullptr);
    NX_ASSERT(glfw_window, "create glfw_window failed.");

    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);
    NX_ASSERT(glewInit() == GLEW_OK, "glew init failed");

    return glfw_window;
}

void destroy_glfw_window(GLFWwindow* glfw_window)
{
    glfwDestroyWindow(glfw_window);
}

void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    unused(window);
    NX_LOG_D("%d %d %d %d", key, scancode, action, mods);
}

void glfw_error_callback(int error, const char* description)
{
    NX_LOG_E("Glfw Error %d: %s\n", error, description);
}

} // namespace

} // namespace nx::gfx