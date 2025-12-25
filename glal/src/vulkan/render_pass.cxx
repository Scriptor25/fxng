#include <glal/vulkan.hxx>

glal::vulkan::RenderPassT::RenderPassT(DeviceT *device, const RenderPassDesc &desc)
    : m_Device(device),
      m_Attachments(desc.Attachments, desc.Attachments + desc.AttachmentCount),
      m_Handle()
{
    std::vector<VkAttachmentDescription> attachments(desc.AttachmentCount);
    for (std::uint32_t i = 0; i < attachments.size(); ++i)
    {
        const auto attachment = desc.Attachments + i;

        // TODO
        attachments[i] = {
            .flags = {},
            .format = {},
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = attachment->Clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = attachment->Clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };
    }

    const VkRenderPassCreateInfo render_pass_create_info
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 0,
        .pSubpasses = nullptr,
        .dependencyCount = 0,
        .pDependencies = nullptr,
    };

    vkCreateRenderPass(m_Device->GetHandle(), &render_pass_create_info, nullptr, &m_Handle);
}

std::uint32_t glal::vulkan::RenderPassT::GetAttachmentCount() const
{
    return m_Attachments.size();
}

const glal::Attachment &glal::vulkan::RenderPassT::GetAttachment(const std::uint32_t index) const
{
    return m_Attachments.at(index);
}

VkRenderPass glal::vulkan::RenderPassT::GetHandle() const
{
    return m_Handle;
}
