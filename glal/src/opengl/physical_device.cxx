#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::PhysicalDeviceT::PhysicalDeviceT(InstanceT *instance)
    : m_Instance(instance),
      m_Limits()
{
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

    m_Limits.MaxTextureSize2D = max_texture_size;
    m_Limits.MaxUniformBuffers = 16;
    m_Limits.MaxBufferSize = 1ull << 30;
}

glal::opengl::PhysicalDeviceT::~PhysicalDeviceT()
{
    common::Assert(m_Devices.empty(), "not all devices were explicitly destroyed");
}

glal::Device glal::opengl::PhysicalDeviceT::CreateDevice()
{
    return m_Devices.emplace_back(new DeviceT(this));
}

void glal::opengl::PhysicalDeviceT::DestroyDevice(Device device)
{
    for (auto it = m_Devices.begin(); it != m_Devices.end(); ++it)
        if (*it == device)
        {
            m_Devices.erase(it);
            delete device;
            return;
        }
    common::Fatal(
        "device {} is not owned by physical device {}",
        static_cast<const void *>(device),
        static_cast<const void *>(this));
}

bool glal::opengl::PhysicalDeviceT::Supports(const DeviceFeature feature) const
{
    return feature == DeviceFeature_GeometryShader
           || feature == DeviceFeature_Tessellation
           || feature == DeviceFeature_Compute;
}

const glal::DeviceLimits &glal::opengl::PhysicalDeviceT::GetLimits() const
{
    return m_Limits;
}
