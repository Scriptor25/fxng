#include <glal/vulkan.hxx>

glal::vulkan::FramebufferT::FramebufferT(DeviceT *device, const FramebufferDesc &desc)
    : m_Device(device),
      m_Attachments(desc.Attachments, desc.Attachments + desc.AttachmentCount),
      m_Handle()
{
    std::vector<VkImageView> attachments(desc.AttachmentCount);
    for (std::uint32_t i = 0; i < attachments.size(); ++i)
        attachments[i] = dynamic_cast<ImageViewT *>(desc.Attachments[i])->GetHandle();

    const auto render_pass_impl = dynamic_cast<RenderPassT *>(desc.Pass);

    // TODO: width, height and layers
    const VkFramebufferCreateInfo framebuffer_create_info
    {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = render_pass_impl->GetHandle(),
        .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .width = {},
        .height = {},
        .layers = {},
    };

    vkCreateFramebuffer(m_Device->GetHandle(), &framebuffer_create_info, nullptr, &m_Handle);
}

std::uint32_t glal::vulkan::FramebufferT::GetAttachmentCount() const
{
    return m_Attachments.size();
}

glal::ImageView glal::vulkan::FramebufferT::GetAttachment(const std::uint32_t index) const
{
    return m_Attachments.at(index);
}

VkFramebuffer glal::vulkan::FramebufferT::GetHandle() const
{
    return m_Handle;
}
