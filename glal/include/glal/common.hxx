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
        union
        {
            float Float[4];
            std::int32_t Int[4];
            std::uint32_t UInt[4];
        } Color;

        struct
        {
            float Depth;
            std::uint32_t Stencil;
        } DepthStencil;
    };

    enum ClearValueMask : std::uint32_t
    {
        ClearValueMask_Color_Float,
        ClearValueMask_Color_Int,
        ClearValueMask_Color_UInt,
        ClearValueMask_DepthStencil,
    };

    /**
     * Attachment - describes an attachment to a specified target type
     */
    struct Attachment
    {
        AttachmentType Type;

        bool Clear;
        ClearValueMask Mask;
        ClearValue Value;
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
        ShaderModule Module;
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

    struct VertexBinding
    {
        std::uint32_t Binding;
        std::uint32_t Stride;
        bool Instance;
    };
}
