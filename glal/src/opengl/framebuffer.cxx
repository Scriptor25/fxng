#include <glal/opengl.hxx>

glal::opengl::FramebufferT::FramebufferT(DeviceT *device, const FramebufferDesc &desc)
    : m_Device(device),
      m_Attachments(desc.Attachments, desc.Attachments + desc.AttachmentCount)
{
    glCreateFramebuffers(1, &m_Handle);
}

std::uint32_t glal::opengl::FramebufferT::GetAttachmentCount() const
{
    return m_Attachments.size();
}

glal::ImageView glal::opengl::FramebufferT::GetAttachment(const std::uint32_t index) const
{
    return m_Attachments.at(index);
}

GLuint glal::opengl::FramebufferT::GetHandle() const
{
    return m_Handle;
}
