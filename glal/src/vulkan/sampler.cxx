#include <glal/vulkan.hxx>

glal::vulkan::SamplerT::SamplerT(DeviceT *device, const SamplerDesc &desc)
    : m_Device(device)
{
    const VkSamplerCreateInfo sampler_create_info
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = ToVkFilter(desc.MagFilter),
        .minFilter = ToVkFilter(desc.MinFilter),
        // TODO
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        .addressModeU = ToVkAddressMode(desc.AddressU),
        .addressModeV = ToVkAddressMode(desc.AddressV),
        .addressModeW = ToVkAddressMode(desc.AddressW),
        // TODO
        .mipLodBias = 0.f,
        // TODO
        .anisotropyEnable = false,
        // TODO
        .maxAnisotropy = 0.f,
        // TODO
        .compareEnable = false,
        // TODO
        .compareOp = VK_COMPARE_OP_NEVER,
        // TODO
        .minLod = 0.f,
        // TODO
        .maxLod = 0.f,
        // TODO
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        // TODO
        .unnormalizedCoordinates = false,
    };
    vkCreateSampler(m_Device->GetHandle(), &sampler_create_info, nullptr, &m_Handle);
}
