#pragma once

#include <glal/common.hxx>
#include <glal/enum.hxx>
#include <glal/forward.hxx>

namespace glal
{
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
        ImageDimension Dimension;
        Extent3D Extent;
        std::uint32_t MipLevelCount;
        std::uint32_t ArrayLayerCount;
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
        std::size_t Size;
    };

    /**
     * Pipeline Layout Descriptor
     */
    struct PipelineLayoutDesc
    {
        DescriptorSetLayout *const*DescriptorSetLayouts;
        std::uint32_t DescriptorSetLayoutCount;

        // TODO: push constant ranges
    };

    /**
     * Pipeline Descriptor - describes a full graphics, compute or ray-tracing pipeline with multiple stages
     */
    struct PipelineDesc
    {
        PipelineType Type;

        const PipelineStage *Stages;
        std::uint32_t StageCount;

        const VertexAttribute *VertexAttributes;
        std::uint32_t VertexAttributeCount;

        PipelineLayout *Layout;

        bool DepthTest;
        bool DepthWrite;
        bool BlendEnable;
    };

    /**
     * Descriptor Set Layout Descriptor
     */
    struct DescriptorSetLayoutDesc
    {
        std::uint32_t Set;

        const DescriptorBinding *DescriptorBindings;
        std::uint32_t DescriptorBindingCount;
    };

    /**
     * Descriptor Set Descriptor
     */
    struct DescriptorSetDesc
    {
        DescriptorSetLayout *const*DescriptorSetLayouts;
        std::uint32_t DescriptorSetLayoutCount;
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

    /**
     * Render Pass Descriptor - describes a render pass with multiple render targets
     */
    struct RenderPassDesc
    {
        const RenderTarget *Color;
        std::uint32_t ColorCount;

        const RenderTarget *Depth;
        std::uint32_t DepthCount;

        const RenderTarget *Stencil;
        std::uint32_t StencilCount;
    };
}
