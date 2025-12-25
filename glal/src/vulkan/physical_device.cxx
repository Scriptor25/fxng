#include <glal/vulkan.hxx>

glal::vulkan::PhysicalDeviceT::PhysicalDeviceT(InstanceT *instance, VkPhysicalDevice handle)
    : m_Instance(instance),
      m_Handle(handle)
{
}

glal::Device glal::vulkan::PhysicalDeviceT::CreateDevice()
{
    return m_Devices.emplace_back(new DeviceT(this));
}

void glal::vulkan::PhysicalDeviceT::DestroyDevice(Device device)
{
    for (auto it = m_Devices.begin(); it != m_Devices.end(); ++it)
        if (*it == device)
        {
            m_Devices.erase(it);
            delete device;
            break;
        }
}

glal::Instance glal::vulkan::PhysicalDeviceT::GetInstance() const
{
    return m_Instance;
}

bool glal::vulkan::PhysicalDeviceT::Supports(DeviceFeature feature) const
{
    // TODO: features
    return true;
}

const glal::DeviceLimits &glal::vulkan::PhysicalDeviceT::GetLimits() const
{
    // TODO: limits
    static DeviceLimits limits;
    return limits;
}

VkPhysicalDevice glal::vulkan::PhysicalDeviceT::GetHandle() const
{
    return m_Handle;
}
