#include <common/log.hxx>
#include <glal/vulkan.hxx>

glal::vulkan::InstanceT::InstanceT(const InstanceDesc &desc)
    : m_Handle()
{
    const VkApplicationInfo application_info
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = desc.ApplicationName,
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "GLAL",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_1,
    };

    const VkInstanceCreateInfo instance_create_info
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
    };

    const auto result = vkCreateInstance(&instance_create_info, nullptr, &m_Handle);
    common::Assert(result == VK_SUCCESS, "failed to create vulkan instance");

    std::uint32_t count;
    vkEnumeratePhysicalDevices(m_Handle, &count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(count);
    vkEnumeratePhysicalDevices(m_Handle, &count, physical_devices.data());

    for (const auto physical_device : physical_devices)
        m_PhysicalDevices.push_back(PhysicalDeviceT(this, physical_device));
}

glal::vulkan::InstanceT::~InstanceT()
{
    vkDestroyInstance(m_Handle, nullptr);
}

std::uint32_t glal::vulkan::InstanceT::EnumeratePhysicalDevices(PhysicalDevice *devices)
{
    if (devices)
        *devices = m_PhysicalDevices.data();
    return m_PhysicalDevices.size();
}
