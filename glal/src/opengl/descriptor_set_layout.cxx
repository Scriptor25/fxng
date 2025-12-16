#include <glal/opengl.hxx>

glal::opengl::DescriptorSetLayout::DescriptorSetLayout(
    const Ptr<Device> device,
    const DescriptorSetLayoutDesc &desc)
    : m_Device(device),
      m_Set(desc.Set),
      m_DescriptorBindings(desc.DescriptorBindingCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorBindingCount; ++i)
        m_DescriptorBindings.at(i) = desc.DescriptorBindings[i];
}

std::uint32_t glal::opengl::DescriptorSetLayout::GetSet() const
{
    return m_Set;
}

const glal::DescriptorBinding &glal::opengl::DescriptorSetLayout::GetDescriptorBinding(
    const std::uint32_t index) const
{
    return m_DescriptorBindings.at(index);
}

std::uint32_t glal::opengl::DescriptorSetLayout::GetDescriptorBindingCount() const
{
    return m_DescriptorBindings.size();
}

std::vector<glal::DescriptorBinding>::const_iterator glal::opengl::DescriptorSetLayout::begin() const
{
    return m_DescriptorBindings.begin();
}

std::vector<glal::DescriptorBinding>::const_iterator glal::opengl::DescriptorSetLayout::end() const
{
    return m_DescriptorBindings.end();
}
