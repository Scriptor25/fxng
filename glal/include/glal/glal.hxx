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
    class InstanceT
    {
    public:
        virtual ~InstanceT() = default;

        virtual std::uint32_t EnumeratePhysicalDevices(PhysicalDevice *devices) = 0;
    };

    class PhysicalDeviceT
    {
    public:
        virtual ~PhysicalDeviceT() = default;

        virtual Device CreateDevice() = 0;
        virtual void DestroyDevice(Device device) = 0;

        [[nodiscard]] virtual bool Supports(DeviceFeature feature) const = 0;
        [[nodiscard]] virtual const DeviceLimits &GetLimits() const = 0;
    };

    class DeviceT
    {
    public:
        virtual ~DeviceT() = default;

        virtual Buffer CreateBuffer(const BufferDesc &desc) = 0;
        virtual void DestroyBuffer(Buffer buffer) = 0;

        virtual Image CreateImage(const ImageDesc &desc) = 0;
        virtual void DestroyImage(Image image) = 0;

        virtual ImageView CreateImageView(const ImageViewDesc &desc) = 0;
        virtual void DestroyImageView(ImageView image_view) = 0;

        virtual Sampler CreateSampler(const SamplerDesc &desc) = 0;
        virtual void DestroySampler(Sampler sampler) = 0;

        virtual ShaderModule CreateShaderModule(const ShaderModuleDesc &desc) = 0;
        virtual void DestroyShaderModule(ShaderModule shader_module) = 0;

        virtual PipelineLayout CreatePipelineLayout(const PipelineLayoutDesc &desc) = 0;
        virtual void DestroyPipelineLayout(PipelineLayout pipeline_layout) = 0;

        virtual Pipeline CreatePipeline(const PipelineDesc &desc) = 0;
        virtual void DestroyPipeline(Pipeline pipeline) = 0;

        virtual DescriptorSetLayout CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc) = 0;
        virtual void DestroyDescriptorSetLayout(DescriptorSetLayout descriptor_set_layout) = 0;

        virtual DescriptorSet CreateDescriptorSet(const DescriptorSetDesc &desc) = 0;
        virtual void DestroyDescriptorSet(DescriptorSet descriptor_set) = 0;

        virtual Swapchain CreateSwapchain(const SwapchainDesc &desc) = 0;
        virtual void DestroySwapchain(Swapchain swapchain) = 0;

        virtual CommandBuffer CreateCommandBuffer(CommandBufferUsage usage) = 0;
        virtual void DestroyCommandBuffer(CommandBuffer command_buffer) = 0;

        virtual Fence CreateFence() = 0;
        virtual void DestroyFence(Fence fence) = 0;

        virtual Queue GetQueue(QueueType type) = 0;

        [[nodiscard]] virtual bool Supports(DeviceFeature feature) const = 0;
        [[nodiscard]] virtual const DeviceLimits &GetLimits() const = 0;
    };

    class ResourceT
    {
    public:
        virtual ~ResourceT() = default;
    };

    class BufferT : public ResourceT
    {
    public:
        [[nodiscard]] virtual std::size_t GetSize() const = 0;
        [[nodiscard]] virtual BufferUsage GetUsage() const = 0;

        virtual void *Map() = 0;
        virtual void Unmap() = 0;
    };

    class ImageT : public ResourceT
    {
    public:
        [[nodiscard]] virtual ImageFormat GetFormat() const = 0;
        [[nodiscard]] virtual ImageType GetType() const = 0;
        [[nodiscard]] virtual Extent3D GetExtent() const = 0;
        [[nodiscard]] virtual std::uint32_t GetMipLevelCount() const = 0;
        [[nodiscard]] virtual std::uint32_t GetArrayLayerCount() const = 0;
    };

    class ImageViewT
    {
    public:
        virtual ~ImageViewT() = default;

        [[nodiscard]] virtual Image GetImage() const = 0;
        [[nodiscard]] virtual ImageFormat GetFormat() const = 0;
        [[nodiscard]] virtual ImageType GetType() const = 0;
    };

    class SamplerT
    {
    public:
        virtual ~SamplerT() = default;
    };

    class PipelineLayoutT
    {
    public:
        virtual ~PipelineLayoutT() = default;

        [[nodiscard]] virtual DescriptorSetLayout GetDescriptorSetLayout(std::uint32_t index) const = 0;
        [[nodiscard]] virtual std::uint32_t GetDescriptorSetLayoutCount() const = 0;
    };

    class PipelineT
    {
    public:
        virtual ~PipelineT() = default;

        [[nodiscard]] virtual PipelineType GetType() const = 0;
    };

    class ShaderModuleT
    {
    public:
        virtual ~ShaderModuleT() = default;

        [[nodiscard]] virtual ShaderStage GetStage() const = 0;
    };

    class SwapchainT
    {
    public:
        virtual ~SwapchainT() = default;

        [[nodiscard]] virtual std::uint32_t GetImageCount() const = 0;
        [[nodiscard]] virtual ImageView GetImageView(std::uint32_t index) const = 0;
        [[nodiscard]] virtual Extent2D GetExtent() const = 0;

        virtual std::uint32_t AcquireNextImage(Fence fence) = 0;
        virtual void Present() const = 0;
    };

    class DescriptorSetLayoutT
    {
    public:
        virtual ~DescriptorSetLayoutT() = default;

        [[nodiscard]] virtual std::uint32_t GetSet() const = 0;
        [[nodiscard]] virtual std::uint32_t GetDescriptorBindingCount() const = 0;
        [[nodiscard]] virtual const DescriptorBinding &GetDescriptorBinding(std::uint32_t index) const = 0;
    };

    class DescriptorSetT
    {
    public:
        virtual ~DescriptorSetT() = default;

        virtual void BindBuffer(
            std::uint32_t binding,
            Buffer buffer) = 0;

        virtual void BindBuffer(
            std::uint32_t binding,
            Buffer buffer,
            std::uint32_t offset,
            std::uint32_t size) = 0;

        virtual void BindImageView(
            std::uint32_t binding,
            ImageView image_view,
            Sampler sampler) = 0;
    };

    class CommandBufferT
    {
    public:
        virtual ~CommandBufferT() = default;

        virtual void Begin() = 0;
        virtual void End() = 0;

        virtual void BeginRenderPass(const RenderPassDesc &desc) = 0;
        virtual void EndRenderPass() = 0;

        virtual void SetViewport(float x, float y, float w, float h) = 0;
        virtual void SetScissor(int x, int y, int w, int h) = 0;

        virtual void BindPipeline(Pipeline pipeline) = 0;
        virtual void BindVertexBuffer(Buffer buffer, std::size_t offset) = 0;
        virtual void BindIndexBuffer(Buffer buffer, DataType type) = 0;
        virtual void BindDescriptorSets(
            std::uint32_t first_set,
            std::uint32_t set_count,
            const DescriptorSet *descriptor_sets) = 0;

        virtual void Draw(std::uint32_t vertex_count, std::uint32_t first_vertex) = 0;
        virtual void DrawIndexed(std::uint32_t index_count, std::uint32_t first_index) = 0;

        virtual void Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z) = 0;

        virtual void CopyBuffer(
            Buffer src_buffer,
            Buffer dst_buffer,
            std::size_t src_offset,
            std::size_t dst_offset,
            std::size_t size) = 0;
        virtual void CopyBufferToImage(Buffer src_buffer, Image dst_image) = 0;

        virtual void Transition(Resource resource, ResourceState state) = 0;
    };

    class FenceT
    {
    public:
        virtual ~FenceT() = default;

        virtual void Wait() = 0;
        virtual void Reset() = 0;
    };

    class QueueT
    {
    public:
        virtual ~QueueT() = default;

        virtual void Submit(
            CommandBuffer command_buffer,
            std::uint32_t command_buffer_count,
            Fence fence) = 0;

        virtual void Present(Swapchain swapchain) = 0;
    };

    Instance CreateInstanceOpenGL(const InstanceDesc &desc);
    Instance CreateInstanceVulkan(const InstanceDesc &desc);

    void DestroyInstance(Instance instance);
}
