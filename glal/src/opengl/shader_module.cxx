#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::ShaderModule::ShaderModule(const Ptr<Device> device, const ShaderModuleDesc &desc)
    : m_Device(device),
      m_Stage(desc.Stage)
{
    GLenum type;
    switch (m_Stage)
    {
    case ShaderStage_Vertex:
        type = GL_VERTEX_SHADER;
        break;
    case ShaderStage_Geometry:
        type = GL_GEOMETRY_SHADER;
        break;
    case ShaderStage_TessellationControl:
        type = GL_TESS_CONTROL_SHADER;
        break;
    case ShaderStage_TessellationEvaluation:
        type = GL_TESS_EVALUATION_SHADER;
        break;
    case ShaderStage_Fragment:
        type = GL_FRAGMENT_SHADER;
        break;
    case ShaderStage_Compute:
        type = GL_COMPUTE_SHADER;
        break;
    default:
        common::Fatal("shader stage not supported");
    }

    m_Handle = glCreateShader(type);
    glShaderBinary(1, &m_Handle, GL_SHADER_BINARY_FORMAT_SPIR_V, desc.Code, static_cast<GLsizei>(desc.Size));
    glSpecializeShader(m_Handle, "main", 0, nullptr, nullptr);
}

glal::opengl::ShaderModule::~ShaderModule()
{
    glDeleteShader(m_Handle);
}

glal::ShaderStage glal::opengl::ShaderModule::GetStage() const
{
    return m_Stage;
}

std::uint32_t glal::opengl::ShaderModule::GetHandle() const
{
    return m_Handle;
}
