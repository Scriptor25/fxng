#include <glal/vulkan.hxx>

glal::Instance glal::CreateInstanceVulkan(const InstanceDesc &desc)
{
    return new vulkan::InstanceT(desc);
}
