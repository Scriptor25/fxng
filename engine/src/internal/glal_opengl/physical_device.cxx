#include <GL/glew.h>

#include <fxng/internal/glal_opengl.hxx>

fxng::glal::opengl::PhysicalDevice::PhysicalDevice(Instance *instance)
    : m_Instance(instance),
      m_Limits()
{
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

    m_Limits.MaxTextureSize2D = max_texture_size;
    m_Limits.MaxUniformBuffers = 16;
    m_Limits.MaxBufferSize = 1ull << 30;
}

fxng::glal::opengl::PhysicalDevice::~PhysicalDevice()
{
    for (const auto device : m_Devices)
        delete device;
    m_Devices.clear();
}

fxng::glal::Device *fxng::glal::opengl::PhysicalDevice::CreateDevice()
{
    return m_Devices.emplace_back(new Device(this));
}

void fxng::glal::opengl::PhysicalDevice::DestroyDevice(glal::Device *device)
{
    for (auto it = m_Devices.begin(); it != m_Devices.end(); ++it)
        if (*it == device)
        {
            m_Devices.erase(it);
            delete device;
            break;
        }
}

bool fxng::glal::opengl::PhysicalDevice::Supports(const DeviceFeature feature) const
{
    return feature == DeviceFeature_GeometryShader
           || feature == DeviceFeature_Tessellation
           || feature == DeviceFeature_Compute;
}

const fxng::glal::DeviceLimits &fxng::glal::opengl::PhysicalDevice::GetLimits() const
{
    return m_Limits;
}
