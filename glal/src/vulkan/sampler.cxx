#include <glal/vulkan.hxx>

glal::vulkan::SamplerT::SamplerT(DeviceT *device, const SamplerDesc &desc)
    : m_Device(device),
      m_Handle(nullptr)
{
    // TODO
    const VkSamplerCreateInfo sampler_create_info
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = ToVkFilter(desc.MagFilter),
        .minFilter = ToVkFilter(desc.MinFilter),
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        .addressModeU = ToVkAddressMode(desc.AddressU),
        .addressModeV = ToVkAddressMode(desc.AddressV),
        .addressModeW = ToVkAddressMode(desc.AddressW),
        .mipLodBias = 0.f,
        .anisotropyEnable = false,
        .maxAnisotropy = 0.f,
        .compareEnable = false,
        .compareOp = VK_COMPARE_OP_NEVER,
        .minLod = 0.f,
        .maxLod = 0.f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = false,
    };
    vkCreateSampler(m_Device->GetHandle(), &sampler_create_info, nullptr, &m_Handle);
}

VkSampler glal::vulkan::SamplerT::GetHandle() const
{
    return m_Handle;
}
