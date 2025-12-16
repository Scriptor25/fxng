#define GLFW_INCLUDE_NONE

#include <filesystem>
#include <fstream>
#include <common/log.hxx>
#include <fxng/engine.hxx>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <yaml-cpp/yaml.h>

static void glfw_error_callback(int error_code, const char *description)
{
    common::Log(common::LogLevel_Info, "[glfw {:#08x}] {}", error_code, description);
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

    common::Log(common::LogLevel_Info, "[gl {:#08x}] {}", id, message);
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
    glfwSwapInterval(GLFW_FALSE);

    if (config.Main)
    {
        if (const auto error = glewInit())
        {
            auto message = reinterpret_cast<const char *>(glewGetErrorString(error));
            common::Fatal("failed to initialize glew: {}", message);
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
    IndexAssets();

    glfwSetErrorCallback(glfw_error_callback);

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    common::Assert(glfwInit(), "failed to initialize glfw");

    const auto primary_monitor = glfwGetPrimaryMonitor();

    for (auto &entry : config.Windows)
    {
        if (!entry.Main)
            continue;

        m_PrimaryWindow = create_window(entry, config.Application.Name, "Primary", primary_monitor, nullptr);
        common::Assert(m_PrimaryWindow, "failed to create glfw window");
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
        common::Assert(window, "failed to create glfw window");
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

fxng::Scene &fxng::Engine::GetScene()
{
    return m_Scene;
}

void fxng::Engine::InitScene()
{
    m_Scene.OnInit();
}

void fxng::Engine::ExitScene()
{
    m_Scene.OnExit();
}

void fxng::Engine::ClearScene()
{
    m_Scene.Clear();
}

void fxng::Engine::IndexAssets()
{
#ifdef FXNG_PACKAGE

    Fatal("TODO");

#else

    IndexYaml("engine/assets");
    IndexYaml("game/assets");

#endif
}

void fxng::Engine::IndexYaml(std::filesystem::path path)
{
    if (is_directory(path))
        path = path / "index.yaml";

    std::ifstream stream(path);
    common::Assert(stream.is_open(), "failed to open {}", path);

    auto root = YAML::Load(stream);
    auto type = root["type"].as<std::string>();

    if (type == "index")
    {
        for (auto index = root["index"].as<std::vector<std::string>>(); auto &filename : index)
        {
            auto subpath = path.parent_path() / filename;
            if (is_directory(subpath))
                subpath = subpath / "index.yaml";
            IndexYaml(subpath);
        }
        return;
    }

    if (type == "shader")
    {
        auto id = root["id"].as<std::string>();
        auto name = root["name"].as<std::string>();
        auto source = root["source"].as<std::string>();

        common::Log(common::LogLevel_Info, "index shader id={} name={} source={}", id, name, source);

        for (auto node : root["input"])
        {
            auto input_name = node["name"].as<std::string>();
            auto input_type = node["type"].as<std::string>();
            auto input_reference = node["reference"].as<std::optional<std::string>>();

            common::Log(
                common::LogLevel_Info,
                "  input name={} type={} reference={}",
                input_name,
                input_type,
                input_reference);
        }

        for (auto node : root["output"])
        {
            auto output_name = node["name"].as<std::string>();
            auto output_type = node["type"].as<std::string>();
            auto output_reference = node["reference"].as<std::optional<std::string>>();

            common::Log(
                common::LogLevel_Info,
                "  output name={} type={} reference={}",
                output_name,
                output_type,
                output_reference);
        }

        for (auto node : root["uniform"])
        {
            auto uniform_name = node["name"].as<std::string>();
            auto uniform_type = node["type"].as<std::string>();
            auto uniform_reference = node["reference"].as<std::optional<std::string>>();

            common::Log(
                common::LogLevel_Info,
                "  uniform name={} type={} reference={}",
                uniform_name,
                uniform_type,
                uniform_reference);
        }

        return;
    }

    if (type == "material")
    {
        auto id = root["id"].as<std::string>();
        auto name = root["name"].as<std::string>();
        auto stages = root["stages"].as<std::map<std::string, std::string>>();

        common::Log(common::LogLevel_Info, "index material id={} name={}", id, name);

        for (auto stage : stages)
        {
            common::Log(common::LogLevel_Info, "  stage key={} value={}", stage.first, stage.second);
        }

        return;
    }

    if (type == "mesh")
    {
        auto id = root["id"].as<std::string>();
        auto name = root["name"].as<std::string>();
        auto source = root["source"].as<std::string>();

        common::Log(common::LogLevel_Info, "index mesh id={} name={} source={}", id, name, source);

        return;
    }

    if (type == "scene")
    {
        auto id = root["id"].as<std::string>();
        auto name = root["name"].as<std::string>();

        common::Log(common::LogLevel_Info, "index scene id={} name={}", id, name);

        for (auto node : root["entities"])
        {
            auto entity_id = node["id"].as<std::string>();
            auto entity_name = node["name"].as<std::string>();

            common::Log(common::LogLevel_Info, "  entity id={} name={}", entity_id, entity_name);

            for (auto component_node : node["components"])
            {
                auto component_type = component_node["type"].as<std::string>();

                common::Log(common::LogLevel_Info, "    component type={}", component_type);
            }
        }

        return;
    }

    common::Log(common::LogLevel_Error, "invalid yaml file type {}", type);
}

void fxng::Engine::Frame()
{
    m_Scene.PreFrame();
    m_Scene.OnFrame();
    m_Scene.PostFrame();
}
