#include <glal/vulkan.hxx>

glal::vulkan::DescriptorSetT::DescriptorSetT(DeviceT *device, const DescriptorSetDesc &desc)
    : m_Device(device),
      m_Layout(dynamic_cast<DescriptorSetLayoutT *>(desc.Layout)),
      m_Handle(nullptr)
{
    std::vector<VkDescriptorPoolSize> descriptor_pool_sizes(m_Layout->GetDescriptorBindingCount());
    for (std::uint32_t i = 0; i < m_Layout->GetDescriptorBindingCount(); ++i)
    {
        auto &descriptor_binding = m_Layout->GetDescriptorBinding(i);

        descriptor_pool_sizes[i] = {
            .type = ToVkDescriptorType(descriptor_binding.Type),
            .descriptorCount = descriptor_binding.Count,
        };
    }

    const VkDescriptorPoolCreateInfo descriptor_pool_create_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1,
        .poolSizeCount = static_cast<std::uint32_t>(descriptor_pool_sizes.size()),
        .pPoolSizes = descriptor_pool_sizes.data(),
    };

    vkCreateDescriptorPool(m_Device->GetHandle(), &descriptor_pool_create_info, nullptr, &m_PoolHandle);

    auto layout = m_Layout->GetHandle();

    const VkDescriptorSetAllocateInfo descriptor_set_allocate_info
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_PoolHandle,
        .descriptorSetCount = 1,
        .pSetLayouts = &layout,
    };

    vkAllocateDescriptorSets(m_Device->GetHandle(), &descriptor_set_allocate_info, &m_Handle);
}

glal::vulkan::DescriptorSetT::~DescriptorSetT()
{
    vkFreeDescriptorSets(m_Device->GetHandle(), m_PoolHandle, 1, &m_Handle);
    vkDestroyDescriptorPool(m_Device->GetHandle(), m_PoolHandle, nullptr);
}

void glal::vulkan::DescriptorSetT::BindBuffer(const std::uint32_t binding, Buffer buffer)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);

    BindBuffer(binding, buffer, 0, buffer_impl->GetSize());
}

void glal::vulkan::DescriptorSetT::BindBuffer(
    const std::uint32_t binding,
    Buffer buffer,
    const std::uint32_t offset,
    const std::uint32_t size)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);

    const VkDescriptorBufferInfo descriptor_buffer_info
    {
        .buffer = buffer_impl->GetHandle(),
        .offset = offset,
        .range = size,
    };

    const auto descriptor_binding = m_Layout->FindDescriptorBinding(binding);

    VkWriteDescriptorSet write_descriptor_set
    {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = m_Handle,
        .dstBinding = binding,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = ToVkDescriptorType(descriptor_binding->Type),
        .pImageInfo = nullptr,
        .pBufferInfo = &descriptor_buffer_info,
        .pTexelBufferView = nullptr,
    };

    m_WriteDescriptorSets.emplace_back(write_descriptor_set);
}

void glal::vulkan::DescriptorSetT::BindImageView(const std::uint32_t binding, ImageView image_view, Sampler sampler)
{
    const auto image_view_impl = dynamic_cast<ImageViewT *>(image_view);
    const auto sampler_impl = dynamic_cast<SamplerT *>(sampler);

    // TODO: image layout
    const VkDescriptorImageInfo descriptor_image_info
    {
        .sampler = sampler_impl->GetHandle(),
        .imageView = image_view_impl->GetHandle(),
        .imageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    const auto descriptor_binding = m_Layout->FindDescriptorBinding(binding);

    VkWriteDescriptorSet write_descriptor_set
    {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = m_Handle,
        .dstBinding = binding,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = ToVkDescriptorType(descriptor_binding->Type),
        .pImageInfo = &descriptor_image_info,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr,
    };

    m_WriteDescriptorSets.emplace_back(write_descriptor_set);
}

void glal::vulkan::DescriptorSetT::Bind() const
{
    vkUpdateDescriptorSets(
        m_Device->GetHandle(),
        m_WriteDescriptorSets.size(),
        m_WriteDescriptorSets.data(),
        0,
        nullptr);
}
