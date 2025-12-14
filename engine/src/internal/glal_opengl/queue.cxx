#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::Queue::Queue(Device *device)
    : m_Device(device)
{
}

void fxng::glal::opengl::Queue::Submit(
    const glal::CommandBuffer *command_buffer,
    const std::uint32_t command_buffer_count,
    glal::Fence *fence)
{
    // noop

    if (fence)
        fence->Reset();
}

void fxng::glal::opengl::Queue::Present(const glal::Swapchain *swapchain)
{
    swapchain->Present();
}
