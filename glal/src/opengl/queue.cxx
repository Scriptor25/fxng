#include <glal/opengl.hxx>

glal::opengl::QueueT::QueueT(DeviceT *device)
    : m_Device(device)
{
}

void glal::opengl::QueueT::Submit(
    CommandBuffer command_buffer,
    const std::uint32_t command_buffer_count,
    const Fence fence)
{
    // noop

    if (fence)
        fence->Reset();
}

void glal::opengl::QueueT::Present(const Swapchain swapchain)
{
    swapchain->Present();
}
