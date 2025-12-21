#include <glal/vulkan.hxx>

glal::vulkan::FenceT::FenceT(DeviceT *device)
    : m_Device(device)
{
    const VkFenceCreateInfo fence_create_info
    {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    };
    vkCreateFence(m_Device->GetHandle(), &fence_create_info, nullptr, &m_Handle);
}

glal::vulkan::FenceT::~FenceT()
{
    vkDestroyFence(m_Device->GetHandle(), m_Handle, nullptr);
}

void glal::vulkan::FenceT::Wait()
{
    vkWaitForFences(m_Device->GetHandle(), 1, &m_Handle, true, UINT64_MAX);
}

void glal::vulkan::FenceT::Reset()
{
    vkResetFences(m_Device->GetHandle(), 1, &m_Handle);
}
