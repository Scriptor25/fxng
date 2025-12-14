#include <fxng/log.hxx>
#include <fxng/internal/glal_opengl.hxx>

static void gl_debug_message_callback(
    const GLenum source,
    const GLenum type,
    const GLuint id,
    const GLenum severity,
    const GLsizei /* length */,
    const GLchar *message,
    const void * /* user_param */)
{
    fxng::LogLevel log_level;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        log_level = fxng::LogLevel_Verbose;
        break;
    case GL_DEBUG_SEVERITY_LOW:
        log_level = fxng::LogLevel_Info;
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        log_level = fxng::LogLevel_Warning;
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        log_level = fxng::LogLevel_Error;
        break;
    default:
        log_level = fxng::LogLevel_Debug;
        break;
    }

    const char *source_string;
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        source_string = "api";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_string = "application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_string = "other";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_string = "shader compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_string = "third party";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_string = "window system";
        break;
    default:
        source_string = "unknown";
        break;
    }

    const char *type_string;
    switch (type)
    {
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_string = "deprecated behavior";
        break;
    case GL_DEBUG_TYPE_ERROR:
        type_string = "error";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_string = "marker";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_string = "other";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_string = "performance";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        type_string = "pop group";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_string = "portability";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        type_string = "push group";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_string = "undefined behavior";
        break;
    default:
        type_string = "unknown";
        break;
    }

    Log(log_level, "[GL] {} {} {}: {}", source_string, type_string, id, message);
}

fxng::glal::opengl::Instance::Instance(const InstanceDesc &desc)
{
    glewContextInit();

    if (desc.EnableValidation)
    {
        glDebugMessageCallback(gl_debug_message_callback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    m_PhysicalDevices = new PhysicalDevice[1]{ PhysicalDevice(this) };
    m_PhysicalDeviceCount = 1;
}

fxng::glal::opengl::Instance::~Instance()
{
    delete[] m_PhysicalDevices;

    m_PhysicalDevices = nullptr;
    m_PhysicalDeviceCount = 0;
}

std::uint32_t fxng::glal::opengl::Instance::EnumeratePhysicalDevices(glal::PhysicalDevice **devices)
{
    if (devices)
        *devices = m_PhysicalDevices;
    return m_PhysicalDeviceCount;
}
