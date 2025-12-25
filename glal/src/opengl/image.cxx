#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::ImageT::ImageT(DeviceT *device, const ImageDesc &desc)
    : m_Device(device),
      m_Format(desc.Format),
      m_Type(desc.Type),
      m_Extent(desc.Extent),
      m_MipLevelCount(desc.MipLevelCount),
      m_ArrayLayerCount(desc.ArrayLayerCount),
      m_Handle()
{
    GLenum internal_format, external_format, type;
    TranslateImageFormat(m_Format, &internal_format, &external_format, &type);

    switch (m_Type)
    {
    case ImageType_1D:
        glCreateTextures(GL_TEXTURE_1D, 1, &m_Handle);
        glTextureStorage1D(
            m_Handle,
            static_cast<int>(m_MipLevelCount),
            internal_format,
            static_cast<GLsizei>(m_Extent.Width));
        glTextureSubImage1D(
            m_Handle,
            0,
            0,
            static_cast<GLsizei>(m_Extent.Width),
            external_format,
            type,
            nullptr);
        break;
    case ImageType_2D:
        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        glTextureStorage2D(
            m_Handle,
            static_cast<int>(m_MipLevelCount),
            internal_format,
            static_cast<GLsizei>(m_Extent.Width),
            static_cast<GLsizei>(m_Extent.Height));
        glTextureSubImage2D(
            m_Handle,
            0,
            0,
            0,
            static_cast<GLsizei>(m_Extent.Width),
            static_cast<GLsizei>(m_Extent.Height),
            external_format,
            type,
            nullptr);
        break;
    case ImageType_3D:
        glCreateTextures(GL_TEXTURE_3D, 1, &m_Handle);
        glTextureStorage3D(
            m_Handle,
            static_cast<int>(m_MipLevelCount),
            internal_format,
            static_cast<GLsizei>(m_Extent.Width),
            static_cast<GLsizei>(m_Extent.Height),
            static_cast<GLsizei>(m_Extent.Depth));
        glTextureSubImage3D(
            m_Handle,
            0,
            0,
            0,
            0,
            static_cast<GLsizei>(m_Extent.Width),
            static_cast<GLsizei>(m_Extent.Height),
            static_cast<GLsizei>(m_Extent.Depth),
            external_format,
            type,
            nullptr);
        break;
    }
}

glal::opengl::ImageT::~ImageT()
{
    glDeleteTextures(1, &m_Handle);
}

glal::ImageFormat glal::opengl::ImageT::GetFormat() const
{
    return m_Format;
}

glal::ImageType glal::opengl::ImageT::GetType() const
{
    return m_Type;
}

glal::Extent3D glal::opengl::ImageT::GetExtent() const
{
    return m_Extent;
}

std::uint32_t glal::opengl::ImageT::GetMipLevelCount() const
{
    return m_MipLevelCount;
}

std::uint32_t glal::opengl::ImageT::GetArrayLayerCount() const
{
    return m_ArrayLayerCount;
}

GLuint glal::opengl::ImageT::GetHandle() const
{
    return m_Handle;
}
