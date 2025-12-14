#include <fxng/log.hxx>
#include <fxng/internal/glal_opengl.hxx>
#include <GL/glew.h>

fxng::glal::opengl::Pipeline::Pipeline(Device *device, const PipelineDesc &desc)
    : m_Device(device),
      m_Type(desc.Type),
      m_VertexStride(5 * sizeof(GLfloat)) // TODO: vertex stride
{
    // TODO: depth and blend
    (void) desc.DepthTest;
    (void) desc.DepthWrite;
    (void) desc.BlendEnable;

    m_Handle = glCreateProgram();

    for (std::uint32_t i = 0; i < desc.StageCount; ++i)
    {
        const auto stage = desc.Stages + i;
        const auto shader_module_impl = dynamic_cast<const ShaderModule *>(stage->Module);

        glAttachShader(m_Handle, shader_module_impl->GetHandle());
    }

    glLinkProgram(m_Handle);

    GLint link_status;
    glGetProgramiv(m_Handle, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLsizei log_length;
        GLchar message[1024];
        glGetProgramInfoLog(m_Handle, 1024, &log_length, message);
        message[log_length] = 0;

        Fatal("failed to link program: {}", message);
    }

    glValidateProgram(m_Handle);

    GLint validate_status;
    glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &validate_status);
    if (validate_status != GL_TRUE)
    {
        GLsizei log_length;
        GLchar message[1024];
        glGetProgramInfoLog(m_Handle, 1024, &log_length, message);
        message[log_length] = 0;

        Fatal("failed to validate program: {}", message);
    }
}

fxng::glal::opengl::Pipeline::~Pipeline()
{
    glDeleteProgram(m_Handle);
}

fxng::glal::PipelineType fxng::glal::opengl::Pipeline::GetType() const
{
    return m_Type;
}

std::uint32_t fxng::glal::opengl::Pipeline::GetVertexStride() const
{
    return m_VertexStride;
}

std::uint32_t fxng::glal::opengl::Pipeline::GetHandle() const
{
    return m_Handle;
}
