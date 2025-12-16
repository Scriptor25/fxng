#include <glal/opengl.hxx>

glal::opengl::ImageView::ImageView(const Ptr<Image> image)
    : m_Image(image)
{
}

glal::Ptr<glal::Image> glal::opengl::ImageView::GetImage() const
{
    return m_Image;
}

std::uint32_t glal::opengl::ImageView::GetHandle() const
{
    return m_Image->GetHandle();
}
