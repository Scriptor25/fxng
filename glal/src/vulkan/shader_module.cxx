#include <glal/vulkan.hxx>

glal::vulkan::ShaderModuleT::ShaderModuleT(DeviceT *device, const ShaderModuleDesc &desc)
    : m_Device(device),
      m_Stage(desc.Stage),
      m_Handle()
{
    const VkShaderModuleCreateInfo shader_module_create_info
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = desc.Size,
        .pCode = static_cast<const std::uint32_t *>(desc.Code),
    };
    vkCreateShaderModule(m_Device->GetHandle(), &shader_module_create_info, nullptr, &m_Handle);
}

glal::vulkan::ShaderModuleT::~ShaderModuleT()
{
    vkDestroyShaderModule(m_Device->GetHandle(), m_Handle, nullptr);
}

glal::ShaderStage glal::vulkan::ShaderModuleT::GetStage() const
{
    return m_Stage;
}

VkShaderModule glal::vulkan::ShaderModuleT::GetHandle() const
{
    return m_Handle;
}
