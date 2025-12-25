#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::opengl::BufferT::BufferT(DeviceT *device, const BufferDesc &desc)
    : m_Device(device),
      m_Size(desc.Size),
      m_Usage(desc.Usage),
      m_Memory(desc.Memory),
      m_Handle()
{
    GLbitfield flags{};
    switch (m_Memory)
    {
    case MemoryUsage_HostToDevice:
        flags = GL_MAP_WRITE_BIT;
        break;
    case MemoryUsage_DeviceToHost:
        flags = GL_MAP_READ_BIT;
        break;
    case MemoryUsage_DeviceLocal:
        flags = 0;
        break;
    }

    glCreateBuffers(1, &m_Handle);
    glNamedBufferStorage(m_Handle, static_cast<GLsizeiptr>(m_Size), nullptr, flags);
}

glal::opengl::BufferT::~BufferT()
{
    glDeleteBuffers(1, &m_Handle);
}

std::size_t glal::opengl::BufferT::GetSize() const
{
    return m_Size;
}

glal::BufferUsage glal::opengl::BufferT::GetUsage() const
{
    return m_Usage;
}

void *glal::opengl::BufferT::Map()
{
    GLenum access{};
    switch (m_Memory)
    {
    case MemoryUsage_HostToDevice:
        access = GL_WRITE_ONLY;
        break;
    case MemoryUsage_DeviceToHost:
        access = GL_READ_ONLY;
        break;
    case MemoryUsage_DeviceLocal:
        common::Fatal("device local memory not accessible");
    }

    return glMapNamedBuffer(m_Handle, access);
}

void glal::opengl::BufferT::Unmap()
{
    glUnmapNamedBuffer(m_Handle);
}

GLuint glal::opengl::BufferT::GetHandle() const
{
    return m_Handle;
}
