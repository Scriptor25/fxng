#include <glal/opengl.hxx>

glal::opengl::QueueT::QueueT(DeviceT *device)
    : m_Device(device)
{
}

void glal::opengl::QueueT::Submit(
    const CommandBuffer *command_buffers,
    const std::uint32_t command_buffer_count,
    Fence fence)
{
    // noop

    if (fence)
        fence->Reset();
}

void glal::opengl::QueueT::Present(Swapchain swapchain)
{
    swapchain->Present();
}
