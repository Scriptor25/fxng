#include <common/log.hxx>
#include <glal/vulkan.hxx>

static constexpr std::array extensions
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

glal::vulkan::DeviceT::DeviceT(PhysicalDeviceT *physical_device)
    : m_PhysicalDevice(physical_device),
      m_Handle()
{
    std::uint32_t queue_family_property_count;
    vkGetPhysicalDeviceQueueFamilyProperties(
        m_PhysicalDevice->GetHandle(),
        &queue_family_property_count,
        nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(
        m_PhysicalDevice->GetHandle(),
        &queue_family_property_count,
        queue_family_properties.data());

    auto queue_priority = 1.f;

    std::vector<VkDeviceQueueCreateInfo> device_queue_create_infos(queue_family_property_count);
    for (std::uint32_t i = 0; i < queue_family_property_count; ++i)
        device_queue_create_infos[i] = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = i,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        };

    // TODO: layers, extensions and queue families
    const VkDeviceCreateInfo device_create_info
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<std::uint32_t>(device_queue_create_infos.size()),
        .pQueueCreateInfos = device_queue_create_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = extensions.size(),
        .ppEnabledExtensionNames = extensions.data(),
        .pEnabledFeatures = nullptr,
    };

    vkCreateDevice(physical_device->GetHandle(), &device_create_info, nullptr, &m_Handle);
}

glal::vulkan::DeviceT::~DeviceT()
{
    common::Assert(m_Buffers.empty(), "not all buffers were explicitly destroyed");
    common::Assert(m_Images.empty(), "not all images were explicitly destroyed");
    common::Assert(m_Samplers.empty(), "not all samplers were explicitly destroyed");
    common::Assert(m_ShaderModules.empty(), "not all shader modules were explicitly destroyed");
    common::Assert(m_PipelineLayouts.empty(), "not all pipeline layouts were explicitly destroyed");
    common::Assert(m_Pipelines.empty(), "not all pipelines were explicitly destroyed");
    common::Assert(m_DescriptorSetLayouts.empty(), "not all descriptor set layouts were explicitly destroyed");
    common::Assert(m_DescriptorSets.empty(), "not all descriptor sets were explicitly destroyed");
    common::Assert(m_Swapchains.empty(), "not all swapchains were explicitly destroyed");
    common::Assert(m_CommandBuffers.empty(), "not all command buffers were explicitly destroyed");
    common::Assert(m_Fences.empty(), "not all fences were explicitly destroyed");
}

glal::vulkan::PhysicalDeviceT *glal::vulkan::DeviceT::GetPhysicalDevice() const
{
    return m_PhysicalDevice;
}

glal::Buffer glal::vulkan::DeviceT::CreateBuffer(const BufferDesc &desc)
{
    return m_Buffers.emplace_back(new BufferT(this, desc));
}

void glal::vulkan::DeviceT::DestroyBuffer(Buffer buffer)
{
    for (auto it = m_Buffers.begin(); it != m_Buffers.end(); ++it)
        if (*it == buffer)
        {
            m_Buffers.erase(it);
            delete buffer;
            return;
        }
    common::Fatal(
        "buffer {} is not owned by device {}",
        static_cast<const void *>(buffer),
        static_cast<const void *>(this));
}

glal::Image glal::vulkan::DeviceT::CreateImage(const ImageDesc &desc)
{
    return m_Images.emplace_back(new ImageT(this, desc));
}

void glal::vulkan::DeviceT::DestroyImage(Image image)
{
    for (auto it = m_Images.begin(); it != m_Images.end(); ++it)
        if (*it == image)
        {
            m_Images.erase(it);
            delete image;
            return;
        }
    common::Fatal(
        "image {} is not owned by device {}",
        static_cast<const void *>(image),
        static_cast<const void *>(this));
}

glal::ImageView glal::vulkan::DeviceT::CreateImageView(const ImageViewDesc &desc)
{
    return m_ImageViews.emplace_back(new ImageViewT(this, desc));
}

void glal::vulkan::DeviceT::DestroyImageView(ImageView image_view)
{
    for (auto it = m_ImageViews.begin(); it != m_ImageViews.end(); ++it)
        if (*it == image_view)
        {
            m_ImageViews.erase(it);
            delete image_view;
            return;
        }
    common::Fatal(
        "image view {} is not owned by device {}",
        static_cast<const void *>(image_view),
        static_cast<const void *>(this));
}

glal::Sampler glal::vulkan::DeviceT::CreateSampler(const SamplerDesc &desc)
{
    return m_Samplers.emplace_back(new SamplerT(this, desc));
}

void glal::vulkan::DeviceT::DestroySampler(Sampler sampler)
{
    for (auto it = m_Samplers.begin(); it != m_Samplers.end(); ++it)
        if (*it == sampler)
        {
            m_Samplers.erase(it);
            delete sampler;
            return;
        }
    common::Fatal(
        "sampler {} is not owned by device {}",
        static_cast<const void *>(sampler),
        static_cast<const void *>(this));
}

glal::ShaderModule glal::vulkan::DeviceT::CreateShaderModule(const ShaderModuleDesc &desc)
{
    return m_ShaderModules.emplace_back(new ShaderModuleT(this, desc));
}

void glal::vulkan::DeviceT::DestroyShaderModule(ShaderModule shader_module)
{
    for (auto it = m_ShaderModules.begin(); it != m_ShaderModules.end(); ++it)
        if (*it == shader_module)
        {
            m_ShaderModules.erase(it);
            delete shader_module;
            return;
        }
    common::Fatal(
        "shader module {} is not owned by device {}",
        static_cast<const void *>(shader_module),
        static_cast<const void *>(this));
}

glal::PipelineLayout glal::vulkan::DeviceT::CreatePipelineLayout(const PipelineLayoutDesc &desc)
{
    return m_PipelineLayouts.emplace_back(new PipelineLayoutT(this, desc));
}

void glal::vulkan::DeviceT::DestroyPipelineLayout(PipelineLayout pipeline_layout)
{
    for (auto it = m_PipelineLayouts.begin(); it != m_PipelineLayouts.end(); ++it)
        if (*it == pipeline_layout)
        {
            m_PipelineLayouts.erase(it);
            delete pipeline_layout;
            return;
        }
    common::Fatal(
        "pipeline layout {} is not owned by device {}",
        static_cast<const void *>(pipeline_layout),
        static_cast<const void *>(this));
}

glal::Pipeline glal::vulkan::DeviceT::CreatePipeline(const PipelineDesc &desc)
{
    return m_Pipelines.emplace_back(new PipelineT(this, desc));
}

void glal::vulkan::DeviceT::DestroyPipeline(Pipeline pipeline)
{
    for (auto it = m_Pipelines.begin(); it != m_Pipelines.end(); ++it)
        if (*it == pipeline)
        {
            m_Pipelines.erase(it);
            delete pipeline;
            return;
        }
    common::Fatal(
        "pipeline {} is not owned by device {}",
        static_cast<const void *>(pipeline),
        static_cast<const void *>(this));
}

glal::DescriptorSetLayout glal::vulkan::DeviceT::CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc)
{
    return m_DescriptorSetLayouts.emplace_back(new DescriptorSetLayoutT(this, desc));
}

void glal::vulkan::DeviceT::DestroyDescriptorSetLayout(DescriptorSetLayout descriptor_set_layout)
{
    for (auto it = m_DescriptorSetLayouts.begin(); it != m_DescriptorSetLayouts.end(); ++it)
        if (*it == descriptor_set_layout)
        {
            m_DescriptorSetLayouts.erase(it);
            delete descriptor_set_layout;
            return;
        }
    common::Fatal(
        "descriptor set layout {} is not owned by device {}",
        static_cast<const void *>(descriptor_set_layout),
        static_cast<const void *>(this));
}

glal::DescriptorSet glal::vulkan::DeviceT::CreateDescriptorSet(const DescriptorSetDesc &desc)
{
    return m_DescriptorSets.emplace_back(new DescriptorSetT(this, desc));
}

void glal::vulkan::DeviceT::DestroyDescriptorSet(DescriptorSet descriptor_set)
{
    for (auto it = m_DescriptorSets.begin(); it != m_DescriptorSets.end(); ++it)
        if (*it == descriptor_set)
        {
            m_DescriptorSets.erase(it);
            delete descriptor_set;
            return;
        }
    common::Fatal(
        "descriptor set {} is not owned by device {}",
        static_cast<const void *>(descriptor_set),
        static_cast<const void *>(this));
}

glal::Swapchain glal::vulkan::DeviceT::CreateSwapchain(const SwapchainDesc &desc)
{
    return m_Swapchains.emplace_back(new SwapchainT(this, desc));
}

void glal::vulkan::DeviceT::DestroySwapchain(Swapchain swapchain)
{
    for (auto it = m_Swapchains.begin(); it != m_Swapchains.end(); ++it)
        if (*it == swapchain)
        {
            m_Swapchains.erase(it);
            delete swapchain;
            return;
        }
    common::Fatal(
        "swapchain {} is not owned by device {}",
        static_cast<const void *>(swapchain),
        static_cast<const void *>(this));
}

glal::RenderPass glal::vulkan::DeviceT::CreateRenderPass(const RenderPassDesc &desc)
{
    return m_RenderPasses.emplace_back(new RenderPassT(this, desc));
}

void glal::vulkan::DeviceT::DestroyRenderPass(RenderPass render_pass)
{
    for (auto it = m_RenderPasses.begin(); it != m_RenderPasses.end(); ++it)
        if (*it == render_pass)
        {
            m_RenderPasses.erase(it);
            delete render_pass;
            return;
        }
    common::Fatal(
        "render pass {} is not owned by device {}",
        static_cast<const void *>(render_pass),
        static_cast<const void *>(this));
}

glal::Framebuffer glal::vulkan::DeviceT::CreateFramebuffer(const FramebufferDesc &desc)
{
    return m_Framebuffers.emplace_back(new FramebufferT(this, desc));
}

void glal::vulkan::DeviceT::DestroyFramebuffer(Framebuffer framebuffer)
{
    for (auto it = m_Framebuffers.begin(); it != m_Framebuffers.end(); ++it)
        if (*it == framebuffer)
        {
            m_Framebuffers.erase(it);
            delete framebuffer;
            return;
        }
    common::Fatal(
        "framebuffer {} is not owned by device {}",
        static_cast<const void *>(framebuffer),
        static_cast<const void *>(this));
}

glal::CommandBuffer glal::vulkan::DeviceT::CreateCommandBuffer(const CommandBufferUsage usage)
{
    return m_CommandBuffers.emplace_back(new CommandBufferT(this, usage));
}

void glal::vulkan::DeviceT::DestroyCommandBuffer(CommandBuffer command_buffer)
{
    for (auto it = m_CommandBuffers.begin(); it != m_CommandBuffers.end(); ++it)
        if (*it == command_buffer)
        {
            m_CommandBuffers.erase(it);
            delete command_buffer;
            return;
        }
    common::Fatal(
        "command buffer {} is not owned by device {}",
        static_cast<const void *>(command_buffer),
        static_cast<const void *>(this));
}

glal::Fence glal::vulkan::DeviceT::CreateFence()
{
    return m_Fences.emplace_back(new FenceT(this));
}

void glal::vulkan::DeviceT::DestroyFence(Fence fence)
{
    for (auto it = m_Fences.begin(); it != m_Fences.end(); ++it)
        if (*it == fence)
        {
            m_Fences.erase(it);
            delete fence;
            return;
        }
    common::Fatal(
        "fence {} is not owned by device {}",
        static_cast<const void *>(fence),
        static_cast<const void *>(this));
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

VkDevice glal::vulkan::DeviceT::GetHandle() const
{
    return m_Handle;
}
