#include <glal/opengl.hxx>

glal::opengl::PipelineLayoutT::PipelineLayoutT(DeviceT *device, const PipelineLayoutDesc &desc)
    : m_Device(device),
      m_DescriptorSetLayouts(desc.DescriptorSetLayoutCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorSetLayoutCount; ++i)
        m_DescriptorSetLayouts.at(i) = dynamic_cast<DescriptorSetLayoutT *>(desc.DescriptorSetLayouts[i]);
}

glal::DescriptorSetLayout glal::opengl::PipelineLayoutT::GetDescriptorSetLayout(
    const std::uint32_t index) const
{
    return m_DescriptorSetLayouts.at(index);
}

std::uint32_t glal::opengl::PipelineLayoutT::GetDescriptorSetLayoutCount() const
{
    return m_DescriptorSetLayouts.size();
}

std::vector<glal::opengl::DescriptorSetLayoutT *>::const_iterator glal::opengl::PipelineLayoutT::begin() const
{
    return m_DescriptorSetLayouts.begin();
}

std::vector<glal::opengl::DescriptorSetLayoutT *>::const_iterator glal::opengl::PipelineLayoutT::end() const
{
    return m_DescriptorSetLayouts.end();
}
