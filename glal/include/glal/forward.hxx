#pragma once
#include <algorithm>

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
    using CommandBuffer = CommandBufferT *;
    using Fence = FenceT *;
    using Queue = QueueT *;

    template<typename T>
    class Vec
    {
    public:
        explicit Vec(const std::uint32_t element_count = 0)
            : m_Elements(static_cast<T *>(std::malloc(element_count * sizeof(T)))),
              m_ElementCount(element_count)
        {
        }

        explicit Vec(T *begin, T *end)
            : Vec(end - begin)
        {
            std::copy(begin, end, m_Elements);
        }

        Vec(const Vec &other)
            : Vec(other.begin(), other.end())
        {
        }

        Vec(Vec &&other) noexcept
            : m_Elements(nullptr),
              m_ElementCount(0)
        {
            std::swap(m_Elements, other.m_Elements);
            std::swap(m_ElementCount, other.m_ElementCount);
        }

        Vec &operator=(const Vec &other)
        {
            if (this != &other)
            {
                for (auto &element : *this)
                    element.~T();

                std::free(m_Elements);

                m_Elements = static_cast<T *>(std::malloc(other.m_ElementCount * sizeof(T)));
                m_ElementCount = other.m_ElementCount;

                std::copy(other.begin(), other.end(), m_Elements);
            }
            return *this;
        }

        Vec &operator=(Vec &&other) noexcept
        {
            std::swap(m_Elements, other.m_Elements);
            std::swap(m_ElementCount, other.m_ElementCount);
            return *this;
        }

        ~Vec()
        {
            for (auto &element : *this)
                element.~T();

            std::free(m_Elements);

            m_Elements = nullptr;
            m_ElementCount = 0;
        }

        T *data()
        {
            return m_Elements;
        }

        const T *data() const
        {
            return m_Elements;
        }

        std::uint32_t size() const
        {
            return m_ElementCount;
        }

        T *begin()
        {
            return m_Elements;
        }

        T *end()
        {
            return m_Elements + m_ElementCount;
        }

        const T *begin() const
        {
            return m_Elements;
        }

        const T *end() const
        {
            return m_Elements + m_ElementCount;
        }

        T &front()
        {
            return *m_Elements;
        }

        T &back()
        {
            return *(m_Elements + m_ElementCount - 1);
        }

        const T &front() const
        {
            return *m_Elements;
        }

        const T &back() const
        {
            return *(m_Elements + m_ElementCount - 1);
        }

        void push_back(const T &element)
        {
            m_ElementCount++;
            m_Elements = static_cast<T *>(std::realloc(m_Elements, m_ElementCount * sizeof(T)));
            m_Elements[m_ElementCount - 1] = element;
        }

        void push_back(T &&element)
        {
            m_ElementCount++;
            m_Elements = static_cast<T *>(std::realloc(m_Elements, m_ElementCount * sizeof(T)));
            m_Elements[m_ElementCount - 1] = element;
        }

        template<typename... Args>
        void emplace_back(Args &&... args)
        {
            m_ElementCount++;
            m_Elements = static_cast<T *>(std::realloc(m_Elements, m_ElementCount * sizeof(T)));
            m_Elements[m_ElementCount - 1] = T(std::forward<Args>(args)...);
        }

    private:
        T *m_Elements;
        std::uint32_t m_ElementCount;
    };
}
