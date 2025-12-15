#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::DescriptorSetLayout::DescriptorSetLayout(Device *device, const DescriptorSetLayoutDesc &desc)
    : m_Device(device),
      m_Set(desc.Set),
      m_DescriptorBindings(desc.DescriptorBindingCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorBindingCount; ++i)
        m_DescriptorBindings.at(i) = desc.DescriptorBindings[i];
}

std::uint32_t fxng::glal::opengl::DescriptorSetLayout::GetSet() const
{
    return m_Set;
}

const fxng::glal::DescriptorBinding &fxng::glal::opengl::DescriptorSetLayout::GetDescriptorBinding(
    const std::uint32_t index) const
{
    return m_DescriptorBindings.at(index);
}

std::uint32_t fxng::glal::opengl::DescriptorSetLayout::GetDescriptorBindingCount() const
{
    return m_DescriptorBindings.size();
}

std::vector<fxng::glal::DescriptorBinding>::const_iterator fxng::glal::opengl::DescriptorSetLayout::begin() const
{
    return m_DescriptorBindings.begin();
}

std::vector<fxng::glal::DescriptorBinding>::const_iterator fxng::glal::opengl::DescriptorSetLayout::end() const
{
    return m_DescriptorBindings.end();
}
