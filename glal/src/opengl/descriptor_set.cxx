#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::DescriptorSetT::DescriptorSetT(DeviceT *device, const DescriptorSetDesc &desc)
    : m_Device(device),
      m_Layouts(desc.DescriptorSetLayoutCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorSetLayoutCount; ++i)
        m_Layouts.at(i) = dynamic_cast<DescriptorSetLayoutT *>(desc.DescriptorSetLayouts[i]);
}

void glal::opengl::DescriptorSetT::BindBuffer(const std::uint32_t binding, Buffer buffer)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);

    GLenum target = 0;
    for (const auto layout : m_Layouts)
        for (auto &desc : *layout)
            if (desc.Binding == binding)
            {
                switch (desc.Type)
                {
                case DescriptorType_UniformBuffer:
                    target = GL_UNIFORM_BUFFER;
                    break;
                default:
                    target = GL_SHADER_STORAGE_BUFFER;
                    break;
                }
                break;
            }
    common::Assert(target, "missing descriptor for binding {}", binding);

    m_BufferBindings[binding] = {
        .Target = target,
        .BufferImpl = buffer_impl,
        .Base = true,
        .Offset = 0,
        .Size = 0,
    };
}

void glal::opengl::DescriptorSetT::BindBuffer(
    const std::uint32_t binding,
    Buffer buffer,
    const std::uint32_t offset,
    const std::uint32_t size)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);

    GLenum target = 0;
    for (const auto layout : m_Layouts)
        for (auto &desc : *layout)
            if (desc.Binding == binding)
            {
                switch (desc.Type)
                {
                case DescriptorType_UniformBuffer:
                    target = GL_UNIFORM_BUFFER;
                    break;
                default:
                    target = GL_SHADER_STORAGE_BUFFER;
                    break;
                }
                break;
            }
    common::Assert(target, "missing descriptor for binding {}", binding);

    m_BufferBindings[binding] = {
        .Target = target,
        .BufferImpl = buffer_impl,
        .Base = false,
        .Offset = offset,
        .Size = size,
    };
}

void glal::opengl::DescriptorSetT::BindImageView(
    const std::uint32_t binding,
    ImageView image_view,
    Sampler sampler)
{
    const auto image_view_impl = dynamic_cast<ImageViewT *>(image_view);
    const auto sampler_impl = dynamic_cast<SamplerT *>(sampler);

    m_ImageBindings[binding] = {
        .ImageViewImpl = image_view_impl,
        .SamplerImpl = sampler_impl,
    };
}

void glal::opengl::DescriptorSetT::Bind(const std::uint32_t index) const
{
    const auto binding_base = index * 8;

    for (auto &[binding, element] : m_BufferBindings)
    {
        if (element.Base)
            glBindBufferBase(
                element.Target,
                binding_base + binding,
                element.BufferImpl->GetHandle());
        else
            glBindBufferRange(
                element.Target,
                binding_base + binding,
                element.BufferImpl->GetHandle(),
                element.Offset,
                element.Size);
    }

    for (auto &[binding, element] : m_ImageBindings)
    {
        glBindTextureUnit(binding_base + binding, element.ImageViewImpl->GetImageHandle());
        glBindSampler(binding_base + binding, element.SamplerImpl->GetHandle());
    }
}
