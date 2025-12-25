#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::PipelineT::PipelineT(DeviceT *device, const PipelineDesc &desc)
    : m_Device(device),
      m_Layout(dynamic_cast<PipelineLayoutT *>(desc.Layout)),
      m_Type(desc.Type),
      m_Topology(desc.Topology),
      m_VertexBindings(desc.VertexBindings, desc.VertexBindings + desc.VertexBindingCount),
      m_VertexAttributes(desc.VertexAttributes, desc.VertexAttributes + desc.VertexAttributeCount)
{
    // TODO
    (void) desc.DepthTest;
    (void) desc.DepthWrite;
    (void) desc.BlendEnable;
    (void) desc.PrimitiveRestartEnable;

    m_Handle = glCreateProgram();

    for (std::uint32_t i = 0; i < desc.StageCount; ++i)
    {
        const auto stage = desc.Stages + i;
        const auto shader_module_impl = dynamic_cast<ShaderModuleT *>(stage->Module);

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

        common::Fatal("failed to link program: {}", message);
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

        common::Fatal("failed to validate program: {}", message);
    }
}

glal::opengl::PipelineT::~PipelineT()
{
    glDeleteProgram(m_Handle);
}

glal::PipelineType glal::opengl::PipelineT::GetType() const
{
    return m_Type;
}

glal::PrimitiveTopology glal::opengl::PipelineT::GetTopology() const
{
    return m_Topology;
}

void glal::opengl::PipelineT::BindVertexArray(GLuint vertex_array) const
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

void glal::opengl::PipelineT::BindVertexBuffer(
    GLuint vertex_array,
    GLuint buffer,
    const std::uint32_t binding,
    const std::uint32_t offset) const
{
    for (auto &vertex_binding : m_VertexBindings)
        if (vertex_binding.Binding == binding)
        {
            glVertexArrayVertexBuffer(
                vertex_array,
                vertex_binding.Binding,
                buffer,
                static_cast<GLintptr>(offset),
                static_cast<GLsizei>(vertex_binding.Stride));
            break;
        }
}

GLuint glal::opengl::PipelineT::GetHandle() const
{
    return m_Handle;
}
