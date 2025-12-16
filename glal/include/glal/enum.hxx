#pragma once

#include <cstdint>

namespace glal
{
    enum AddressMode
    {
        AddressMode_Repeat,
        AddressMode_Clamp,
        AddressMode_Mirror,
    };

    enum BufferUsage
    {
        BufferUsage_Vertex,
        BufferUsage_Index,
        BufferUsage_Uniform,
        BufferUsage_Storage,
    };

    enum CommandBufferUsage
    {
        CommandBufferUsage_Once,
        CommandBufferUsage_Reusable,
    };

    enum DataType
    {
        DataType_None,
        DataType_UInt8,
        DataType_UInt16,
        DataType_UInt32,
        DataType_Int8,
        DataType_Int16,
        DataType_Int32,
        DataType_Half,
        DataType_Float,
        DataType_Fixed,
        DataType_Double,
    };

    enum DescriptorType
    {
        DescriptorType_UniformBuffer,
        DescriptorType_StorageBuffer,
        DescriptorType_ReadOnlyStorageBuffer,
        DescriptorType_CombinedImageSampler,
        DescriptorType_SampledImage,
        DescriptorType_StorageImage,
        DescriptorType_Sampler,
        DescriptorType_PushConstant,
    };

    enum DeviceFeature
    {
        DeviceFeature_Compute,
        DeviceFeature_GeometryShader,
        DeviceFeature_Tessellation,
        DeviceFeature_RayTracing,

        DeviceFeature_ExplicitBarriers,
        DeviceFeature_DescriptorSets,
        DeviceFeature_TimelineSemaphore,
    };

    enum Filter
    {
        Filter_Nearest,
        Filter_Linear,
    };

    enum ImageType
    {
        ImageType_1D,
        ImageType_2D,
        ImageType_3D,
    };

    enum ImageFormat
    {
        ImageFormat_RGBA8_UNorm,
        ImageFormat_RGBA8_SRGB,
        ImageFormat_BGRA8_UNorm,

        ImageFormat_RG16F,
        ImageFormat_RGBA16F,
        ImageFormat_RGBA32F,

        ImageFormat_D24S8,
        ImageFormat_D32F,
    };

    enum MemoryUsage
    {
        MemoryUsage_DeviceLocal,
        MemoryUsage_HostToDevice,
        MemoryUsage_DeviceToHost,
    };

    enum PipelineType
    {
        PipelineType_Graphics,
        PipelineType_Compute,
        PipelineType_RayTracing,
    };

    enum QueueType : std::uint32_t
    {
        QueueType_None     = 0,
        QueueType_Graphics = 1 << 0,
        QueueType_Compute  = 1 << 1,
        QueueType_Transfer = 1 << 2,
        QueueType_Present  = 1 << 3,
    };

    enum ResourceState
    {
        ResourceState_VertexBuffer,
        ResourceState_IndexBuffer,
        ResourceState_ConstantBuffer,
        ResourceState_ShaderResource,
        ResourceState_UnorderedAccess,
        ResourceState_RenderTarget,
        ResourceState_DepthStencil,
        ResourceState_CopySrc,
        ResourceState_CopyDst,
        ResourceState_Present,
    };

    enum ShaderStage : std::uint32_t
    {
        ShaderStage_None = 0,

        ShaderStage_Vertex                 = 1 << 0,
        ShaderStage_Geometry               = 1 << 1,
        ShaderStage_TessellationControl    = 1 << 2,
        ShaderStage_TessellationEvaluation = 1 << 3,
        ShaderStage_Fragment               = 1 << 4,

        ShaderStage_Compute = 1 << 5,

        ShaderStage_RayGeneration = 1 << 6,
        ShaderStage_RayHit        = 1 << 7,
        ShaderStage_RayMiss       = 1 << 8,
    };
}
