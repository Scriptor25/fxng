#include <fxng/log.hxx>
#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::DescriptorSet::DescriptorSet(Device *device, const DescriptorSetDesc &desc)
    : m_Device(device),
      m_Layouts(desc.DescriptorSetLayoutCount)
{
    for (std::uint32_t i = 0; i < desc.DescriptorSetLayoutCount; ++i)
        m_Layouts.at(i) = dynamic_cast<DescriptorSetLayout *>(desc.DescriptorSetLayouts[i]);
}

void fxng::glal::opengl::DescriptorSet::BindBuffer(const std::uint32_t binding, const glal::Buffer *buffer)
{
    const auto buffer_impl = dynamic_cast<const Buffer *>(buffer);

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
    Assert(target, "missing descriptor for binding {}", binding);

    m_BufferBindings[binding] = {
        .Target = target,
        .BufferImpl = buffer_impl,
        .Base = true,
        .Offset = 0,
        .Size = 0,
    };
}

void fxng::glal::opengl::DescriptorSet::BindBuffer(
    const std::uint32_t binding,
    const glal::Buffer *buffer,
    const std::uint32_t offset,
    const std::uint32_t size)
{
    const auto buffer_impl = dynamic_cast<const Buffer *>(buffer);

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
    Assert(target, "missing descriptor for binding {}", binding);

    m_BufferBindings[binding] = {
        .Target = target,
        .BufferImpl = buffer_impl,
        .Base = false,
        .Offset = offset,
        .Size = size,
    };
}

void fxng::glal::opengl::DescriptorSet::BindImageView(
    const std::uint32_t binding,
    const glal::ImageView *image_view,
    const glal::Sampler *sampler)
{
    const auto image_view_impl = dynamic_cast<const ImageView *>(image_view);
    const auto sampler_impl = dynamic_cast<const Sampler *>(sampler);

    m_ImageBindings[binding] = {
        .ImageViewImpl = image_view_impl,
        .SamplerImpl = sampler_impl,
    };
}

void fxng::glal::opengl::DescriptorSet::Bind(const std::uint32_t index) const
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
        glBindTextureUnit(binding_base + binding, element.ImageViewImpl->GetHandle());
        glBindSampler(binding_base + binding, element.SamplerImpl->GetHandle());
    }
}
