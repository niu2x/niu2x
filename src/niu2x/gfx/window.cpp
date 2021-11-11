#include <niu2x/gfx.h>

#include <chrono>

#include <niu2x/log.h>

#include "niu2x/assert.h"
#include "niu2x/utils.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace nx::gfx {

namespace {

void glfw_setup();
void glfw_cleanup();

GLFWwindow* create_glfw_window(const window_config& c);
void destroy_glfw_window(GLFWwindow* window);

void glfw_error_callback(int error, const char* description);

void key_callback(GLFWwindow*, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow*, int w, int h);

void imgui_setup(GLFWwindow* window);
void imgui_cleanup();
void imgui_update();

} // namespace

void run(const window_config& c)
{
    glfw_setup();

    auto glfw_window = create_glfw_window(c);

    glfwSetWindowUserPointer(glfw_window, (void*)&c);

    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);

    glEnable(GL_POLYGON_SMOOTH_HINT);

    auto last_now = std::chrono::steady_clock::now();
    auto now = last_now;
    float dt = 0;

    auto update = c.update;

    imgui_setup(glfw_window);

    if (c.setup)
        c.setup();

#define duration_cast(t)                                                       \
    std::chrono::duration_cast<std::chrono::microseconds>((t))

    while (!glfwWindowShouldClose(glfw_window)) {
        now = std::chrono::steady_clock::now();
        dt = duration_cast(now - last_now).count() / 1000000.0;
        last_now = now;

        glfwPollEvents();

        update(dt);

        imgui_update();

        glfwSwapBuffers(glfw_window);
    }

    if (c.cleanup)
        c.cleanup();

    imgui_cleanup();

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

void imgui_setup(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void imgui_cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void imgui_update()
{
    static bool show_demo_window = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    void* ud = glfwGetWindowUserPointer(window);
    if (ud) {
        const window_config* c = reinterpret_cast<const window_config*>(ud);
        double scale = std::min(double(w) / c->width, double(h) / c->height);
        int viewport_width = scale * c->width;
        int viewport_height = scale * c->height;
        int viewport_x = (w - viewport_width) >> 1;
        int viewport_y = (h - viewport_height) >> 1;
        glViewport(viewport_x, viewport_y, viewport_width, viewport_height);
    }
}

} // namespace

} // namespace nx::gfx