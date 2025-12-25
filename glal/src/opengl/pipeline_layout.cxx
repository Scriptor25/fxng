#include <glal/opengl.hxx>

glal::opengl::PipelineLayoutT::PipelineLayoutT(DeviceT *device, const PipelineLayoutDesc &desc)
    : m_Device(device),
      m_DescriptorSetLayouts(desc.DescriptorSetLayouts, desc.DescriptorSetLayouts + desc.DescriptorSetLayoutCount)
{
}

std::uint32_t glal::opengl::PipelineLayoutT::GetDescriptorSetLayoutCount() const
{
    return m_DescriptorSetLayouts.size();
}

glal::DescriptorSetLayout glal::opengl::PipelineLayoutT::GetDescriptorSetLayout(
    const std::uint32_t index) const
{
    return m_DescriptorSetLayouts.at(index);
}
