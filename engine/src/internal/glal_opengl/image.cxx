#include <fxng/internal/glal_opengl.hxx>
#include <GL/glew.h>

fxng::glal::opengl::Image::Image(Device *device, const ImageDesc &desc)
    : m_Device(device),
      m_Format(desc.Format),
      m_Extent(desc.Extent),
      m_MipLevels(desc.MipLevels),
      m_Handle()
{
    GLenum internal_format, external_format, type;
    TranslateFormat(m_Format, &internal_format, &external_format, &type);

    glCreateTextures(GL_TEXTURE_3D, 1, &m_Handle);
    glTextureStorage3D(
        m_Handle,
        static_cast<int>(m_MipLevels),
        internal_format,
        m_Extent.Width,
        m_Extent.Height,
        m_Extent.Depth);
    glTextureSubImage3D(
        m_Handle,
        0,
        0,
        0,
        0,
        m_Extent.Width,
        m_Extent.Height,
        m_Extent.Depth,
        external_format,
        type,
        nullptr);
}

fxng::glal::opengl::Image::~Image()
{
    glDeleteTextures(1, &m_Handle);
}

fxng::glal::ImageFormat fxng::glal::opengl::Image::GetFormat() const
{
    return m_Format;
}

fxng::glal::Extent3D fxng::glal::opengl::Image::GetExtent() const
{
    return m_Extent;
}

std::uint32_t fxng::glal::opengl::Image::GetMipLevels() const
{
    return m_MipLevels;
}

std::uint32_t fxng::glal::opengl::Image::GetHandle() const
{
    return m_Handle;
}
