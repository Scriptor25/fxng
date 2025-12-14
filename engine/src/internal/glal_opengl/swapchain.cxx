#define GLFW_INCLUDE_NONE

#include <fxng/internal/glal_opengl.hxx>
#include <GLFW/glfw3.h>

fxng::glal::opengl::Swapchain::Swapchain(Device *device, const SwapchainDesc &desc)
    : m_Device(device),
      m_Extent(desc.Extent),
      m_ImageCount(desc.ImageCount),
      m_Images(desc.ImageCount),
      m_ImageViews(desc.ImageCount),
      m_NativeWindowHandle(desc.NativeWindowHandle)
{
    for (std::uint32_t i = 0; i < m_ImageCount; ++i)
    {
        m_Images[i] = new Image(
            device,
            {
                .Format = desc.Format,
                .Extent = { m_Extent.Width, m_Extent.Height, 1 },
                .MipLevels = 1,
                .ArrayLayers = 1,
            });
        m_ImageViews[i] = new ImageView(m_Images[i]);
    }
}

fxng::glal::opengl::Swapchain::~Swapchain()
{
    for (const auto image_view : m_ImageViews)
        delete image_view;
    for (const auto image : m_Images)
        delete image;
}

fxng::glal::Extent2D fxng::glal::opengl::Swapchain::GetExtent() const
{
    return m_Extent;
}

std::uint32_t fxng::glal::opengl::Swapchain::GetImageCount() const
{
    return m_ImageCount;
}

fxng::glal::ImageView *fxng::glal::opengl::Swapchain::GetImageView(const std::uint32_t index) const
{
    return m_ImageViews.at(index);
}

std::uint32_t fxng::glal::opengl::Swapchain::AcquireNextImage(glal::Fence *fence)
{
    return 0;
}

void fxng::glal::opengl::Swapchain::Present() const
{
    glfwSwapBuffers(static_cast<GLFWwindow *>(m_NativeWindowHandle));
}
