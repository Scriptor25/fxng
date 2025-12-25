#include <glal/vulkan.hxx>

glal::vulkan::PipelineLayoutT::PipelineLayoutT(DeviceT *device, const PipelineLayoutDesc &desc)
    : m_Device(device),
      m_DescriptorSetLayouts(desc.DescriptorSetLayouts, desc.DescriptorSetLayouts + desc.DescriptorSetLayoutCount),
      m_Handle(nullptr)
{
    std::vector<VkDescriptorSetLayout> descriptor_set_layouts(desc.DescriptorSetLayoutCount);
    for (std::uint32_t i = 0; i < descriptor_set_layouts.size(); ++i)
        descriptor_set_layouts[i] = dynamic_cast<DescriptorSetLayoutT *>(desc.DescriptorSetLayouts[i])->GetHandle();

    const VkPipelineLayoutCreateInfo pipeline_layout_create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = static_cast<std::uint32_t>(descriptor_set_layouts.size()),
        .pSetLayouts = descriptor_set_layouts.data(),
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    vkCreatePipelineLayout(device->GetHandle(), &pipeline_layout_create_info, nullptr, &m_Handle);
}

std::uint32_t glal::vulkan::PipelineLayoutT::GetDescriptorSetLayoutCount() const
{
    return m_DescriptorSetLayouts.size();
}

glal::DescriptorSetLayout glal::vulkan::PipelineLayoutT::GetDescriptorSetLayout(std::uint32_t index) const
{
    return m_DescriptorSetLayouts.at(index);
}

VkPipelineLayout glal::vulkan::PipelineLayoutT::GetHandle() const
{
    return m_Handle;
}
