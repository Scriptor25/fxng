#include <glal/opengl.hxx>

glal::opengl::Queue::Queue(const Ptr<Device> device)
    : m_Device(device)
{
}

void glal::opengl::Queue::Submit(
    Ptr<glal::CommandBuffer> command_buffer,
    const std::uint32_t command_buffer_count,
    const Ptr<glal::Fence> fence)
{
    // noop

    if (fence)
        fence->Reset();
}

void glal::opengl::Queue::Present(const Ptr<glal::Swapchain> swapchain)
{
    swapchain->Present();
}
