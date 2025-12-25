#include <glal/opengl.hxx>

glal::opengl::RenderPassT::RenderPassT(DeviceT *device, const RenderPassDesc &desc)
    : m_Device(device),
      m_Attachments(desc.Attachments, desc.Attachments + desc.AttachmentCount)
{
}

std::uint32_t glal::opengl::RenderPassT::GetAttachmentCount() const
{
    return m_Attachments.size();
}

const glal::Attachment &glal::opengl::RenderPassT::GetAttachment(const std::uint32_t index) const
{
    return m_Attachments.at(index);
}
