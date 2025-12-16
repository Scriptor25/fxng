#pragma once

#include <cstddef>
#include <cstdint>
#include <glal/common.hxx>
#include <glal/desc.hxx>
#include <glal/enum.hxx>
#include <glal/forward.hxx>

/**
 * GLAL - Graphics Library Abstraction Layer
 */
namespace glal
{
    class Instance
    {
    public:
        virtual ~Instance() = default;

        virtual std::uint32_t EnumeratePhysicalDevices(Ptr<Ptr<PhysicalDevice>> devices) = 0;
    };

    class PhysicalDevice
    {
    public:
        virtual ~PhysicalDevice() = default;

        virtual Ptr<Device> CreateDevice() = 0;
        virtual void DestroyDevice(Ptr<Device> device) = 0;

        [[nodiscard]] virtual bool Supports(DeviceFeature feature) const = 0;
        [[nodiscard]] virtual const DeviceLimits &GetLimits() const = 0;
    };

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual Ptr<Buffer> CreateBuffer(const BufferDesc &desc) = 0;
        virtual void DestroyBuffer(Ptr<Buffer> buffer) = 0;

        virtual Ptr<Image> CreateImage(const ImageDesc &desc) = 0;
        virtual void DestroyImage(Ptr<Image> image) = 0;

        virtual Ptr<Sampler> CreateSampler(const SamplerDesc &desc) = 0;
        virtual void DestroySampler(Ptr<Sampler> sampler) = 0;

        virtual Ptr<ShaderModule> CreateShaderModule(const ShaderModuleDesc &desc) = 0;
        virtual void DestroyShaderModule(Ptr<ShaderModule> shader_module) = 0;

        virtual Ptr<PipelineLayout> CreatePipelineLayout(const PipelineLayoutDesc &desc) = 0;
        virtual void DestroyPipelineLayout(Ptr<PipelineLayout> pipeline_layout) = 0;

        virtual Ptr<Pipeline> CreatePipeline(const PipelineDesc &desc) = 0;
        virtual void DestroyPipeline(Ptr<Pipeline> pipeline) = 0;

        virtual Ptr<DescriptorSetLayout> CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc) = 0;
        virtual void DestroyDescriptorSetLayout(Ptr<DescriptorSetLayout> descriptor_set_layout) = 0;

        virtual Ptr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc &desc) = 0;
        virtual void DestroyDescriptorSet(Ptr<DescriptorSet> descriptor_set) = 0;

        virtual Ptr<Swapchain> CreateSwapchain(const SwapchainDesc &desc) = 0;
        virtual void DestroySwapchain(Ptr<Swapchain> swapchain) = 0;

        virtual Ptr<CommandBuffer> CreateCommandBuffer(CommandBufferUsage usage) = 0;
        virtual void DestroyCommandBuffer(Ptr<CommandBuffer> command_buffer) = 0;

        virtual Ptr<Fence> CreateFence() = 0;
        virtual void DestroyFence(Ptr<Fence> fence) = 0;

        virtual Ptr<Queue> GetQueue(QueueType type) = 0;

        [[nodiscard]] virtual bool Supports(DeviceFeature feature) const = 0;
        [[nodiscard]] virtual const DeviceLimits &GetLimits() const = 0;
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
        [[nodiscard]] virtual ImageDimension GetDimension() const = 0;
        [[nodiscard]] virtual Extent3D GetExtent() const = 0;
        [[nodiscard]] virtual std::uint32_t GetMipLevelCount() const = 0;
        [[nodiscard]] virtual std::uint32_t GetArrayLayerCount() const = 0;
    };

    class ImageView
    {
    public:
        virtual ~ImageView() = default;

        [[nodiscard]] virtual Ptr<Image> GetImage() const = 0;
    };

    class Sampler
    {
    public:
        virtual ~Sampler() = default;
    };

    class PipelineLayout
    {
    public:
        virtual ~PipelineLayout() = default;

        [[nodiscard]] virtual Ptr<DescriptorSetLayout> GetDescriptorSetLayout(std::uint32_t index) const = 0;
        [[nodiscard]] virtual std::uint32_t GetDescriptorSetLayoutCount() const = 0;
    };

    class Pipeline
    {
    public:
        virtual ~Pipeline() = default;

        [[nodiscard]] virtual PipelineType GetType() const = 0;
    };

    class ShaderModule
    {
    public:
        virtual ~ShaderModule() = default;

        [[nodiscard]] virtual ShaderStage GetStage() const = 0;
    };

    class Swapchain
    {
    public:
        virtual ~Swapchain() = default;

        [[nodiscard]] virtual std::uint32_t GetImageCount() const = 0;
        [[nodiscard]] virtual Ptr<ImageView> GetImageView(std::uint32_t index) const = 0;
        [[nodiscard]] virtual Extent2D GetExtent() const = 0;

        virtual std::uint32_t AcquireNextImage(Ptr<Fence> fence) = 0;
        virtual void Present() const = 0;
    };

    class DescriptorSetLayout
    {
    public:
        virtual ~DescriptorSetLayout() = default;

        [[nodiscard]] virtual std::uint32_t GetSet() const = 0;
        [[nodiscard]] virtual std::uint32_t GetDescriptorBindingCount() const = 0;
        [[nodiscard]] virtual const DescriptorBinding &GetDescriptorBinding(std::uint32_t index) const = 0;
    };

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        virtual void BindBuffer(
            std::uint32_t binding,
            Ptr<Buffer> buffer) = 0;

        virtual void BindBuffer(
            std::uint32_t binding,
            Ptr<Buffer> buffer,
            std::uint32_t offset,
            std::uint32_t size) = 0;

        virtual void BindImageView(
            std::uint32_t binding,
            Ptr<ImageView> image_view,
            Ptr<Sampler> sampler) = 0;
    };

    class CommandBuffer
    {
    public:
        virtual ~CommandBuffer() = default;

        virtual void Begin() = 0;
        virtual void End() = 0;

        virtual void BeginRenderPass(const RenderPassDesc &desc) = 0;
        virtual void EndRenderPass() = 0;

        virtual void SetViewport(float x, float y, float w, float h) = 0;
        virtual void SetScissor(int x, int y, int w, int h) = 0;

        virtual void BindPipeline(Ptr<Pipeline> pipeline) = 0;
        virtual void BindVertexBuffer(Ptr<Buffer> buffer, std::size_t offset) = 0;
        virtual void BindIndexBuffer(Ptr<Buffer> buffer, DataType type) = 0;
        virtual void BindDescriptorSets(
            std::uint32_t first_set,
            std::uint32_t set_count,
            Ptr<const Ptr<DescriptorSet>> descriptor_sets) = 0;

        virtual void Draw(std::uint32_t vertex_count, std::uint32_t first_vertex) = 0;
        virtual void DrawIndexed(std::uint32_t index_count, std::uint32_t first_index) = 0;

        virtual void Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z) = 0;

        virtual void CopyBuffer(
            Ptr<Buffer> src_buffer,
            Ptr<Buffer> dst_buffer,
            std::size_t src_offset,
            std::size_t dst_offset,
            std::size_t size) = 0;
        virtual void CopyBufferToImage(Ptr<Buffer> src_buffer, Ptr<Image> dst_image) = 0;

        virtual void Transition(Ptr<Resource> resource, ResourceState state) = 0;
    };

    class Fence
    {
    public:
        virtual ~Fence() = default;

        virtual void Wait() = 0;
        virtual void Reset() = 0;
    };

    class Queue
    {
    public:
        virtual ~Queue() = default;

        virtual void Submit(
            Ptr<CommandBuffer> command_buffer,
            std::uint32_t command_buffer_count,
            Ptr<Fence> fence) = 0;

        virtual void Present(Ptr<Swapchain> swapchain) = 0;
    };

    Ptr<Instance> CreateInstanceOpenGL(const InstanceDesc &desc);

    void DestroyInstance(Ptr<Instance> instance);
}
