#include <glal/opengl.hxx>

glal::opengl::PipelineLayout::PipelineLayout(const Ptr<Device> device, const PipelineLayoutDesc &desc)
    : m_Device(device),
      m_DescriptorSetLayouts(desc.DescriptorSetLayoutCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorSetLayoutCount; ++i)
        m_DescriptorSetLayouts.at(i) = dynamic_cast<Ptr<DescriptorSetLayout>>(desc.DescriptorSetLayouts[i]);
}

glal::Ptr<glal::DescriptorSetLayout> glal::opengl::PipelineLayout::GetDescriptorSetLayout(
    const std::uint32_t index) const
{
    return m_DescriptorSetLayouts.at(index);
}

std::uint32_t glal::opengl::PipelineLayout::GetDescriptorSetLayoutCount() const
{
    return m_DescriptorSetLayouts.size();
}

std::vector<glal::Ptr<glal::opengl::DescriptorSetLayout>>::const_iterator glal::opengl::PipelineLayout::begin() const
{
    return m_DescriptorSetLayouts.begin();
}

std::vector<glal::Ptr<glal::opengl::DescriptorSetLayout>>::const_iterator glal::opengl::PipelineLayout::end() const
{
    return m_DescriptorSetLayouts.end();
}
