#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::Device::Device(PhysicalDevice *physical_device)
    : m_PhysicalDevice(physical_device)
{
    m_Queue = new Queue(this);
}

fxng::glal::opengl::Device::~Device()
{
    for (const auto x : m_Buffers)
        delete x;
    for (const auto x : m_Images)
        delete x;
    for (const auto x : m_Samplers)
        delete x;
    for (const auto x : m_ShaderModules)
        delete x;
    for (const auto x : m_Pipelines)
        delete x;
    for (const auto x : m_Swapchains)
        delete x;
    for (const auto x : m_CommandBuffers)
        delete x;
    for (const auto x : m_Fences)
        delete x;

    delete m_Queue;
}

fxng::glal::Buffer *fxng::glal::opengl::Device::CreateBuffer(const BufferDesc &desc)
{
    auto buffer = new Buffer(this, desc);
    m_Buffers.emplace_back(buffer);
    return buffer;
}

fxng::glal::Image *fxng::glal::opengl::Device::CreateImage(const ImageDesc &desc)
{
    auto image = new Image(this, desc);
    m_Images.emplace_back(image);
    return image;
}

fxng::glal::Sampler *fxng::glal::opengl::Device::CreateSampler(const SamplerDesc &desc)
{
    auto sampler = new Sampler(this, desc);
    m_Samplers.emplace_back(sampler);
    return sampler;
}

fxng::glal::ShaderModule *fxng::glal::opengl::Device::CreateShaderModule(const ShaderModuleDesc &desc)
{
    auto shader_module = new ShaderModule(this, desc);
    m_ShaderModules.emplace_back(shader_module);
    return shader_module;
}

fxng::glal::Pipeline *fxng::glal::opengl::Device::CreatePipeline(const PipelineDesc &desc)
{
    auto pipeline = new Pipeline(this, desc);
    m_Pipelines.emplace_back(pipeline);
    return pipeline;
}

fxng::glal::Swapchain *fxng::glal::opengl::Device::CreateSwapchain(const SwapchainDesc &desc)
{
    auto swapchain = new Swapchain(this, desc);
    m_Swapchains.emplace_back(swapchain);
    return swapchain;
}

fxng::glal::CommandBuffer *fxng::glal::opengl::Device::CreateCommandBuffer(const CommandBufferUsage usage)
{
    auto command_buffer = new CommandBuffer(this, usage);
    m_CommandBuffers.emplace_back(command_buffer);
    return command_buffer;
}

fxng::glal::Fence *fxng::glal::opengl::Device::CreateFence()
{
    auto fence = new Fence(this);
    m_Fences.emplace_back(fence);
    return fence;
}

fxng::glal::Queue *fxng::glal::opengl::Device::GetQueue(QueueType type)
{
    return m_Queue;
}

bool fxng::glal::opengl::Device::Supports(const DeviceFeature feature) const
{
    return m_PhysicalDevice->Supports(feature);
}

const fxng::glal::DeviceLimits &fxng::glal::opengl::Device::GetLimits() const
{
    return m_PhysicalDevice->GetLimits();
}
