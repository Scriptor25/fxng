#include <fxng/log.hxx>
#include <fxng/internal/glal_opengl.hxx>
#include <GL/glew.h>

fxng::glal::opengl::Buffer::Buffer(Device *device, const BufferDesc &desc)
    : m_Device(device),
      m_Size(desc.Size),
      m_Usage(desc.Usage),
      m_Memory(desc.Memory),
      m_Handle()
{
    GLbitfield flags;
    switch (m_Memory)
    {
    case MemoryUsage_HostToDevice:
        flags = GL_MAP_WRITE_BIT;
        break;
    case MemoryUsage_DeviceToHost:
        flags = GL_MAP_READ_BIT;
        break;
    default:
        flags = 0;
        break;
    }

    glCreateBuffers(1, &m_Handle);
    glNamedBufferStorage(m_Handle, static_cast<GLsizeiptr>(m_Size), nullptr, flags);
}

fxng::glal::opengl::Buffer::~Buffer()
{
    glDeleteBuffers(1, &m_Handle);
}

std::size_t fxng::glal::opengl::Buffer::GetSize() const
{
    return m_Size;
}

fxng::glal::BufferUsage fxng::glal::opengl::Buffer::GetUsage() const
{
    return m_Usage;
}

void *fxng::glal::opengl::Buffer::Map()
{
    GLenum access;
    switch (m_Memory)
    {
    case MemoryUsage_HostToDevice:
        access = GL_WRITE_ONLY;
        break;
    case MemoryUsage_DeviceToHost:
        access = GL_READ_ONLY;
        break;
    default:
        Fatal("cannot map buffer with memory usage {}", m_Memory);
    }

    return glMapNamedBuffer(m_Handle, access);
}

void fxng::glal::opengl::Buffer::Unmap()
{
    glUnmapNamedBuffer(m_Handle);
}

std::uint32_t fxng::glal::opengl::Buffer::GetHandle() const
{
    return m_Handle;
}
