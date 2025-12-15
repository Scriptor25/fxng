#pragma once

#include <vector>
#include <fxng/glal.hxx>
#include <GL/glew.h>

namespace fxng::glal::opengl
{
    class Instance;
    class PhysicalDevice;
    class Device;
    class Queue;

    class DescriptorSetLayout;
    class DescriptorSet;

    class PipelineLayout;
    class Pipeline;

    class Buffer;
    class Image;
    class ImageView;
    class Sampler;
    class Swapchain;

    class ShaderModule;
    class CommandBuffer;
    class Fence;

    class Instance final : public glal::Instance
    {
    public:
        explicit Instance(const InstanceDesc &desc);
        ~Instance() override;

        std::uint32_t EnumeratePhysicalDevices(glal::PhysicalDevice **devices) override;

    private:
        PhysicalDevice *m_PhysicalDevices;
        std::uint32_t m_PhysicalDeviceCount;
    };

    class PhysicalDevice final : public glal::PhysicalDevice
    {
        friend Instance;

    public:
        ~PhysicalDevice() override;

        glal::Device *CreateDevice() override;
        void DestroyDevice(glal::Device *device) override;

        [[nodiscard]] bool Supports(DeviceFeature feature) const override;
        [[nodiscard]] const DeviceLimits &GetLimits() const override;

    protected:
        explicit PhysicalDevice(Instance *instance);

    private:
        Instance *m_Instance;
        DeviceLimits m_Limits;

        std::vector<Device *> m_Devices;
    };

    class Device final : public glal::Device
    {
        friend PhysicalDevice;

    public:
        ~Device() override;

        glal::Buffer *CreateBuffer(const BufferDesc &desc) override;
        void DestroyBuffer(glal::Buffer *buffer) override;

        glal::Image *CreateImage(const ImageDesc &desc) override;
        void DestroyImage(glal::Image *image) override;

        glal::Sampler *CreateSampler(const SamplerDesc &desc) override;
        void DestroySampler(glal::Sampler *sampler) override;

        glal::ShaderModule *CreateShaderModule(const ShaderModuleDesc &desc) override;
        void DestroyShaderModule(glal::ShaderModule *shader_module) override;

        glal::PipelineLayout *CreatePipelineLayout(const PipelineLayoutDesc &desc) override;
        void DestroyPipelineLayout(glal::PipelineLayout *pipeline_layout) override;

        glal::Pipeline *CreatePipeline(const PipelineDesc &desc) override;
        void DestroyPipeline(glal::Pipeline *pipeline) override;

        glal::DescriptorSetLayout *CreateDescriptorSetLayout(const DescriptorSetLayoutDesc &desc) override;
        void DestroyDescriptorSetLayout(glal::DescriptorSetLayout *descriptor_set_layout) override;

        glal::DescriptorSet *CreateDescriptorSet(const DescriptorSetDesc &desc) override;
        void DestroyDescriptorSet(glal::DescriptorSet *descriptor_set) override;

        glal::Swapchain *CreateSwapchain(const SwapchainDesc &desc) override;
        void DestroySwapchain(glal::Swapchain *swapchain) override;

        glal::CommandBuffer *CreateCommandBuffer(CommandBufferUsage usage) override;
        void DestroyCommandBuffer(glal::CommandBuffer *command_buffer) override;

        glal::Fence *CreateFence() override;
        void DestroyFence(glal::Fence *fence) override;

        glal::Queue *GetQueue(QueueType type) override;

        [[nodiscard]] bool Supports(DeviceFeature feature) const override;
        [[nodiscard]] const DeviceLimits &GetLimits() const override;

    protected:
        explicit Device(PhysicalDevice *physical_device);

    private:
        PhysicalDevice *m_PhysicalDevice;

        std::vector<Buffer *> m_Buffers;
        std::vector<Image *> m_Images;
        std::vector<Sampler *> m_Samplers;
        std::vector<ShaderModule *> m_ShaderModules;
        std::vector<PipelineLayout *> m_PipelineLayouts;
        std::vector<Pipeline *> m_Pipelines;
        std::vector<DescriptorSetLayout *> m_DescriptorSetLayouts;
        std::vector<DescriptorSet *> m_DescriptorSets;
        std::vector<Swapchain *> m_Swapchains;
        std::vector<CommandBuffer *> m_CommandBuffers;
        std::vector<Fence *> m_Fences;

        Queue *m_Queue;
    };

    class Queue final : public glal::Queue
    {
        friend Device;

    public:
        void Submit(
            const glal::CommandBuffer *command_buffer,
            std::uint32_t command_buffer_count,
            glal::Fence *fence) override;
        void Present(const glal::Swapchain *swapchain) override;

    protected:
        explicit Queue(Device *device);

    private:
        Device *m_Device;
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
        explicit DescriptorSetLayout(Device *device, const DescriptorSetLayoutDesc &desc);

        [[nodiscard]] std::vector<DescriptorBinding>::const_iterator begin() const;
        [[nodiscard]] std::vector<DescriptorBinding>::const_iterator end() const;

    private:
        Device *m_Device;

        std::uint32_t m_Set;
        std::vector<DescriptorBinding> m_DescriptorBindings;
    };

    struct BufferBinding
    {
        GLenum Target;
        const Buffer *BufferImpl;
        bool Base;
        GLsizeiptr Offset;
        GLintptr Size;
    };

    struct ImageBinding
    {
        const ImageView *ImageViewImpl;
        const Sampler *SamplerImpl;
    };

    class DescriptorSet final : public glal::DescriptorSet
    {
        friend Device;
        friend CommandBuffer;

    public:
        void BindBuffer(
            std::uint32_t binding,
            const glal::Buffer *buffer) override;

        void BindBuffer(
            std::uint32_t binding,
            const glal::Buffer *buffer,
            std::uint32_t offset,
            std::uint32_t size) override;

        void BindImageView(
            std::uint32_t binding,
            const glal::ImageView *image_view,
            const glal::Sampler *sampler) override;

    protected:
        explicit DescriptorSet(Device *device, const DescriptorSetDesc &desc);

        void Bind(std::uint32_t index) const;

    private:
        Device *m_Device;
        std::vector<DescriptorSetLayout *> m_Layouts;

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

        void BindPipeline(const glal::Pipeline *pipeline) override;
        void BindVertexBuffer(const glal::Buffer *buffer, std::size_t offset) override;
        void BindIndexBuffer(const glal::Buffer *buffer, DataType type) override;
        void BindDescriptorSet(std::uint32_t index, const glal::DescriptorSet *descriptor_set) override;

        void Draw(std::uint32_t vertex_count, std::uint32_t first_vertex) override;
        void DrawIndexed(std::uint32_t index_count, std::uint32_t first_index) override;

        void Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z) override;

        void CopyBuffer(
            const glal::Buffer *src_buffer,
            const glal::Buffer *dst_buffer,
            std::size_t src_offset,
            std::size_t dst_offset,
            std::size_t size) override;
        void CopyBufferToImage(const glal::Buffer *src_buffer, const glal::Image *dst_image) override;

        void Transition(const Resource *resource, ResourceState state) override;

    protected:
        explicit CommandBuffer(Device *device, CommandBufferUsage usage);

    private:
        Device *m_Device;
        CommandBufferUsage m_Usage;

        const Pipeline *m_Pipeline;

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
        explicit Fence(Device *device);

    private:
        Device *m_Device;
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
        explicit Buffer(Device *device, const BufferDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Device *m_Device;

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
        explicit Image(Device *device, const ImageDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Device *m_Device;

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
        [[nodiscard]] const glal::Image *GetImage() const override;

    protected:
        explicit ImageView(Image *image);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Image *m_Image;
    };

    class Sampler final : public glal::Sampler
    {
        friend Device;
        friend CommandBuffer;
        friend DescriptorSet;

    public:
        ~Sampler() override;

    protected:
        explicit Sampler(Device *device, const SamplerDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Device *m_Device;

        std::uint32_t m_Handle;
    };

    class Swapchain final : public glal::Swapchain
    {
        friend Device;

    public:
        ~Swapchain() override;

        [[nodiscard]] std::uint32_t GetImageCount() const override;
        [[nodiscard]] glal::ImageView *GetImageView(std::uint32_t index) const override;
        [[nodiscard]] Extent2D GetExtent() const override;

        std::uint32_t AcquireNextImage(glal::Fence *fence) override;
        void Present() const override;

    protected:
        explicit Swapchain(Device *device, const SwapchainDesc &desc);

    private:
        Device *m_Device;

        Extent2D m_Extent;
        std::uint32_t m_ImageCount;
        std::uint32_t m_ImageIndex;
        std::vector<Image *> m_Images;
        std::vector<glal::Fence *> m_Fences;
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
        explicit ShaderModule(Device *device, const ShaderModuleDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;

    private:
        Device *m_Device;

        ShaderStage m_Stage;

        std::uint32_t m_Handle;
    };

    class PipelineLayout final : public glal::PipelineLayout
    {
        friend Device;
        friend Pipeline;

    public:
        [[nodiscard]] glal::DescriptorSetLayout *GetDescriptorSetLayout(std::uint32_t index) const override;
        [[nodiscard]] std::uint32_t GetDescriptorSetLayoutCount() const override;

    protected:
        explicit PipelineLayout(Device *device, const PipelineLayoutDesc &desc);

        [[nodiscard]] std::vector<DescriptorSetLayout *>::const_iterator begin() const;
        [[nodiscard]] std::vector<DescriptorSetLayout *>::const_iterator end() const;

    private:
        Device *m_Device;

        std::vector<DescriptorSetLayout *> m_DescriptorSetLayouts;
    };

    class Pipeline final : public glal::Pipeline
    {
        friend Device;
        friend CommandBuffer;

    public:
        ~Pipeline() override;

        [[nodiscard]] PipelineType GetType() const override;

    protected:
        explicit Pipeline(Device *device, const PipelineDesc &desc);

        [[nodiscard]] std::uint32_t GetHandle() const;
        [[nodiscard]] std::uint32_t GetVertexStride() const;

        void LayoutVertexArray(std::uint32_t vertex_array) const;

    private:
        Device *m_Device;
        PipelineLayout *m_Layout;

        PipelineType m_Type;
        std::vector<VertexAttributeDesc> m_VertexAttributes;
        std::uint32_t m_VertexStride;

        std::uint32_t m_Handle;
    };

    void TranslateImageFormat(
        ImageFormat image_format,
        GLenum *internal_format,
        GLenum *external_format,
        GLenum *type);

    void TranslateDataType(
        DataType data_type,
        std::uint32_t *size,
        GLenum *type,
        GLboolean *normalized);
}
