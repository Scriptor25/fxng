#include <glal/vulkan.hxx>

glal::vulkan::DeviceT::DeviceT(PhysicalDeviceT *physical_device)
    : m_PhysicalDevice(physical_device)
{
    const VkDeviceCreateInfo device_create_info
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 0,
        .pQueueCreateInfos = nullptr,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures = nullptr,
    };

    vkCreateDevice(physical_device->GetHandle(), &device_create_info, nullptr, &m_Handle);
}

glal::Buffer glal::vulkan::DeviceT::CreateBuffer(const BufferDesc &desc)
{
    return m_Buffers.emplace_back(new BufferT(this, desc));
}

void glal::vulkan::DeviceT::DestroyBuffer(const Buffer buffer)
{
    for (auto it = m_Buffers.begin(); it != m_Buffers.end(); ++it)
        if (*it == buffer)
        {
            m_Buffers.erase(it);
            delete buffer;
            break;
        }
}

glal::Image glal::vulkan::DeviceT::CreateImage(const ImageDesc &desc)
{
    return m_Images.emplace_back(new ImageT(this, desc));
}

void glal::vulkan::DeviceT::DestroyImage(const Image image)
{
    for (auto it = m_Images.begin(); it != m_Images.end(); ++it)
        if (*it == image)
        {
            m_Images.erase(it);
            delete image;
            break;
        }
}

glal::ImageView glal::vulkan::DeviceT::CreateImageView(const ImageViewDesc &desc)
{
    return m_ImageViews.emplace_back(new ImageViewT(this, desc));
}

void glal::vulkan::DeviceT::DestroyImageView(const ImageView image_view)
{
    for (auto it = m_ImageViews.begin(); it != m_ImageViews.end(); ++it)
        if (*it == image_view)
        {
            m_ImageViews.erase(it);
            delete image_view;
            break;
        }
}

glal::Sampler glal::vulkan::DeviceT::CreateSampler(const SamplerDesc &desc)
{
    return m_Samplers.emplace_back(new SamplerT(this, desc));
}

void glal::vulkan::DeviceT::DestroySampler(const Sampler sampler)
{
    for (auto it = m_Samplers.begin(); it != m_Samplers.end(); ++it)
        if (*it == sampler)
        {
            m_Samplers.erase(it);
            delete sampler;
            break;
        }
}

glal::ShaderModule glal::vulkan::DeviceT::CreateShaderModule(const ShaderModuleDesc &desc)
{
    return m_ShaderModules.emplace_back(new ShaderModuleT(this, desc));
}

void glal::vulkan::DeviceT::DestroyShaderModule(const ShaderModule shader_module)
{
    for (auto it = m_ShaderModules.begin(); it != m_ShaderModules.end(); ++it)
        if (*it == shader_module)
        {
            m_ShaderModules.erase(it);
            delete shader_module;
            break;
        }
}

glal::PipelineLayout glal::vulkan::DeviceT::CreatePipelineLayout(const PipelineLayoutDesc &desc)
{
    return m_PipelineLayouts.emplace_back(new PipelineLayoutT(this, desc));
}

void glal::vulkan::DeviceT::DestroyPipelineLayout(const PipelineLayout pipeline_layout)
{
    for (auto it = m_PipelineLayouts.begin(); it != m_PipelineLayouts.end(); ++it)
        if (*it == pipeline_layout)
        {
            m_PipelineLayouts.erase(it);
            delete pipeline_layout;
            break;
        }
}

glal::Pipeline glal::vulkan::DeviceT::CreatePipeline(const PipelineDesc &desc)
{
    return m_Pipelines.emplace_back(new PipelineT(this, desc));
}

void glal::vulkan::DeviceT::DestroyPipeline(const Pipeline pipeline)
{
    for (auto it = m_Pipelines.begin(); it != m_Pipelines.end(); ++it)
        if (*it == pipeline)
        {
            m_Pipelines.erase(it);
            delete pipeline;
            break;
        }
}

glal::DescriptorSetLayout glal::vulkan::DeviceT::CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc)
{
    return m_DescriptorSetLayouts.emplace_back(new DescriptorSetLayoutT(this, desc));
}

void glal::vulkan::DeviceT::DestroyDescriptorSetLayout(const DescriptorSetLayout descriptor_set_layout)
{
    for (auto it = m_DescriptorSetLayouts.begin(); it != m_DescriptorSetLayouts.end(); ++it)
        if (*it == descriptor_set_layout)
        {
            m_DescriptorSetLayouts.erase(it);
            delete descriptor_set_layout;
            break;
        }
}

glal::DescriptorSet glal::vulkan::DeviceT::CreateDescriptorSet(const DescriptorSetDesc &desc)
{
    return m_DescriptorSets.emplace_back(new DescriptorSetT(this, desc));
}

void glal::vulkan::DeviceT::DestroyDescriptorSet(const DescriptorSet descriptor_set)
{
    for (auto it = m_DescriptorSets.begin(); it != m_DescriptorSets.end(); ++it)
        if (*it == descriptor_set)
        {
            m_DescriptorSets.erase(it);
            delete descriptor_set;
            break;
        }
}

glal::Swapchain glal::vulkan::DeviceT::CreateSwapchain(const SwapchainDesc &desc)
{
    return m_Swapchains.emplace_back(new SwapchainT(this, desc));
}

void glal::vulkan::DeviceT::DestroySwapchain(const Swapchain swapchain)
{
    for (auto it = m_Swapchains.begin(); it != m_Swapchains.end(); ++it)
        if (*it == swapchain)
        {
            m_Swapchains.erase(it);
            delete swapchain;
            break;
        }
}

glal::CommandBuffer glal::vulkan::DeviceT::CreateCommandBuffer(const CommandBufferUsage usage)
{
    return m_CommandBuffers.emplace_back(new CommandBufferT(this, usage));
}

void glal::vulkan::DeviceT::DestroyCommandBuffer(const CommandBuffer command_buffer)
{
    for (auto it = m_CommandBuffers.begin(); it != m_CommandBuffers.end(); ++it)
        if (*it == command_buffer)
        {
            m_CommandBuffers.erase(it);
            delete command_buffer;
            break;
        }
}

glal::Fence glal::vulkan::DeviceT::CreateFence()
{
    return m_Fences.emplace_back(new FenceT(this));
}

void glal::vulkan::DeviceT::DestroyFence(const Fence fence)
{
    for (auto it = m_Fences.begin(); it != m_Fences.end(); ++it)
        if (*it == fence)
        {
            m_Fences.erase(it);
            delete fence;
            break;
        }
}

glal::Queue glal::vulkan::DeviceT::GetQueue(QueueType type)
{
    for (auto queue : m_Queues); // TODO
    return nullptr;
}

bool glal::vulkan::DeviceT::Supports(const DeviceFeature feature) const
{
    return m_PhysicalDevice->Supports(feature);
}

const glal::DeviceLimits &glal::vulkan::DeviceT::GetLimits() const
{
    return m_PhysicalDevice->GetLimits();
}

glal::vulkan::PhysicalDeviceT *glal::vulkan::DeviceT::GetPhysicalDevice() const
{
    return m_PhysicalDevice;
}

VkDevice glal::vulkan::DeviceT::GetHandle() const
{
    return m_Handle;
}
