#pragma once

#include <vector>
#include <glal/glal.hxx>
#include <vulkan/vulkan.h>

namespace glal::vulkan
{
    class InstanceT;
    class PhysicalDeviceT;
    class DeviceT;

    class BufferT;
    class ImageT;
    class ImageViewT;
    class SamplerT;
    class ShaderModuleT;
    class PipelineLayoutT;
    class PipelineT;
    class DescriptorSetLayoutT;
    class DescriptorSetT;
    class SwapchainT;
    class CommandBufferT;
    class FenceT;
    class QueueT;

    class InstanceT final : public glal::InstanceT
    {
    public:
        explicit InstanceT(const InstanceDesc &desc);
        ~InstanceT() override;

        std::uint32_t EnumeratePhysicalDevices(PhysicalDevice *devices) override;

    private:
        VkInstance m_Handle;
        std::vector<PhysicalDeviceT> m_PhysicalDevices;
    };

    class PhysicalDeviceT final : public glal::PhysicalDeviceT
    {
    public:
        explicit PhysicalDeviceT(InstanceT *instance, VkPhysicalDevice handle);

        Device CreateDevice() override;
        void DestroyDevice(Device device) override;

        [[nodiscard]] bool Supports(DeviceFeature feature) const override;
        [[nodiscard]] const DeviceLimits &GetLimits() const override;

        VkPhysicalDevice GetHandle() const;

    private:
        InstanceT *m_Instance;
        std::vector<DeviceT *> m_Devices;

        VkPhysicalDevice m_Handle;
    };

    class DeviceT final : public glal::DeviceT
    {
    public:
        explicit DeviceT(PhysicalDeviceT *physical_device);
        ~DeviceT() override;

        Buffer CreateBuffer(const BufferDesc &desc) override;
        void DestroyBuffer(Buffer buffer) override;

        ImageView CreateImageView(const ImageViewDesc &desc) override;
        void DestroyImageView(ImageView image_view) override;

        Image CreateImage(const ImageDesc &desc) override;
        void DestroyImage(Image image) override;

        Sampler CreateSampler(const SamplerDesc &desc) override;
        void DestroySampler(Sampler sampler) override;

        ShaderModule CreateShaderModule(const ShaderModuleDesc &desc) override;
        void DestroyShaderModule(ShaderModule shader_module) override;

        PipelineLayout CreatePipelineLayout(const PipelineLayoutDesc &desc) override;
        void DestroyPipelineLayout(PipelineLayout pipeline_layout) override;

        Pipeline CreatePipeline(const PipelineDesc &desc) override;
        void DestroyPipeline(Pipeline pipeline) override;

        DescriptorSetLayout CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc) override;
        void DestroyDescriptorSetLayout(DescriptorSetLayout descriptor_set_layout) override;

        DescriptorSet CreateDescriptorSet(const DescriptorSetDesc &desc) override;
        void DestroyDescriptorSet(DescriptorSet descriptor_set) override;

        Swapchain CreateSwapchain(const SwapchainDesc &desc) override;
        void DestroySwapchain(Swapchain swapchain) override;

        CommandBuffer CreateCommandBuffer(CommandBufferUsage usage) override;
        void DestroyCommandBuffer(CommandBuffer command_buffer) override;

        Fence CreateFence() override;
        void DestroyFence(Fence fence) override;

        Queue GetQueue(QueueType type) override;

        [[nodiscard]] bool Supports(DeviceFeature feature) const override;
        [[nodiscard]] const DeviceLimits &GetLimits() const override;

        PhysicalDeviceT *GetPhysicalDevice() const;
        VkDevice GetHandle() const;

    private:
        PhysicalDeviceT *m_PhysicalDevice;

        std::vector<BufferT *> m_Buffers;
        std::vector<ImageT *> m_Images;
        std::vector<ImageViewT *> m_ImageViews;
        std::vector<SamplerT *> m_Samplers;
        std::vector<ShaderModuleT *> m_ShaderModules;
        std::vector<PipelineLayoutT *> m_PipelineLayouts;
        std::vector<PipelineT *> m_Pipelines;
        std::vector<DescriptorSetLayoutT *> m_DescriptorSetLayouts;
        std::vector<DescriptorSetT *> m_DescriptorSets;
        std::vector<SwapchainT *> m_Swapchains;
        std::vector<CommandBufferT *> m_CommandBuffers;
        std::vector<FenceT *> m_Fences;

        std::vector<QueueT *> m_Queues;

        VkDevice m_Handle;
    };

    class BufferT final : public glal::BufferT
    {
    public:
        explicit BufferT(DeviceT *device, const BufferDesc &desc);

        [[nodiscard]] std::size_t GetSize() const override;
        [[nodiscard]] BufferUsage GetUsage() const override;

        void *Map() override;
        void Unmap() override;

    private:
        DeviceT *m_Device;

        std::size_t m_Size;
        BufferUsage m_Usage;

        VkBuffer m_Handle;
        VkDeviceMemory m_MemoryHandle;
    };

    class ImageT final : public glal::ImageT
    {
    public:
        explicit ImageT(DeviceT *device, const ImageDesc &desc);

        [[nodiscard]] ImageFormat GetFormat() const override;
        [[nodiscard]] ImageType GetType() const override;
        [[nodiscard]] Extent3D GetExtent() const override;
        [[nodiscard]] std::uint32_t GetMipLevelCount() const override;
        [[nodiscard]] std::uint32_t GetArrayLayerCount() const override;

        VkImage GetHandle() const;

    private:
        DeviceT *m_Device;

        ImageFormat m_Format;
        ImageType m_Type;
        Extent3D m_Extent;
        std::uint32_t m_MipLevelCount;
        std::uint32_t m_ArrayLayerCount;

        VkImage m_Handle;
    };

    class ImageViewT final : public glal::ImageViewT
    {
    public:
        explicit ImageViewT(DeviceT *device, const ImageViewDesc &desc);

        [[nodiscard]] Image GetImage() const override;
        [[nodiscard]] ImageFormat GetFormat() const override;
        [[nodiscard]] ImageType GetType() const override;

    private:
        DeviceT *m_Device;
        ImageT *m_Image;

        ImageFormat m_Format;
        ImageType m_Type;

        VkImageView m_Handle;
    };

    class SamplerT final : public glal::SamplerT
    {
    public:
        explicit SamplerT(DeviceT *device, const SamplerDesc &desc);

    private:
        DeviceT *m_Device;

        VkSampler m_Handle;
    };

    class ShaderModuleT final : public glal::ShaderModuleT
    {
    public:
        explicit ShaderModuleT(DeviceT *device, const ShaderModuleDesc &desc);
        ~ShaderModuleT() override;

        [[nodiscard]] ShaderStage GetStage() const override;

        VkShaderModule GetHandle() const;

    private:
        DeviceT *m_Device;

        ShaderStage m_Stage;

        VkShaderModule m_Handle;
    };

    class PipelineLayoutT final : public glal::PipelineLayoutT
    {
    public:
        explicit PipelineLayoutT(DeviceT *device, const PipelineLayoutDesc &desc);

        [[nodiscard]] DescriptorSetLayout GetDescriptorSetLayout(std::uint32_t index) const override;
        [[nodiscard]] std::uint32_t GetDescriptorSetLayoutCount() const override;

        VkPipelineLayout GetHandle() const;

    private:
        DeviceT *m_Device;

        VkPipelineLayout m_Handle;
    };

    class PipelineT final : public glal::PipelineT
    {
    public:
        explicit PipelineT(DeviceT *device, const PipelineDesc &desc);
        ~PipelineT() override;

        [[nodiscard]] PipelineType GetType() const override;
        [[nodiscard]] PrimitiveTopology GetTopology() const override;

        VkPipeline GetHandle() const;

    private:
        DeviceT *m_Device;

        PipelineType m_Type;
        PrimitiveTopology m_Topology;

        VkPipeline m_Handle;
    };

    class DescriptorSetLayoutT final : public glal::DescriptorSetLayoutT
    {
    public:
        explicit DescriptorSetLayoutT(DeviceT *device, const DescriptorSetLayoutDesc &desc);

        [[nodiscard]] std::uint32_t GetSet() const override;

        [[nodiscard]] std::uint32_t GetDescriptorBindingCount() const override;
        [[nodiscard]] const DescriptorBinding &GetDescriptorBinding(std::uint32_t index) const override;

    private:
        DeviceT *m_Device;
    };

    class DescriptorSetT final : public glal::DescriptorSetT
    {
    public:
        explicit DescriptorSetT(DeviceT *device, const DescriptorSetDesc &desc);

        void BindBuffer(std::uint32_t binding, Buffer buffer) override;
        void BindBuffer(
            std::uint32_t binding,
            Buffer buffer,
            std::uint32_t offset,
            std::uint32_t size) override;

        void BindImageView(
            std::uint32_t binding,
            ImageView image_view,
            Sampler sampler) override;

    private:
        DeviceT *m_Device;
    };

    class SwapchainT final : public glal::SwapchainT
    {
    public:
        explicit SwapchainT(DeviceT *device, const SwapchainDesc &desc);

        [[nodiscard]] std::uint32_t GetImageCount() const override;
        [[nodiscard]] ImageView GetImageView(std::uint32_t index) const override;

        [[nodiscard]] Extent2D GetExtent() const override;

        std::uint32_t AcquireNextImage(Fence fence) override;

        void Present() const override;

    private:
        DeviceT *m_Device;
    };

    class CommandBufferT final : public glal::CommandBufferT
    {
    public:
        explicit CommandBufferT(DeviceT *device, CommandBufferUsage usage);
        ~CommandBufferT() override;

        void Begin() override;
        void End() override;

        void BeginRenderPass(const RenderPassDesc &desc) override;
        void EndRenderPass() override;

        void SetViewport(float x, float y, float width, float height, float min_depth, float max_depth) override;
        void SetScissor(std::int32_t x, std::int32_t y, std::uint32_t width, std::uint32_t height) override;

        void BindPipeline(Pipeline pipeline) override;

        void BindVertexBuffer(Buffer buffer, std::uint32_t binding, std::size_t offset) override;
        void BindIndexBuffer(Buffer buffer, DataType type) override;

        void BindDescriptorSets(
            std::uint32_t first_set,
            std::uint32_t set_count,
            const DescriptorSet *descriptor_sets) override;

        void Draw(std::uint32_t vertex_count, std::uint32_t first_vertex) override;
        void DrawIndexed(std::uint32_t index_count, std::uint32_t first_index) override;

        void Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z) override;

        void CopyBuffer(
            Buffer src_buffer,
            Buffer dst_buffer,
            std::size_t src_offset,
            std::size_t dst_offset,
            std::size_t size) override;
        void CopyBufferToImage(Buffer src_buffer, Image dst_image) override;

        void Transition(Resource resource, ResourceState state) override;

    private:
        DeviceT *m_Device;

        VkCommandPool m_PoolHandle;
        VkCommandBuffer m_Handle;
    };

    class FenceT final : public glal::FenceT
    {
    public:
        explicit FenceT(DeviceT *device);
        ~FenceT() override;

        void Wait() override;
        void Reset() override;

    private:
        DeviceT *m_Device;

        VkFence m_Handle;
    };

    class QueueT final : public glal::QueueT
    {
    public:
        void Submit(
            const CommandBuffer *command_buffers,
            std::uint32_t command_buffer_count,
            Fence fence) override;

        void Present(Swapchain swapchain) override;
    };

    VkFilter ToVkFilter(Filter filter);
    VkSamplerAddressMode ToVkAddressMode(AddressMode address_mode);
    VkPipelineBindPoint ToVkPipelineBindPoint(PipelineType pipeline_type);
    VkShaderStageFlagBits ToVkShaderStage(ShaderStage shader_stage);
    VkFormat ToVkFormat(DataType data_type, std::uint32_t count);
    VkPrimitiveTopology ToVkPrimitiveTopology(PrimitiveTopology primitive_topology);
}
