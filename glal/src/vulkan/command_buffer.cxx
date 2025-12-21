#include <glal/vulkan.hxx>

glal::vulkan::CommandBufferT::CommandBufferT(DeviceT *device, CommandBufferUsage usage)
    : m_Device(device),
      m_Handle(),
      m_PoolHandle()
{
    // TODO
    const VkCommandPoolCreateInfo command_pool_create_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex = 0,
    };
    vkCreateCommandPool(m_Device->GetHandle(), &command_pool_create_info, nullptr, &m_PoolHandle);

    const VkCommandBufferAllocateInfo command_buffer_allocate_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_PoolHandle,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    vkAllocateCommandBuffers(m_Device->GetHandle(), &command_buffer_allocate_info, &m_Handle);
}

glal::vulkan::CommandBufferT::~CommandBufferT()
{
    vkFreeCommandBuffers(m_Device->GetHandle(), m_PoolHandle, 1, &m_Handle);
    vkDestroyCommandPool(m_Device->GetHandle(), m_PoolHandle, nullptr);
}

void glal::vulkan::CommandBufferT::Begin()
{
    const VkCommandBufferBeginInfo command_buffer_begin_info
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pInheritanceInfo = nullptr,
    };
    vkBeginCommandBuffer(m_Handle, &command_buffer_begin_info);
}

void glal::vulkan::CommandBufferT::End()
{
    vkEndCommandBuffer(m_Handle);
}

void glal::vulkan::CommandBufferT::BeginRenderPass(const RenderPassDesc &desc)
{
    // TODO
    const VkRenderPassBeginInfo render_pass_begin_info
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = {},
        .framebuffer = {},
        .renderArea = {
            .offset = { 0, 0 },
            .extent = { 0, 0 },
        },
        .clearValueCount = {},
        .pClearValues = {},
    };
    vkCmdBeginRenderPass(m_Handle, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

void glal::vulkan::CommandBufferT::EndRenderPass()
{
    vkCmdEndRenderPass(m_Handle);
}

void glal::vulkan::CommandBufferT::SetViewport(
    const float x,
    const float y,
    const float width,
    const float height,
    const float min_depth,
    const float max_depth)
{
    const VkViewport viewport
    {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .minDepth = min_depth,
        .maxDepth = max_depth,
    };
    vkCmdSetViewportWithCount(m_Handle, 1, &viewport);
}

void glal::vulkan::CommandBufferT::SetScissor(
    const std::int32_t x,
    const std::int32_t y,
    const std::uint32_t width,
    const std::uint32_t height)
{
    const VkRect2D scissor
    {
        .offset = { x, y },
        .extent = { width, height },
    };
    vkCmdSetScissorWithCount(m_Handle, 1, &scissor);
}

void glal::vulkan::CommandBufferT::BindPipeline(Pipeline pipeline)
{
    auto pipeline_impl = dynamic_cast<PipelineT *>(pipeline);
    vkCmdBindPipeline(m_Handle, ToVkPipelineBindPoint(pipeline_impl->GetType()), pipeline_impl->GetHandle());
}

void glal::vulkan::CommandBufferT::BindVertexBuffer(Buffer buffer, std::uint32_t binding, std::size_t offset)
{
}

void glal::vulkan::CommandBufferT::BindIndexBuffer(Buffer buffer, DataType type)
{
}

void glal::vulkan::CommandBufferT::BindDescriptorSets(
    std::uint32_t first_set,
    std::uint32_t set_count,
    const DescriptorSet *descriptor_sets)
{
}

void glal::vulkan::CommandBufferT::Draw(std::uint32_t vertex_count, std::uint32_t first_vertex)
{
}

void glal::vulkan::CommandBufferT::DrawIndexed(std::uint32_t index_count, std::uint32_t first_index)
{
}

void glal::vulkan::CommandBufferT::Dispatch(std::uint32_t x, std::uint32_t y, std::uint32_t z)
{
}

void glal::vulkan::CommandBufferT::CopyBuffer(
    Buffer src_buffer,
    Buffer dst_buffer,
    std::size_t src_offset,
    std::size_t dst_offset,
    std::size_t size)
{
}

void glal::vulkan::CommandBufferT::CopyBufferToImage(Buffer src_buffer, Image dst_image)
{
}

void glal::vulkan::CommandBufferT::Transition(Resource resource, ResourceState state)
{
}
