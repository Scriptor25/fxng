#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <unordered_map>

/**
 * GLAL - Graphics Library Abstraction Layer
 */
namespace fxng::glal
{
    class Instance;
    class PhysicalDevice;
    class Device;
    class Queue;
    class CommandBuffer;
    class Fence;

    class Resource;
    class Buffer;
    class Image;
    class ImageView;
    class Sampler;
    class Swapchain;

    class ShaderModule;
    class Pipeline;

    enum DeviceFeature : std::uint32_t
    {
        DeviceFeature_None = 0,

        DeviceFeature_Compute        = 1 << 0,
        DeviceFeature_GeometryShader = 1 << 1,
        DeviceFeature_Tessellation   = 1 << 2,
        DeviceFeature_RayTracing     = 1 << 3,

        DeviceFeature_ExplicitBarriers  = 1 << 4,
        DeviceFeature_DescriptorSets    = 1 << 5,
        DeviceFeature_TimelineSemaphore = 1 << 6,
    };

    enum QueueType : std::uint32_t
    {
        QueueType_None     = 0,
        QueueType_Graphics = 1 << 0,
        QueueType_Compute  = 1 << 1,
        QueueType_Transfer = 1 << 2,
        QueueType_Present  = 1 << 3,
    };

    enum CommandBufferUsage : std::uint32_t
    {
        CommandBufferUsage_None     = 0,
        CommandBufferUsage_Once     = 1 << 0,
        CommandBufferUsage_Reusable = 1 << 1,
    };

    enum ShaderStage : std::uint32_t
    {
        ShaderStage_None                   = 0,
        ShaderStage_Vertex                 = 1 << 0,
        ShaderStage_Geometry               = 1 << 1,
        ShaderStage_TessellationControl    = 1 << 2,
        ShaderStage_TessellationEvaluation = 1 << 3,
        ShaderStage_Fragment               = 1 << 4,
        ShaderStage_Compute                = 1 << 5,
        ShaderStage_RayGeneration          = 1 << 6,
        ShaderStage_RayHit                 = 1 << 7,
        ShaderStage_RayMiss                = 1 << 8,
    };

    enum PipelineType : std::uint32_t
    {
        PipelineType_None       = 0,
        PipelineType_Graphics   = 1 << 0,
        PipelineType_Compute    = 1 << 1,
        PipelineType_RayTracing = 1 << 2,
    };

    enum BufferUsage : std::uint32_t
    {
        BufferUsage_None          = 0,
        BufferUsage_VertexBuffer  = 1 << 0,
        BufferUsage_IndexBuffer   = 1 << 1,
        BufferUsage_StorageBuffer = 1 << 2,
    };

    enum MemoryUsage : std::uint32_t
    {
        MemoryUsage_None         = 0,
        MemoryUsage_DeviceLocal  = 1 << 0,
        MemoryUsage_HostToDevice = 1 << 1,
        MemoryUsage_DeviceToHost = 1 << 2,
    };

    enum ImageFormat
    {
        ImageFormat_None,

        ImageFormat_RGBA8_UNorm,
        ImageFormat_RGBA8_SRGB,
        ImageFormat_BGRA8_UNorm,

        ImageFormat_RG16F,
        ImageFormat_RGBA16F,
        ImageFormat_RGBA32F,

        ImageFormat_D24S8,
        ImageFormat_D32F,
    };

    enum IndexType: std::uint32_t
    {
        IndexType_None   = 0,
        IndexType_UInt16 = 1 << 0,
        IndexType_UInt32 = 1 << 1,
    };

    enum ResourceState : std::uint32_t
    {
        ResourceState_None            = 0,
        ResourceState_VertexBuffer    = 1 << 0,
        ResourceState_IndexBuffer     = 1 << 1,
        ResourceState_ConstantBuffer  = 1 << 2,
        ResourceState_ShaderResource  = 1 << 3,
        ResourceState_UnorderedAccess = 1 << 4,
        ResourceState_RenderTarget    = 1 << 5,
        ResourceState_DepthStencil    = 1 << 6,
        ResourceState_CopySrc         = 1 << 7,
        ResourceState_CopyDst         = 1 << 8,
        ResourceState_Present         = 1 << 9,
    };

    enum Filter : std::uint32_t
    {
        Filter_None = 0,
        Filter_Nearest,
        Filter_Linear,
    };

    enum AddressMode : std::uint32_t
    {
        AddressMode_None = 0,
        AddressMode_Repeat,
        AddressMode_Clamp,
        AddressMode_Mirror,
    };

    /**
     * Extent 2D - width and height
     */
    struct Extent2D
    {
        int Width;
        int Height;
    };

    /**
     * Extent 3D - width, height and depth
     */
    struct Extent3D
    {
        int Width;
        int Height;
        int Depth;
    };

    /**
     * Device Limits
     */
    struct DeviceLimits
    {
        std::uint32_t MaxTextureSize2D;
        std::uint32_t MaxUniformBuffers;
        std::uint64_t MaxBufferSize;
    };

    /**
     * Instance Descriptor
     */
    struct InstanceDesc
    {
        bool EnableValidation;
        const char *ApplicationName;
    };

    /**
     * Buffer Descriptor - describes a single buffer
     */
    struct BufferDesc
    {
        std::size_t Size;
        BufferUsage Usage;
        MemoryUsage Memory;
    };

    /**
     * Image Descriptor - describes a single image
     */
    struct ImageDesc
    {
        ImageFormat Format;
        Extent3D Extent;
        std::uint32_t MipLevels;
        std::uint32_t ArrayLayers;
    };

    /**
     * Sampler Descriptor - describes how to sample an image view
     */
    struct SamplerDesc
    {
        Filter MinFilter;
        Filter MagFilter;
        AddressMode AddressU;
        AddressMode AddressV;
        AddressMode AddressW;
    };

    /**
     * Shader Module Descriptor - describes a single-stage shader module
     */
    struct ShaderModuleDesc
    {
        ShaderStage Stage;
        const void *Code;
        size_t Size;
    };

    /**
     * Pipeline Stage Descriptor - describes a single executable pipeline stage
     */
    struct PipelineStageDesc
    {
        ShaderStage Stage;
        ShaderModule *Module;
    };

    /**
     * Pipeline Descriptor - describes a full graphics, compute or ray-tracing pipeline with multiple stages
     */
    struct PipelineDesc
    {
        PipelineType Type;

        const PipelineStageDesc *Stages;
        std::uint32_t StageCount;

        bool DepthTest;
        bool DepthWrite;
        bool BlendEnable;
    };

    /**
     * Swapchain Descriptor - describes a specific swapchain
     */
    struct SwapchainDesc
    {
        void *NativeWindowHandle;

        Extent2D Extent;
        ImageFormat Format;
        std::uint32_t ImageCount;
    };

    union ClearValue
    {
        float Color[4];
        float Depth;
        int Stencil;
    };

    /**
     * Render Target Descriptor - describes an image view as a render target
     */
    struct RenderTargetDesc
    {
        ImageView *View;

        bool Clear;
        ClearValue Value;
    };

    /**
     * Render Pass Descriptor - describes a render pass with multiple render targets
     */
    struct RenderPassDesc
    {
        const RenderTargetDesc *Color;
        std::uint32_t ColorCount;

        const RenderTargetDesc *Depth;
        std::uint32_t DepthCount;
    };

    class Instance
    {
    public:
        virtual ~Instance() = default;

        virtual std::uint32_t EnumeratePhysicalDevices(PhysicalDevice **devices) = 0;
    };

    class PhysicalDevice
    {
    public:
        virtual ~PhysicalDevice() = default;

        virtual Device *CreateDevice() = 0;

        [[nodiscard]] virtual bool Supports(DeviceFeature feature) const = 0;
        [[nodiscard]] virtual const DeviceLimits &GetLimits() const = 0;
    };

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual Buffer *CreateBuffer(const BufferDesc &desc) = 0;
        virtual Image *CreateImage(const ImageDesc &desc) = 0;
        virtual Sampler *CreateSampler(const SamplerDesc &desc) = 0;

        virtual ShaderModule *CreateShaderModule(const ShaderModuleDesc &desc) = 0;
        virtual Pipeline *CreatePipeline(const PipelineDesc &desc) = 0;
        virtual Swapchain *CreateSwapchain(const SwapchainDesc &desc) = 0;

        virtual CommandBuffer *CreateCommandBuffer(CommandBufferUsage usage) = 0;

        virtual Fence *CreateFence() = 0;

        virtual Queue *GetQueue(QueueType type) = 0;

        [[nodiscard]] virtual bool Supports(DeviceFeature feature) const = 0;
        [[nodiscard]] virtual const DeviceLimits &GetLimits() const = 0;
    };

    class Queue
    {
    public:
        virtual ~Queue() = default;

        virtual void Submit(
            const CommandBuffer *command_buffer,
            std::uint32_t command_buffer_count,
            Fence *fence) = 0;

        virtual void Present(const Swapchain *swapchain) = 0;
    };

    class ShaderModule
    {
    public:
        virtual ~ShaderModule() = default;

        [[nodiscard]] virtual ShaderStage GetStage() const = 0;
    };

    class Pipeline
    {
    public:
        virtual ~Pipeline() = default;

        [[nodiscard]] virtual PipelineType GetType() const = 0;
    };

    class Resource
    {
    public:
        virtual ~Resource() = default;
    };

    class Buffer : public Resource
    {
    public:
        [[nodiscard]] virtual std::size_t GetSize() const = 0;
        [[nodiscard]] virtual BufferUsage GetUsage() const = 0;

        virtual void *Map() = 0;
        virtual void Unmap() = 0;
    };

    class Image : public Resource
    {
    public:
        [[nodiscard]] virtual ImageFormat GetFormat() const = 0;
        [[nodiscard]] virtual Extent3D GetExtent() const = 0;
        [[nodiscard]] virtual std::uint32_t GetMipLevels() const = 0;
    };

    class ImageView
    {
    public:
        virtual ~ImageView() = default;

        [[nodiscard]] virtual const Image *GetImage() const = 0;
    };

    class Sampler
    {
    public:
        virtual ~Sampler() = default;
    };

    class Swapchain
    {
    public:
        virtual ~Swapchain() = default;

        [[nodiscard]] virtual std::uint32_t GetImageCount() const = 0;
        [[nodiscard]] virtual ImageView *GetImageView(std::uint32_t index) const = 0;
        [[nodiscard]] virtual Extent2D GetExtent() const = 0;

        virtual std::uint32_t AcquireNextImage(Fence *fence) = 0;
        virtual void Present() const = 0;
    };

    class CommandBuffer
    {
    public:
        virtual ~CommandBuffer() = default;

        virtual void Begin() = 0;
        virtual void End() = 0;

        virtual void BeginRenderPass(const RenderPassDesc &desc) = 0;
        virtual void EndRenderPass() = 0;

        virtual void SetPipeline(const Pipeline *pipeline) = 0;

        virtual void SetViewport(float x, float y, float w, float h) = 0;
        virtual void SetScissor(int x, int y, int w, int h) = 0;

        virtual void BindVertexBuffer(const Buffer *buffer, std::size_t offset) = 0;
        virtual void BindIndexBuffer(const Buffer *buffer, IndexType type) = 0;

        virtual void BindBuffer(
            std::uint32_t slot,
            ShaderStage stages,
            const Buffer *buffer) = 0;
        virtual void BindImageView(
            std::uint32_t slot,
            ShaderStage stages,
            const ImageView *image_view,
            const Sampler *sampler) = 0;

        virtual void Draw(std::uint32_t vertex_count, std::uint32_t first_vertex) = 0;
        virtual void DrawIndexed(std::uint32_t index_count, std::uint32_t first_index) = 0;

        virtual void Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z) = 0;

        virtual void CopyBuffer(
            const Buffer *src_buffer,
            const Buffer *dst_buffer,
            std::size_t src_offset,
            std::size_t dst_offset,
            std::size_t size) = 0;
        virtual void CopyBufferToImage(const Buffer *src_buffer, const Image *dst_image) = 0;

        virtual void Transition(const Resource *resource, ResourceState state) = 0;
    };

    class Fence
    {
    public:
        virtual ~Fence() = default;

        virtual void Wait() = 0;
        virtual void Reset() = 0;
    };

    Instance *CreateInstanceOpenGL(const InstanceDesc &desc);

    void DestroyInstance(const Instance *instance);
}

template<>
struct std::formatter<fxng::glal::MemoryUsage> : std::formatter<std::string>
{
    template<typename Context>
    auto format(const fxng::glal::MemoryUsage &usage, Context &context) const
    {
        static const std::unordered_map<fxng::glal::MemoryUsage, const char *> map
        {
            { fxng::glal::MemoryUsage_None, "None" },
            { fxng::glal::MemoryUsage_DeviceLocal, "DeviceLocal" },
            { fxng::glal::MemoryUsage_HostToDevice, "HostToDevice" },
            { fxng::glal::MemoryUsage_DeviceToHost, "DeviceToHost" },
        };

        return std::formatter<std::string>::format(map.at(usage), context);
    }
};
