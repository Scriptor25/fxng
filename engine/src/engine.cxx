#include <fxng/engine.hxx>
#include <fxng/log.hxx>
#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error_code, const char *description)
{
    fxng::Log(fxng::LogLevel_Info, "[glfw {:#08x}] {}", error_code, description);
}

static void gl_debug_message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *user_param)
{
    const auto window = static_cast<const GLFWwindow *>(user_param);
    (void) window;

    fxng::Log(fxng::LogLevel_Info, "[gl {:#08x}] {}", id, message);
}

static GLFWwindow *create_window(
    const fxng::WindowConfig &config,
    const std::string &name,
    const std::string &title,
    GLFWmonitor *monitor,
    GLFWwindow *share)
{
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, config.Visible ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, config.Decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, config.Resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, name.c_str());

    const auto window = glfwCreateWindow(
        static_cast<int>(config.Width),
        static_cast<int>(config.Height),
        title.c_str(),
        config.Fullscreen ? monitor : nullptr,
        share);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(GLFW_TRUE);

    if (config.Main)
    {
        if (const auto error = glewContextInit())
        {
            auto message = reinterpret_cast<const char *>(glewGetErrorString(error));
            fxng::Fatal("failed to initialize glew: {}", message);
        }
    }

    glDebugMessageCallback(gl_debug_message_callback, window);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    return window;
}

struct color_t
{
    float r, g, b, a;
};

static constexpr std::array colors
{
    color_t
    {
        .r = 0.2f,
        .g = 0.3f,
        .b = 0.9f,
        .a = 1.0f,
    },
    color_t
    {
        .r = 0.2f,
        .g = 0.9f,
        .b = 0.3f,
        .a = 1.0f,
    },
    color_t
    {
        .r = 0.9f,
        .g = 0.3f,
        .b = 0.2f,
        .a = 1.0f,
    },
};

fxng::Engine::Engine(const EngineConfig &config)
{
    glfwSetErrorCallback(glfw_error_callback);

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    Assert(glfwInit(), "failed to initialize glfw");

    const auto primary_monitor = glfwGetPrimaryMonitor();

    for (auto &entry : config.Windows)
    {
        if (!entry.Main)
            continue;

        m_PrimaryWindow = create_window(entry, config.Application.Name, "Primary", primary_monitor, nullptr);
        Assert(m_PrimaryWindow, "failed to create glfw window");
        break;
    }
    for (auto &entry : config.Windows)
    {
        if (entry.Main)
        {
            m_Windows.push_back(m_PrimaryWindow);
            continue;
        }

        const auto window = create_window(
            entry,
            config.Application.Name,
            "Secondary",
            primary_monitor,
            m_PrimaryWindow);
        Assert(window, "failed to create glfw window");
        m_Windows.push_back(window);
    }

    while (!glfwWindowShouldClose(m_PrimaryWindow))
    {
        glfwPollEvents();

        auto index = 0u;
        auto active = false;
        for (const auto window : m_Windows)
        {
            const auto color_index = index++ % colors.size();

            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
                continue;

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            if (!width || !height)
                continue;

            active = true;

            glfwMakeContextCurrent(window);

            glViewport(0, 0, width, height);
            glClearColor(
                colors[color_index].r,
                colors[color_index].g,
                colors[color_index].b,
                colors[color_index].a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Frame();

            glfwSwapInterval(0);
            glfwSwapBuffers(window);
        }

        if (!active)
            glfwWaitEvents();
    }
}

fxng::Engine::~Engine()
{
    for (const auto window : m_Windows)
        glfwDestroyWindow(window);

    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

void fxng::Engine::Frame()
{
}
