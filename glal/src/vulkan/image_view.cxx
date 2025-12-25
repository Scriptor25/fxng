#include <glal/vulkan.hxx>

glal::vulkan::ImageViewT::ImageViewT(DeviceT *device, const ImageViewDesc &desc)
    : m_Device(device),
      m_Image(dynamic_cast<ImageT *>(desc.ImageResource)),
      m_Format(desc.Format),
      m_Type(desc.Type),
      m_Handle()
{
    VkImageViewType view_type{};
    switch (desc.Type)
    {
    case ImageType_1D:
        view_type = VK_IMAGE_VIEW_TYPE_1D;
        break;
    case ImageType_2D:
        view_type = VK_IMAGE_VIEW_TYPE_2D;
        break;
    case ImageType_3D:
        view_type = VK_IMAGE_VIEW_TYPE_3D;
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

    const VkImageViewCreateInfo image_view_create_info
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_Image->GetHandle(),
        .viewType = view_type,
        .format = format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            // TODO
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = m_Image->GetMipLevelCount(),
            .baseArrayLayer = 0,
            .layerCount = m_Image->GetArrayLayerCount(),
        },
    };
    vkCreateImageView(m_Device->GetHandle(), &image_view_create_info, nullptr, &m_Handle);
}

glal::Image glal::vulkan::ImageViewT::GetImage() const
{
    return m_Image;
}

glal::ImageFormat glal::vulkan::ImageViewT::GetFormat() const
{
    return m_Format;
}

glal::ImageType glal::vulkan::ImageViewT::GetType() const
{
    return m_Type;
}

VkImageView glal::vulkan::ImageViewT::GetHandle() const
{
    return m_Handle;
}
