#pragma once

namespace glal
{
    class Instance;
    class PhysicalDevice;
    class Device;

    class Resource;
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

    template<typename T>
    using Ptr = T *;
}
