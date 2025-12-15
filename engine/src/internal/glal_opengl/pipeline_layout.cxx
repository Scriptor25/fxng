#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::PipelineLayout::PipelineLayout(Device *device, const PipelineLayoutDesc &desc)
    : m_Device(device),
      m_DescriptorSetLayouts(desc.DescriptorSetLayoutCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorSetLayoutCount; ++i)
        m_DescriptorSetLayouts.at(i) = dynamic_cast<DescriptorSetLayout *>(desc.DescriptorSetLayouts[i]);
}

fxng::glal::DescriptorSetLayout *fxng::glal::opengl::PipelineLayout::GetDescriptorSetLayout(
    const std::uint32_t index) const
{
    return m_DescriptorSetLayouts.at(index);
}

std::uint32_t fxng::glal::opengl::PipelineLayout::GetDescriptorSetLayoutCount() const
{
    return m_DescriptorSetLayouts.size();
}

std::vector<fxng::glal::opengl::DescriptorSetLayout *>::const_iterator fxng::glal::opengl::PipelineLayout::begin() const
{
    return m_DescriptorSetLayouts.begin();
}

std::vector<fxng::glal::opengl::DescriptorSetLayout *>::const_iterator fxng::glal::opengl::PipelineLayout::end() const
{
    return m_DescriptorSetLayouts.end();
}
