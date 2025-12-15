#include <fxng/log.hxx>
#include <fxng/internal/glal_opengl.hxx>
#include <GL/glew.h>

fxng::glal::opengl::Image::Image(Device *device, const ImageDesc &desc)
    : m_Device(device),
      m_Format(desc.Format),
      m_Extent(desc.Extent),
      m_MipLevels(desc.MipLevelCount),
      m_Handle()
{
    GLenum internal_format, external_format, type;
    TranslateImageFormat(m_Format, &internal_format, &external_format, &type);

    switch (desc.Dimension)
    {
    case ImageDimension_1D:
        glCreateTextures(GL_TEXTURE_1D, 1, &m_Handle);
        glTextureStorage1D(
            m_Handle,
            static_cast<int>(m_MipLevels),
            internal_format,
            m_Extent.Width);
        glTextureSubImage1D(
            m_Handle,
            0,
            0,
            m_Extent.Width,
            external_format,
            type,
            nullptr);
        break;
    case ImageDimension_2D:
        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        glTextureStorage2D(
            m_Handle,
            static_cast<int>(m_MipLevels),
            internal_format,
            m_Extent.Width,
            m_Extent.Height);
        glTextureSubImage2D(
            m_Handle,
            0,
            0,
            0,
            m_Extent.Width,
            m_Extent.Height,
            external_format,
            type,
            nullptr);
        break;
    case ImageDimension_3D:
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
        break;
    default:
        Fatal("image dimension not supported");
    }
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
