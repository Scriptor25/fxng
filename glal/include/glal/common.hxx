#pragma once

#include <cstdint>
#include <glal/enum.hxx>
#include <glal/forward.hxx>

namespace glal
{
    /**
     * Clear Value
     */
    union ClearValue
    {
        float Color[4];
        float Depth;
        int Stencil;
    };

    /**
     * Descriptor Binding
     */
    struct DescriptorBinding
    {
        std::uint32_t Binding;
        DescriptorType Type;
        std::uint32_t Count;
        ShaderStage Stages;
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
     * Extent 2D - width and height
     */
    struct Extent2D
    {
        std::uint32_t Width;
        std::uint32_t Height;
    };

    /**
     * Extent 3D - width, height and depth
     */
    struct Extent3D
    {
        std::uint32_t Width;
        std::uint32_t Height;
        std::uint32_t Depth;
    };

    /**
     * Pipeline Stage - describes a single executable pipeline stage
     */
    struct PipelineStage
    {
        ShaderStage Stage;
        ShaderModuleT *Module;
    };

    /**
     * Render Target - describes an image view as a render target
     */
    struct RenderTarget
    {
        ImageViewT *View;

        bool Clear;
        ClearValue Value;
    };

    /**
     * Vertex Attribute
     */
    struct VertexAttribute
    {
        std::uint32_t Binding;
        std::uint32_t Location;
        DataType Type;
        std::uint32_t Count;
        std::uint32_t Offset;
    };
}
