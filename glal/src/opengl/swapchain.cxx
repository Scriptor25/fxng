#define GLFW_INCLUDE_NONE

#include <glal/opengl.hxx>
#include <GLFW/glfw3.h>

glal::opengl::Swapchain::Swapchain(const Ptr<Device> device, const SwapchainDesc &desc)
    : m_Device(device),
      m_Extent(desc.Extent),
      m_ImageCount(desc.ImageCount),
      m_ImageIndex(),
      m_Images(desc.ImageCount),
      m_Fences(desc.ImageCount),
      m_ImageViews(desc.ImageCount),
      m_NativeWindowHandle(desc.NativeWindowHandle)
{
    for (std::uint32_t i = 0; i < m_ImageCount; ++i)
    {
        const auto image = device->CreateImage(
            {
                .Format = desc.Format,
                .Dimension = ImageDimension_2D,
                .Extent = { m_Extent.Width, m_Extent.Height, 0 },
                .MipLevelCount = 1,
                .ArrayLayerCount = 1,
            });
        const auto image_impl = dynamic_cast<Ptr<Image>>(image);
        m_Images[i] = image_impl;
        m_ImageViews[i] = new ImageView(image_impl);
    }
}

glal::opengl::Swapchain::~Swapchain()
{
    for (const auto image_view : m_ImageViews)
        delete image_view;
}

glal::Extent2D glal::opengl::Swapchain::GetExtent() const
{
    return m_Extent;
}

std::uint32_t glal::opengl::Swapchain::GetImageCount() const
{
    return m_ImageCount;
}

glal::Ptr<glal::ImageView> glal::opengl::Swapchain::GetImageView(const std::uint32_t index) const
{
    return m_ImageViews.at(index);
}

std::uint32_t glal::opengl::Swapchain::AcquireNextImage(const Ptr<glal::Fence> fence)
{
    m_ImageIndex = (m_ImageIndex + 1) % m_ImageCount;

    if (const auto in_flight = m_Fences.at(m_ImageIndex))
        in_flight->Wait();

    m_Fences.at(m_ImageIndex) = fence;
    return m_ImageIndex;
}

void glal::opengl::Swapchain::Present() const
{
    const auto image_view = m_ImageViews.at(m_ImageIndex);

    GLuint framebuffer;
    glCreateFramebuffers(1, &framebuffer);
    glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, image_view->GetHandle(), 0);
    glBlitNamedFramebuffer(
        framebuffer,
        0,
        0,
        0,
        m_Extent.Width,
        m_Extent.Height,
        0,
        0,
        m_Extent.Width,
        m_Extent.Height,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST);
    glDeleteFramebuffers(1, &framebuffer);

    glfwSwapBuffers(static_cast<GLFWwindow *>(m_NativeWindowHandle));
}
