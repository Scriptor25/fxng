#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::Image::Image(const Ptr<Device> device, const ImageDesc &desc)
    : m_Device(device),
      m_Format(desc.Format),
      m_Dimension(desc.Dimension),
      m_Extent(desc.Extent),
      m_MipLevelCount(desc.MipLevelCount),
      m_ArrayLayerCount(desc.ArrayLayerCount),
      m_Handle()
{
    GLenum internal_format, external_format, type;
    TranslateImageFormat(m_Format, &internal_format, &external_format, &type);

    switch (m_Dimension)
    {
    case ImageDimension_1D:
        glCreateTextures(GL_TEXTURE_1D, 1, &m_Handle);
        glTextureStorage1D(
            m_Handle,
            static_cast<int>(m_MipLevelCount),
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
            static_cast<int>(m_MipLevelCount),
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
            static_cast<int>(m_MipLevelCount),
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
        common::Fatal("image dimension not supported");
    }
}

glal::opengl::Image::~Image()
{
    glDeleteTextures(1, &m_Handle);
}

glal::ImageFormat glal::opengl::Image::GetFormat() const
{
    return m_Format;
}

glal::ImageDimension glal::opengl::Image::GetDimension() const
{
    return m_Dimension;
}

glal::Extent3D glal::opengl::Image::GetExtent() const
{
    return m_Extent;
}

std::uint32_t glal::opengl::Image::GetMipLevelCount() const
{
    return m_MipLevelCount;
}

std::uint32_t glal::opengl::Image::GetArrayLayerCount() const
{
    return m_ArrayLayerCount;
}

std::uint32_t glal::opengl::Image::GetHandle() const
{
    return m_Handle;
}
