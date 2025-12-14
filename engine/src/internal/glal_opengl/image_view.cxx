#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::ImageView::ImageView(Image *image)
    : m_Image(image)
{
}

fxng::glal::opengl::ImageView::~ImageView()
{
}

const fxng::glal::Image *fxng::glal::opengl::ImageView::GetImage() const
{
    return m_Image;
}

std::uint32_t fxng::glal::opengl::ImageView::GetHandle() const
{
    return m_Image->GetHandle();
}
