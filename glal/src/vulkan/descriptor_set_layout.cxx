#include <glal/vulkan.hxx>

glal::vulkan::DescriptorSetLayoutT::DescriptorSetLayoutT(DeviceT *device, const DescriptorSetLayoutDesc &desc)
    : m_Device(device),
      m_Set(desc.Set),
      m_DescriptorBindings(desc.DescriptorBindings, desc.DescriptorBindings + desc.DescriptorBindingCount),
      m_Handle(nullptr)
{
    std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(desc.DescriptorBindingCount);
    for (std::uint32_t i = 0; i < descriptor_set_layout_bindings.size(); ++i)
    {
        const auto descriptor_binding = desc.DescriptorBindings + i;

        VkShaderStageFlags shader_stage_flags{};
        if (descriptor_binding->Stages & ShaderStage_Vertex)
            shader_stage_flags |= VK_SHADER_STAGE_VERTEX_BIT;
        if (descriptor_binding->Stages & ShaderStage_Geometry)
            shader_stage_flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
        if (descriptor_binding->Stages & ShaderStage_TessellationControl)
            shader_stage_flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if (descriptor_binding->Stages & ShaderStage_TessellationEvaluation)
            shader_stage_flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if (descriptor_binding->Stages & ShaderStage_Fragment)
            shader_stage_flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if (descriptor_binding->Stages & ShaderStage_Compute)
            shader_stage_flags |= VK_SHADER_STAGE_COMPUTE_BIT;
        if (descriptor_binding->Stages & ShaderStage_RayGeneration)
            shader_stage_flags |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        if (descriptor_binding->Stages & ShaderStage_RayHit)
            shader_stage_flags |= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        if (descriptor_binding->Stages & ShaderStage_RayMiss)
            shader_stage_flags |= VK_SHADER_STAGE_MISS_BIT_KHR;

        descriptor_set_layout_bindings[i] = {
            .binding = descriptor_binding->Binding,
            .descriptorType = ToVkDescriptorType(descriptor_binding->Type),
            .descriptorCount = descriptor_binding->Count,
            .stageFlags = shader_stage_flags,
            .pImmutableSamplers = nullptr,
        };
    }

    const VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<std::uint32_t>(descriptor_set_layout_bindings.size()),
        .pBindings = descriptor_set_layout_bindings.data(),
    };

    vkCreateDescriptorSetLayout(device->GetHandle(), &descriptor_set_layout_create_info, nullptr, &m_Handle);
}

std::uint32_t glal::vulkan::DescriptorSetLayoutT::GetSet() const
{
    return m_Set;
}

std::uint32_t glal::vulkan::DescriptorSetLayoutT::GetDescriptorBindingCount() const
{
    return m_DescriptorBindings.size();
}

const glal::DescriptorBinding &glal::vulkan::DescriptorSetLayoutT::GetDescriptorBinding(const std::uint32_t index) const
{
    return m_DescriptorBindings.at(index);
}

const glal::DescriptorBinding *glal::vulkan::DescriptorSetLayoutT::FindDescriptorBinding(
    const std::uint32_t binding) const
{
    for (auto &descriptor_binding : m_DescriptorBindings)
        if (descriptor_binding.Binding == binding)
            return &descriptor_binding;
    return nullptr;
}

VkDescriptorSetLayout glal::vulkan::DescriptorSetLayoutT::GetHandle() const
{
    return m_Handle;
}
