#include <glal/opengl.hxx>

glal::opengl::ImageViewT::ImageViewT(DeviceT *device, const ImageViewDesc &desc)
    : m_Device(device),
      m_Image(dynamic_cast<ImageT *>(desc.ImageResource)),
      m_Format(desc.Format),
      m_Type(desc.Type)
{
}

glal::Image glal::opengl::ImageViewT::GetImage() const
{
    return m_Image;
}

glal::ImageFormat glal::opengl::ImageViewT::GetFormat() const
{
    return m_Format;
}

glal::ImageType glal::opengl::ImageViewT::GetType() const
{
    return m_Type;
}

std::uint32_t glal::opengl::ImageViewT::GetImageHandle() const
{
    return m_Image->GetHandle();
}
