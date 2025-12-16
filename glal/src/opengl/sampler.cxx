#include <glal/opengl.hxx>

glal::opengl::SamplerT::SamplerT(DeviceT *device, const SamplerDesc &desc)
    : m_Device(device),
      m_Handle()
{
    glCreateSamplers(1, &m_Handle);

    GLint min_filter, mag_filter, wrap_s, wrap_t, wrap_r;

    switch (desc.MinFilter)
    {
    case Filter_Nearest:
        min_filter = GL_NEAREST;
        break;
    case Filter_Linear:
        min_filter = GL_LINEAR;
        break;
    default:
        min_filter = 0;
        break;
    }

    switch (desc.MagFilter)
    {
    case Filter_Nearest:
        mag_filter = GL_NEAREST;
        break;
    case Filter_Linear:
        mag_filter = GL_LINEAR;
        break;
    default:
        mag_filter = 0;
        break;
    }

    switch (desc.AddressU)
    {
    case AddressMode_Repeat:
        wrap_s = GL_REPEAT;
        break;
    case AddressMode_Clamp:
        wrap_s = GL_CLAMP_TO_EDGE;
        break;
    case AddressMode_Mirror:
        wrap_s = GL_MIRRORED_REPEAT;
        break;
    default:
        wrap_s = 0;
        break;
    }

    switch (desc.AddressV)
    {
    case AddressMode_Repeat:
        wrap_t = GL_REPEAT;
        break;
    case AddressMode_Clamp:
        wrap_t = GL_CLAMP_TO_EDGE;
        break;
    case AddressMode_Mirror:
        wrap_t = GL_MIRRORED_REPEAT;
        break;
    default:
        wrap_t = 0;
        break;
    }

    switch (desc.AddressW)
    {
    case AddressMode_Repeat:
        wrap_r = GL_REPEAT;
        break;
    case AddressMode_Clamp:
        wrap_r = GL_CLAMP_TO_EDGE;
        break;
    case AddressMode_Mirror:
        wrap_r = GL_MIRRORED_REPEAT;
        break;
    default:
        wrap_r = 0;
        break;
    }

    glSamplerParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, min_filter);
    glSamplerParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, mag_filter);
    glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_S, wrap_s);
    glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_T, wrap_t);
    glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_R, wrap_r);
}

glal::opengl::SamplerT::~SamplerT()
{
    glDeleteSamplers(1, &m_Handle);
}

std::uint32_t glal::opengl::SamplerT::GetHandle() const
{
    return m_Handle;
}
