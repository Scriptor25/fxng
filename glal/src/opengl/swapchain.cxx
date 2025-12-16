#define GLFW_INCLUDE_NONE

#include <glal/opengl.hxx>
#include <GLFW/glfw3.h>

glal::opengl::SwapchainT::SwapchainT(DeviceT *device, const SwapchainDesc &desc)
    : m_Device(device),
      m_Extent(desc.Extent),
      m_FrameCount(desc.ImageCount),
      m_FrameIndex(),
      m_Frames(desc.ImageCount),
      m_NativeWindowHandle(desc.NativeWindowHandle)
{
    for (std::uint32_t i = 0; i < m_FrameCount; ++i)
    {
        const auto image = device->CreateImage(
            {
                .Format = desc.Format,
                .Type = ImageType_2D,
                .Extent = { m_Extent.Width, m_Extent.Height, 0 },
                .MipLevelCount = 1,
                .ArrayLayerCount = 1,
            });
        const auto image_view = device->CreateImageView(
            {
                .Format = desc.Format,
                .Type = ImageType_2D,
                .ImageResource = image,
            });
        m_Frames[i] = {
            .ImageRef = dynamic_cast<ImageT *>(image),
            .ImageViewRef = dynamic_cast<ImageViewT *>(image_view),
            .FenceRef = nullptr,
        };
    }
}

glal::opengl::SwapchainT::~SwapchainT()
{
    for (const auto frame : m_Frames)
    {
        m_Device->DestroyImageView(frame.ImageViewRef);
        m_Device->DestroyImage(frame.ImageRef);
    }
}

glal::Extent2D glal::opengl::SwapchainT::GetExtent() const
{
    return m_Extent;
}

std::uint32_t glal::opengl::SwapchainT::GetImageCount() const
{
    return m_FrameCount;
}

glal::ImageView glal::opengl::SwapchainT::GetImageView(const std::uint32_t index) const
{
    return m_Frames.at(index).ImageViewRef;
}

std::uint32_t glal::opengl::SwapchainT::AcquireNextImage(const Fence fence)
{
    m_FrameIndex = (m_FrameIndex + 1) % m_FrameCount;

    if (const auto in_flight = m_Frames.at(m_FrameIndex).FenceRef)
        in_flight->Wait();

    m_Frames.at(m_FrameIndex).FenceRef = dynamic_cast<FenceT *>(fence);
    return m_FrameIndex;
}

void glal::opengl::SwapchainT::Present() const
{
    const auto image = m_Frames.at(m_FrameIndex).ImageRef;

    GLuint framebuffer;
    glCreateFramebuffers(1, &framebuffer);
    glNamedFramebufferTexture(
        framebuffer,
        GL_COLOR_ATTACHMENT0,
        image->GetHandle(),
        0);
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
