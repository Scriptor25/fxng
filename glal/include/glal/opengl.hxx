#pragma once

#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <glal/glal.hxx>

namespace glal::opengl
{
    class Instance;
    class PhysicalDevice;
    class Device;

    class Buffer;
    class Image;
    class ImageView;
    class Sampler;
    class ShaderModule;
    class PipelineLayout;
    class Pipeline;
    class DescriptorSetLayout;
    class DescriptorSet;
    class Swapchain;
    class CommandBuffer;
    class Fence;
    class Queue;

    class Instance final : public glal::Instance
    {
    public:
        explicit Instance(const InstanceDesc &desc);
        ~Instance() override;

        std::uint32_t EnumeratePhysicalDevices(Ptr<Ptr<glal::PhysicalDevice>> devices) override;

    private:
        Ptr<PhysicalDevice> m_PhysicalDevices;
        std::uint32_t m_PhysicalDeviceCount;
    };

    class PhysicalDevice final : public glal::PhysicalDevice
    {
        friend Instance;

    public:
        ~PhysicalDevice() override;

        Ptr<glal::Device> CreateDevice() override;
        void DestroyDevice(Ptr<glal::Device> device) override;

        [[nodiscard]] bool Supports(DeviceFeature feature) const override;
        [[nodiscard]] const DeviceLimits &GetLimits() const override;

    protected:
        explicit PhysicalDevice(Ptr<Instance> instance);

    private:
        Ptr<Instance> m_Instance;
        DeviceLimits m_Limits;

        std::vector<Ptr<Device>> m_Devices;
    };

    class Device final : public glal::Device
    {
        friend PhysicalDevice;

    public:
        ~Device() override;

        Ptr<glal::Buffer> CreateBuffer(const BufferDesc &desc) override;
        void DestroyBuffer(Ptr<glal::Buffer> buffer) override;

        Ptr<glal::Image> CreateImage(const ImageDesc &desc) override;
        void DestroyImage(Ptr<glal::Image> image) override;

        Ptr<glal::Sampler> CreateSampler(const SamplerDesc &desc) override;
        void DestroySampler(Ptr<glal::Sampler> sampler) override;

        Ptr<glal::ShaderModule> CreateShaderModule(const ShaderModuleDesc &desc) override;
        void DestroyShaderModule(Ptr<glal::ShaderModule> shader_module) override;

        Ptr<glal::PipelineLayout> CreatePipelineLayout(const PipelineLayoutDesc &desc) override;
        void DestroyPipelineLayout(Ptr<glal::PipelineLayout> pipeline_layout) override;

        Ptr<glal::Pipeline> CreatePipeline(const PipelineDesc &desc) override;
        void DestroyPipeline(Ptr<glal::Pipeline> pipeline) override;

        Ptr<glal::DescriptorSetLayout> CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc) override;
        void DestroyDescriptorSetLayout(Ptr<glal::DescriptorSetLayout> descriptor_set_layout) override;

        Ptr<glal::DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc &desc) override;
        void DestroyDescriptorSet(Ptr<glal::DescriptorSet> descriptor_set) override;

        Ptr<glal::Swapchain> CreateSwapchain(const SwapchainDesc &desc) override;
        void DestroySwapchain(Ptr<glal::Swapchain> swapchain) override;

        Ptr<glal::CommandBuffer> CreateCommandBuffer(CommandBufferUsage usage) override;
        void DestroyCommandBuffer(Ptr<glal::CommandBuffer> command_buffer) override;

        Ptr<glal::Fence> CreateFence() override;
        void DestroyFence(Ptr<glal::Fence> fence) override;

        Ptr<glal::Queue> GetQueue(QueueType type) override;

        [[nodiscard]] bool Supports(DeviceFeature feature) const override;
        [[nodiscard]] const DeviceLimits &GetLimits() const override;

    protected:
        explicit Device(Ptr<PhysicalDevice> physical_device);

    private:
        Ptr<PhysicalDevice> m_PhysicalDevice;

        std::vector<Ptr<Buffer>> m_Buffers;
        std::vector<Ptr<Image>> m_Images;
        std::vector<Ptr<Sampler>> m_Samplers;
        std::vector<Ptr<ShaderModule>> m_ShaderModules;
        std::vector<Ptr<PipelineLayout>> m_PipelineLayouts;
        std::vector<Ptr<Pipeline>> m_Pipelines;
        std::vector<Ptr<DescriptorSetLayout>> m_DescriptorSetLayouts;
        std::vector<Ptr<DescriptorSet>> m_DescriptorSets;
        std::vector<Ptr<Swapchain>> m_Swapchains;
        std::vector<Ptr<CommandBuffer>> m_CommandBuffers;
        std::vector<Ptr<Fence>> m_Fences;

        Ptr<Queue> m_Queue;
    };

    class Queue final : public glal::Queue
    {
        friend Device;

    public:
        void Submit(
            Ptr<glal::CommandBuffer> command_buffer,
            std::uint32_t command_buffer_count,
            Ptr<glal::Fence> fence) override;
        void Present(Ptr<glal::Swapchain> swapchain) override;

    protected:
        explicit Queue(Ptr<Device> device);

    private:
        Ptr<Device> m_Device;
    };

    class DescriptorSetLayout final : public glal::DescriptorSetLayout
    {
        friend Device;
        friend Pipeline;
        friend DescriptorSet;

    public:
        [[nodiscard]] std::uint32_t GetSet() const override;
        [[nodiscard]] std::uint32_t GetDescriptorBindingCount() const override;
        [[nodiscard]] const DescriptorBinding &GetDescriptorBinding(std::uint32_t index) const override;

    protected:
        explicit DescriptorSetLayout(Ptr<Device> device, const DescriptorSetLayoutDesc &desc);

        [[nodiscard]] std::vector<DescriptorBinding>::const_iterator begin() const;
        [[nodiscard]] std::vector<DescriptorBinding>::const_iterator end() const;

    private:
        Ptr<Device> m_Device;

        std::uint32_t m_Set;
        std::vector<DescriptorBinding> m_DescriptorBindings;
    };

    struct BufferBinding
    {
        GLenum Target;
        Ptr<const Buffer> BufferImpl;
        bool Base;
        GLsizeiptr Offset;
        GLintptr Size;
    };

    struct ImageBinding
    {
        Ptr<const ImageView> ImageViewImpl;
        Ptr<const Sampler> SamplerImpl;
    };

    class DescriptorSet final : public glal::DescriptorSet
    {
        friend Device;
        friend CommandBuffer;

    public:
        void BindBuffer(
            std::uint32_t binding,
            Ptr<glal::Buffer> buffer) override;

        void BindBuffer(
            std::uint32_t binding,
            Ptr<glal::Buffer> buffer,
            std::uint32_t offset,
            std::uint32_t size) override;

        void BindImageView(
            std::uint32_t binding,
            Ptr<glal::ImageView> image_view,
            Ptr<glal::Sampler> sampler) override;

    protected:
        explicit DescriptorSet(Ptr<Device> device, const DescriptorSetDesc &desc);

        void Bind(std::uint32_t index) const;

    private:
        Ptr<Device> m_Device;
        std::vector<Ptr<DescriptorSetLayout>> m_Layouts;

        std::unordered_map<GLuint, BufferBinding> m_BufferBindings;
        std::unordered_map<GLuint, ImageBinding> m_ImageBindings;
    };

    class CommandBuffer final : public glal::CommandBuffer
    {
        friend Device;

    public:
        void Begin() override;
        void End() override;

        void BeginRenderPass(const RenderPassDesc &desc) override;
        void EndRenderPass() override;

        void SetViewport(float x, float y, float w, float h) override;
        void SetScissor(int x, int y, int w, int h) override;

        void BindPipeline(Ptr<glal::Pipeline> pipeline) override;
        void BindVertexBuffer(Ptr<glal::Buffer> buffer, std::size_t offset) override;
        void BindIndexBuffer(Ptr<glal::Buffer> buffer, DataType type) override;
        void BindDescriptorSets(
            std::uint32_t first_set,
            std::uint32_t set_count,
            Ptr<const Ptr<glal::DescriptorSet>> descriptor_sets) override;

        void Draw(std::uint32_t vertex_count, std::uint32_t first_vertex) override;
        void DrawIndexed(std::uint32_t index_count, std::uint32_t first_index) override;

        void Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z) override;

        void CopyBuffer(
            Ptr<glal::Buffer> src_buffer,
            Ptr<glal::Buffer> dst_buffer,
            std::size_t src_offset,
            std::size_t dst_offset,
            std::size_t size) override;
        void CopyBufferToImage(Ptr<glal::Buffer> src_buffer, Ptr<glal::Image> dst_image) override;

        void Transition(Ptr<Resource> resource, ResourceState state) override;

    protected:
        explicit CommandBuffer(Ptr<Device> device, CommandBufferUsage usage);

    private:
        Ptr<Device> m_Device;
        CommandBufferUsage m_Usage;

        Ptr<Pipeline> m_Pipeline;

        std::uint32_t m_VertexArray;
        std::uint32_t m_Framebuffer;

        DataType m_IndexType;
    };

    class Fence final : public glal::Fence
    {
        friend Device;

    public:
        void Wait() override;
        void Reset() override;

    protected:
        explicit Fence(Ptr<Device> device);

    private:
        Ptr<Device> m_Device;
        GLsync m_Sync;
    };

    class Buffer final : public glal::Buffer
    {
        friend Device;
        friend CommandBuffer;
        friend DescriptorSet;

    public:
        ~Buffer() override;

        [[nodiscard]] std::size_t GetSize() const override;
        [[nodiscard]] BufferUsage GetUsage() const override;

        void *Map() override;
        void Unmap() override;

    protected:
        explicit Buffer(Ptr<Device> device, const BufferDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Ptr<Device> m_Device;

        std::size_t m_Size;
        BufferUsage m_Usage;
        MemoryUsage m_Memory;

        std::uint32_t m_Handle;
    };

    class Image final : public glal::Image
    {
        friend Device;
        friend CommandBuffer;
        friend ImageView;
        friend Swapchain;

    public:
        ~Image() override;

        [[nodiscard]] ImageFormat GetFormat() const override;
        [[nodiscard]] ImageDimension GetDimension() const override;
        [[nodiscard]] Extent3D GetExtent() const override;
        [[nodiscard]] std::uint32_t GetMipLevelCount() const override;
        [[nodiscard]] std::uint32_t GetArrayLayerCount() const override;

    protected:
        explicit Image(Ptr<Device> device, const ImageDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Ptr<Device> m_Device;

        ImageFormat m_Format;
        ImageDimension m_Dimension;
        Extent3D m_Extent;
        std::uint32_t m_MipLevelCount;
        std::uint32_t m_ArrayLayerCount;

        std::uint32_t m_Handle;
    };

    class ImageView final : public glal::ImageView
    {
        friend CommandBuffer;
        friend Swapchain;
        friend DescriptorSet;

    public:
        [[nodiscard]] Ptr<glal::Image> GetImage() const override;

    protected:
        explicit ImageView(Ptr<Image> image);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Ptr<Image> m_Image;
    };

    class Sampler final : public glal::Sampler
    {
        friend Device;
        friend CommandBuffer;
        friend DescriptorSet;

    public:
        ~Sampler() override;

    protected:
        explicit Sampler(Ptr<Device> device, const SamplerDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Ptr<Device> m_Device;

        std::uint32_t m_Handle;
    };

    class Swapchain final : public glal::Swapchain
    {
        friend Device;

    public:
        ~Swapchain() override;

        [[nodiscard]] std::uint32_t GetImageCount() const override;
        [[nodiscard]] Ptr<glal::ImageView> GetImageView(std::uint32_t index) const override;
        [[nodiscard]] Extent2D GetExtent() const override;

        std::uint32_t AcquireNextImage(Ptr<glal::Fence> fence) override;
        void Present() const override;

    protected:
        explicit Swapchain(Ptr<Device> device, const SwapchainDesc &desc);

    private:
        Ptr<Device> m_Device;

        Extent2D m_Extent;
        std::uint32_t m_ImageCount;
        std::uint32_t m_ImageIndex;
        std::vector<Ptr<Image>> m_Images;
        std::vector<Ptr<glal::Fence>> m_Fences;
        std::vector<ImageView *> m_ImageViews;

        void *m_NativeWindowHandle;
    };

    class ShaderModule final : public glal::ShaderModule
    {
        friend Device;
        friend Pipeline;

    public:
        ~ShaderModule() override;

        [[nodiscard]] ShaderStage GetStage() const override;

    protected:
        explicit ShaderModule(Ptr<Device> device, const ShaderModuleDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Ptr<Device> m_Device;

        ShaderStage m_Stage;

        std::uint32_t m_Handle;
    };

    class PipelineLayout final : public glal::PipelineLayout
    {
        friend Device;
        friend Pipeline;

    public:
        [[nodiscard]] Ptr<glal::DescriptorSetLayout> GetDescriptorSetLayout(std::uint32_t index) const override;
        [[nodiscard]] std::uint32_t GetDescriptorSetLayoutCount() const override;

    protected:
        explicit PipelineLayout(Ptr<Device> device, const PipelineLayoutDesc &desc);

        [[nodiscard]] std::vector<Ptr<DescriptorSetLayout>>::const_iterator begin() const;
        [[nodiscard]] std::vector<Ptr<DescriptorSetLayout>>::const_iterator end() const;

    private:
        Ptr<Device> m_Device;

        std::vector<Ptr<DescriptorSetLayout>> m_DescriptorSetLayouts;
    };

    class Pipeline final : public glal::Pipeline
    {
        friend Device;
        friend CommandBuffer;

    public:
        ~Pipeline() override;

        [[nodiscard]] PipelineType GetType() const override;

    protected:
        explicit Pipeline(Ptr<Device> device, const PipelineDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;
        [[nodiscard]] std::uint32_t GetVertexStride() const;

        void LayoutVertexArray(std::uint32_t vertex_array) const;

    private:
        Ptr<Device> m_Device;
        Ptr<PipelineLayout> m_Layout;

        PipelineType m_Type;
        std::vector<VertexAttribute> m_VertexAttributes;
        std::uint32_t m_VertexStride;

        std::uint32_t m_Handle;
    };

    void TranslateImageFormat(
        ImageFormat image_format,
        Ptr<GLenum> internal_format,
        Ptr<GLenum> external_format,
        Ptr<GLenum> type);

    void TranslateDataType(
        DataType data_type,
        Ptr<std::uint32_t> size,
        Ptr<GLenum> type,
        Ptr<GLboolean> normalized);
}
