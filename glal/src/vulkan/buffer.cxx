#include <common/log.hxx>
#include <glal/vulkan.hxx>

static std::uint32_t find_memory_type_index(
    const VkPhysicalDeviceMemoryProperties &memory_properties,
    const std::uint32_t type_bits,
    const VkMemoryPropertyFlags property_flags)
{
    for (std::uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i)
        if ((type_bits & (1 << i))
            && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags)
            return i;

    common::Fatal("unsupported memory requirements");
}

glal::vulkan::BufferT::BufferT(DeviceT *device, const BufferDesc &desc)
    : m_Device(device),
      m_Size(desc.Size),
      m_Usage(desc.Usage),
      m_Handle(),
      m_MemoryHandle()
{
    VkBufferUsageFlags usage{};
    switch (m_Usage)
    {
    case BufferUsage_Vertex:
        usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        break;
    case BufferUsage_Index:
        usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        break;
    case BufferUsage_Uniform:
        usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        break;
    case BufferUsage_Storage:
        usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        break;
    }

    VkMemoryPropertyFlags memory_property_flags{};
    switch (desc.Memory)
    {
    case MemoryUsage_DeviceLocal:
        memory_property_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        break;
    case MemoryUsage_HostToDevice:
        memory_property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        break;
    case MemoryUsage_DeviceToHost:
        memory_property_flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        break;
    }

    const VkBufferCreateInfo buffer_create_info
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = m_Size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_CONCURRENT,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
    };
    vkCreateBuffer(device->GetHandle(), &buffer_create_info, nullptr, &m_Handle);

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(m_Device->GetHandle(), m_Handle, &memory_requirements);

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(device->GetPhysicalDevice()->GetHandle(), &memory_properties);

    const VkMemoryAllocateInfo memory_allocate_info
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memory_requirements.size,
        .memoryTypeIndex = find_memory_type_index(
            memory_properties,
            memory_requirements.memoryTypeBits,
            memory_property_flags),
    };
    vkAllocateMemory(device->GetHandle(), &memory_allocate_info, nullptr, &m_MemoryHandle);
}

std::size_t glal::vulkan::BufferT::GetSize() const
{
    return m_Size;
}

glal::BufferUsage glal::vulkan::BufferT::GetUsage() const
{
    return m_Usage;
}

void *glal::vulkan::BufferT::Map()
{
    void *data;
    vkMapMemory(m_Device->GetHandle(), m_MemoryHandle, 0, m_Size, 0, &data);
    return data;
}

void glal::vulkan::BufferT::Unmap()
{
    vkUnmapMemory(m_Device->GetHandle(), m_MemoryHandle);
}

VkBuffer glal::vulkan::BufferT::GetHandle() const
{
    return m_Handle;
}
