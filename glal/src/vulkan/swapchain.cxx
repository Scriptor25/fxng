#include <glal/vulkan.hxx>
#include <GLFW/glfw3.h>

glal::vulkan::SwapchainT::SwapchainT(DeviceT *device, const SwapchainDesc &desc)
    : m_Device(device),
      m_Extent(desc.Extent)
{
    glfwCreateWindowSurface(
        dynamic_cast<InstanceT *>(m_Device->GetPhysicalDevice()->GetInstance())->GetHandle(),
        static_cast<GLFWwindow *>(desc.NativeWindowHandle),
        nullptr,
        &m_Surface);

    // TODO: queue families
    const VkSwapchainCreateInfoKHR swapchain_create_info
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = m_Surface,
        .minImageCount = desc.ImageCount,
        .imageFormat = ToVkFormat(desc.Format),
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = { desc.Extent.Width, desc.Extent.Height },
        .imageArrayLayers = 0,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_CONCURRENT,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = false,
        .oldSwapchain = nullptr,
    };

    vkCreateSwapchainKHR(m_Device->GetHandle(), &swapchain_create_info, nullptr, &m_Handle);

    std::uint32_t swapchain_image_count;
    vkGetSwapchainImagesKHR(m_Device->GetHandle(), m_Handle, &swapchain_image_count, nullptr);

    std::vector<VkImage> swapchain_images(swapchain_image_count);
    vkGetSwapchainImagesKHR(m_Device->GetHandle(), m_Handle, &swapchain_image_count, swapchain_images.data());

    for (std::uint32_t i = 0; i < swapchain_image_count; ++i)
    {
        const auto image = new ImageT(
            swapchain_images[i],
            {
                .Format = desc.Format,
                .Type = ImageType_2D,
                .Extent = { m_Extent.Width, m_Extent.Height, 0 },
                .MipLevelCount = 1,
                .ArrayLayerCount = 0,
            });

        m_Frames.push_back(
            {
                .Resource = image,
                .View = m_Device->CreateImageView(
                    {
                        .Format = desc.Format,
                        .Type = ImageType_2D,
                        .ImageResource = image,
                    }),
            });
    }
}

std::uint32_t glal::vulkan::SwapchainT::GetImageCount() const
{
    return m_Frames.size();
}

glal::ImageView glal::vulkan::SwapchainT::GetImageView(const std::uint32_t index) const
{
    return m_Frames.at(index).View;
}

glal::Extent2D glal::vulkan::SwapchainT::GetExtent() const
{
    return m_Extent;
}

std::uint32_t glal::vulkan::SwapchainT::AcquireNextImage(Fence fence)
{
    std::uint32_t image_index;
    // TODO: semaphore and fence
    vkAcquireNextImageKHR(m_Device->GetHandle(), m_Handle, 0, nullptr, nullptr, &image_index);
    return image_index;
}

void glal::vulkan::SwapchainT::Present() const
{
    // TODO
    const VkPresentInfoKHR present_info
    {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .swapchainCount = 1,
        .pSwapchains = &m_Handle,
        .pImageIndices = nullptr,
        .pResults = nullptr,
    };

    // TODO: queue
    vkQueuePresentKHR(nullptr, &present_info);
}
