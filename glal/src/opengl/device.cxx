#include <glal/opengl.hxx>

glal::opengl::Device::Device(const Ptr<PhysicalDevice> physical_device)
    : m_PhysicalDevice(physical_device)
{
    m_Queue = new Queue(this);
}

glal::opengl::Device::~Device()
{
    for (const auto x : m_Buffers)
        delete x;
    for (const auto x : m_Images)
        delete x;
    for (const auto x : m_Samplers)
        delete x;
    for (const auto x : m_ShaderModules)
        delete x;
    for (const auto x : m_PipelineLayouts)
        delete x;
    for (const auto x : m_Pipelines)
        delete x;
    for (const auto x : m_DescriptorSetLayouts)
        delete x;
    for (const auto x : m_DescriptorSets)
        delete x;
    for (const auto x : m_Swapchains)
        delete x;
    for (const auto x : m_CommandBuffers)
        delete x;
    for (const auto x : m_Fences)
        delete x;

    delete m_Queue;
}

glal::Ptr<glal::Buffer> glal::opengl::Device::CreateBuffer(const BufferDesc &desc)
{
    return m_Buffers.emplace_back(new Buffer(this, desc));
}

void glal::opengl::Device::DestroyBuffer(const Ptr<glal::Buffer> buffer)
{
    for (auto it = m_Buffers.begin(); it != m_Buffers.end(); ++it)
        if (*it == buffer)
        {
            m_Buffers.erase(it);
            delete buffer;
            break;
        }
}

glal::Ptr<glal::Image> glal::opengl::Device::CreateImage(const ImageDesc &desc)
{
    return m_Images.emplace_back(new Image(this, desc));
}

void glal::opengl::Device::DestroyImage(const Ptr<glal::Image> image)
{
    for (auto it = m_Images.begin(); it != m_Images.end(); ++it)
        if (*it == image)
        {
            m_Images.erase(it);
            delete image;
            break;
        }
}

glal::Ptr<glal::Sampler> glal::opengl::Device::CreateSampler(const SamplerDesc &desc)
{
    return m_Samplers.emplace_back(new Sampler(this, desc));
}

void glal::opengl::Device::DestroySampler(const Ptr<glal::Sampler> sampler)
{
    for (auto it = m_Samplers.begin(); it != m_Samplers.end(); ++it)
        if (*it == sampler)
        {
            m_Samplers.erase(it);
            delete sampler;
            break;
        }
}

glal::Ptr<glal::ShaderModule> glal::opengl::Device::CreateShaderModule(const ShaderModuleDesc &desc)
{
    return m_ShaderModules.emplace_back(new ShaderModule(this, desc));
}

void glal::opengl::Device::DestroyShaderModule(const Ptr<glal::ShaderModule> shader_module)
{
    for (auto it = m_ShaderModules.begin(); it != m_ShaderModules.end(); ++it)
        if (*it == shader_module)
        {
            m_ShaderModules.erase(it);
            delete shader_module;
            break;
        }
}

glal::Ptr<glal::PipelineLayout> glal::opengl::Device::CreatePipelineLayout(
    const PipelineLayoutDesc &desc)
{
    return m_PipelineLayouts.emplace_back(new PipelineLayout(this, desc));
}

void glal::opengl::Device::DestroyPipelineLayout(const Ptr<glal::PipelineLayout> pipeline_layout)
{
    for (auto it = m_PipelineLayouts.begin(); it != m_PipelineLayouts.end(); ++it)
        if (*it == pipeline_layout)
        {
            m_PipelineLayouts.erase(it);
            delete pipeline_layout;
            break;
        }
}

glal::Ptr<glal::Pipeline> glal::opengl::Device::CreatePipeline(const PipelineDesc &desc)
{
    return m_Pipelines.emplace_back(new Pipeline(this, desc));
}

void glal::opengl::Device::DestroyPipeline(const Ptr<glal::Pipeline> pipeline)
{
    for (auto it = m_Pipelines.begin(); it != m_Pipelines.end(); ++it)
        if (*it == pipeline)
        {
            m_Pipelines.erase(it);
            delete pipeline;
            break;
        }
}

glal::Ptr<glal::DescriptorSetLayout> glal::opengl::Device::CreateDescriptorSetLayout(
    const DescriptorSetLayoutDesc &desc)
{
    return m_DescriptorSetLayouts.emplace_back(new DescriptorSetLayout(this, desc));
}

void glal::opengl::Device::DestroyDescriptorSetLayout(const Ptr<glal::DescriptorSetLayout> descriptor_set_layout)
{
    for (auto it = m_DescriptorSetLayouts.begin(); it != m_DescriptorSetLayouts.end(); ++it)
        if (*it == descriptor_set_layout)
        {
            m_DescriptorSetLayouts.erase(it);
            delete descriptor_set_layout;
            break;
        }
}

glal::Ptr<glal::DescriptorSet> glal::opengl::Device::CreateDescriptorSet(
    const DescriptorSetDesc &desc)
{
    return m_DescriptorSets.emplace_back(new DescriptorSet(this, desc));
}

void glal::opengl::Device::DestroyDescriptorSet(const Ptr<glal::DescriptorSet> descriptor_set)
{
    for (auto it = m_DescriptorSets.begin(); it != m_DescriptorSets.end(); ++it)
        if (*it == descriptor_set)
        {
            m_DescriptorSets.erase(it);
            delete descriptor_set;
            break;
        }
}

glal::Ptr<glal::Swapchain> glal::opengl::Device::CreateSwapchain(const SwapchainDesc &desc)
{
    return m_Swapchains.emplace_back(new Swapchain(this, desc));
}

void glal::opengl::Device::DestroySwapchain(const Ptr<glal::Swapchain> swapchain)
{
    for (auto it = m_Swapchains.begin(); it != m_Swapchains.end(); ++it)
        if (*it == swapchain)
        {
            m_Swapchains.erase(it);
            delete swapchain;
            break;
        }
}

glal::Ptr<glal::CommandBuffer> glal::opengl::Device::CreateCommandBuffer(
    const CommandBufferUsage usage)
{
    return m_CommandBuffers.emplace_back(new CommandBuffer(this, usage));
}

void glal::opengl::Device::DestroyCommandBuffer(const Ptr<glal::CommandBuffer> command_buffer)
{
    for (auto it = m_CommandBuffers.begin(); it != m_CommandBuffers.end(); ++it)
        if (*it == command_buffer)
        {
            m_CommandBuffers.erase(it);
            delete command_buffer;
            break;
        }
}

glal::Ptr<glal::Fence> glal::opengl::Device::CreateFence()
{
    return m_Fences.emplace_back(new Fence(this));
}

void glal::opengl::Device::DestroyFence(const Ptr<glal::Fence> fence)
{
    for (auto it = m_Fences.begin(); it != m_Fences.end(); ++it)
        if (*it == fence)
        {
            m_Fences.erase(it);
            delete fence;
            break;
        }
}

glal::Ptr<glal::Queue> glal::opengl::Device::GetQueue(QueueType type)
{
    return m_Queue;
}

bool glal::opengl::Device::Supports(const DeviceFeature feature) const
{
    return m_PhysicalDevice->Supports(feature);
}

const glal::DeviceLimits &glal::opengl::Device::GetLimits() const
{
    return m_PhysicalDevice->GetLimits();
}
