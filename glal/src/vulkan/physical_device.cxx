#include <glal/vulkan.hxx>

glal::vulkan::PhysicalDeviceT::PhysicalDeviceT(InstanceT *instance, const VkPhysicalDevice handle)
    : m_Instance(instance),
      m_Handle(handle)
{
}

glal::Device glal::vulkan::PhysicalDeviceT::CreateDevice()
{
    return m_Devices.emplace_back(new DeviceT(this));
}

void glal::vulkan::PhysicalDeviceT::DestroyDevice(const Device device)
{
    for (auto it = m_Devices.begin(); it != m_Devices.end(); ++it)
        if (*it == device)
        {
            m_Devices.erase(it);
            delete device;
            break;
        }
}

bool glal::vulkan::PhysicalDeviceT::Supports(DeviceFeature feature) const
{
    return true; // TODO
}

const glal::DeviceLimits &glal::vulkan::PhysicalDeviceT::GetLimits() const
{
    static DeviceLimits limits;
    return limits; // TODO
}

VkPhysicalDevice glal::vulkan::PhysicalDeviceT::GetHandle() const
{
    return m_Handle;
}
