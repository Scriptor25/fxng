#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::CommandBufferT::CommandBufferT(DeviceT *device, const CommandBufferUsage usage)
    : m_Device(device),
      m_Usage(usage),
      m_Pipeline(nullptr),
      m_VertexArray(0),
      m_Framebuffer(0),
      m_IndexType(DataType_None)
{
}

void glal::opengl::CommandBufferT::Begin()
{
    glCreateVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);
}

void glal::opengl::CommandBufferT::End()
{
    glUseProgram(0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_VertexArray);

    m_Pipeline = nullptr;
    m_IndexType = DataType_None;
}

void glal::opengl::CommandBufferT::BeginRenderPass(const RenderPassDesc &desc)
{
    common::Assert(desc.DepthCount <= 1, "opengl only supports one depth attachment per render pass");
    common::Assert(desc.StencilCount <= 1, "opengl only supports one stencil attachment per render pass");

    glCreateFramebuffers(1, &m_Framebuffer);

    std::vector<GLenum> attachments;

    for (std::uint32_t i = 0; i < desc.ColorCount; ++i)
    {
        const auto attachment = desc.Color + i;
        const auto image_view_impl = dynamic_cast<ImageViewT *>(attachment->View);

        glNamedFramebufferTexture(m_Framebuffer, GL_COLOR_ATTACHMENT0 + i, image_view_impl->GetImageHandle(), 0);

        attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);

        if (attachment->Clear)
            glClearNamedFramebufferfv(
                m_Framebuffer,
                GL_COLOR,
                static_cast<GLint>(i),
                attachment->Value.Color);
    }

    for (std::uint32_t i = 0; i < desc.DepthCount; ++i)
    {
        const auto attachment = desc.Depth + i;
        const auto image_view_impl = dynamic_cast<ImageViewT *>(attachment->View);

        glNamedFramebufferTexture(m_Framebuffer, GL_DEPTH_ATTACHMENT, image_view_impl->GetImageHandle(), 0);

        attachments.emplace_back(GL_DEPTH_ATTACHMENT);

        if (attachment->Clear)
            glClearNamedFramebufferfi(
                m_Framebuffer,
                GL_DEPTH,
                static_cast<GLint>(i),
                attachment->Value.Depth,
                0);
    }

    for (std::uint32_t i = 0; i < desc.StencilCount; ++i)
    {
        const auto attachment = desc.Stencil + i;
        const auto image_view_impl = dynamic_cast<ImageViewT *>(attachment->View);

        glNamedFramebufferTexture(m_Framebuffer, GL_STENCIL_ATTACHMENT, image_view_impl->GetImageHandle(), 0);

        attachments.emplace_back(GL_STENCIL_ATTACHMENT);

        if (attachment->Clear)
            glClearNamedFramebufferfi(
                m_Framebuffer,
                GL_STENCIL,
                static_cast<GLint>(i),
                0.f,
                attachment->Value.Stencil);
    }

    glNamedFramebufferDrawBuffers(m_Framebuffer, static_cast<GLsizei>(attachments.size()), attachments.data());

    const auto ok = glCheckNamedFramebufferStatus(m_Framebuffer, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    common::Assert(ok, "incomplete framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void glal::opengl::CommandBufferT::EndRenderPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_Framebuffer);
}

void glal::opengl::CommandBufferT::BindPipeline(Pipeline pipeline)
{
    const auto pipeline_impl = dynamic_cast<PipelineT *>(pipeline);
    glUseProgram(pipeline_impl->GetHandle());

    pipeline_impl->BindVertexArray(m_VertexArray);

    m_Pipeline = pipeline_impl;
}

void glal::opengl::CommandBufferT::SetViewport(
    const float x,
    const float y,
    const float width,
    const float height,
    const float min_depth,
    const float max_depth)
{
    glViewport(
        static_cast<GLint>(x),
        static_cast<GLint>(y),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height));
}

void glal::opengl::CommandBufferT::SetScissor(std::int32_t x, std::int32_t y, std::uint32_t width, std::uint32_t height)
{
    glScissor(x, y, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void glal::opengl::CommandBufferT::BindVertexBuffer(
    Buffer buffer,
    const std::uint32_t binding,
    const std::size_t offset)
{
    common::Assert(m_Pipeline, "pipeline not set");

    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);
    m_Pipeline->BindVertexBuffer(m_VertexArray, buffer_impl->GetHandle(), binding, offset);
}

void glal::opengl::CommandBufferT::BindIndexBuffer(Buffer buffer, const DataType type)
{
    const auto buffer_impl = dynamic_cast<BufferT *>(buffer);
    glVertexArrayElementBuffer(m_VertexArray, buffer_impl->GetHandle());

    m_IndexType = type;
}

void glal::opengl::CommandBufferT::BindDescriptorSets(
    const std::uint32_t first_set,
    const std::uint32_t set_count,
    const DescriptorSet *descriptor_sets)
{
    for (std::uint32_t i = 0; i < set_count; ++i)
    {
        const auto set_impl = dynamic_cast<DescriptorSetT *>(descriptor_sets[i]);
        set_impl->Bind(first_set + i);
    }
}

void glal::opengl::CommandBufferT::Draw(const std::uint32_t vertex_count, const std::uint32_t first_vertex)
{
    common::Assert(m_Pipeline, "pipeline not set");
    common::Assert(m_Pipeline->GetType() == PipelineType_Graphics, "pipeline is not graphics");

    GLenum mode;
    TranslatePrimitiveTopology(m_Pipeline->GetTopology(), &mode);

    glBindVertexArray(m_VertexArray);
    glDrawArrays(
        mode,
        static_cast<GLint>(first_vertex),
        static_cast<GLsizei>(vertex_count));
}

void glal::opengl::CommandBufferT::DrawIndexed(const std::uint32_t index_count, const std::uint32_t first_index)
{
    common::Assert(m_Pipeline, "pipeline not set");
    common::Assert(m_Pipeline->GetType() == PipelineType_Graphics, "pipeline is not graphics");

    std::uint32_t size;
    GLenum type;

    TranslateDataType(m_IndexType, &size, &type, nullptr);

    GLenum mode;
    TranslatePrimitiveTopology(m_Pipeline->GetTopology(), &mode);

    glBindVertexArray(m_VertexArray);
    glDrawElementsBaseVertex(
        mode,
        static_cast<GLsizei>(index_count),
        type,
        reinterpret_cast<void *>(first_index * size),
        0);
}

void glal::opengl::CommandBufferT::Dispatch(const std::uint32_t x, const std::uint32_t y, const std::uint32_t z)
{
    common::Assert(m_Pipeline, "pipeline not set");
    common::Assert(m_Pipeline->GetType() == PipelineType_Compute, "pipeline is not compute");

    glDispatchCompute(x, y, z);
}

void glal::opengl::CommandBufferT::CopyBuffer(
    Buffer src_buffer,
    Buffer dst_buffer,
    const std::size_t src_offset,
    const std::size_t dst_offset,
    const std::size_t size)
{
    common::Assert(src_buffer, "missing src buffer");
    common::Assert(dst_buffer, "missing dst buffer");

    const auto src_buffer_impl = dynamic_cast<BufferT *>(src_buffer);
    const auto dst_buffer_impl = dynamic_cast<BufferT *>(dst_buffer);

    glCopyNamedBufferSubData(
        src_buffer_impl->GetHandle(),
        dst_buffer_impl->GetHandle(),
        static_cast<GLintptr>(src_offset),
        static_cast<GLintptr>(dst_offset),
        static_cast<GLsizei>(size));
}

void glal::opengl::CommandBufferT::CopyBufferToImage(
    Buffer src_buffer,
    Image dst_image)
{
    const auto src_buffer_impl = dynamic_cast<BufferT *>(src_buffer);
    const auto dst_image_impl = dynamic_cast<ImageT *>(dst_image);

    GLenum format, type;
    TranslateImageFormat(dst_image_impl->GetFormat(), nullptr, &format, &type);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, src_buffer_impl->GetHandle());
    switch (dst_image_impl->GetType())
    {
    case ImageType_1D:
        glTextureSubImage1D(
            dst_image_impl->GetHandle(),
            0,
            0,
            static_cast<GLsizei>(dst_image_impl->GetExtent().Width),
            format,
            type,
            nullptr);
        break;
    case ImageType_2D:
        glTextureSubImage2D(
            dst_image_impl->GetHandle(),
            0,
            0,
            0,
            static_cast<GLsizei>(dst_image_impl->GetExtent().Width),
            static_cast<GLsizei>(dst_image_impl->GetExtent().Height),
            format,
            type,
            nullptr);
        break;
    case ImageType_3D:
        glTextureSubImage3D(
            dst_image_impl->GetHandle(),
            0,
            0,
            0,
            0,
            static_cast<GLsizei>(dst_image_impl->GetExtent().Width),
            static_cast<GLsizei>(dst_image_impl->GetExtent().Height),
            static_cast<GLsizei>(dst_image_impl->GetExtent().Depth),
            format,
            type,
            nullptr);
        break;
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void glal::opengl::CommandBufferT::Transition(Resource resource, ResourceState state)
{
    // noop
}
