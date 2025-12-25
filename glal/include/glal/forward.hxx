#pragma once

namespace glal
{
    class InstanceT;
    class PhysicalDeviceT;
    class DeviceT;

    class ResourceT;
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
    class RenderPassT;
    class FramebufferT;
    class CommandBufferT;
    class FenceT;
    class QueueT;

    using Instance = InstanceT *;
    using PhysicalDevice = PhysicalDeviceT *;
    using Device = DeviceT *;

    using Resource = ResourceT *;
    using Buffer = BufferT *;
    using Image = ImageT *;
    using ImageView = ImageViewT *;
    using Sampler = SamplerT *;
    using ShaderModule = ShaderModuleT *;
    using PipelineLayout = PipelineLayoutT *;
    using Pipeline = PipelineT *;
    using DescriptorSetLayout = DescriptorSetLayoutT *;
    using DescriptorSet = DescriptorSetT *;
    using Swapchain = SwapchainT *;
    using RenderPass = RenderPassT *;
    using Framebuffer = FramebufferT *;
    using CommandBuffer = CommandBufferT *;
    using Fence = FenceT *;
    using Queue = QueueT *;
}
