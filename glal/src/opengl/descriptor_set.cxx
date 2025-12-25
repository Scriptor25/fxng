#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::DescriptorSetT::DescriptorSetT(DeviceT *device, const DescriptorSetDesc &desc)
    : m_Device(device),
      m_Layout(desc.Layout)
{
}

void glal::opengl::DescriptorSetT::BindBuffer(const std::uint32_t binding, Buffer buffer)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);

    BindBuffer(binding, buffer, 0, buffer_impl->GetSize());
}

void glal::opengl::DescriptorSetT::BindBuffer(
    const std::uint32_t binding,
    Buffer buffer,
    const std::uint32_t offset,
    const std::uint32_t size)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);

    const auto descriptor_binding = m_Layout->FindDescriptorBinding(binding);
    common::Assert(descriptor_binding, "missing descriptor for binding {}", binding);

    GLenum target;
    switch (descriptor_binding->Type)
    {
    case DescriptorType_UniformBuffer:
        target = GL_UNIFORM_BUFFER;
        break;
    default:
        target = GL_SHADER_STORAGE_BUFFER;
        break;
    }

    m_BufferBindings[binding] = {
        .Target = target,
        .BufferImpl = buffer_impl,
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

void glal::opengl::DescriptorSetT::Bind(const std::uint32_t set) const
{
    const auto binding_base = set * 8;

    for (auto &[binding, element] : m_BufferBindings)
        glBindBufferRange(
            element.Target,
            binding_base + binding,
            element.BufferImpl->GetHandle(),
            element.Offset,
            element.Size);

    for (auto &[binding, element] : m_ImageBindings)
    {
        glBindTextureUnit(binding_base + binding, element.ImageViewImpl->GetImageHandle());
        glBindSampler(binding_base + binding, element.SamplerImpl->GetHandle());
    }
}
