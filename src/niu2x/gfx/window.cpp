#include "gfx.h"

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
void mouse_key_callback(GLFWwindow*, int key, int action, int mods);
void framebuffer_size_callback(GLFWwindow*, int w, int h);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

void imgui_setup(GLFWwindow* window);
void imgui_cleanup();
void imgui_update(double dt);

} // namespace

math::vec2 window_size;

double now_seconds = 0;

static GLFWwindow* current_glfw_window = nullptr;
// static int window_width, window_height;

void run(const window_config& c)
{
    NX_ASSERT(current_glfw_window == nullptr, "already had a glfw_window.");

    glfw_setup();

    auto glfw_window = create_glfw_window(c);
    current_glfw_window = glfw_window;

    glfwSetWindowUserPointer(glfw_window, (void*)&c);

    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_pos_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_key_callback);

    auto last_now = std::chrono::steady_clock::now();
    auto now = last_now;
    float dt = 0;

    auto update = c.update;

    imgui_setup(glfw_window);

    setup();

    if (c.setup)
        c.setup();

#define duration_cast(t)                                                       \
    std::chrono::duration_cast<std::chrono::microseconds>((t))

    static double average_dt = 1 / 60.0;
    static double scale = 1 / 1000000.0;
    while (!glfwWindowShouldClose(glfw_window)) {
        now = std::chrono::steady_clock::now();
        dt = duration_cast(now - last_now).count() * scale;
        last_now = now;

        now_seconds = duration_cast(now.time_since_epoch()).count() * scale;

        glfwPollEvents();

        update(dt);

        imgui_update(dt);

        average_dt = average_dt * 0.99 + dt * 0.01;
        printf(0, 0, "FP_S: %.2f", (1 / average_dt));
        glfwSwapBuffers(glfw_window);
    }

    if (c.cleanup)
        c.cleanup();

    cleanup();

    imgui_cleanup();

    current_glfw_window = nullptr;
    destroy_glfw_window(glfw_window);

    glfw_cleanup();

    if (global::mem.used()) {
        NX_LOG_E("mem used: %lu", global::mem.used());
    }
}

void exit()
{
    NX_ASSERT(current_glfw_window != nullptr, "no glfw_window.");
    glfwSetWindowShouldClose(current_glfw_window, GLFW_TRUE);
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

    window_size[0] = c.width;
    window_size[1] = c.height;

    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);
    NX_ASSERT(glewInit() == GLEW_OK, "glew init failed");

    return glfw_window;
}

void destroy_glfw_window(GLFWwindow* glfw_window)
{
    glfwDestroyWindow(glfw_window);
}

const window_config* get_window_option(GLFWwindow* window)
{
    void* ud = glfwGetWindowUserPointer(window);
    if (ud) {
        const window_config* c = reinterpret_cast<const window_config*>(ud);
        return c;
    } else
        return nullptr;
}

void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // NX_LOG_D("%d %d %d %d", key, scancode, action, mods);
    unused(scancode);
    auto* c = get_window_option(window);
    if (c && c->key_callback) {
        c->key_callback(key, action, mods);
    }
}

void mouse_key_callback(GLFWwindow* window, int key, int action, int mods)
{
    // NX_LOG_D("%d %d %d", key, action, mods);
    auto* c = get_window_option(window);
    if (c && c->key_callback) {
        c->key_callback(key, action, mods);
    }
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

void imgui_update(double dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::LabelText("FPS", "%d", (int)(1 / dt));
    ImGui::LabelText("dt", "%.4f", dt);

    ImGui::EndFrame();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    auto* c = get_window_option(window);
    if (c) {
        double scale = std::min(double(w) / c->width, double(h) / c->height);
        int viewport_width = scale * c->width;
        int viewport_height = scale * c->height;
        int viewport_x = (w - viewport_width) >> 1;
        int viewport_y = (h - viewport_height) >> 1;
        glViewport(viewport_x, viewport_y, viewport_width, viewport_height);

        window_size[0] = viewport_width;
        window_size[1] = viewport_height;
    }
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto* c = get_window_option(window);
    if (c && c->mouse_pos_callback) {
        c->mouse_pos_callback(xpos, ypos);
    }
}

} // namespace

} // namespace nx::gfx