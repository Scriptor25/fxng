#include <glal/opengl.hxx>

glal::opengl::DescriptorSetLayoutT::DescriptorSetLayoutT(
    DeviceT *device,
    const DescriptorSetLayoutDesc &desc)
    : m_Device(device),
      m_Set(desc.Set),
      m_DescriptorBindings(desc.DescriptorBindings, desc.DescriptorBindings + desc.DescriptorBindingCount)
{
}

std::uint32_t glal::opengl::DescriptorSetLayoutT::GetSet() const
{
    return m_Set;
}

const glal::DescriptorBinding &glal::opengl::DescriptorSetLayoutT::GetDescriptorBinding(
    const std::uint32_t index) const
{
    return m_DescriptorBindings.at(index);
}

const glal::DescriptorBinding *glal::opengl::DescriptorSetLayoutT::FindDescriptorBinding(std::uint32_t binding) const
{
    for (auto &descriptor_binding : m_DescriptorBindings)
        if (descriptor_binding.Binding == binding)
            return &descriptor_binding;
    return nullptr;
}

std::uint32_t glal::opengl::DescriptorSetLayoutT::GetDescriptorBindingCount() const
{
    return m_DescriptorBindings.size();
}
