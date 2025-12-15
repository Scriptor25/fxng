#include <fxng/log.hxx>
#include <fxng/internal/glal_opengl.hxx>
#include <GL/glew.h>

fxng::glal::opengl::Pipeline::Pipeline(Device *device, const PipelineDesc &desc)
    : m_Device(device),
      m_Type(desc.Type),
      m_VertexAttributes(desc.VertexAttributeCount),
      m_VertexStride()
{
    // TODO: depth and blend
    (void) desc.DepthTest;
    (void) desc.DepthWrite;
    (void) desc.BlendEnable;

    for (std::uint32_t i = 0; i < desc.VertexAttributeCount; ++i)
    {
        const auto vertex_attribute = desc.VertexAttributes + i;
        m_VertexAttributes[i] = *vertex_attribute;

        std::uint32_t size;
        TranslateDataType(vertex_attribute->Type, &size, nullptr, nullptr);

        m_VertexStride += size * vertex_attribute->Count;
    }

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

    glUniformBlockBinding(m_Handle, 0, 0);
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

void fxng::glal::opengl::Pipeline::LayoutVertexArray(const std::uint32_t vertex_array) const
{
    for (auto &vertex_attribute : m_VertexAttributes)
    {
        GLenum type;
        GLboolean normalized;

        TranslateDataType(vertex_attribute.Type, nullptr, &type, &normalized);

        glEnableVertexArrayAttrib(vertex_array, vertex_attribute.Location);
        glVertexArrayAttribBinding(vertex_array, vertex_attribute.Location, vertex_attribute.Binding);
        glVertexArrayAttribFormat(
            vertex_array,
            vertex_attribute.Location,
            static_cast<GLint>(vertex_attribute.Count),
            type,
            normalized,
            vertex_attribute.Offset);
    }
}

std::uint32_t fxng::glal::opengl::Pipeline::GetHandle() const
{
    return m_Handle;
}
