#include <glal/vulkan.hxx>

glal::vulkan::ImageT::ImageT(DeviceT *device, const ImageDesc &desc)
    : m_Device(device),
      m_Format(desc.Format),
      m_Type(desc.Type),
      m_Extent(desc.Extent),
      m_MipLevelCount(desc.MipLevelCount),
      m_ArrayLayerCount(desc.ArrayLayerCount)
{
    VkImageType image_type{};
    switch (m_Type)
    {
    case ImageType_1D:
        image_type = VK_IMAGE_TYPE_1D;
        break;
    case ImageType_2D:
        image_type = VK_IMAGE_TYPE_2D;
        break;
    case ImageType_3D:
        image_type = VK_IMAGE_TYPE_3D;
        break;
    }

    VkFormat format{};
    switch (m_Format)
    {
    case ImageFormat_RGBA8_UNorm:
        format = VK_FORMAT_R8G8B8A8_UNORM;
        break;
    case ImageFormat_RGBA8_SRGB:
        format = VK_FORMAT_R8G8B8A8_SRGB;
        break;
    case ImageFormat_BGRA8_UNorm:
        format = VK_FORMAT_B8G8R8A8_UNORM;
        break;
    case ImageFormat_RG16F:
        format = VK_FORMAT_R16G16_SFLOAT;
        break;
    case ImageFormat_RGBA16F:
        format = VK_FORMAT_R16G16B16A16_SFLOAT;
        break;
    case ImageFormat_RGBA32F:
        format = VK_FORMAT_R32G32B32A32_SFLOAT;
        break;
    case ImageFormat_D24S8:
        format = VK_FORMAT_D24_UNORM_S8_UINT;
        break;
    case ImageFormat_D32F:
        format = VK_FORMAT_D32_SFLOAT;
        break;
    }

    VkImageUsageFlags usage{}; // TODO
    VkImageLayout layout{};    // TODO

    const VkImageCreateInfo image_create_info
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = image_type,
        .format = format,
        .extent = { .width = m_Extent.Width, .height = m_Extent.Height, .depth = m_Extent.Depth },
        .mipLevels = m_MipLevelCount,
        .arrayLayers = m_ArrayLayerCount,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_CONCURRENT,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = layout,
    };
    vkCreateImage(device->GetHandle(), &image_create_info, nullptr, &m_Handle);
}

glal::ImageFormat glal::vulkan::ImageT::GetFormat() const
{
    return m_Format;
}

glal::ImageType glal::vulkan::ImageT::GetType() const
{
    return m_Type;
}

glal::Extent3D glal::vulkan::ImageT::GetExtent() const
{
    return m_Extent;
}

std::uint32_t glal::vulkan::ImageT::GetMipLevelCount() const
{
    return m_MipLevelCount;
}

std::uint32_t glal::vulkan::ImageT::GetArrayLayerCount() const
{
    return m_ArrayLayerCount;
}

VkImage glal::vulkan::ImageT::GetHandle() const
{
    return m_Handle;
}
