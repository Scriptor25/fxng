#include <common/log.hxx>
#include <glal/vulkan.hxx>

glal::Instance glal::CreateInstanceVulkan(const InstanceDesc &desc)
{
    return new vulkan::InstanceT(desc);
}

VkFilter glal::vulkan::ToVkFilter(const Filter filter)
{
    switch (filter)
    {
    case Filter_Nearest:
        return VK_FILTER_NEAREST;
    case Filter_Linear:
        return VK_FILTER_LINEAR;
    default:
        common::Fatal("filter not supported");
    }
}

VkSamplerAddressMode glal::vulkan::ToVkAddressMode(const AddressMode address_mode)
{
    switch (address_mode)
    {
    case AddressMode_Repeat:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case AddressMode_Clamp:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case AddressMode_Mirror:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    default:
        common::Fatal("address mode not supported");
    }
}

VkPipelineBindPoint glal::vulkan::ToVkPipelineBindPoint(const PipelineType pipeline_type)
{
    switch (pipeline_type)
    {
    case PipelineType_Graphics:
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    case PipelineType_Compute:
        return VK_PIPELINE_BIND_POINT_COMPUTE;
    case PipelineType_RayTracing:
        return VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
    default:
        common::Fatal("pipeline type not supported");
    }
}

VkShaderStageFlagBits glal::vulkan::ToVkShaderStage(const ShaderStage shader_stage)
{
    switch (shader_stage)
    {
    case ShaderStage_Vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderStage_Geometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case ShaderStage_TessellationControl:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case ShaderStage_TessellationEvaluation:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case ShaderStage_Fragment:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderStage_Compute:
        return VK_SHADER_STAGE_COMPUTE_BIT;
    case ShaderStage_RayGeneration:
        return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
    case ShaderStage_RayHit:
        return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    case ShaderStage_RayMiss:
        return VK_SHADER_STAGE_MISS_BIT_KHR;
    default:
        common::Fatal("shader stage not supported");
    }
}

VkFormat glal::vulkan::ToVkFormat(const DataType data_type, const std::uint32_t count)
{
    switch (data_type)
    {
    case DataType_UInt8:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R8_UINT;
        case 2:
            return VK_FORMAT_R8G8_UINT;
        case 3:
            return VK_FORMAT_R8G8B8_UINT;
        case 4:
            return VK_FORMAT_R8G8B8A8_UINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_UInt16:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R16_UINT;
        case 2:
            return VK_FORMAT_R16G16_UINT;
        case 3:
            return VK_FORMAT_R16G16B16_UINT;
        case 4:
            return VK_FORMAT_R16G16B16A16_UINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_UInt32:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R32_UINT;
        case 2:
            return VK_FORMAT_R32G32_UINT;
        case 3:
            return VK_FORMAT_R32G32B32_UINT;
        case 4:
            return VK_FORMAT_R32G32B32A32_UINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Int8:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R8_SINT;
        case 2:
            return VK_FORMAT_R8G8_SINT;
        case 3:
            return VK_FORMAT_R8G8B8_SINT;
        case 4:
            return VK_FORMAT_R8G8B8A8_SINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Int16:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R16_SINT;
        case 2:
            return VK_FORMAT_R16G16_SINT;
        case 3:
            return VK_FORMAT_R16G16B16_SINT;
        case 4:
            return VK_FORMAT_R16G16B16A16_SINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Int32:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R32_SINT;
        case 2:
            return VK_FORMAT_R32G32_SINT;
        case 3:
            return VK_FORMAT_R32G32B32_SINT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Half:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R16_SFLOAT;
        case 2:
            return VK_FORMAT_R16G16_SFLOAT;
        case 3:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case 4:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Float:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R32_SFLOAT;
        case 2:
            return VK_FORMAT_R32G32_SFLOAT;
        case 3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Fixed:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R32_UINT;
        case 2:
            return VK_FORMAT_R32G32_UINT;
        case 3:
            return VK_FORMAT_R32G32B32_UINT;
        case 4:
            return VK_FORMAT_R32G32B32A32_UINT;
        default:
            common::Fatal("count not supported");
        }
    case DataType_Double:
        switch (count)
        {
        case 1:
            return VK_FORMAT_R64_SFLOAT;
        case 2:
            return VK_FORMAT_R64G64_SFLOAT;
        case 3:
            return VK_FORMAT_R64G64B64_SFLOAT;
        case 4:
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        default:
            common::Fatal("count not supported");
        }
    default:
        common::Fatal("data type not supported");
    }
}

VkFormat glal::vulkan::ToVkFormat(const ImageFormat image_format)
{
    switch (image_format)
    {
    case ImageFormat_RGBA8_UNorm:
        return VK_FORMAT_R8G8B8_UNORM;
    case ImageFormat_RGBA8_SRGB:
        return VK_FORMAT_R8G8B8A8_SRGB;
    case ImageFormat_BGRA8_UNorm:
        return VK_FORMAT_B8G8R8_UNORM;
    case ImageFormat_RG16F:
        return VK_FORMAT_R16G16B16_SFLOAT;
    case ImageFormat_RGBA16F:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    case ImageFormat_RGBA32F:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case ImageFormat_D24S8:
        return VK_FORMAT_D24_UNORM_S8_UINT;
    case ImageFormat_D32F:
        return VK_FORMAT_D32_SFLOAT;
    default:
        common::Fatal("image format not supported");
    }
}

VkPrimitiveTopology glal::vulkan::ToVkPrimitiveTopology(const PrimitiveTopology primitive_topology)
{
    switch (primitive_topology)
    {
    case VertexTopology_PointList:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case VertexTopology_LineList:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case VertexTopology_LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case VertexTopology_TriangleList:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case VertexTopology_TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    case VertexTopology_TriangleFan:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    default:
        common::Fatal("primitive topology not supported");
    }
}

VkDescriptorType glal::vulkan::ToVkDescriptorType(const DescriptorType descriptor_type)
{
    switch (descriptor_type)
    {
    case DescriptorType_UniformBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType_StorageBuffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case DescriptorType_CombinedImageSampler:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case DescriptorType_SampledImage:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case DescriptorType_StorageImage:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case DescriptorType_Sampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    default:
        common::Fatal("descriptor type not supported");
    }
}
