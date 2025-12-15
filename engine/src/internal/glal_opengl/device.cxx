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
    return m_Buffers.emplace_back(new Buffer(this, desc));
}

void fxng::glal::opengl::Device::DestroyBuffer(glal::Buffer *buffer)
{
    for (auto it = m_Buffers.begin(); it != m_Buffers.end(); ++it)
        if (*it == buffer)
        {
            m_Buffers.erase(it);
            delete buffer;
            break;
        }
}

fxng::glal::Image *fxng::glal::opengl::Device::CreateImage(const ImageDesc &desc)
{
    return m_Images.emplace_back(new Image(this, desc));
}

void fxng::glal::opengl::Device::DestroyImage(glal::Image *image)
{
    for (auto it = m_Images.begin(); it != m_Images.end(); ++it)
        if (*it == image)
        {
            m_Images.erase(it);
            delete image;
            break;
        }
}

fxng::glal::Sampler *fxng::glal::opengl::Device::CreateSampler(const SamplerDesc &desc)
{
    return m_Samplers.emplace_back(new Sampler(this, desc));
}

void fxng::glal::opengl::Device::DestroySampler(glal::Sampler *sampler)
{
    for (auto it = m_Samplers.begin(); it != m_Samplers.end(); ++it)
        if (*it == sampler)
        {
            m_Samplers.erase(it);
            delete sampler;
            break;
        }
}

fxng::glal::ShaderModule *fxng::glal::opengl::Device::CreateShaderModule(const ShaderModuleDesc &desc)
{
    return m_ShaderModules.emplace_back(new ShaderModule(this, desc));
}

void fxng::glal::opengl::Device::DestroyShaderModule(glal::ShaderModule *shader_module)
{
    for (auto it = m_ShaderModules.begin(); it != m_ShaderModules.end(); ++it)
        if (*it == shader_module)
        {
            m_ShaderModules.erase(it);
            delete shader_module;
            break;
        }
}

fxng::glal::Pipeline *fxng::glal::opengl::Device::CreatePipeline(const PipelineDesc &desc)
{
    return m_Pipelines.emplace_back(new Pipeline(this, desc));
}

void fxng::glal::opengl::Device::DestroyPipeline(glal::Pipeline *pipeline)
{
    for (auto it = m_Pipelines.begin(); it != m_Pipelines.end(); ++it)
        if (*it == pipeline)
        {
            m_Pipelines.erase(it);
            delete pipeline;
            break;
        }
}

fxng::glal::Swapchain *fxng::glal::opengl::Device::CreateSwapchain(const SwapchainDesc &desc)
{
    return m_Swapchains.emplace_back(new Swapchain(this, desc));
}

void fxng::glal::opengl::Device::DestroySwapchain(glal::Swapchain *swapchain)
{
    for (auto it = m_Swapchains.begin(); it != m_Swapchains.end(); ++it)
        if (*it == swapchain)
        {
            m_Swapchains.erase(it);
            delete swapchain;
            break;
        }
}

fxng::glal::CommandBuffer *fxng::glal::opengl::Device::CreateCommandBuffer(const CommandBufferUsage usage)
{
    return m_CommandBuffers.emplace_back(new CommandBuffer(this, usage));
}

void fxng::glal::opengl::Device::DestroyCommandBuffer(glal::CommandBuffer *command_buffer)
{
    for (auto it = m_CommandBuffers.begin(); it != m_CommandBuffers.end(); ++it)
        if (*it == command_buffer)
        {
            m_CommandBuffers.erase(it);
            delete command_buffer;
            break;
        }
}

fxng::glal::Fence *fxng::glal::opengl::Device::CreateFence()
{
    return m_Fences.emplace_back(new Fence(this));
}

void fxng::glal::opengl::Device::DestroyFence(glal::Fence *fence)
{
    for (auto it = m_Fences.begin(); it != m_Fences.end(); ++it)
        if (*it == fence)
        {
            m_Fences.erase(it);
            delete fence;
            break;
        }
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
