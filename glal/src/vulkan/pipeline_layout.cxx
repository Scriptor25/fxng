#include <glal/vulkan.hxx>

VkPipelineLayout glal::vulkan::PipelineLayoutT::GetHandle() const
{
    return m_Handle;
}
